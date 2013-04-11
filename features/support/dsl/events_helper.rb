module EventsHelper

  MESSAGES = [ "Everything is broked and fecked up",
               "Mysterious beards descended from a preternaturally bright cosmos",
               "Fire! Fire! Oh, wait. Sorry. I mean 404! 404!",
               "Multiline,\nmultiline\nriding through the glen.",
               "Inexplicable { \"hash\": \"object\" } in message",
               "Inexplicable, tight {\"hash\":\"object\"} in message",
               "Ruby-style { \"hash\" => \"object\" } in message",
               "Tight, Ruby-style {\"hash\"=>\"object\"} in message"
             ]

  def db
    @db ||= Mongo::MongoClient.from_uri( "mongodb://localhost" ).db( CONFIG[:db] )
  end

  def send_events( type )
    collection = db.collection( CONFIG[:collection] )
    origins = [ "server01", "server02", "server03" ]
    levels = [ "debug", "warn", "error" ]
    events = MESSAGES.map do |message|
      { "logid"   => type.to_s,
        "origin"  => origins.shuffle.first,
        "level"   => levels.shuffle.first,
        "message" => message
      }
    end
    split = rand( events.size - 2 ) + 1
    collection.insert( events[0...split] )
    sleep( 0.25 )
    collection.insert( events[split..-1] )
    events
  end

  def processed_events( type )
    db.collection( "#{ type.to_s }_events" ).find.map( &:to_hash )
  end
  
end

World( EventsHelper )
