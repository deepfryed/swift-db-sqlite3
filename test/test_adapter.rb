require 'helper'

describe 'sqlite3 adapter' do
  it 'should initialize' do
    assert db
  end

  it 'should execute sql' do
    assert db.execute("select name from sqlite_master")
  end

  it 'should return result on #execute' do
    now = Time.now
    assert db.execute('create table users (id integer primary key, name text, age integer, created_at timestamp)')
    assert db.execute('insert into users(name, age, created_at) values(?, ?, ?)', 'test', nil, now)

    result = db.execute('select * from users')

    assert_equal 1, result.rows
    assert_equal 4, result.columns
    assert_equal %w(id name age created_at).map(&:to_sym), result.fields
    assert_equal %w(integer text integer timestamp), result.field_types

    row = result.first
    assert_equal 1,      row[:id]
    assert_equal 'test', row[:name]
    assert_equal nil,    row[:age]
    assert_equal now,    row[:created_at].to_time
  end
end
