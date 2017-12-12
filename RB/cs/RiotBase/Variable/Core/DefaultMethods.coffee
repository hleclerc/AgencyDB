Method = require "./Method.js"

# set_o
Method.methods.self_set_o.add_surdef
    test: ( type_0 ) -> true
    func: ( a, b ) -> a.rp = b.rp
    pert: 0

#
Method.methods.equ_immediate.add_surdef
    test: ( type_0, type_1 ) -> Method.methods.to_Number.pert_surdef_for( type_0 ) >= 0 and Method.methods.to_Number.pert_surdef_for( type_1 ) >= 0
    func: ( a, b ) -> Method.methods.to_Number.call_1( a ) == Method.methods.to_Number.call_1( b )
    pert: 0

Method.methods.inf_immediate.add_surdef
    test: ( type_0, type_1 ) -> Method.methods.to_Number.pert_surdef_for( type_0 ) >= 0 and Method.methods.to_Number.pert_surdef_for( type_1 ) >= 0
    func: ( a, b ) -> Method.methods.to_Number.call_1( a ) < Method.methods.to_Number.call_1( b )
    pert: 0

Method.methods.sup_immediate.add_surdef
    test: ( type_0, type_1 ) -> Method.methods.to_Number.pert_surdef_for( type_0 ) >= 0 and Method.methods.to_Number.pert_surdef_for( type_1 ) >= 0
    func: ( a, b ) -> Method.methods.to_Number.call_1( a ) > Method.methods.to_Number.call_1( b )
    pert: 0

