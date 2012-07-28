#!/usr/bin/env ruby

require 'mkmf'

$CFLAGS = '-std=c99 -Os'

def ensure_library name, hint = nil
  unless find_library(name, 'main', '/usr/lib /usr/local/lib /opt/lib /opt/local/lib')
    puts $/, '#' * 80, hint, '#' * 80, $/ if hint
    raise "unable to find #{name}"
  end
end

ensure_library 'sqlite3',  'please install sqlite3 development libraries'
ensure_library 'uuid',     'please install uuid development libraries'
create_makefile('swift_db_sqlite3_ext')
