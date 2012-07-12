# Swift Sqlite3 adapter

MRI adapter for sqlite3 for use in Swift ORM.

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

## License

MIT