# xx_b -> xx_o
for n, m of Method.std_mts
    do ( n ) ->
        switch m.nb_rp_args
            when 1
                Method.methods[ "#{n}_b" ].add_surdef
                    test: ( type_0 ) -> Method.methods[ "#{n}_o" ].pert_surdef_for( type_0 ) >= 0
                    func: ( a, args... ) -> Method.methods[ "#{n}_o" ].call_1.apply Method.methods[ "#{n}_o" ], [ a.copy() ].concat args
                    pert: -1
            when 2
                Method.methods[ "#{n}_bb" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "#{n}_bo" ].pert_surdef_for( type_0, type_1 ) >= 0
                    func: ( a, b, args... ) -> Method.methods[ "#{n}_bo" ].call_2.apply Method.methods[ "#{n}_bo" ], [ a, b.copy() ].concat args
                    pert: -1.0
                Method.methods[ "#{n}_bb" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "#{n}_ob" ].pert_surdef_for( type_0, type_1 ) >= 0
                    func: ( a, b, args... ) -> Method.methods[ "#{n}_ob" ].call_2.apply Method.methods[ "#{n}_ob" ], [ a.copy(), b ].concat args
                    pert: -1.1
                Method.methods[ "#{n}_bb" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "#{n}_oo" ].pert_surdef_for( type_0, type_1 ) >= 0
                    func: ( a, b, args... ) -> Method.methods[ "#{n}_oo" ].call_2.apply Method.methods[ "#{n}_oo" ], [ a.copy(), b.copy() ].concat args
                    pert: -1.2

                Method.methods[ "#{n}_ob" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "#{n}_oo" ].pert_surdef_for( type_0, type_1 ) >= 0
                    func: ( a, b, args... ) -> Method.methods[ "#{n}_oo" ].call_2.apply Method.methods[ "#{n}_oo" ], [ a, b.copy() ].concat args
                    pert: -1.2

                Method.methods[ "#{n}_bo" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "#{n}_oo" ].pert_surdef_for( type_0, type_1 ) >= 0
                    func: ( a, b, args... ) -> Method.methods[ "#{n}_oo" ].call_2.apply Method.methods[ "#{n}_oo" ], [ a.copy(), b ].concat args
                    pert: -1.2
            when 3
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_bbo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_bbo" ].call_3.apply Method.methods[ "#{n}_bbo" ], [ a, b, c.copy() ].concat args
                    pert: -1.0
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_bob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_bob" ].call_3.apply Method.methods[ "#{n}_bob" ], [ a, b.copy(), c ].concat args
                    pert: -1.1
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_obb" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_obb" ].call_3.apply Method.methods[ "#{n}_obb" ], [ a.copy(), b, c ].concat args
                    pert: -1.2
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_boo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_boo" ].call_3.apply Method.methods[ "#{n}_boo" ], [ a, b.copy(), c.copy() ].concat args
                    pert: -1.3
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_obo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_obo" ].call_3.apply Method.methods[ "#{n}_obo" ], [ a.copy(), b, c.copy() ].concat args
                    pert: -1.4
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_oob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_oob" ].call_3.apply Method.methods[ "#{n}_oob" ], [ a.copy(), b.copy(), c ].concat args
                    pert: -1.5
                Method.methods[ "#{n}_bbb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a.copy(), b.copy(), c.copy() ].concat args
                    pert: -1.5

                Method.methods[ "#{n}_bbo" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_boo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_boo" ].call_3.apply Method.methods[ "#{n}_boo" ], [ a, b.copy(), c ].concat args
                    pert: -1.3
                Method.methods[ "#{n}_bbo" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_obo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_obo" ].call_3.apply Method.methods[ "#{n}_obo" ], [ a.copy(), b, c ].concat args
                    pert: -1.4
                Method.methods[ "#{n}_bbo" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a.copy(), b.copy(), c ].concat args
                    pert: -1.5

                Method.methods[ "#{n}_bob" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_boo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_boo" ].call_3.apply Method.methods[ "#{n}_boo" ], [ a, b, c.copy() ].concat args
                    pert: -1.3
                Method.methods[ "#{n}_bob" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_oob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_oob" ].call_3.apply Method.methods[ "#{n}_oob" ], [ a.copy(), b, c ].concat args
                    pert: -1.5
                Method.methods[ "#{n}_bob" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a.copy(), b, c.copy() ].concat args
                    pert: -1.5

                Method.methods[ "#{n}_obb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_obo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_obo" ].call_3.apply Method.methods[ "#{n}_obo" ], [ a, b, c.copy() ].concat args
                    pert: -1.4
                Method.methods[ "#{n}_obb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_oob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_oob" ].call_3.apply Method.methods[ "#{n}_oob" ], [ a, b.copy(), c ].concat args
                    pert: -1.5
                Method.methods[ "#{n}_obb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a, b.copy(), c.copy() ].concat args
                    pert: -1.5

                Method.methods[ "#{n}_oob" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a, b, c.copy() ].concat args
                    pert: -1.5

                Method.methods[ "#{n}_obo" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a, b.copy(), c ].concat args
                    pert: -1.5

                Method.methods[ "#{n}_boo" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_ooo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "#{n}_ooo" ].call_3.apply Method.methods[ "#{n}_ooo" ], [ a.copy(), b, c ].concat args
                    pert: -1.5

            else
                console.log "TODO", n

for n, m of Method.slf_mts
    do ( n ) ->
        switch m.nb_rp_args
            when 1
                null
            when 2
                Method.methods[ "self_#{n}_b" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "self_#{n}_o" ].pert_surdef_for( type_0, type_1 ) >= 0
                    func: ( a, args... ) -> Method.methods[ "self_#{n}_o" ].call_2.apply Method.methods[ "self_#{n}_o" ], [ a, b.copy() ].concat args
                    pert: -1
            when 3
                Method.methods[ "self_#{n}_bb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "self_#{n}_bo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "self_#{n}_bo" ].call_3.apply Method.methods[ "self_#{n}_bo" ], [ a, b, c.copy() ].concat args
                    pert: -1.0
                Method.methods[ "self_#{n}_bb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "self_#{n}_ob" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "self_#{n}_ob" ].call_3.apply Method.methods[ "self_#{n}_ob" ], [ a, b.copy(), c ].concat args
                    pert: -1.1
                Method.methods[ "self_#{n}_bb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "self_#{n}_oo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "self_#{n}_oo" ].call_3.apply Method.methods[ "self_#{n}_oo" ], [ a, b.copy(), c.copy() ].concat args
                    pert: -1.2

                Method.methods[ "self_#{n}_ob" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "self_#{n}_oo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "self_#{n}_oo" ].call_3.apply Method.methods[ "self_#{n}_oo" ], [ a, b, c.copy() ].concat args
                    pert: -1.2

                Method.methods[ "self_#{n}_bo" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "self_#{n}_oo" ].pert_surdef_for( type_0, type_1, type_2 ) >= 0
                    func: ( a, b, c, args... ) -> Method.methods[ "self_#{n}_oo" ].call_3.apply Method.methods[ "self_#{n}_oo" ], [ a, b.copy(), c ].concat args
                    pert: -1.2

            else
                console.log "TODO", n

# self_xx a, ... -> set a, xx a, ...
for n, m of Method.slf_mts
    do ( n ) ->
        switch m.nb_rp_args
            when 1
                Method.methods[ "self_#{n}" ].add_surdef
                    test: ( type_0 ) -> Method.methods[ "#{n}_b" ].has_surdef_for type_0
                    func: ( args... ) -> Method.methods.self_set_o.call_1 args[ 0 ], Method.methods[ "#{n}_b" ].call_1.apply Method.methods[ "#{n}_b" ], args
                    pert: 0
            when 2
                Method.methods[ "self_#{n}_b" ].add_surdef
                    test: ( type_0, type_1 ) -> Method.methods[ "#{n}_bb" ].has_surdef_for type_0, type_1
                    func: ( args... ) -> Method.methods.self_set_o.call_1 args[ 0 ], Method.methods[ "#{n}_bb" ].call_2.apply Method.methods[ "#{n}_bb" ], args
                    pert: 0
            when 3
                Method.methods[ "self_#{n}_bb" ].add_surdef
                    test: ( type_0, type_1, type_2 ) -> Method.methods[ "#{n}_bbb" ].has_surdef_for type_0, type_1, type_2
                    func: ( args... ) -> Method.methods.self_set_o.call_1 args[ 0 ], Method.methods[ "#{n}_bbb" ].call_3.apply Method.methods[ "#{n}_bbb" ], args
                    pert: 0
            else
                console.log "TODO", n
