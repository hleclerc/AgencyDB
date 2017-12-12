#pragma once

namespace RiotBase {

/// Carac must contain { using Node = ...; static Node *prev( Node * ) { ... } static Node *next( Node * ) { ... } }
template<class Carac,bool owning=true>
class IntrusiveSet {
public:
    using Node = typename Carac::Node;

    void insert( Node *n ) {

    }
};


} // namespace RiotBase
