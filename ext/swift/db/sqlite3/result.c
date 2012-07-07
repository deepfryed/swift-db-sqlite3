// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#include "result.h"

/* declaration */

typedef struct Result {
    VALUE fields;
    VALUE types;
    VALUE rows;
    VALUE statement;
    sqlite3* c;
    sqlite3_stmt *s;
} Result;

typedef struct Type {
    int value;
    const char *definition;
} Type;

VALUE cDSR;

Statement* db_sqlite3_statement_handle(VALUE);

/* definition */

Result* db_sqlite3_result_handle(VALUE self) {
    Result *r;
    Data_Get_Struct(self, Result, r);
    if (!r)
        rb_raise(eSwiftRuntimeError, "Invalid sqlite3 result");
    return r;
}

void db_sqlite3_result_mark(Result *r) {
    if (r) {
        if (r->statement)
            rb_gc_mark(r->statement);
        if (r->fields)
            rb_gc_mark(r->fields);
        if (r->types)
            rb_gc_mark(r->types);
        if (r->rows)
            rb_gc_mark(r->rows);
    }
}

VALUE db_sqlite3_result_deallocate(Result *r) {
    if (r) free(r);
}

VALUE db_sqlite3_result_allocate(VALUE klass) {
    Result *r = (Result*)malloc(sizeof(Result));
    return Data_Wrap_Struct(klass, db_sqlite3_result_mark, db_sqlite3_result_deallocate, r);
}

VALUE db_sqlite3_result_initialize(VALUE self, VALUE statement) {
    Result *r    = db_sqlite3_result_handle(self);
    Statement *s = db_sqlite3_statement_handle(statement);

    r->statement = statement;
    r->s         = s->s;
    r->c         = s->c;
    r->fields    = rb_ary_new();
    r->types     = rb_ary_new();
    r->rows      = rb_ary_new();

    return self;
}

VALUE db_sqlite3_result_consume(VALUE self) {
    int n, i, rc;
    const char *type, *data;
    Result *r = db_sqlite3_result_handle(self);

    Type types[] = {
        {SWIFT_TYPE_INT,        "int"},
        {SWIFT_TYPE_INT,        "integer"},
        {SWIFT_TYPE_INT,        "bigint"},
        {SWIFT_TYPE_FLOAT,      "real"},
        {SWIFT_TYPE_FLOAT,      "float"},
        {SWIFT_TYPE_NUMERIC,    "decimal"},
        {SWIFT_TYPE_NUMERIC,    "numeric"},
        {SWIFT_TYPE_BOOLEAN,    "bool"},
        {SWIFT_TYPE_BOOLEAN,    "boolean"},
        {SWIFT_TYPE_DATE,       "date"},
        {SWIFT_TYPE_TIME,       "time"},
        {SWIFT_TYPE_TIMESTAMP,  "timestamp"},
        {SWIFT_TYPE_TIMESTAMP,  "datetime"},
        {SWIFT_TYPE_TEXT,       "text"},
        {SWIFT_TYPE_TEXT,       "char"},
        {SWIFT_TYPE_TEXT,       "varchar"},
        {SWIFT_TYPE_BLOB,       "blob"},
        {SWIFT_TYPE_UNKNOWN,    "unknown"}
    };

    size_t ntypes = sizeof(types) / sizeof(Type);

    rb_ary_clear(r->fields);
    rb_ary_clear(r->types);
    rb_ary_clear(r->rows);

    for (n = 0; n < sqlite3_column_count(r->s); n++) {
        rb_ary_push(r->fields, ID2SYM(rb_intern(sqlite3_column_name(r->s, n))));

        type = sqlite3_column_decltype(r->s, n);
        for (i = 0; i < (int)ntypes; i++) {
            if (strcmp(type, types[i].definition) == 0 || types[i].value == 9) {
                rb_ary_push(r->types, INT2NUM(types[i].value));
                break;
            }
        }
    }

    while ((rc = sqlite3_step(r->s)) == SQLITE_ROW) {
        VALUE row = rb_ary_new();
        for (n = 0; n < sqlite3_column_count(r->s); n++) {
            switch (sqlite3_column_type(r->s, n)) {
                case SQLITE_NULL:
                    rb_ary_push(row, Qnil);
                    break;
                case SQLITE_TEXT:
                case SQLITE_BLOB:
                    data = sqlite3_column_blob(r->s, n);
                    rb_ary_push(row, typecast_detect(data, strlen(data), NUM2INT(rb_ary_entry(r->types, n))));
                    break;
                default:
                    data = sqlite3_column_text(r->s, n);
                    rb_ary_push(row, typecast_detect(data, strlen(data), NUM2INT(rb_ary_entry(r->types, n))));
            }
        }
        rb_ary_push(r->rows, row);
    }

    if (rc != SQLITE_DONE)
        rb_raise(eSwiftRuntimeError, "%s\nSQL: %s", sqlite3_errmsg(r->c), sqlite3_sql(r->s));

    return self;
}

VALUE db_sqlite3_result_each(VALUE self) {
    int n, f;
    Result *r = db_sqlite3_result_handle(self);

    for (n = 0; n < RARRAY_LEN(r->rows); n++) {
        VALUE tuple = rb_hash_new();
        VALUE row   = rb_ary_entry(r->rows, n);
        for (f = 0; f < RARRAY_LEN(r->fields); f++)
            rb_hash_aset(tuple, rb_ary_entry(r->fields, f), rb_ary_entry(row, f));
        rb_yield(tuple);
    }
}

VALUE db_sqlite3_result_rows(VALUE self) {
    Result *r = db_sqlite3_result_handle(self);
    return SIZET2NUM(RARRAY_LEN(r->rows));
}

VALUE db_sqlite3_result_columns(VALUE self) {
    Result *r = db_sqlite3_result_handle(self);
    return INT2NUM(RARRAY_LEN(r->fields));
}

VALUE db_sqlite3_result_fields(VALUE self) {
    Result *r = db_sqlite3_result_handle(self);
    return r->fields;
}

VALUE db_sqlite3_result_field_types(VALUE self) {
    Result *r = db_sqlite3_result_handle(self);
    return typecast_description(r->types);
}

void init_swift_db_sqlite3_result() {
    rb_require("bigdecimal");
    rb_require("stringio");
    rb_require("date");

    cDSR = rb_define_class_under(cDSA, "Result", rb_cObject);

    rb_include_module(cDSR, rb_mEnumerable);
    rb_define_alloc_func(cDSR, db_sqlite3_result_allocate);
    rb_define_method(cDSR, "initialize",  db_sqlite3_result_initialize,  1);
    rb_define_method(cDSR, "each",        db_sqlite3_result_each,        0);
    rb_define_method(cDSR, "rows",        db_sqlite3_result_rows,        0);
    rb_define_method(cDSR, "columns",     db_sqlite3_result_columns,     0);
    rb_define_method(cDSR, "fields",      db_sqlite3_result_fields,      0);
    rb_define_method(cDSR, "field_types", db_sqlite3_result_field_types, 0);
}
