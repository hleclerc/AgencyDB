/// This file is generated (by bin/Method_template.py). Do not modify it directly.
#include "Type.h"

namespace RiotBase {

// _surdef_for (helper for TypeTempl<>::surdef_for)
template<class T,class FuncName>
typename FuncName::TF *_surdef_for( FuncName, double &pertinence ) { return 0; }


template<class T> struct DynMethToStatFunc_del { static void f( Rp *v0 ) { return static_cast<T *>( v0 )->del(  ); } };
template<class T> typename EnableIf<1,RF_del::TF,decltype(&T::del)>::T *_surdef_for( RF_del, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_del<T>::f; }
template<class T> struct DynMethToStatFunc_copy { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->copy(  ); } };
template<class T> typename EnableIf<1,RF_copy::TF,decltype(&T::copy)>::T *_surdef_for( RF_copy, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_copy<T>::f; }
template<class T> struct DynMethToStatFunc_write_to_stream { static void f( const Rp *v0, std::ostream &os ) { return static_cast<const T *>( v0 )->write_to_stream( os ); } };
template<class T> typename EnableIf<1,RF_write_to_stream::TF,decltype(&T::write_to_stream)>::T *_surdef_for( RF_write_to_stream, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_write_to_stream<T>::f; }
template<class T> struct DynMethToStatFunc_write_structure { static void f( const Rp *v0, std::ostream &os ) { return static_cast<const T *>( v0 )->write_structure( os ); } };
template<class T> typename EnableIf<1,RF_write_structure::TF,decltype(&T::write_structure)>::T *_surdef_for( RF_write_structure, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_write_structure<T>::f; }
template<class T> struct DynMethToStatFunc_write_graphviz { static void f( const Rp *v0, std::ostream &os ) { return static_cast<const T *>( v0 )->write_graphviz( os ); } };
template<class T> typename EnableIf<1,RF_write_graphviz::TF,decltype(&T::write_graphviz)>::T *_surdef_for( RF_write_graphviz, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_write_graphviz<T>::f; }
template<class T> struct DynMethToStatFunc_category { static const Category *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->category(  ); } };
template<class T> typename EnableIf<1,RF_category::TF,decltype(&T::category)>::T *_surdef_for( RF_category, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_category<T>::f; }
template<class T> struct DynMethToStatFunc_to_std_string { static bool f( const Rp *v0, std::string *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_std_string( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_std_string::TF,decltype(&T::to_std_string)>::T *_surdef_for( RF_to_std_string, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_std_string<T>::f; }
template<class T> struct DynMethToStatFunc_to_Inode { static bool f( const Rp *v0, Inode *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_Inode( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_Inode::TF,decltype(&T::to_Inode)>::T *_surdef_for( RF_to_Inode, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_Inode<T>::f; }
template<class T> struct DynMethToStatFunc_to_SI64 { static bool f( const Rp *v0, SI64 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_SI64( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_SI64::TF,decltype(&T::to_SI64)>::T *_surdef_for( RF_to_SI64, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_SI64<T>::f; }
template<class T> struct DynMethToStatFunc_to_PI64 { static bool f( const Rp *v0, PI64 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_PI64( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_PI64::TF,decltype(&T::to_PI64)>::T *_surdef_for( RF_to_PI64, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_PI64<T>::f; }
template<class T> struct DynMethToStatFunc_to_SI32 { static bool f( const Rp *v0, SI32 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_SI32( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_SI32::TF,decltype(&T::to_SI32)>::T *_surdef_for( RF_to_SI32, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_SI32<T>::f; }
template<class T> struct DynMethToStatFunc_to_PI32 { static bool f( const Rp *v0, PI32 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_PI32( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_PI32::TF,decltype(&T::to_PI32)>::T *_surdef_for( RF_to_PI32, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_PI32<T>::f; }
template<class T> struct DynMethToStatFunc_to_SI16 { static bool f( const Rp *v0, SI16 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_SI16( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_SI16::TF,decltype(&T::to_SI16)>::T *_surdef_for( RF_to_SI16, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_SI16<T>::f; }
template<class T> struct DynMethToStatFunc_to_PI16 { static bool f( const Rp *v0, PI16 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_PI16( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_PI16::TF,decltype(&T::to_PI16)>::T *_surdef_for( RF_to_PI16, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_PI16<T>::f; }
template<class T> struct DynMethToStatFunc_to_SI8 { static bool f( const Rp *v0, SI8 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_SI8( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_SI8::TF,decltype(&T::to_SI8)>::T *_surdef_for( RF_to_SI8, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_SI8<T>::f; }
template<class T> struct DynMethToStatFunc_to_PI8 { static bool f( const Rp *v0, PI8 *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_PI8( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_PI8::TF,decltype(&T::to_PI8)>::T *_surdef_for( RF_to_PI8, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_PI8<T>::f; }
template<class T> struct DynMethToStatFunc_to_Bool { static bool f( const Rp *v0, Bool *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_Bool( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_Bool::TF,decltype(&T::to_Bool)>::T *_surdef_for( RF_to_Bool, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_Bool<T>::f; }
template<class T> struct DynMethToStatFunc_to_PT { static bool f( const Rp *v0, PT *res, PT nb_items ) { return static_cast<const T *>( v0 )->to_PT( res, nb_items ); } };
template<class T> typename EnableIf<1,RF_to_PT::TF,decltype(&T::to_PT)>::T *_surdef_for( RF_to_PT, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_to_PT<T>::f; }
template<class T> struct DynMethToStatFunc_dasi { static DaSi f( const Rp *v0 ) { return static_cast<const T *>( v0 )->dasi(  ); } };
template<class T> typename EnableIf<1,RF_dasi::TF,decltype(&T::dasi)>::T *_surdef_for( RF_dasi, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_dasi<T>::f; }
template<class T> struct DynMethToStatFunc_apply_DaSi { static bool f( const Rp *v0, const std::function<void(DaSi)> &f ) { return static_cast<const T *>( v0 )->apply_DaSi( f ); } };
template<class T> typename EnableIf<1,RF_apply_DaSi::TF,decltype(&T::apply_DaSi)>::T *_surdef_for( RF_apply_DaSi, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_apply_DaSi<T>::f; }
template<class T> struct DynMethToStatFunc_neg_o { static Rp *f( Rp *v0 ) { return static_cast<T *>( v0 )->neg_o(  ); } };
template<class T> typename EnableIf<1,RF_neg_o::TF,decltype(&T::neg_o)>::T *_surdef_for( RF_neg_o, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_neg_o<T>::f; }
template<class T> struct DynMethToStatFunc_neg_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->neg_b(  ); } };
template<class T> typename EnableIf<1,RF_neg_b::TF,decltype(&T::neg_b)>::T *_surdef_for( RF_neg_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_neg_b<T>::f; }
template<class T> struct DynMethToStatFunc_make_ot_wrapper { static Rp *f( Rp *v0 ) { return static_cast<T *>( v0 )->make_ot_wrapper(  ); } };
template<class T> typename EnableIf<1,RF_make_ot_wrapper::TF,decltype(&T::make_ot_wrapper)>::T *_surdef_for( RF_make_ot_wrapper, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_make_ot_wrapper<T>::f; }
template<class T> struct DynMethToStatFunc_pull_ot_changes { static void f( Rp *&v0, BBQ changes ) { return static_cast<T *>( v0 )->pull_ot_changes( changes ); } };
template<class T> typename EnableIf<1,RF_pull_ot_changes::TF,decltype(&T::pull_ot_changes)>::T *_surdef_for( RF_pull_ot_changes, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_pull_ot_changes<T>::f; }
template<class T> struct DynMethToStatFunc_write_ot_data { static void f( Rp *&v0, std::ostream &os, CbStringPtr data ) { return static_cast<T *>( v0 )->write_ot_data( os, data ); } };
template<class T> typename EnableIf<1,RF_write_ot_data::TF,decltype(&T::write_ot_data)>::T *_surdef_for( RF_write_ot_data, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_write_ot_data<T>::f; }
template<class T> struct DynMethToStatFunc_new_ot_merger { static OtMerger *f( Rp *&v0 ) { return static_cast<T *>( v0 )->new_ot_merger(  ); } };
template<class T> typename EnableIf<1,RF_new_ot_merger::TF,decltype(&T::new_ot_merger)>::T *_surdef_for( RF_new_ot_merger, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_new_ot_merger<T>::f; }
template<class T> struct DynMethToStatFunc_new_cursor { static Rp *f( Rp *&v0, void *int_id ) { return static_cast<T *>( v0 )->new_cursor( int_id ); } };
template<class T> typename EnableIf<1,RF_new_cursor::TF,decltype(&T::new_cursor)>::T *_surdef_for( RF_new_cursor, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_new_cursor<T>::f; }
template<class T> struct DynMethToStatFunc_rem_cursor { static void f( Rp *v0, void *int_id ) { return static_cast<T *>( v0 )->rem_cursor( int_id ); } };
template<class T> typename EnableIf<1,RF_rem_cursor::TF,decltype(&T::rem_cursor)>::T *_surdef_for( RF_rem_cursor, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_rem_cursor<T>::f; }
template<class T> struct DynMethToStatFunc_get_cursors { static void f( const Rp *v0, Vec<CursorData> &res ) { return static_cast<const T *>( v0 )->get_cursors( res ); } };
template<class T> typename EnableIf<1,RF_get_cursors::TF,decltype(&T::get_cursors)>::T *_surdef_for( RF_get_cursors, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_cursors<T>::f; }
template<class T> struct DynMethToStatFunc_get_possible_rights { static Vec<std::string> f( Rp *&v0 ) { return static_cast<T *>( v0 )->get_possible_rights(  ); } };
template<class T> typename EnableIf<1,RF_get_possible_rights::TF,decltype(&T::get_possible_rights)>::T *_surdef_for( RF_get_possible_rights, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_possible_rights<T>::f; }
template<class T> struct DynMethToStatFunc_get_groups_in_acl { static Vec<GrpId> f( const Rp *v0 ) { return static_cast<const T *>( v0 )->get_groups_in_acl(  ); } };
template<class T> typename EnableIf<1,RF_get_groups_in_acl::TF,decltype(&T::get_groups_in_acl)>::T *_surdef_for( RF_get_groups_in_acl, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_groups_in_acl<T>::f; }
template<class T> struct DynMethToStatFunc_get_users_in_acl { static Vec<UsrId> f( const Rp *v0 ) { return static_cast<const T *>( v0 )->get_users_in_acl(  ); } };
template<class T> typename EnableIf<1,RF_get_users_in_acl::TF,decltype(&T::get_users_in_acl)>::T *_surdef_for( RF_get_users_in_acl, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_users_in_acl<T>::f; }
template<class T> struct DynMethToStatFunc_get_grp_rights { static Vec<std::string> f( const Rp *v0, GrpId grp ) { return static_cast<const T *>( v0 )->get_grp_rights( grp ); } };
template<class T> typename EnableIf<1,RF_get_grp_rights::TF,decltype(&T::get_grp_rights)>::T *_surdef_for( RF_get_grp_rights, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_grp_rights<T>::f; }
template<class T> struct DynMethToStatFunc_get_usr_rights { static Vec<std::string> f( const Rp *v0, UsrId usr ) { return static_cast<const T *>( v0 )->get_usr_rights( usr ); } };
template<class T> typename EnableIf<1,RF_get_usr_rights::TF,decltype(&T::get_usr_rights)>::T *_surdef_for( RF_get_usr_rights, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_usr_rights<T>::f; }
template<class T> struct DynMethToStatFunc_add_grp_right { static bool f( Rp *&v0, GrpId grp, const std::string &right ) { return static_cast<T *>( v0 )->add_grp_right( grp, right ); } };
template<class T> typename EnableIf<1,RF_add_grp_right::TF,decltype(&T::add_grp_right)>::T *_surdef_for( RF_add_grp_right, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_add_grp_right<T>::f; }
template<class T> struct DynMethToStatFunc_add_usr_right { static bool f( Rp *&v0, UsrId usr, const std::string &right ) { return static_cast<T *>( v0 )->add_usr_right( usr, right ); } };
template<class T> typename EnableIf<1,RF_add_usr_right::TF,decltype(&T::add_usr_right)>::T *_surdef_for( RF_add_usr_right, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_add_usr_right<T>::f; }
template<class T> struct DynMethToStatFunc_rem_grp_right { static bool f( Rp *&v0, GrpId grp, const std::string &right ) { return static_cast<T *>( v0 )->rem_grp_right( grp, right ); } };
template<class T> typename EnableIf<1,RF_rem_grp_right::TF,decltype(&T::rem_grp_right)>::T *_surdef_for( RF_rem_grp_right, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_rem_grp_right<T>::f; }
template<class T> struct DynMethToStatFunc_rem_usr_right { static bool f( Rp *&v0, UsrId usr, const std::string &right ) { return static_cast<T *>( v0 )->rem_usr_right( usr, right ); } };
template<class T> typename EnableIf<1,RF_rem_usr_right::TF,decltype(&T::rem_usr_right)>::T *_surdef_for( RF_rem_usr_right, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_rem_usr_right<T>::f; }
template<class T> struct DynMethToStatFunc_self_on_change { static void f( Rp *&v0, Callback *c ) { return static_cast<T *>( v0 )->self_on_change( c ); } };
template<class T> typename EnableIf<1,RF_self_on_change::TF,decltype(&T::self_on_change)>::T *_surdef_for( RF_self_on_change, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_self_on_change<T>::f; }
template<class T> struct DynMethToStatFunc_on_change_s { static Rp *f( Rp *v0, Callback *c ) { return static_cast<T *>( v0 )->on_change_s( c ); } };
template<class T> typename EnableIf<1,RF_on_change_s::TF,decltype(&T::on_change_s)>::T *_surdef_for( RF_on_change_s, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_on_change_s<T>::f; }
template<class T> struct DynMethToStatFunc_on_change_s_extr { static Rp *f( Rp *v0, Callback *c, const Vec<Rp *> &extr ) { return static_cast<T *>( v0 )->on_change_s_extr( c, extr ); } };
template<class T> typename EnableIf<1,RF_on_change_s_extr::TF,decltype(&T::on_change_s_extr)>::T *_surdef_for( RF_on_change_s_extr, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_on_change_s_extr<T>::f; }
template<class T> struct DynMethToStatFunc_on_change_o { static Rp *f( Rp *v0, Callback *c ) { return static_cast<T *>( v0 )->on_change_o( c ); } };
template<class T> typename EnableIf<1,RF_on_change_o::TF,decltype(&T::on_change_o)>::T *_surdef_for( RF_on_change_o, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_on_change_o<T>::f; }
template<class T> struct DynMethToStatFunc_on_change_b { static Rp *f( const Rp *v0, Callback *c ) { return static_cast<const T *>( v0 )->on_change_b( c ); } };
template<class T> typename EnableIf<1,RF_on_change_b::TF,decltype(&T::on_change_b)>::T *_surdef_for( RF_on_change_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_on_change_b<T>::f; }
template<class T> struct DynMethToStatFunc_self_then { static void f( Rp *&v0, Callback *c ) { return static_cast<T *>( v0 )->self_then( c ); } };
template<class T> typename EnableIf<1,RF_self_then::TF,decltype(&T::self_then)>::T *_surdef_for( RF_self_then, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_self_then<T>::f; }
template<class T> struct DynMethToStatFunc_then_s { static Rp *f( Rp *v0, Callback *c ) { return static_cast<T *>( v0 )->then_s( c ); } };
template<class T> typename EnableIf<1,RF_then_s::TF,decltype(&T::then_s)>::T *_surdef_for( RF_then_s, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_then_s<T>::f; }
template<class T> struct DynMethToStatFunc_then_s_extr { static Rp *f( Rp *v0, Callback *c, const Vec<Rp *> &extr ) { return static_cast<T *>( v0 )->then_s_extr( c, extr ); } };
template<class T> typename EnableIf<1,RF_then_s_extr::TF,decltype(&T::then_s_extr)>::T *_surdef_for( RF_then_s_extr, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_then_s_extr<T>::f; }
template<class T> struct DynMethToStatFunc_then_o { static Rp *f( Rp *v0, Callback *c ) { return static_cast<T *>( v0 )->then_o( c ); } };
template<class T> typename EnableIf<1,RF_then_o::TF,decltype(&T::then_o)>::T *_surdef_for( RF_then_o, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_then_o<T>::f; }
template<class T> struct DynMethToStatFunc_then_b { static Rp *f( const Rp *v0, Callback *c ) { return static_cast<const T *>( v0 )->then_b( c ); } };
template<class T> typename EnableIf<1,RF_then_b::TF,decltype(&T::then_b)>::T *_surdef_for( RF_then_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_then_b<T>::f; }
template<class T> struct DynMethToStatFunc_sig_change { static void f( Rp *v0 ) { return static_cast<T *>( v0 )->sig_change(  ); } };
template<class T> typename EnableIf<1,RF_sig_change::TF,decltype(&T::sig_change)>::T *_surdef_for( RF_sig_change, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_sig_change<T>::f; }
template<class T> struct DynMethToStatFunc_common_symbolic_data { static CommonSymbolicData *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->common_symbolic_data(  ); } };
template<class T> typename EnableIf<1,RF_common_symbolic_data::TF,decltype(&T::common_symbolic_data)>::T *_surdef_for( RF_common_symbolic_data, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_common_symbolic_data<T>::f; }
template<class T> struct DynMethToStatFunc_nb_symbolic_children { static int f( const Rp *v0 ) { return static_cast<const T *>( v0 )->nb_symbolic_children(  ); } };
template<class T> typename EnableIf<1,RF_nb_symbolic_children::TF,decltype(&T::nb_symbolic_children)>::T *_surdef_for( RF_nb_symbolic_children, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_nb_symbolic_children<T>::f; }
template<class T> struct DynMethToStatFunc_symbolic_child { static const Rp *f( const Rp *v0, int n ) { return static_cast<const T *>( v0 )->symbolic_child( n ); } };
template<class T> typename EnableIf<1,RF_symbolic_child::TF,decltype(&T::symbolic_child)>::T *_surdef_for( RF_symbolic_child, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_symbolic_child<T>::f; }
template<class T> struct DynMethToStatFunc_is_symbolic { static bool f( const Rp *v0 ) { return static_cast<const T *>( v0 )->is_symbolic(  ); } };
template<class T> typename EnableIf<1,RF_is_symbolic::TF,decltype(&T::is_symbolic)>::T *_surdef_for( RF_is_symbolic, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_is_symbolic<T>::f; }
template<class T> struct DynMethToStatFunc_is_a_s_op { static bool f( const Rp *v0 ) { return static_cast<const T *>( v0 )->is_a_s_op(  ); } };
template<class T> typename EnableIf<1,RF_is_a_s_op::TF,decltype(&T::is_a_s_op)>::T *_surdef_for( RF_is_a_s_op, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_is_a_s_op<T>::f; }
template<class T> struct DynMethToStatFunc_exec { static Rp *f( const Rp *v0, BitFieldExtSize own, Rp **ch ) { return static_cast<const T *>( v0 )->exec( own, ch ); } };
template<class T> typename EnableIf<1,RF_exec::TF,decltype(&T::exec)>::T *_surdef_for( RF_exec, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_exec<T>::f; }
template<class T> struct DynMethToStatFunc_launch { static void f( Rp *v0 ) { return static_cast<T *>( v0 )->launch(  ); } };
template<class T> typename EnableIf<1,RF_launch::TF,decltype(&T::launch)>::T *_surdef_for( RF_launch, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_launch<T>::f; }
template<class T> struct DynMethToStatFunc_exec_mp { static Rp *f( Rp *v0 ) { return static_cast<T *>( v0 )->exec_mp(  ); } };
template<class T> typename EnableIf<1,RF_exec_mp::TF,decltype(&T::exec_mp)>::T *_surdef_for( RF_exec_mp, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_exec_mp<T>::f; }
template<class T> struct DynMethToStatFunc_del_or_cancel_exec { static void f( Rp *v0 ) { return static_cast<T *>( v0 )->del_or_cancel_exec(  ); } };
template<class T> typename EnableIf<1,RF_del_or_cancel_exec::TF,decltype(&T::del_or_cancel_exec)>::T *_surdef_for( RF_del_or_cancel_exec, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_del_or_cancel_exec<T>::f; }
template<class T> struct DynMethToStatFunc_get_lazy_source_info { static void f( const Rp *v0, LazySourceInfo &info, const LazySourceInfo **children_info ) { return static_cast<const T *>( v0 )->get_lazy_source_info( info, children_info ); } };
template<class T> typename EnableIf<1,RF_get_lazy_source_info::TF,decltype(&T::get_lazy_source_info)>::T *_surdef_for( RF_get_lazy_source_info, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_lazy_source_info<T>::f; }
template<class T> struct DynMethToStatFunc_serialize { static void f( const Rp *v0, BBQ bq, const PT *num_children ) { return static_cast<const T *>( v0 )->serialize( bq, num_children ); } };
template<class T> typename EnableIf<1,RF_serialize::TF,decltype(&T::serialize)>::T *_surdef_for( RF_serialize, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_serialize<T>::f; }
template<class T> struct DynMethToStatFunc_set_child { static void f( Rp *v0, int num_child, Rp *new_child, bool del_prev ) { return static_cast<T *>( v0 )->set_child( num_child, new_child, del_prev ); } };
template<class T> typename EnableIf<1,RF_set_child::TF,decltype(&T::set_child)>::T *_surdef_for( RF_set_child, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_set_child<T>::f; }
template<class T> struct DynMethToStatFunc_prox_ptr { static Rp **f( Rp *v0 ) { return static_cast<T *>( v0 )->prox_ptr(  ); } };
template<class T> typename EnableIf<1,RF_prox_ptr::TF,decltype(&T::prox_ptr)>::T *_surdef_for( RF_prox_ptr, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_prox_ptr<T>::f; }
template<class T> struct DynMethToStatFunc_prox_ref { static const Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->prox_ref(  ); } };
template<class T> typename EnableIf<1,RF_prox_ref::TF,decltype(&T::prox_ref)>::T *_surdef_for( RF_prox_ref, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_prox_ref<T>::f; }
template<class T> struct DynMethToStatFunc_prox_val { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->prox_val(  ); } };
template<class T> typename EnableIf<1,RF_prox_val::TF,decltype(&T::prox_val)>::T *_surdef_for( RF_prox_val, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_prox_val<T>::f; }
template<class T> struct DynMethToStatFunc_get_size { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->get_size(  ); } };
template<class T> typename EnableIf<1,RF_get_size::TF,decltype(&T::get_size)>::T *_surdef_for( RF_get_size, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_size<T>::f; }
template<class T> struct DynMethToStatFunc_get_size_PT { static PT f( const Rp *v0 ) { return static_cast<const T *>( v0 )->get_size_PT(  ); } };
template<class T> typename EnableIf<1,RF_get_size_PT::TF,decltype(&T::get_size_PT)>::T *_surdef_for( RF_get_size_PT, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_get_size_PT<T>::f; }
template<class T> struct DynMethToStatFunc_front_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->front_b(  ); } };
template<class T> typename EnableIf<1,RF_front_b::TF,decltype(&T::front_b)>::T *_surdef_for( RF_front_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_front_b<T>::f; }
template<class T> struct DynMethToStatFunc_back_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->back_b(  ); } };
template<class T> typename EnableIf<1,RF_back_b::TF,decltype(&T::back_b)>::T *_surdef_for( RF_back_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_back_b<T>::f; }
template<class T> struct DynMethToStatFunc_front_list_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->front_list_b(  ); } };
template<class T> typename EnableIf<1,RF_front_list_b::TF,decltype(&T::front_list_b)>::T *_surdef_for( RF_front_list_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_front_list_b<T>::f; }
template<class T> struct DynMethToStatFunc_back_list_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->back_list_b(  ); } };
template<class T> typename EnableIf<1,RF_back_list_b::TF,decltype(&T::back_list_b)>::T *_surdef_for( RF_back_list_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_back_list_b<T>::f; }
template<class T> struct DynMethToStatFunc_head_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->head_b(  ); } };
template<class T> typename EnableIf<1,RF_head_b::TF,decltype(&T::head_b)>::T *_surdef_for( RF_head_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_head_b<T>::f; }
template<class T> struct DynMethToStatFunc_tail_b { static Rp *f( const Rp *v0 ) { return static_cast<const T *>( v0 )->tail_b(  ); } };
template<class T> typename EnableIf<1,RF_tail_b::TF,decltype(&T::tail_b)>::T *_surdef_for( RF_tail_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_tail_b<T>::f; }
template<class T> struct DynMethToStatFunc_apply { static void f( const Rp *v0, std::function<void(const Rp *)> func ) { return static_cast<const T *>( v0 )->apply( func ); } };
template<class T> typename EnableIf<1,RF_apply::TF,decltype(&T::apply)>::T *_surdef_for( RF_apply, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_apply<T>::f; }
template<class T> struct DynMethToStatFunc_bind_by_path { static Rp *f( const Rp *v0, const Category *cat ) { return static_cast<const T *>( v0 )->bind_by_path( cat ); } };
template<class T> typename EnableIf<1,RF_bind_by_path::TF,decltype(&T::bind_by_path)>::T *_surdef_for( RF_bind_by_path, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_bind_by_path<T>::f; }
template<class T> struct DynMethToStatFunc_bind_by_object_id { static Rp *f( Rp *&v0, const Category *cat ) { return static_cast<T *>( v0 )->bind_by_object_id( cat ); } };
template<class T> typename EnableIf<1,RF_bind_by_object_id::TF,decltype(&T::bind_by_object_id)>::T *_surdef_for( RF_bind_by_object_id, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_bind_by_object_id<T>::f; }
template<class T> struct DynMethToStatFunc_bind_by_kn_object_id_o { static Rp *f( Rp *v0, const Category *cat ) { return static_cast<T *>( v0 )->bind_by_kn_object_id_o( cat ); } };
template<class T> typename EnableIf<1,RF_bind_by_kn_object_id_o::TF,decltype(&T::bind_by_kn_object_id_o)>::T *_surdef_for( RF_bind_by_kn_object_id_o, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_bind_by_kn_object_id_o<T>::f; }
template<class T> struct DynMethToStatFunc_bind_by_kn_object_id_b { static Rp *f( const Rp *v0, const Category *cat ) { return static_cast<const T *>( v0 )->bind_by_kn_object_id_b( cat ); } };
template<class T> typename EnableIf<1,RF_bind_by_kn_object_id_b::TF,decltype(&T::bind_by_kn_object_id_b)>::T *_surdef_for( RF_bind_by_kn_object_id_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_bind_by_kn_object_id_b<T>::f; }
template<class T> struct DynMethToStatFunc_self_make_inode_if_z { static void f( Rp *&v0, const Category *cat ) { return static_cast<T *>( v0 )->self_make_inode_if_z( cat ); } };
template<class T> typename EnableIf<1,RF_self_make_inode_if_z::TF,decltype(&T::self_make_inode_if_z)>::T *_surdef_for( RF_self_make_inode_if_z, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_self_make_inode_if_z<T>::f; }
template<class T> struct DynMethToStatFunc_make_inode_if_z_s { static Rp *f( Rp *v0, const Category *cat ) { return static_cast<T *>( v0 )->make_inode_if_z_s( cat ); } };
template<class T> typename EnableIf<1,RF_make_inode_if_z_s::TF,decltype(&T::make_inode_if_z_s)>::T *_surdef_for( RF_make_inode_if_z_s, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_make_inode_if_z_s<T>::f; }
template<class T> struct DynMethToStatFunc_make_inode_if_z_s_extr { static Rp *f( Rp *v0, const Category *cat, const Vec<Rp *> &extr ) { return static_cast<T *>( v0 )->make_inode_if_z_s_extr( cat, extr ); } };
template<class T> typename EnableIf<1,RF_make_inode_if_z_s_extr::TF,decltype(&T::make_inode_if_z_s_extr)>::T *_surdef_for( RF_make_inode_if_z_s_extr, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_make_inode_if_z_s_extr<T>::f; }
template<class T> struct DynMethToStatFunc_make_inode_if_z_o { static Rp *f( Rp *v0, const Category *cat ) { return static_cast<T *>( v0 )->make_inode_if_z_o( cat ); } };
template<class T> typename EnableIf<1,RF_make_inode_if_z_o::TF,decltype(&T::make_inode_if_z_o)>::T *_surdef_for( RF_make_inode_if_z_o, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_make_inode_if_z_o<T>::f; }
template<class T> struct DynMethToStatFunc_make_inode_if_z_b { static Rp *f( const Rp *v0, const Category *cat ) { return static_cast<const T *>( v0 )->make_inode_if_z_b( cat ); } };
template<class T> typename EnableIf<1,RF_make_inode_if_z_b::TF,decltype(&T::make_inode_if_z_b)>::T *_surdef_for( RF_make_inode_if_z_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_make_inode_if_z_b<T>::f; }
template<class T> struct DynMethToStatFunc_self_special_func { static void f( Rp *&v0, const char *func, Rp **parm, int nb_parms ) { return static_cast<T *>( v0 )->self_special_func( func, parm, nb_parms ); } };
template<class T> typename EnableIf<1,RF_self_special_func::TF,decltype(&T::self_special_func)>::T *_surdef_for( RF_self_special_func, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_self_special_func<T>::f; }
template<class T> struct DynMethToStatFunc_special_func_s { static Rp *f( Rp *v0, const char *func, Rp **parm, int nb_parms ) { return static_cast<T *>( v0 )->special_func_s( func, parm, nb_parms ); } };
template<class T> typename EnableIf<1,RF_special_func_s::TF,decltype(&T::special_func_s)>::T *_surdef_for( RF_special_func_s, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_special_func_s<T>::f; }
template<class T> struct DynMethToStatFunc_special_func_s_extr { static Rp *f( Rp *v0, const char *func, Rp **parm, int nb_parms, const Vec<Rp *> &extr ) { return static_cast<T *>( v0 )->special_func_s_extr( func, parm, nb_parms, extr ); } };
template<class T> typename EnableIf<1,RF_special_func_s_extr::TF,decltype(&T::special_func_s_extr)>::T *_surdef_for( RF_special_func_s_extr, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_special_func_s_extr<T>::f; }
template<class T> struct DynMethToStatFunc_special_func_o { static Rp *f( Rp *v0, const char *func, Rp **parm, int nb_parms ) { return static_cast<T *>( v0 )->special_func_o( func, parm, nb_parms ); } };
template<class T> typename EnableIf<1,RF_special_func_o::TF,decltype(&T::special_func_o)>::T *_surdef_for( RF_special_func_o, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_special_func_o<T>::f; }
template<class T> struct DynMethToStatFunc_special_func_b { static Rp *f( const Rp *v0, const char *func, Rp **parm, int nb_parms ) { return static_cast<const T *>( v0 )->special_func_b( func, parm, nb_parms ); } };
template<class T> typename EnableIf<1,RF_special_func_b::TF,decltype(&T::special_func_b)>::T *_surdef_for( RF_special_func_b, double &pertinence ) { pertinence = 2; return &DynMethToStatFunc_special_func_b<T>::f; }


} // namespace RiotBase
