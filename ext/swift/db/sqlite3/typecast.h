// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#pragma once

#define SWIFT_TYPE_INT       0
#define SWIFT_TYPE_FLOAT     1
#define SWIFT_TYPE_NUMERIC   2
#define SWIFT_TYPE_BOOLEAN   3
#define SWIFT_TYPE_DATE      4
#define SWIFT_TYPE_TIME      5
#define SWIFT_TYPE_TIMESTAMP 6
#define SWIFT_TYPE_TEXT      7
#define SWIFT_TYPE_BLOB      8
#define SWIFT_TYPE_UNKNOWN   9

VALUE typecast_to_string(VALUE);
VALUE typecast_string(const char *, size_t);
VALUE typecast_detect(const char *, size_t, int);
VALUE typecast_description(VALUE list);
void  init_swift_db_sqlite3_typecast();
