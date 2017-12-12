#pragma once

#include "../../System/Rcast.h"
#include "../Core/RpAndUse.h"
#include "../Core/RpTempl.h"

namespace RiotBase {

///
class GenString : public RpTempl<GenString> {
public:
    enum { term_value = true, prereservation_size = 8 };

    GenString( const char *str, PT size, PT rese );
    GenString( const char *str, PT size );
    GenString( const char *str );
    GenString( PT size, PT rese );

    static PT               additionnal_size_for( const char *str, PT size, PT rese ) { return rese; }
    static PT               additionnal_size_for( const char *str, PT size ) { return size; }
    static PT               additionnal_size_for( const char *str ) { return strlen( str ); }
    static PT               additionnal_size_for( PT size, PT rese ) { return rese; }
    PT                      additionnal_size    () { return rese; }
    void                    take_rese           ( PT val ) { rese = val; }

    static std::string      type_name           ();
    static const Category  *static_category     ();
    const Category         *category            () const;
    void                    write_to_stream     ( std::ostream & ) const;
    DaSi                    dasi                () const;
    bool                    apply_DaSi          ( const std::function<void(DaSi)> &f ) const;
    bool                    to_std_string       ( std::string *str, PT nb_items ) const;
    Rp                     *get_size            () const;
    PT                      get_size_PT         () const;
    Rp                     *copy                () const;
    Rp                     *make_ot_wrapper     ();
    void                    serialize           ( BBQ bq, const PT *num_children ) const;
    static Rp              *deserialize         ( BinStream<CbStringPtr> bq, Vec<RpAndUse> &children );
    void                    get_lazy_source_info( LazySourceInfo &info, const LazySourceInfo **children_info ) const;

    const char             *data                () const { return rcast( this + 1 ); }
    char                   *data                ()       { return rcast( this + 1 ); }

    //
    static void             reserve             ( GenString *&a, PT new_size );
    static void             resize              ( GenString *&a, PT new_size );
    static void             self_add            ( GenString *&a, const GenString *b );
    static void             self_insert         ( GenString *&a, PT pos, DaSi s );
    static void             self_remove         ( GenString *&a, PT pos, PT len );


    PT                      size;
    PT                      rese;
};

} // namespace RiotBase
