// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#pragma once

#include "common.h"
#include "adapter.h"

void init_swift_db_sqlite3_statement();

typedef struct Statement {
    sqlite3 *c;
    sqlite3_stmt *s;
    VALUE adapter;
} Statement;

