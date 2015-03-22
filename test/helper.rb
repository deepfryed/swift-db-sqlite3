require 'bundler/setup'
require 'minitest/autorun'
require 'swift/db/sqlite3'

class MiniTest::Spec
  def db
    @db ||= Swift::DB::Sqlite3.new(db: ':memory:')
  end
end
