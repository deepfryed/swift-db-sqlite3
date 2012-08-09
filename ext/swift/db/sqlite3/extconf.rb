#!/usr/bin/env ruby

require 'mkmf'

$CFLAGS = '-std=c99 -Os'

inc_paths = %w(
  /usr/include
  /usr/local/include
  /opt/local/include
  /sw/include
)

lib_paths = %w(
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  /sw/lib
)

uuid_inc,  uuid_lib      = dir_config('uuid',  '/usr/include/uuid', '/usr/lib')
sqlite3_inc, sqlite3_lib = dir_config('sqlite3')

find_header 'uuid/uuid.h', *inc_paths.dup.unshift(uuid_inc).compact
find_header 'sqlite3.h',   *inc_paths.dup.unshift(sqlite3_inc).compact

find_library 'uuid',    'main', *lib_paths.dup.unshift(uuid_lib).compact
find_library 'sqlite3', 'main', *lib_paths.dup.unshift(sqlite3_lib).compact

create_makefile('swift_db_sqlite3_ext')
