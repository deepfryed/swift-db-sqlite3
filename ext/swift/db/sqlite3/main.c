// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#include "common.h"
#include "adapter.h"
#include "statement.h"
#include "result.h"
#include "datetime.h"

VALUE mSwift, mDB;
VALUE eSwiftError, eSwiftArgumentError, eSwiftRuntimeError, eSwiftConnectionError;

void Init_swift_db_sqlite3_ext() {
    mSwift = rb_define_module("Swift");
    mDB    = rb_define_module_under(mSwift, "DB");

    eSwiftError           = rb_define_class("SwiftError", rb_eStandardError);
    eSwiftArgumentError   = rb_define_class("SwiftArgumentError",   eSwiftError);
    eSwiftRuntimeError    = rb_define_class("SwiftRuntimeError",    eSwiftError);
    eSwiftConnectionError = rb_define_class("SwiftConnectionError", eSwiftError);

    init_swift_db_sqlite3_adapter();
    init_swift_db_sqlite3_statement();
    init_swift_db_sqlite3_result();
    init_swift_datetime();
    init_swift_db_sqlite3_typecast();
}
