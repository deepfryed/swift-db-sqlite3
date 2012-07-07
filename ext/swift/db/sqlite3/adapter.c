// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#include "adapter.h"

/* declaration */
VALUE cDSA;
VALUE db_sqlite3_statement_allocate(VALUE);
VALUE db_sqlite3_statement_initialize(VALUE, VALUE, VALUE);
VALUE db_sqlite3_statement_execute(int argc, VALUE *argv, VALUE self);

/* definition */
Adapter* db_sqlite3_adapter_handle(VALUE self) {
    Adapter *a;
    Data_Get_Struct(self, Adapter, a);
    if (!a)
        rb_raise(eSwiftRuntimeError, "Invalid sqlite3 adapter");
    return a;
}

VALUE db_sqlite3_adapter_deallocate(Adapter *a) {
    if (a && a->connection)
        sqlite3_close(a->connection);
    if (a)
        free(a);
}

VALUE db_sqlite3_adapter_allocate(VALUE klass) {
    Adapter *a = (Adapter*)malloc(sizeof(Adapter));

    a->connection = 0;
    a->t_nesting  = 0;
    return Data_Wrap_Struct(klass, 0, db_sqlite3_adapter_deallocate, a);
}

VALUE db_sqlite3_adapter_initialize(VALUE self, VALUE options) {
    VALUE db;
    sqlite3* connection;
    Adapter *a = db_sqlite3_adapter_handle(self);

    if (TYPE(options) != T_HASH)
        rb_raise(eSwiftArgumentError, "options needs to be a hash");

    db = rb_hash_aref(options, ID2SYM(rb_intern("db")));
    if (NIL_P(db))
        rb_raise(eSwiftConnectionError, "Invalid db name");

    if (sqlite3_open_v2(RSTRING_PTR(db), &connection, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 0) != SQLITE_OK)
       rb_raise(eSwiftConnectionError, "%s", sqlite3_errmsg(connection));

    a->connection = connection;
    return Qnil;
}

VALUE db_sqlite3_adapter_execute(int argc, VALUE *argv, VALUE self) {
    VALUE sql, bind, statement;
    rb_scan_args(argc, argv, "10*", &sql, &bind);
    return db_sqlite3_statement_execute(
        1,
        &bind,
        db_sqlite3_statement_initialize(db_sqlite3_statement_allocate(cDSS), self, sql)
    );
}

VALUE db_sqlite3_adapter_begin(int argc, VALUE *argv, VALUE self) {
    int auto_commit;
    VALUE savepoint, sql;
    char command[256];

    Adapter *a = db_sqlite3_adapter_handle(self);
    rb_scan_args(argc, argv, "01", &savepoint);

    if (a->t_nesting == 0) {
        sql = rb_str_new2("begin");
        db_sqlite3_adapter_execute(1, &sql, self);
        a->t_nesting++;
        if (NIL_P(savepoint))
            return Qtrue;
    }

    if (NIL_P(savepoint))
        savepoint = rb_uuid_string();

    snprintf(command, 256, "savepoint %s", RSTRING_PTR(savepoint));
    sql = rb_str_new2(command);
    db_sqlite3_adapter_execute(1, &sql, self);
    a->t_nesting++;
    return savepoint;
}

VALUE db_sqlite3_adapter_commit(int argc, VALUE *argv, VALUE self) {
    VALUE savepoint, sql;
    char command[256];

    Adapter *a = db_sqlite3_adapter_handle(self);
    rb_scan_args(argc, argv, "01", &savepoint);

    if (a->t_nesting == 0)
        return Qfalse;

    if (NIL_P(savepoint)) {
        sql = rb_str_new2("commit");
        db_sqlite3_adapter_execute(1, &sql, self);
        a->t_nesting--;
    }
    else {
        snprintf(command, 256, "release savepoint %s", RSTRING_PTR(savepoint));
        sql = rb_str_new2(command);
        db_sqlite3_adapter_execute(1, &sql, self);
        a->t_nesting--;
    }
    return Qtrue;
}

VALUE db_sqlite3_adapter_rollback(int argc, VALUE *argv, VALUE self) {
    VALUE savepoint, sql;
    char command[256];

    Adapter *a = db_sqlite3_adapter_handle(self);
    rb_scan_args(argc, argv, "01", &savepoint);

    if (a->t_nesting == 0)
        return Qfalse;

    if (NIL_P(savepoint)) {
        sql = rb_str_new2("rollback");
        db_sqlite3_adapter_execute(1, &sql, self);
        a->t_nesting--;
    }
    else {
        snprintf(command, 256, "rollback to savepoint %s", RSTRING_PTR(savepoint));
        sql = rb_str_new2(command);
        db_sqlite3_adapter_execute(1, &sql, self);
        a->t_nesting--;
    }
    return Qtrue;
}

VALUE db_sqlite3_adapter_transaction(int argc, VALUE *argv, VALUE self) {
    int status;
    VALUE savepoint, block, block_result;

    Adapter *a = db_sqlite3_adapter_handle(self);
    rb_scan_args(argc, argv, "01&", &savepoint, &block);

    if (NIL_P(block))
        rb_raise(eSwiftRuntimeError, "sqlite3 transaction requires a block");

    if (a->t_nesting == 0) {
        db_sqlite3_adapter_begin(1, &savepoint, self);
        block_result = rb_protect(rb_yield, self, &status);
        if (!status) {
            db_sqlite3_adapter_commit(1, &savepoint, self);
            if (!NIL_P(savepoint))
                db_sqlite3_adapter_commit(0, 0, self);
        }
        else {
            db_sqlite3_adapter_rollback(1, &savepoint, self);
            if (!NIL_P(savepoint))
                db_sqlite3_adapter_rollback(0, 0, self);
            rb_jump_tag(status);
        }
    }
    else {
        if (NIL_P(savepoint))
            savepoint = rb_uuid_string();
        db_sqlite3_adapter_begin(1, &savepoint, self);
        block_result = rb_protect(rb_yield, self, &status);
        if (!status)
            db_sqlite3_adapter_commit(1, &savepoint, self);
        else {
            db_sqlite3_adapter_rollback(1, &savepoint, self);
            rb_jump_tag(status);
        }
    }

    return block_result;
}

void init_swift_db_sqlite3_adapter() {
    cDSA = rb_define_class_under(mDB, "Sqlite3", rb_cObject);
    rb_define_alloc_func(cDSA, db_sqlite3_adapter_allocate);

    rb_define_method(cDSA, "initialize",  db_sqlite3_adapter_initialize,   1);
    rb_define_method(cDSA, "execute",     db_sqlite3_adapter_execute,     -1);
    rb_define_method(cDSA, "begin",       db_sqlite3_adapter_begin,       -1);
    rb_define_method(cDSA, "commit",      db_sqlite3_adapter_commit,      -1);
    rb_define_method(cDSA, "rollback",    db_sqlite3_adapter_rollback,    -1);
    rb_define_method(cDSA, "transaction", db_sqlite3_adapter_transaction, -1);
}
