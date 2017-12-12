AgencyDB is a peer to peer reactive database, where one stores operations (compressed) rather than states.

Merges (for operations made in parallel) are handled using operational transform. One of the main goal of AgencyDB is to provide a framework to describe types (content, handling of scheme evolution), operations and relations between them in a clear and language neutral way.

For every kinds of client type, AgencyDB tries to be as **idiomatic** and **transparent** as possible. Queries are constructed using lazy evaluation with types supporting the usual methods one can find in the target languages. It enables notably to handle asynchrony, out-of-core data and complex queries using usual (synchronous) language construct.

For instance 

```javascript
// create a new db instance (asynchronous)
let db = new Db( "my_tracker.io" );
db.connect( "bob", "pass" );

// bind local or remote variables to javascript variables (asynchronous)
let ar = db.bind( "~/my_array", Array );
let re = db.bind( "~/my_sum", Number );

// query is executed taking "data gravity" into account
// the lambda function is executed symbolically so that 
// the query can be sent and executed elsewhere.
// Of course, the call is asynchronous (re will be a
// pointer to a graph node).
ar.forEach( x => re += x ) );
```

APIs may change a lot in the future (near or not, depending on time available to work on the project). AgencyDB can be seen as an experimental tool to think about implementations of operational transform and some possible features of future databases.