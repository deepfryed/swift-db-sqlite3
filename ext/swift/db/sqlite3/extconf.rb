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

def ensure_library name, lib_paths, hint = nil
  unless find_library(name, 'main', *lib_paths)
    puts $/, '#' * 80, hint, '#' * 80, $/ if hint
    raise "unable to find #{name}"
  end
end

find_header('sqlite3.h',   *inc_paths) or raise 'unable to locate sqlite3 headers'
find_header('uuid/uuid.h', *inc_paths) or raise 'unable to locate uuid headers'

ensure_library 'sqlite3',  lib_paths, 'please install sqlite3 development libraries'
ensure_library 'uuid',     lib_paths, 'please install uuid development libraries'

create_makefile('swift_db_sqlite3_ext')
