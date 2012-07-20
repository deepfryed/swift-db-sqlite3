# Swift Sqlite3 adapter

MRI adapter for sqlite3 for use in Swift ORM.

## Features

* Lightweight & fast
* Result typecasting
* Prepared statements
* Nested transactions

## API

```
  Swift::DB::Sqlite3
    .new(options)
    #execute(sql, *bind)
    #prepare(sql)
    #begin(savepoint = nil)
    #commit(savepoint = nil)
    #rollback(savepoint = nil)
    #transaction(savepoint = nil, &block)
    #close
    #closed?
    #escape(text)

  Swift::DB::Sqlite3::Statement
    .new(Swift::DB::Sqlite3, sql)
    #execute(*bind)
    #release

  Swift::DB::Sqlite3::Result
    #selected_rows
    #affected_rows
    #fields
    #types
    #each
    #insert_id
```

## Example

```ruby
require 'swift/db/sqlite3'

db = Swift::DB::Sqlite3.new(db: ':memory:')

db.execute('drop table if exists users')
db.execute('create table users (id integer primary key, name text, age integer, created_at datetime)')
db.execute('insert into users(name, age, created_at) values(?, ?, ?)', 'test', 30, Time.now.utc)

row = db.execute('select * from users').first
p row #=> {:id => 1, :name => 'test', :age => 30, :created_at=> #<Swift::DateTime>}
```

## License

MIT
