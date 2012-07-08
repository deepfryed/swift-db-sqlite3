#!/usr/bin/env ruby

$:.unshift File.dirname(__FILE__) + '/../ext'
$:.unshift File.dirname(__FILE__) + '/../lib'

require 'bundler/setup'
require 'sqlite3'
require 'swift-db-sqlite3'
require 'benchmark'

dbs = {
  sqlite3: SQLite3::Database.new(':memory:').tap {|db| db.results_as_hash = db.type_translation = true},
  swift:   Swift::DB::Sqlite3.new(db: ':memory:')
}

sql = {
  create: 'create table users(id integer primary key, name text, created_at datetime)',
  insert: 'insert into users(name, created_at) values (?, ?)',
  update: 'update users set name = ?, created_at = ? where id = ?',
  select: 'select * from users where id > ?'
}

rows = 1000
iter = 100

Benchmark.bm(15) do |bm|
  dbs.each do |name, db|
    db.execute(sql[:create])

    bm.report("#{name} insert") do
      rows.times do |n|
        db.execute(sql[:insert], "name #{n}", Time.now.to_s)
      end
    end

    bm.report("#{name} select") do
      iter.times do
        db.execute(sql[:select], 0).entries
      end
    end
  end
end
