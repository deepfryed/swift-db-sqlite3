#!/usr/bin/env ruby

$:.unshift File.dirname(__FILE__) + '/../ext'
$:.unshift File.dirname(__FILE__) + '/../lib'

require 'bundler/setup'
require 'sqlite3'
require 'do_sqlite3'
require 'swift-db-sqlite3'
require 'benchmark'

dbs = {
  do_sqlite3: DataObjects::Connection.new("sqlite3://:memory:"),
  sqlite3:    SQLite3::Database.new(':memory:').tap {|db| db.results_as_hash = db.type_translation = true},
  swift:      Swift::DB::Sqlite3.new(db: ':memory:')
}

sql = {
  create: 'create table users(id integer primary key, name text, created_at datetime)',
  insert: 'insert into users(name, created_at) values (?, ?)',
  update: 'update users set name = ?, created_at = ? where id = ?',
  select: 'select * from users where id > ?'
}

rows = 1000
iter = 100

module DataObjects
  class Connection
    def query sql, *args
      create_command(sql).execute_reader(*args)
    end
    def execute sql, *args
      create_command(sql).execute_non_query(*args)
    end
  end
end

Benchmark.bm(20) do |bm|
  dbs.each do |name, db|
    db.execute(sql[:create])

    bm.report("#{name} insert") do
      rows.times do |n|
        db.execute(sql[:insert], "name #{n}", Time.now.to_s)
      end
    end

    bm.report("#{name} select") do
      case db
        when DataObjects::Sqlite3::Connection
          iter.times do
            db.query(sql[:select], 0).entries
          end
        else
          iter.times do
            db.execute(sql[:select], 0).entries
          end
      end
    end
  end
end
