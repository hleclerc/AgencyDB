// constants
static const unsigned RET_CONT      = 0;
static const unsigned RET_OK        = 1;
static const unsigned RET_KO        = 2;
static const unsigned RET_ENDED_OK  = 3;
static const unsigned RET_ENDED_KO  = 4;
static const unsigned RET_STOP_CONT = 5;
struct HpipeData {
    HpipeData() : inp_cont( 0 ) {}
    void *inp_cont;
};
