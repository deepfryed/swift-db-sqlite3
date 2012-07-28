# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = "swift-db-sqlite3"
  s.version = "0.1.2"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Bharanee Rathna"]
  s.date = "2012-07-28"
  s.description = "Swift adapter for Sqlite3 database"
  s.email = ["deepfryed@gmail.com"]
  s.extensions = ["ext/swift/db/sqlite3/extconf.rb"]
  s.files = ["ext/swift/db/sqlite3/result.c", "ext/swift/db/sqlite3/adapter.c", "ext/swift/db/sqlite3/statement.c", "ext/swift/db/sqlite3/datetime.c", "ext/swift/db/sqlite3/typecast.c", "ext/swift/db/sqlite3/common.c", "ext/swift/db/sqlite3/main.c", "ext/swift/db/sqlite3/adapter.h", "ext/swift/db/sqlite3/statement.h", "ext/swift/db/sqlite3/typecast.h", "ext/swift/db/sqlite3/common.h", "ext/swift/db/sqlite3/result.h", "ext/swift/db/sqlite3/datetime.h", "ext/swift/db/sqlite3/extconf.rb", "test/test_encoding.rb", "test/helper.rb", "test/test_adapter.rb", "lib/swift/db/sqlite3.rb", "lib/swift-db-sqlite3.rb", "README.md", "CHANGELOG"]
  s.homepage = "http://github.com/deepfryed/swift-db-sqlite3"
  s.require_paths = ["lib", "ext"]
  s.rubygems_version = "1.8.24"
  s.summary = "Swift sqlite3 adapter"

  if s.respond_to? :specification_version then
    s.specification_version = 3

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<rake>, [">= 0"])
    else
      s.add_dependency(%q<rake>, [">= 0"])
    end
  else
    s.add_dependency(%q<rake>, [">= 0"])
  end
end
