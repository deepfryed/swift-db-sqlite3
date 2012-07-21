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
    #ping
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

## Performance

Don't read too much into it. Each library has its advantages and disadvantages.

```
# insert 1000 rows and read them back 100 times with typecasting enabled.

$ ruby -v

ruby 1.9.3p125 (2012-02-16 revision 34643) [x86_64-linux]

$ ruby check.rb
                           user     system      total        real
do_sqlite3 insert      0.050000   0.020000   0.070000 (  0.062814)
do_sqlite3 select      0.720000   0.000000   0.720000 (  0.723628)

sqlite3 insert         0.040000   0.000000   0.040000 (  0.046895)
sqlite3 select         4.390000   0.000000   4.390000 (  4.400678)

swift insert           0.030000   0.000000   0.030000 (  0.030628)
swift select           0.480000   0.000000   0.480000 (  0.488608)
```

## License

MIT
