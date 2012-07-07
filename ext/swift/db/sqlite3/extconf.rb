#!/usr/bin/env ruby

require 'mkmf'

find_library('sqlite3', 'main', '/usr/lib /usr/local/lib /opt/lib /opt/local/lib')
find_library('uuid',    'main', '/usr/lib /usr/local/lib /opt/lib /opt/local/lib')
create_makefile('swift_db_sqlite3_ext')

