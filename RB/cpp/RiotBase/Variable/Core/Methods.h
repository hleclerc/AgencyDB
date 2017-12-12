/// This file is generated (by bin/Method_template.py). Do not modify it directly.

#pragma once

#include "../../System/BitFieldExtSize.h"
#include "../../System/ArgType.h"
#include "../../System/Inode.h"
#include "../../System/GrpId.h"
#include "../../System/UsrId.h"
#include "../../System/DaSi.h"
#include "../../System/PVec.h"
#include "CursorData.h"
#include "Rp.h"

namespace RiotBase {
template<class FuncName> struct Surdef_1;
template<class FuncName> struct Surdef_2;
template<class FuncName> struct Surdef_3;
class CommonSymbolicData; // for DeclMethods
class LazySourceInfo;
class OtMessage;
class OtMerger;
class Category;
class Callback;
class Db;

using PairRpPtr = std::pair<Rp *,Rp *>;

extern int sh_instanciated_types; ///< Type::rs_instanciated_types() = pow( 2, sh_instanciated_types )
extern Rp *indestructible_fake;

#define RO_CALL( FUNCNAME, ... ) RiotBase::RF_##FUNCNAME::call( __VA_ARGS__ )

template<class T> struct AutoIndestructibleFakeIfNotRef { ~AutoIndestructibleFakeIfNotRef() { v.rp = 0; } T &v; };
template<class T> struct AutoIndestructibleFakeIfNotRef<T &> { ~AutoIndestructibleFakeIfNotRef() {} T &v; };
template<class T> typename T::__FaKeImpossibleType as_a_Variable( T &a ) { return a; }

struct RF_del {
    using TF = void( Rp * ); enum { nb_args = 1 }; static const char *name() { return "del"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_del> *last_surdef;
};
struct RF_copy {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "copy"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_copy> *last_surdef;
};
struct RF_write_to_stream {
    using TF = void( const Rp *, std::ostream & ); enum { nb_args = 1 }; static const char *name() { return "write_to_stream"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, std::ostream &os ) { return get_f( v0 )( v0, os ); }
    static Surdef_1<RF_write_to_stream> *last_surdef;
};
struct RF_write_structure {
    using TF = void( const Rp *, std::ostream & ); enum { nb_args = 1 }; static const char *name() { return "write_structure"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, std::ostream &os ) { return get_f( v0 )( v0, os ); }
    static Surdef_1<RF_write_structure> *last_surdef;
};
struct RF_write_graphviz {
    using TF = void( const Rp *, std::ostream & ); enum { nb_args = 1 }; static const char *name() { return "write_graphviz"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, std::ostream &os ) { return get_f( v0 )( v0, os ); }
    static Surdef_1<RF_write_graphviz> *last_surdef;
};
struct RF_category {
    using TF = const Category *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "category"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static const Category *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_category> *last_surdef;
};
struct RF_to_std_string {
    using TF = bool( const Rp *, std::string *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_std_string"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, std::string *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_std_string> *last_surdef;
};
struct RF_to_Inode {
    using TF = bool( const Rp *, Inode *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_Inode"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, Inode *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_Inode> *last_surdef;
};
struct RF_to_SI64 {
    using TF = bool( const Rp *, SI64 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_SI64"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, SI64 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_SI64> *last_surdef;
};
struct RF_to_PI64 {
    using TF = bool( const Rp *, PI64 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_PI64"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, PI64 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_PI64> *last_surdef;
};
struct RF_to_SI32 {
    using TF = bool( const Rp *, SI32 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_SI32"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, SI32 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_SI32> *last_surdef;
};
struct RF_to_PI32 {
    using TF = bool( const Rp *, PI32 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_PI32"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, PI32 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_PI32> *last_surdef;
};
struct RF_to_SI16 {
    using TF = bool( const Rp *, SI16 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_SI16"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, SI16 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_SI16> *last_surdef;
};
struct RF_to_PI16 {
    using TF = bool( const Rp *, PI16 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_PI16"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, PI16 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_PI16> *last_surdef;
};
struct RF_to_SI8 {
    using TF = bool( const Rp *, SI8 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_SI8"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, SI8 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_SI8> *last_surdef;
};
struct RF_to_PI8 {
    using TF = bool( const Rp *, PI8 *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_PI8"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, PI8 *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_PI8> *last_surdef;
};
struct RF_to_Bool {
    using TF = bool( const Rp *, Bool *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_Bool"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, Bool *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_Bool> *last_surdef;
};
struct RF_to_PT {
    using TF = bool( const Rp *, PT *, PT ); enum { nb_args = 1 }; static const char *name() { return "to_PT"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, PT *res, PT nb_items ) { return get_f( v0 )( v0, res, nb_items ); }
    static Surdef_1<RF_to_PT> *last_surdef;
};
struct RF_dasi {
    using TF = DaSi( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "dasi"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static DaSi call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_dasi> *last_surdef;
};
struct RF_apply_DaSi {
    using TF = bool( const Rp *, const std::function<void(DaSi)> & ); enum { nb_args = 1 }; static const char *name() { return "apply_DaSi"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, const std::function<void(DaSi)> &f ) { return get_f( v0 )( v0, f ); }
    static Surdef_1<RF_apply_DaSi> *last_surdef;
};
struct RF_self_set_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_set_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_set_o> *last_surdef;
};
struct RF_self_set_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_set_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_set_b> *last_surdef;
};
struct RF_set_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "set_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_set_so> *last_surdef;
};
struct RF_set_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "set_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_set_so_extr> *last_surdef;
};
struct RF_set_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "set_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_set_sb> *last_surdef;
};
struct RF_set_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "set_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_set_sb_extr> *last_surdef;
};
struct RF_set_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "set_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_set_oo> *last_surdef;
};
struct RF_set_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "set_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_set_ob> *last_surdef;
};
struct RF_set_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "set_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_set_bo> *last_surdef;
};
struct RF_set_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "set_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_set_bb> *last_surdef;
};
struct RF_self_add_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_add_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_add_o> *last_surdef;
};
struct RF_self_add_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_add_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_add_b> *last_surdef;
};
struct RF_add_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_so> *last_surdef;
};
struct RF_add_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "add_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_add_so_extr> *last_surdef;
};
struct RF_add_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_sb> *last_surdef;
};
struct RF_add_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "add_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_add_sb_extr> *last_surdef;
};
struct RF_add_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_oo> *last_surdef;
};
struct RF_add_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_ob> *last_surdef;
};
struct RF_add_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_bo> *last_surdef;
};
struct RF_add_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_bb> *last_surdef;
};
struct RF_self_sub_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_sub_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_sub_o> *last_surdef;
};
struct RF_self_sub_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_sub_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_sub_b> *last_surdef;
};
struct RF_sub_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sub_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sub_so> *last_surdef;
};
struct RF_sub_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "sub_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_sub_so_extr> *last_surdef;
};
struct RF_sub_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sub_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sub_sb> *last_surdef;
};
struct RF_sub_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "sub_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_sub_sb_extr> *last_surdef;
};
struct RF_sub_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sub_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sub_oo> *last_surdef;
};
struct RF_sub_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sub_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sub_ob> *last_surdef;
};
struct RF_sub_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sub_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sub_bo> *last_surdef;
};
struct RF_sub_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sub_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sub_bb> *last_surdef;
};
struct RF_self_mul_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_mul_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_mul_o> *last_surdef;
};
struct RF_self_mul_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_mul_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_mul_b> *last_surdef;
};
struct RF_mul_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "mul_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mul_so> *last_surdef;
};
struct RF_mul_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "mul_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_mul_so_extr> *last_surdef;
};
struct RF_mul_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mul_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mul_sb> *last_surdef;
};
struct RF_mul_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "mul_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_mul_sb_extr> *last_surdef;
};
struct RF_mul_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "mul_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mul_oo> *last_surdef;
};
struct RF_mul_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mul_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mul_ob> *last_surdef;
};
struct RF_mul_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "mul_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mul_bo> *last_surdef;
};
struct RF_mul_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mul_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mul_bb> *last_surdef;
};
struct RF_self_div_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_div_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_div_o> *last_surdef;
};
struct RF_self_div_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_div_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_div_b> *last_surdef;
};
struct RF_div_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "div_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_div_so> *last_surdef;
};
struct RF_div_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "div_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_div_so_extr> *last_surdef;
};
struct RF_div_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "div_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_div_sb> *last_surdef;
};
struct RF_div_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "div_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_div_sb_extr> *last_surdef;
};
struct RF_div_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "div_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_div_oo> *last_surdef;
};
struct RF_div_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "div_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_div_ob> *last_surdef;
};
struct RF_div_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "div_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_div_bo> *last_surdef;
};
struct RF_div_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "div_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_div_bb> *last_surdef;
};
struct RF_self_mod_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_mod_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_mod_o> *last_surdef;
};
struct RF_self_mod_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_mod_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_mod_b> *last_surdef;
};
struct RF_mod_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_so> *last_surdef;
};
struct RF_mod_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "mod_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_mod_so_extr> *last_surdef;
};
struct RF_mod_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_sb> *last_surdef;
};
struct RF_mod_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "mod_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_mod_sb_extr> *last_surdef;
};
struct RF_mod_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_oo> *last_surdef;
};
struct RF_mod_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_ob> *last_surdef;
};
struct RF_mod_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_bo> *last_surdef;
};
struct RF_mod_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_bb> *last_surdef;
};
struct RF_self_add_key_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_add_key_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_add_key_o> *last_surdef;
};
struct RF_self_add_key_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_add_key_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_add_key_b> *last_surdef;
};
struct RF_add_key_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_key_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_key_so> *last_surdef;
};
struct RF_add_key_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "add_key_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_add_key_so_extr> *last_surdef;
};
struct RF_add_key_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_key_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_key_sb> *last_surdef;
};
struct RF_add_key_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "add_key_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_add_key_sb_extr> *last_surdef;
};
struct RF_add_key_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_key_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_key_oo> *last_surdef;
};
struct RF_add_key_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_key_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_key_ob> *last_surdef;
};
struct RF_add_key_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_key_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_key_bo> *last_surdef;
};
struct RF_add_key_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "add_key_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_add_key_bb> *last_surdef;
};
struct RF_self_rem_key_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_rem_key_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_rem_key_o> *last_surdef;
};
struct RF_self_rem_key_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_rem_key_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_rem_key_b> *last_surdef;
};
struct RF_rem_key_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "rem_key_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_rem_key_so> *last_surdef;
};
struct RF_rem_key_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "rem_key_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_rem_key_so_extr> *last_surdef;
};
struct RF_rem_key_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "rem_key_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_rem_key_sb> *last_surdef;
};
struct RF_rem_key_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "rem_key_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_rem_key_sb_extr> *last_surdef;
};
struct RF_rem_key_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "rem_key_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_rem_key_oo> *last_surdef;
};
struct RF_rem_key_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "rem_key_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_rem_key_ob> *last_surdef;
};
struct RF_rem_key_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "rem_key_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_rem_key_bo> *last_surdef;
};
struct RF_rem_key_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "rem_key_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_rem_key_bb> *last_surdef;
};
struct RF_self_push_back_o {
    using TF = void( Rp *&, Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_push_back_o"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_push_back_o> *last_surdef;
};
struct RF_self_push_back_b {
    using TF = void( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "self_push_back_b"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_self_push_back_b> *last_surdef;
};
struct RF_push_back_so {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "push_back_so"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_push_back_so> *last_surdef;
};
struct RF_push_back_so_extr {
    using TF = Rp *( Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "push_back_so_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_push_back_so_extr> *last_surdef;
};
struct RF_push_back_sb {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "push_back_sb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_push_back_sb> *last_surdef;
};
struct RF_push_back_sb_extr {
    using TF = Rp *( Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 2 }; static const char *name() { return "push_back_sb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1, const Vec<Rp *> &extr ) { return get_f( v0, v1 )( v0, v1, extr ); }
    static Surdef_2<RF_push_back_sb_extr> *last_surdef;
};
struct RF_push_back_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "push_back_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_push_back_oo> *last_surdef;
};
struct RF_push_back_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "push_back_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_push_back_ob> *last_surdef;
};
struct RF_push_back_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "push_back_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_push_back_bo> *last_surdef;
};
struct RF_push_back_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "push_back_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_push_back_bb> *last_surdef;
};
struct RF_self_insert_oo {
    using TF = void( Rp *&, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_insert_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_insert_oo> *last_surdef;
};
struct RF_self_insert_ob {
    using TF = void( Rp *&, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_insert_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_insert_ob> *last_surdef;
};
struct RF_self_insert_bo {
    using TF = void( Rp *&, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_insert_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_insert_bo> *last_surdef;
};
struct RF_self_insert_bb {
    using TF = void( Rp *&, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_insert_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_insert_bb> *last_surdef;
};
struct RF_insert_soo {
    using TF = Rp *( Rp *, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_soo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_soo> *last_surdef;
};
struct RF_insert_soo_extr {
    using TF = Rp *( Rp *, Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "insert_soo_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_insert_soo_extr> *last_surdef;
};
struct RF_insert_sob {
    using TF = Rp *( Rp *, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_sob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_sob> *last_surdef;
};
struct RF_insert_sob_extr {
    using TF = Rp *( Rp *, Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "insert_sob_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, const Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_insert_sob_extr> *last_surdef;
};
struct RF_insert_sbo {
    using TF = Rp *( Rp *, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_sbo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_sbo> *last_surdef;
};
struct RF_insert_sbo_extr {
    using TF = Rp *( Rp *, const Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "insert_sbo_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_insert_sbo_extr> *last_surdef;
};
struct RF_insert_sbb {
    using TF = Rp *( Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_sbb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_sbb> *last_surdef;
};
struct RF_insert_sbb_extr {
    using TF = Rp *( Rp *, const Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "insert_sbb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, const Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_insert_sbb_extr> *last_surdef;
};
struct RF_insert_ooo {
    using TF = Rp *( Rp *, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_ooo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_ooo> *last_surdef;
};
struct RF_insert_oob {
    using TF = Rp *( Rp *, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_oob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_oob> *last_surdef;
};
struct RF_insert_obo {
    using TF = Rp *( Rp *, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_obo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_obo> *last_surdef;
};
struct RF_insert_obb {
    using TF = Rp *( Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_obb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_obb> *last_surdef;
};
struct RF_insert_boo {
    using TF = Rp *( const Rp *, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_boo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_boo> *last_surdef;
};
struct RF_insert_bob {
    using TF = Rp *( const Rp *, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_bob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_bob> *last_surdef;
};
struct RF_insert_bbo {
    using TF = Rp *( const Rp *, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_bbo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_bbo> *last_surdef;
};
struct RF_insert_bbb {
    using TF = Rp *( const Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "insert_bbb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_insert_bbb> *last_surdef;
};
struct RF_self_remove_oo {
    using TF = void( Rp *&, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_remove_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_remove_oo> *last_surdef;
};
struct RF_self_remove_ob {
    using TF = void( Rp *&, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_remove_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_remove_ob> *last_surdef;
};
struct RF_self_remove_bo {
    using TF = void( Rp *&, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_remove_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_remove_bo> *last_surdef;
};
struct RF_self_remove_bb {
    using TF = void( Rp *&, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "self_remove_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static void call( Rp *&v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_self_remove_bb> *last_surdef;
};
struct RF_remove_soo {
    using TF = Rp *( Rp *, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_soo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_soo> *last_surdef;
};
struct RF_remove_soo_extr {
    using TF = Rp *( Rp *, Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "remove_soo_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_remove_soo_extr> *last_surdef;
};
struct RF_remove_sob {
    using TF = Rp *( Rp *, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_sob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_sob> *last_surdef;
};
struct RF_remove_sob_extr {
    using TF = Rp *( Rp *, Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "remove_sob_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, const Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_remove_sob_extr> *last_surdef;
};
struct RF_remove_sbo {
    using TF = Rp *( Rp *, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_sbo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_sbo> *last_surdef;
};
struct RF_remove_sbo_extr {
    using TF = Rp *( Rp *, const Rp *, Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "remove_sbo_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_remove_sbo_extr> *last_surdef;
};
struct RF_remove_sbb {
    using TF = Rp *( Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_sbb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_sbb> *last_surdef;
};
struct RF_remove_sbb_extr {
    using TF = Rp *( Rp *, const Rp *, const Rp *, const Vec<Rp *> & ); enum { nb_args = 3 }; static const char *name() { return "remove_sbb_extr"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, const Rp *v2, const Vec<Rp *> &extr ) { return get_f( v0, v1, v2 )( v0, v1, v2, extr ); }
    static Surdef_3<RF_remove_sbb_extr> *last_surdef;
};
struct RF_remove_ooo {
    using TF = Rp *( Rp *, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_ooo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_ooo> *last_surdef;
};
struct RF_remove_oob {
    using TF = Rp *( Rp *, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_oob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_oob> *last_surdef;
};
struct RF_remove_obo {
    using TF = Rp *( Rp *, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_obo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_obo> *last_surdef;
};
struct RF_remove_obb {
    using TF = Rp *( Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_obb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_obb> *last_surdef;
};
struct RF_remove_boo {
    using TF = Rp *( const Rp *, Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_boo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_boo> *last_surdef;
};
struct RF_remove_bob {
    using TF = Rp *( const Rp *, Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_bob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_bob> *last_surdef;
};
struct RF_remove_bbo {
    using TF = Rp *( const Rp *, const Rp *, Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_bbo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, const Rp *v1, Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_bbo> *last_surdef;
};
struct RF_remove_bbb {
    using TF = Rp *( const Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "remove_bbb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_remove_bbb> *last_surdef;
};
struct RF_equ_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "equ_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_equ_oo> *last_surdef;
};
struct RF_equ_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "equ_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_equ_ob> *last_surdef;
};
struct RF_equ_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "equ_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_equ_bo> *last_surdef;
};
struct RF_equ_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "equ_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_equ_bb> *last_surdef;
};
struct RF_neq_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "neq_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_neq_oo> *last_surdef;
};
struct RF_neq_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "neq_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_neq_ob> *last_surdef;
};
struct RF_neq_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "neq_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_neq_bo> *last_surdef;
};
struct RF_neq_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "neq_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_neq_bb> *last_surdef;
};
struct RF_inf_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_oo> *last_surdef;
};
struct RF_inf_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_ob> *last_surdef;
};
struct RF_inf_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_bo> *last_surdef;
};
struct RF_inf_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_bb> *last_surdef;
};
struct RF_sup_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_oo> *last_surdef;
};
struct RF_sup_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_ob> *last_surdef;
};
struct RF_sup_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_bo> *last_surdef;
};
struct RF_sup_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_bb> *last_surdef;
};
struct RF_inf_eq_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_eq_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_eq_oo> *last_surdef;
};
struct RF_inf_eq_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_eq_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_eq_ob> *last_surdef;
};
struct RF_inf_eq_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_eq_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_eq_bo> *last_surdef;
};
struct RF_inf_eq_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_eq_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_eq_bb> *last_surdef;
};
struct RF_sup_eq_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_eq_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_eq_oo> *last_surdef;
};
struct RF_sup_eq_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_eq_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_eq_ob> *last_surdef;
};
struct RF_sup_eq_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_eq_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_eq_bo> *last_surdef;
};
struct RF_sup_eq_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "sup_eq_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_sup_eq_bb> *last_surdef;
};
struct RF_bor_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "bor_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_bor_oo> *last_surdef;
};
struct RF_bor_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "bor_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_bor_ob> *last_surdef;
};
struct RF_bor_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "bor_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_bor_bo> *last_surdef;
};
struct RF_bor_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "bor_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_bor_bb> *last_surdef;
};
struct RF_select_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "select_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_select_oo> *last_surdef;
};
struct RF_select_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "select_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_select_ob> *last_surdef;
};
struct RF_select_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "select_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_select_bo> *last_surdef;
};
struct RF_select_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "select_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_select_bb> *last_surdef;
};
struct RF_neg_o {
    using TF = Rp *( Rp * ); enum { nb_args = 1 }; static const char *name() { return "neg_o"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_neg_o> *last_surdef;
};
struct RF_neg_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "neg_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_neg_b> *last_surdef;
};
struct RF_make_ot_wrapper {
    using TF = Rp *( Rp * ); enum { nb_args = 1 }; static const char *name() { return "make_ot_wrapper"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_make_ot_wrapper> *last_surdef;
};
struct RF_pull_ot_changes {
    using TF = void( Rp *&, BBQ ); enum { nb_args = 1 }; static const char *name() { return "pull_ot_changes"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, BBQ changes ) { return get_f( v0 )( v0, changes ); }
    static Surdef_1<RF_pull_ot_changes> *last_surdef;
};
struct RF_write_ot_data {
    using TF = void( Rp *&, std::ostream &, CbStringPtr ); enum { nb_args = 1 }; static const char *name() { return "write_ot_data"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, std::ostream &os, CbStringPtr data ) { return get_f( v0 )( v0, os, data ); }
    static Surdef_1<RF_write_ot_data> *last_surdef;
};
struct RF_new_ot_merger {
    using TF = OtMerger *( Rp *& ); enum { nb_args = 1 }; static const char *name() { return "new_ot_merger"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static OtMerger *call( Rp *&v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_new_ot_merger> *last_surdef;
};
struct RF_new_cursor {
    using TF = Rp *( Rp *&, void * ); enum { nb_args = 1 }; static const char *name() { return "new_cursor"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *&v0, void *int_id ) { return get_f( v0 )( v0, int_id ); }
    static Surdef_1<RF_new_cursor> *last_surdef;
};
struct RF_rem_cursor {
    using TF = void( Rp *, void * ); enum { nb_args = 1 }; static const char *name() { return "rem_cursor"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *v0, void *int_id ) { return get_f( v0 )( v0, int_id ); }
    static Surdef_1<RF_rem_cursor> *last_surdef;
};
struct RF_get_cursors {
    using TF = void( const Rp *, Vec<CursorData> & ); enum { nb_args = 1 }; static const char *name() { return "get_cursors"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, Vec<CursorData> &res ) { return get_f( v0 )( v0, res ); }
    static Surdef_1<RF_get_cursors> *last_surdef;
};
struct RF_get_possible_rights {
    using TF = Vec<std::string>( Rp *& ); enum { nb_args = 1 }; static const char *name() { return "get_possible_rights"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Vec<std::string> call( Rp *&v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_get_possible_rights> *last_surdef;
};
struct RF_get_groups_in_acl {
    using TF = Vec<GrpId>( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "get_groups_in_acl"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Vec<GrpId> call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_get_groups_in_acl> *last_surdef;
};
struct RF_get_users_in_acl {
    using TF = Vec<UsrId>( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "get_users_in_acl"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Vec<UsrId> call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_get_users_in_acl> *last_surdef;
};
struct RF_get_grp_rights {
    using TF = Vec<std::string>( const Rp *, GrpId ); enum { nb_args = 1 }; static const char *name() { return "get_grp_rights"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Vec<std::string> call( const Rp *v0, GrpId grp ) { return get_f( v0 )( v0, grp ); }
    static Surdef_1<RF_get_grp_rights> *last_surdef;
};
struct RF_get_usr_rights {
    using TF = Vec<std::string>( const Rp *, UsrId ); enum { nb_args = 1 }; static const char *name() { return "get_usr_rights"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Vec<std::string> call( const Rp *v0, UsrId usr ) { return get_f( v0 )( v0, usr ); }
    static Surdef_1<RF_get_usr_rights> *last_surdef;
};
struct RF_add_grp_right {
    using TF = bool( Rp *&, GrpId, const std::string & ); enum { nb_args = 1 }; static const char *name() { return "add_grp_right"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( Rp *&v0, GrpId grp, const std::string &right ) { return get_f( v0 )( v0, grp, right ); }
    static Surdef_1<RF_add_grp_right> *last_surdef;
};
struct RF_add_usr_right {
    using TF = bool( Rp *&, UsrId, const std::string & ); enum { nb_args = 1 }; static const char *name() { return "add_usr_right"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( Rp *&v0, UsrId usr, const std::string &right ) { return get_f( v0 )( v0, usr, right ); }
    static Surdef_1<RF_add_usr_right> *last_surdef;
};
struct RF_rem_grp_right {
    using TF = bool( Rp *&, GrpId, const std::string & ); enum { nb_args = 1 }; static const char *name() { return "rem_grp_right"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( Rp *&v0, GrpId grp, const std::string &right ) { return get_f( v0 )( v0, grp, right ); }
    static Surdef_1<RF_rem_grp_right> *last_surdef;
};
struct RF_rem_usr_right {
    using TF = bool( Rp *&, UsrId, const std::string & ); enum { nb_args = 1 }; static const char *name() { return "rem_usr_right"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( Rp *&v0, UsrId usr, const std::string &right ) { return get_f( v0 )( v0, usr, right ); }
    static Surdef_1<RF_rem_usr_right> *last_surdef;
};
struct RF_self_on_change {
    using TF = void( Rp *&, Callback * ); enum { nb_args = 1 }; static const char *name() { return "self_on_change"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_self_on_change> *last_surdef;
};
struct RF_on_change_s {
    using TF = Rp *( Rp *, Callback * ); enum { nb_args = 1 }; static const char *name() { return "on_change_s"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_on_change_s> *last_surdef;
};
struct RF_on_change_s_extr {
    using TF = Rp *( Rp *, Callback *, const Vec<Rp *> & ); enum { nb_args = 1 }; static const char *name() { return "on_change_s_extr"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Callback *c, const Vec<Rp *> &extr ) { return get_f( v0 )( v0, c, extr ); }
    static Surdef_1<RF_on_change_s_extr> *last_surdef;
};
struct RF_on_change_o {
    using TF = Rp *( Rp *, Callback * ); enum { nb_args = 1 }; static const char *name() { return "on_change_o"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_on_change_o> *last_surdef;
};
struct RF_on_change_b {
    using TF = Rp *( const Rp *, Callback * ); enum { nb_args = 1 }; static const char *name() { return "on_change_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_on_change_b> *last_surdef;
};
struct RF_self_then {
    using TF = void( Rp *&, Callback * ); enum { nb_args = 1 }; static const char *name() { return "self_then"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_self_then> *last_surdef;
};
struct RF_then_s {
    using TF = Rp *( Rp *, Callback * ); enum { nb_args = 1 }; static const char *name() { return "then_s"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_then_s> *last_surdef;
};
struct RF_then_s_extr {
    using TF = Rp *( Rp *, Callback *, const Vec<Rp *> & ); enum { nb_args = 1 }; static const char *name() { return "then_s_extr"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Callback *c, const Vec<Rp *> &extr ) { return get_f( v0 )( v0, c, extr ); }
    static Surdef_1<RF_then_s_extr> *last_surdef;
};
struct RF_then_o {
    using TF = Rp *( Rp *, Callback * ); enum { nb_args = 1 }; static const char *name() { return "then_o"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_then_o> *last_surdef;
};
struct RF_then_b {
    using TF = Rp *( const Rp *, Callback * ); enum { nb_args = 1 }; static const char *name() { return "then_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Callback *c ) { return get_f( v0 )( v0, c ); }
    static Surdef_1<RF_then_b> *last_surdef;
};
struct RF_sig_change {
    using TF = void( Rp * ); enum { nb_args = 1 }; static const char *name() { return "sig_change"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_sig_change> *last_surdef;
};
struct RF_common_symbolic_data {
    using TF = CommonSymbolicData *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "common_symbolic_data"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static CommonSymbolicData *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_common_symbolic_data> *last_surdef;
};
struct RF_nb_symbolic_children {
    using TF = int( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "nb_symbolic_children"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static int call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_nb_symbolic_children> *last_surdef;
};
struct RF_symbolic_child {
    using TF = const Rp *( const Rp *, int ); enum { nb_args = 1 }; static const char *name() { return "symbolic_child"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static const Rp *call( const Rp *v0, int n ) { return get_f( v0 )( v0, n ); }
    static Surdef_1<RF_symbolic_child> *last_surdef;
};
struct RF_is_symbolic {
    using TF = bool( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "is_symbolic"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_is_symbolic> *last_surdef;
};
struct RF_is_a_s_op {
    using TF = bool( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "is_a_s_op"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_is_a_s_op> *last_surdef;
};
struct RF_exec {
    using TF = Rp *( const Rp *, BitFieldExtSize, Rp ** ); enum { nb_args = 1 }; static const char *name() { return "exec"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, BitFieldExtSize own, Rp **ch ) { return get_f( v0 )( v0, own, ch ); }
    static Surdef_1<RF_exec> *last_surdef;
};
struct RF_launch {
    using TF = void( Rp * ); enum { nb_args = 1 }; static const char *name() { return "launch"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_launch> *last_surdef;
};
struct RF_exec_mp {
    using TF = Rp *( Rp * ); enum { nb_args = 1 }; static const char *name() { return "exec_mp"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_exec_mp> *last_surdef;
};
struct RF_del_or_cancel_exec {
    using TF = void( Rp * ); enum { nb_args = 1 }; static const char *name() { return "del_or_cancel_exec"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_del_or_cancel_exec> *last_surdef;
};
struct RF_get_lazy_source_info {
    using TF = void( const Rp *, LazySourceInfo &, const LazySourceInfo ** ); enum { nb_args = 1 }; static const char *name() { return "get_lazy_source_info"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, LazySourceInfo &info, const LazySourceInfo **children_info ) { return get_f( v0 )( v0, info, children_info ); }
    static Surdef_1<RF_get_lazy_source_info> *last_surdef;
};
struct RF_serialize {
    using TF = void( const Rp *, BBQ, const PT * ); enum { nb_args = 1 }; static const char *name() { return "serialize"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, BBQ bq, const PT *num_children ) { return get_f( v0 )( v0, bq, num_children ); }
    static Surdef_1<RF_serialize> *last_surdef;
};
struct RF_set_child {
    using TF = void( Rp *, int, Rp *, bool ); enum { nb_args = 1 }; static const char *name() { return "set_child"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *v0, int num_child, Rp *new_child, bool del_prev ) { return get_f( v0 )( v0, num_child, new_child, del_prev ); }
    static Surdef_1<RF_set_child> *last_surdef;
};
struct RF_prox_ptr {
    using TF = Rp **( Rp * ); enum { nb_args = 1 }; static const char *name() { return "prox_ptr"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp **call( Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_prox_ptr> *last_surdef;
};
struct RF_prox_ref {
    using TF = const Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "prox_ref"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static const Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_prox_ref> *last_surdef;
};
struct RF_prox_val {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "prox_val"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_prox_val> *last_surdef;
};
struct RF_mod_select {
    using TF = Rp *( Rp *&, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "mod_select"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *&v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_mod_select> *last_surdef;
};
struct RF_get_size {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "get_size"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_get_size> *last_surdef;
};
struct RF_get_size_PT {
    using TF = PT( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "get_size_PT"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static PT call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_get_size_PT> *last_surdef;
};
struct RF_front_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "front_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_front_b> *last_surdef;
};
struct RF_back_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "back_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_back_b> *last_surdef;
};
struct RF_front_list_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "front_list_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_front_list_b> *last_surdef;
};
struct RF_back_list_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "back_list_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_back_list_b> *last_surdef;
};
struct RF_head_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "head_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_head_b> *last_surdef;
};
struct RF_tail_b {
    using TF = Rp *( const Rp * ); enum { nb_args = 1 }; static const char *name() { return "tail_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0 ) { return get_f( v0 )( v0 ); }
    static Surdef_1<RF_tail_b> *last_surdef;
};
struct RF_slice_bbb {
    using TF = Rp *( const Rp *, const Rp *, const Rp * ); enum { nb_args = 3 }; static const char *name() { return "slice_bbb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1, const Rp *v2 );
    struct Item { TF *f; PI32 t0, t1, t2; };
    static Vec<Item> *table;
    static Rp *call( const Rp *v0, const Rp *v1, const Rp *v2 ) { return get_f( v0, v1, v2 )( v0, v1, v2 ); }
    static Surdef_3<RF_slice_bbb> *last_surdef;
};
struct RF_split_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "split_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_split_oo> *last_surdef;
};
struct RF_split_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "split_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_split_ob> *last_surdef;
};
struct RF_split_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "split_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_split_bo> *last_surdef;
};
struct RF_split_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "split_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_split_bb> *last_surdef;
};
struct RF_join_oo {
    using TF = Rp *( Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "join_oo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_join_oo> *last_surdef;
};
struct RF_join_ob {
    using TF = Rp *( Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "join_ob"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_join_ob> *last_surdef;
};
struct RF_join_bo {
    using TF = Rp *( const Rp *, Rp * ); enum { nb_args = 2 }; static const char *name() { return "join_bo"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_join_bo> *last_surdef;
};
struct RF_join_bb {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "join_bb"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_join_bb> *last_surdef;
};
struct RF_apply {
    using TF = void( const Rp *, std::function<void(const Rp *)> ); enum { nb_args = 1 }; static const char *name() { return "apply"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( const Rp *v0, std::function<void(const Rp *)> func ) { return get_f( v0 )( v0, func ); }
    static Surdef_1<RF_apply> *last_surdef;
};
struct RF_has_item_in {
    using TF = Rp *( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "has_item_in"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_has_item_in> *last_surdef;
};
struct RF_equ_immediate {
    using TF = bool( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "equ_immediate"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_equ_immediate> *last_surdef;
};
struct RF_inf_immediate {
    using TF = bool( const Rp *, const Rp * ); enum { nb_args = 2 }; static const char *name() { return "inf_immediate"; }
    static TF *&get_f( const Rp *v0, const Rp *v1 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + ( v1->type_off << sh_instanciated_types ) + v0->type_off ); }
    static TF **table;
    static bool call( const Rp *v0, const Rp *v1 ) { return get_f( v0, v1 )( v0, v1 ); }
    static Surdef_2<RF_inf_immediate> *last_surdef;
};
struct RF_bind_by_path {
    using TF = Rp *( const Rp *, const Category * ); enum { nb_args = 1 }; static const char *name() { return "bind_by_path"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_bind_by_path> *last_surdef;
};
struct RF_bind_by_object_id {
    using TF = Rp *( Rp *&, const Category * ); enum { nb_args = 1 }; static const char *name() { return "bind_by_object_id"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *&v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_bind_by_object_id> *last_surdef;
};
struct RF_bind_by_kn_object_id_o {
    using TF = Rp *( Rp *, const Category * ); enum { nb_args = 1 }; static const char *name() { return "bind_by_kn_object_id_o"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_bind_by_kn_object_id_o> *last_surdef;
};
struct RF_bind_by_kn_object_id_b {
    using TF = Rp *( const Rp *, const Category * ); enum { nb_args = 1 }; static const char *name() { return "bind_by_kn_object_id_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_bind_by_kn_object_id_b> *last_surdef;
};
struct RF_self_make_inode_if_z {
    using TF = void( Rp *&, const Category * ); enum { nb_args = 1 }; static const char *name() { return "self_make_inode_if_z"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_self_make_inode_if_z> *last_surdef;
};
struct RF_make_inode_if_z_s {
    using TF = Rp *( Rp *, const Category * ); enum { nb_args = 1 }; static const char *name() { return "make_inode_if_z_s"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_make_inode_if_z_s> *last_surdef;
};
struct RF_make_inode_if_z_s_extr {
    using TF = Rp *( Rp *, const Category *, const Vec<Rp *> & ); enum { nb_args = 1 }; static const char *name() { return "make_inode_if_z_s_extr"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Category *cat, const Vec<Rp *> &extr ) { return get_f( v0 )( v0, cat, extr ); }
    static Surdef_1<RF_make_inode_if_z_s_extr> *last_surdef;
};
struct RF_make_inode_if_z_o {
    using TF = Rp *( Rp *, const Category * ); enum { nb_args = 1 }; static const char *name() { return "make_inode_if_z_o"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_make_inode_if_z_o> *last_surdef;
};
struct RF_make_inode_if_z_b {
    using TF = Rp *( const Rp *, const Category * ); enum { nb_args = 1 }; static const char *name() { return "make_inode_if_z_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const Category *cat ) { return get_f( v0 )( v0, cat ); }
    static Surdef_1<RF_make_inode_if_z_b> *last_surdef;
};
struct RF_self_special_func {
    using TF = void( Rp *&, const char *, Rp **, int ); enum { nb_args = 1 }; static const char *name() { return "self_special_func"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static void call( Rp *&v0, const char *func, Rp **parm, int nb_parms ) { return get_f( v0 )( v0, func, parm, nb_parms ); }
    static Surdef_1<RF_self_special_func> *last_surdef;
};
struct RF_special_func_s {
    using TF = Rp *( Rp *, const char *, Rp **, int ); enum { nb_args = 1 }; static const char *name() { return "special_func_s"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const char *func, Rp **parm, int nb_parms ) { return get_f( v0 )( v0, func, parm, nb_parms ); }
    static Surdef_1<RF_special_func_s> *last_surdef;
};
struct RF_special_func_s_extr {
    using TF = Rp *( Rp *, const char *, Rp **, int, const Vec<Rp *> & ); enum { nb_args = 1 }; static const char *name() { return "special_func_s_extr"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const char *func, Rp **parm, int nb_parms, const Vec<Rp *> &extr ) { return get_f( v0 )( v0, func, parm, nb_parms, extr ); }
    static Surdef_1<RF_special_func_s_extr> *last_surdef;
};
struct RF_special_func_o {
    using TF = Rp *( Rp *, const char *, Rp **, int ); enum { nb_args = 1 }; static const char *name() { return "special_func_o"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( Rp *v0, const char *func, Rp **parm, int nb_parms ) { return get_f( v0 )( v0, func, parm, nb_parms ); }
    static Surdef_1<RF_special_func_o> *last_surdef;
};
struct RF_special_func_b {
    using TF = Rp *( const Rp *, const char *, Rp **, int ); enum { nb_args = 1 }; static const char *name() { return "special_func_b"; }
    static TF *&get_f( const Rp *v0 ) { return *reinterpret_cast<TF **>( reinterpret_cast<char *>( table ) + v0->type_off ); }
    static TF **table;
    static Rp *call( const Rp *v0, const char *func, Rp **parm, int nb_parms ) { return get_f( v0 )( v0, func, parm, nb_parms ); }
    static Surdef_1<RF_special_func_b> *last_surdef;
};

struct RF_set {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( set_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( set_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( set_bo, v0.rp, v1.rp );
        return RO_CALL( set_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_add {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( add_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( add_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( add_bo, v0.rp, v1.rp );
        return RO_CALL( add_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_sub {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( sub_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( sub_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( sub_bo, v0.rp, v1.rp );
        return RO_CALL( sub_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_mul {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( mul_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( mul_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( mul_bo, v0.rp, v1.rp );
        return RO_CALL( mul_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_div {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( div_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( div_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( div_bo, v0.rp, v1.rp );
        return RO_CALL( div_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_mod {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( mod_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( mod_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( mod_bo, v0.rp, v1.rp );
        return RO_CALL( mod_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_add_key {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( add_key_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( add_key_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( add_key_bo, v0.rp, v1.rp );
        return RO_CALL( add_key_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_rem_key {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( rem_key_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( rem_key_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( rem_key_bo, v0.rp, v1.rp );
        return RO_CALL( rem_key_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_push_back {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( push_back_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( push_back_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( push_back_bo, v0.rp, v1.rp );
        return RO_CALL( push_back_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_insert {
    template<class T0,class T1,class T2>
    static Rp *call( T0 &&v0, T1 &&v1, T2 &&v2 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        AutoIndestructibleFakeIfNotRef<T2> ai_2{ v2 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 0 ) return RO_CALL( insert_ooo, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 1 ) return RO_CALL( insert_oob, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 and std::is_reference<T2>::value == 0 ) return RO_CALL( insert_obo, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 and std::is_reference<T2>::value == 1 ) return RO_CALL( insert_obb, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 0 ) return RO_CALL( insert_boo, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 1 ) return RO_CALL( insert_bob, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 1 and std::is_reference<T2>::value == 0 ) return RO_CALL( insert_bbo, v0.rp, v1.rp, v2.rp );
        return RO_CALL( insert_bbb, v0.rp, v1.rp, v2.rp );
    }
    // template<class T0,class T1,class T2>
    // static Rp *call_gen( T0 &&v0, T1 &&v1, T2 &&v2 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ), as_a_Variable( std::forward<T2>( v2 ) ) );
    // }
};
struct RF_remove {
    template<class T0,class T1,class T2>
    static Rp *call( T0 &&v0, T1 &&v1, T2 &&v2 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        AutoIndestructibleFakeIfNotRef<T2> ai_2{ v2 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 0 ) return RO_CALL( remove_ooo, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 1 ) return RO_CALL( remove_oob, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 and std::is_reference<T2>::value == 0 ) return RO_CALL( remove_obo, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 and std::is_reference<T2>::value == 1 ) return RO_CALL( remove_obb, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 0 ) return RO_CALL( remove_boo, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 and std::is_reference<T2>::value == 1 ) return RO_CALL( remove_bob, v0.rp, v1.rp, v2.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 1 and std::is_reference<T2>::value == 0 ) return RO_CALL( remove_bbo, v0.rp, v1.rp, v2.rp );
        return RO_CALL( remove_bbb, v0.rp, v1.rp, v2.rp );
    }
    // template<class T0,class T1,class T2>
    // static Rp *call_gen( T0 &&v0, T1 &&v1, T2 &&v2 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ), as_a_Variable( std::forward<T2>( v2 ) ) );
    // }
};
struct RF_equ {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( equ_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( equ_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( equ_bo, v0.rp, v1.rp );
        return RO_CALL( equ_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_neq {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( neq_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( neq_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( neq_bo, v0.rp, v1.rp );
        return RO_CALL( neq_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_inf {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( inf_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( inf_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( inf_bo, v0.rp, v1.rp );
        return RO_CALL( inf_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_sup {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( sup_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( sup_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( sup_bo, v0.rp, v1.rp );
        return RO_CALL( sup_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_inf_eq {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( inf_eq_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( inf_eq_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( inf_eq_bo, v0.rp, v1.rp );
        return RO_CALL( inf_eq_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_sup_eq {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( sup_eq_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( sup_eq_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( sup_eq_bo, v0.rp, v1.rp );
        return RO_CALL( sup_eq_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_bor {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( bor_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( bor_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( bor_bo, v0.rp, v1.rp );
        return RO_CALL( bor_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_select {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( select_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( select_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( select_bo, v0.rp, v1.rp );
        return RO_CALL( select_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_neg {
    template<class T0>
    static Rp *call( T0 &&v0 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        if ( std::is_reference<T0>::value == 0 ) return RO_CALL( neg_o, v0.rp );
        return RO_CALL( neg_b, v0.rp );
    }
    // template<class T0>
    // static Rp *call_gen( T0 &&v0 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ) );
    // }
};
struct RF_on_change {
    template<class T0>
    static Rp *call( T0 &&v0 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        if ( std::is_reference<T0>::value == 0 ) return RO_CALL( on_change_o, v0.rp );
        return RO_CALL( on_change_b, v0.rp );
    }
    // template<class T0>
    // static Rp *call_gen( T0 &&v0 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ) );
    // }
};
struct RF_then {
    template<class T0>
    static Rp *call( T0 &&v0 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        if ( std::is_reference<T0>::value == 0 ) return RO_CALL( then_o, v0.rp );
        return RO_CALL( then_b, v0.rp );
    }
    // template<class T0>
    // static Rp *call_gen( T0 &&v0 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ) );
    // }
};
struct RF_split {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( split_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( split_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( split_bo, v0.rp, v1.rp );
        return RO_CALL( split_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_join {
    template<class T0,class T1>
    static Rp *call( T0 &&v0, T1 &&v1 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        AutoIndestructibleFakeIfNotRef<T1> ai_1{ v1 };
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 0 ) return RO_CALL( join_oo, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 0 and std::is_reference<T1>::value == 1 ) return RO_CALL( join_ob, v0.rp, v1.rp );
        if ( std::is_reference<T0>::value == 1 and std::is_reference<T1>::value == 0 ) return RO_CALL( join_bo, v0.rp, v1.rp );
        return RO_CALL( join_bb, v0.rp, v1.rp );
    }
    // template<class T0,class T1>
    // static Rp *call_gen( T0 &&v0, T1 &&v1 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ), as_a_Variable( std::forward<T1>( v1 ) ) );
    // }
};
struct RF_bind_by_kn_object_id {
    template<class T0>
    static Rp *call( T0 &&v0 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        if ( std::is_reference<T0>::value == 0 ) return RO_CALL( bind_by_kn_object_id_o, v0.rp );
        return RO_CALL( bind_by_kn_object_id_b, v0.rp );
    }
    // template<class T0>
    // static Rp *call_gen( T0 &&v0 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ) );
    // }
};
struct RF_make_inode_if_z {
    template<class T0>
    static Rp *call( T0 &&v0 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        if ( std::is_reference<T0>::value == 0 ) return RO_CALL( make_inode_if_z_o, v0.rp );
        return RO_CALL( make_inode_if_z_b, v0.rp );
    }
    // template<class T0>
    // static Rp *call_gen( T0 &&v0 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ) );
    // }
};
struct RF_special_func {
    template<class T0>
    static Rp *call( T0 &&v0 ) {
        AutoIndestructibleFakeIfNotRef<T0> ai_0{ v0 };
        if ( std::is_reference<T0>::value == 0 ) return RO_CALL( special_func_o, v0.rp );
        return RO_CALL( special_func_b, v0.rp );
    }
    // template<class T0>
    // static Rp *call_gen( T0 &&v0 ) {
    //     return call_var( as_a_Variable( std::forward<T0>( v0 ) ) );
    // }
};



} // namespace RiotBase
