require "yajl"

CONFIG = {
  :file       => "features/config/cuke.json",
  :db         => "cpplscuke",
  :collection => "lsevents",
  :pidfile    => "features/pids/cukerunner.pid",
  :errfile    => "features/log/error.log"
}

CONFIG[:ns] = "#{ CONFIG[:db] }.#{ CONFIG[:collection] }"
CONFIG[:config] = Yajl::load( File.read( CONFIG[:file] ) )
