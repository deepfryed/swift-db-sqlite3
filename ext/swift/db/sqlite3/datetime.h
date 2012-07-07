#pragma once

#include "common.h"
#include <math.h>

extern VALUE cSwiftDateTime;
void init_swift_datetime();
VALUE datetime_parse(VALUE klass, const char *data, size_t size);
