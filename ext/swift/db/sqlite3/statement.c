// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#include "statement.h"

/* declaration */

VALUE cDSS;

Adapter* db_sqlite3_adapter_handle(VALUE);
VALUE    db_sqlite3_result_allocate(VALUE);
VALUE    db_sqlite3_result_initialize(VALUE, VALUE);
VALUE    db_sqlite3_result_consume(VALUE);
VALUE    db_sqlite3_result_each(VALUE);

/* definition */

Statement* db_sqlite3_statement_handle(VALUE self) {
    Statement *s;
    Data_Get_Struct(self, Statement, s);
    if (!s)
        rb_raise(eSwiftRuntimeError, "Invalid sqlite3 statement");
    return s;
}

void db_sqlite3_statement_mark(Statement *s) {
    if (s && !NIL_P(s->adapter))
        rb_gc_mark_maybe(s->adapter);
}

VALUE db_sqlite3_statement_deallocate(Statement *s) {
    if (s && s->s) {
        sqlite3_finalize(s->s);
        free(s);
    }
}

VALUE db_sqlite3_statement_allocate(VALUE klass) {
    Statement *s = (Statement*)malloc(sizeof(Statement));
    return Data_Wrap_Struct(klass, db_sqlite3_statement_mark, db_sqlite3_statement_deallocate, s);
}

VALUE db_sqlite3_statement_initialize(VALUE self, VALUE adapter, VALUE sql) {
    Statement *s = db_sqlite3_statement_handle(self);

    s->s       = 0;
    s->c       = db_sqlite3_adapter_handle(adapter)->connection;
    s->adapter = adapter;

    if (sqlite3_prepare_v2(s->c, RSTRING_PTR(sql), RSTRING_LEN(sql), &(s->s), 0) != SQLITE_OK)
        rb_raise(eSwiftRuntimeError, "%s\nSQL: %s", sqlite3_errmsg(s->c), RSTRING_PTR(sql));

    return self;
}

VALUE db_sqlite3_statement_insert_id(VALUE self) {
    Statement *s = db_sqlite3_statement_handle(self);
    return SIZET2NUM(sqlite3_last_insert_rowid(s->c));
}

VALUE db_sqlite3_statement_execute(int argc, VALUE *argv, VALUE self) {
    int expect, n;
    VALUE bind, result;

    Statement *s = db_sqlite3_statement_handle(self);

    rb_scan_args(argc, argv, "00*", &bind);
    expect = sqlite3_bind_parameter_count(s->s);
    if (expect != RARRAY_LEN(bind))
        rb_raise(eSwiftArgumentError, "expected %d bind values got %d", expect, RARRAY_LEN(bind));

    for (n = 0; n < expect; n++) {
        VALUE value = rb_ary_entry(bind, n);
        if (NIL_P(value))
            sqlite3_bind_null(s->s, n + 1);
        else {
            VALUE text = typecast_to_string(value);
            sqlite3_bind_text(s->s, n + 1, RSTRING_PTR(text), RSTRING_LEN(text), 0);
        }
    }

    result = db_sqlite3_result_allocate(cDSR);
    db_sqlite3_result_initialize(result, self);
    db_sqlite3_result_consume(result);
    return result;
}

void init_swift_db_sqlite3_statement() {
    cDSS = rb_define_class_under(cDSA, "Statement", rb_cObject);
    rb_define_alloc_func(cDSS, db_sqlite3_statement_allocate);
    rb_define_method(cDSS, "initialize", db_sqlite3_statement_initialize, 2);
    rb_define_method(cDSS, "execute",    db_sqlite3_statement_execute,   -1);
    rb_define_method(cDSS, "insert_id",  db_sqlite3_statement_insert_id,  0);
}
