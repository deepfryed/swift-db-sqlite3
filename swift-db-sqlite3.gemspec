# -*- encoding: utf-8 -*-
# stub: swift-db-sqlite3 0.1.7 ruby lib ext
# stub: ext/swift/db/sqlite3/extconf.rb

Gem::Specification.new do |s|
  s.name = "swift-db-sqlite3".freeze
  s.version = "0.1.7"

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.require_paths = ["lib".freeze, "ext".freeze]
  s.authors = ["Bharanee Rathna".freeze]
  s.date = "2018-12-13"
  s.description = "Swift adapter for Sqlite3 database".freeze
  s.email = ["deepfryed@gmail.com".freeze]
  s.extensions = ["ext/swift/db/sqlite3/extconf.rb".freeze]
  s.files = ["CHANGELOG".freeze, "README.md".freeze, "ext/swift/db/sqlite3/adapter.c".freeze, "ext/swift/db/sqlite3/adapter.h".freeze, "ext/swift/db/sqlite3/common.c".freeze, "ext/swift/db/sqlite3/common.h".freeze, "ext/swift/db/sqlite3/datetime.c".freeze, "ext/swift/db/sqlite3/datetime.h".freeze, "ext/swift/db/sqlite3/extconf.rb".freeze, "ext/swift/db/sqlite3/main.c".freeze, "ext/swift/db/sqlite3/result.c".freeze, "ext/swift/db/sqlite3/result.h".freeze, "ext/swift/db/sqlite3/statement.c".freeze, "ext/swift/db/sqlite3/statement.h".freeze, "ext/swift/db/sqlite3/typecast.c".freeze, "ext/swift/db/sqlite3/typecast.h".freeze, "lib/swift-db-sqlite3.rb".freeze, "lib/swift/db/sqlite3.rb".freeze, "test/helper.rb".freeze, "test/test_adapter.rb".freeze, "test/test_encoding.rb".freeze]
  s.homepage = "http://github.com/deepfryed/swift-db-sqlite3".freeze
  s.licenses = ["MIT".freeze]
  s.rubygems_version = "2.6.13".freeze
  s.summary = "Swift sqlite3 adapter".freeze

  if s.respond_to? :specification_version then
    s.specification_version = 4

    if Gem::Version.new(Gem::VERSION) >= Gem::Version.new('1.2.0') then
      s.add_development_dependency(%q<rake>.freeze, ["~> 0"])
    else
      s.add_dependency(%q<rake>.freeze, ["~> 0"])
    end
  else
    s.add_dependency(%q<rake>.freeze, ["~> 0"])
  end
end
