// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#include "common.h"
#include "adapter.h"
#include "statement.h"
#include "result.h"
#include "datetime.h"

VALUE mSwift, mSwiftDB;
VALUE eSwiftError, eSwiftArgumentError, eSwiftRuntimeError, eSwiftConnectionError;

void Init_swift_db_sqlite3_ext() {
    mSwift   = rb_define_module("Swift");
    mSwiftDB = rb_define_module_under(mSwift, "DB");

    eSwiftError           = rb_define_class_under(mSwift, "Error",           rb_eStandardError);
    eSwiftArgumentError   = rb_define_class_under(mSwift, "ArgumentError",   eSwiftError);
    eSwiftRuntimeError    = rb_define_class_under(mSwift, "RuntimeError",    eSwiftError);
    eSwiftConnectionError = rb_define_class_under(mSwift, "ConnectionError", eSwiftError);

    init_swift_db_sqlite3_adapter();
    init_swift_db_sqlite3_statement();
    init_swift_db_sqlite3_result();
    init_swift_datetime();
    init_swift_db_sqlite3_typecast();
}
