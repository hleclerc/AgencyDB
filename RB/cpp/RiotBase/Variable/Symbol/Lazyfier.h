#pragma once

#include "../Core/RpTempl.h"

namespace RiotBase {
class GenSymSelect;

/// ref can be used
class Lazyfier : public RpTempl<Lazyfier> {
public:
    enum { unknown_value = true, };

    Lazyfier( Rp *cur );
    ~Lazyfier();

    void                write_to_stream     ( std::ostream &os ) const;
    void                write_graphviz      ( std::ostream &os ) const;
    void                write_structure     ( std::ostream &os ) const;
    static std::string  type_name           ();
    const Rp           *prox_ref            () const;
    Rp                 *copy                () const;
    int                 nb_symbolic_children() const { return 1; }
    const Rp           *symbolic_child      ( int ) const { return cur; }
    CommonSymbolicData *common_symbolic_data() const { return 0; } ///< because we don't want the `cur` one (@see prox_ref)
    bool                is_symbolic         () const { return true; }

    const Lazyfier     *_wait_for_result    () const;
    Lazyfier           *_wait_for_result    ();

    Rp                 *cur; ///<
    Vec<GenSymSelect *> sym_selects;
};

} // namespace RiotBase
