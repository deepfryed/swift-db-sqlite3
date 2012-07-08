require 'date'
require 'pathname'
require 'rake'
require 'rake/clean'
require 'rake/testtask'

$rootdir = Pathname.new(__FILE__).dirname
$gemspec = Gem::Specification.new do |s|
  s.name              = 'swift-db-sqlite3'
  s.version           = '0.1.0'
  s.date              = Date.today    
  s.authors           = ['Bharanee Rathna']
  s.email             = ['deepfryed@gmail.com']
  s.summary           = 'Swift sqlite3 adapter'
  s.description       = 'Swift adapter for Sqlite3 database'
  s.homepage          = 'http://github.com/deepfryed/swift-db-sqlite3'
  s.files             = Dir['ext/**/*.{c,h}'] + Dir['{ext,test,lib}/**/*.rb'] + %w(README.md CHANGELOG)
  s.extensions        = %w(ext/swift/db/sqlite3/extconf.rb)
  s.require_paths     = %w(lib ext)

  s.add_development_dependency('rake')
end

desc 'Generate gemspec'
task :gemspec do 
  $gemspec.date = Date.today
  File.open('%s.gemspec' % $gemspec.name, 'w') {|fh| fh.write($gemspec.to_ruby)}
end

desc 'compile extension'
task :compile do
  Dir.chdir('ext/swift/db/sqlite3') do
    system('ruby extconf.rb && make -j2') or raise 'unable to compile sqlite3'
  end
end

Rake::TestTask.new(:test) do |test|
  test.libs   << 'ext' << 'lib' << 'test'
  test.pattern = 'test/**/test_*.rb'
  test.verbose = true
end

task default: :test
task :test => [:compile]
