require "mongo"

Given( /^the logging service is running$/ ) do
  run_service
end

When( /^I send simple events to the service$/ ) do
  client = Mongo::MongoClient.from_uri( "mongodb://localhost" )
  db = client.db( CONFIG[:db] )
  collection = db.collection( CONFIG[:collection] )
  origins = [ "server01", "server02", "server03" ]
  levels = [ "debug", "warn", "error" ]
  messages = [ "Everything is broked and fecked up",
               "Mysterious beards descended from a preternaturally bright cosmos",
               "Fire! Fire! Oh, wait. Sorry. I mean 404! 404!",
               "Multiline,\nmultiline\nriding through the glen.",
               "Inexplicable { \"hash\": \"object\" } in message",
               "Inexplicable, tight {\"hash\":\"object\"} in message",
               "Ruby-style { \"hash\" => \"object\" } in message",
               "Tight, Ruby-style {\"hash\"=>\"object\"} in message"
             ]
  events = messages.map do |message|
    { :logid   => "simple",
      :origin  => origins.shuffle.first,
      :level   => levels.shuffle.first,
      :message => message
    }
  end
  split = rand( events.size - 2 ) + 1
  collection.insert( events[0...split] )
  sleep( 0.25 )
  collection.insert( events[split..-1] )
end
