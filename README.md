AgencyDB is a peer to peer database, where one stores operations (in a compressed way) rather than states.

Merges (for operations made in parallel) are handled using operational transform.

For every kinds of client type, AgencyDB tries to be as **idiomatic** and **transparent** as possible. Queries are constructed using lazy evaluation with types supporting the usual methods one can find in the target languages. It enables notably to handle asynchrony, out-of-core data and complex queries using usual (synchronous) language construct.

For instance 

```javascript
// create a new db instance (asynchronous)
let db = new Db( "my_tracker.io" );
db.connect( "bob", "pass" );

// bind local OR remote variables
let ar = db.bind( "~/my_array", Array );
let re = db.bind( "~/my_sum", Number );

// query is executed taking "data gravity" into account
re.set( ar.reduction( x => x * x ) );
```
