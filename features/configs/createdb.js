/*
 * Get and flush the database.
 */
var db = connect( "cpplscuke" );
var coll;
db.runCommand( { fsync: 1 } );

/*
 * Create the cuke events namespace.
 */
coll = db["lsevents"];
if ( typeof db["lsevents"] !== "undefined" )
  db["lsevents"].drop();
db.createCollection( "lsevents", { capped: true, size: 1048576 } );

/*
 * Create the simple events namespace.
 */
if ( typeof db["simple_events"] !== "undefined" )
  db["simple_events"].drop();
db.createCollection( "simple_events" );
db["simple_events"].ensureIndex( { origin: 1 } );

/*
 * Create the time events namespace.
 */
if ( typeof db["time_events"] !== "undefined" )
  db["time_events"].drop();
db.createCollection( "time_events" );
db["time_events"].ensureIndex( { origin: 1 } );
db["time_events"].ensureIndex( { time: 1 }, { expireAfterSeconds: 86400 } );

/*
 * Flush the database.
 */
db.runCommand( { fsync: 1 } );
