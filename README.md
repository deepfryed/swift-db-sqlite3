# Swift Sqlite3 adapter

MRI adapter for sqlite3 for use in Swift ORM. This is an experimental rewrite of the Swift adapters and the API
is not considered to be stable.

## API

```
  Swift::DB::Sqlite3
    .new(options)
    #execute(sql, *bind)
    #begin(savepoint = nil)
    #commit(savepoint = nil)
    #rollback(savepoint = nil)
    #transaction(savepoint = nil, &block)

  Swift::DB::Sqlite3::Statement
    .new(Swift::DB::Sqlite3, sql)
    #execute(*bind)
    #insert_id

  Swift::DB::Sqlite3::Result
    #selected_rows
    #affected_rows
    #fields
    #types
    #each
    #insert_id
```

## License

MIT
