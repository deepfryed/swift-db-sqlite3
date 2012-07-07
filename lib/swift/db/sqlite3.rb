require 'swift/db/sqlite3/swift_db_sqlite3_ext'

module Swift
  module DB
    class Sqlite3
      def returning?
        false
      end

      def tables
        execute('select name from sqlite_master where type = ?', 'table').map(&:values).flatten
      end
    end # Sqlite3
  end # DB
end # Swift
