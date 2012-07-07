require 'helper'

describe 'sqlite3 adapter' do
  it 'should initialize' do
    assert db
  end

  it 'should execute sql' do
    assert db.execute("select name from sqlite_master")
  end

  it 'should typecast values' do
    now = Time.now
    assert db.execute('create table users (id integer primary key, name text, created_at timestamp)')
    assert db.execute('insert into users(name, created_at) values(?, ?)', 'test', now)

    row = db.execute('select * from users').first
    assert_equal 1,      row[:id]
    assert_equal 'test', row[:name]
    assert_equal now,    row[:created_at]
  end
end
