/*
 * Get and flush the database.
 */
var db = connect( "folksylog" );
var coll;
db.runCommand( { fsync: 1 } );

/*
 * Create the folksy events namespace.
 */
coll = db["lsevents"];
if ( typeof db["lsevents"] !== "undefined" )
  db["lsevents"].drop();
db.createCollection( "lsevents", { capped: true, size: 1048576 } );

/*
 * Create the folksylog events namespace.
 */
if ( typeof db["events"] !== "undefined" )
  db["events"].drop();
db.createCollection( "events" );
db["events"].ensureIndex( { origin: 1 } );
db["events"].ensureIndex( { time: 1 }, { expireAfterSeconds: 604800 } );

/*
 * Create the folksylog errors namespace.
 */
if ( typeof db["errors"] !== "undefined" )
  db["errors"].drop();
db.createCollection( "errors" );
db["errors"].ensureIndex( { origin: 1 } );
db["errors"].ensureIndex( { time: 1 }, { expireAfterSeconds: 604800 } );

/*
 * Flush the database.
 */
db.runCommand( { fsync: 1 } );
