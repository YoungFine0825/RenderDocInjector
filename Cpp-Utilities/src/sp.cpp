/*
    CONTRIBUTORS:
        Sean Pesce

*/
#include "sp/sp.h"

#define SP_LIB_MAIN_CPP_WIDE2(x) L##x
#define SP_LIB_MAIN_CPP_WIDE1(x) SP_LIB_MAIN_CPP_WIDE2(x)
#define SP_LIB_MAIN_CPP_DATE_W SP_LIB_MAIN_CPP_WIDE1(__DATE__)
#define SP_LIB_MAIN_CPP_TIME_W SP_LIB_MAIN_CPP_WIDE1(__TIME__)

__SP_NAMESPACE
const char*    sp::author   =  "Sean Pesce";
const wchar_t* sp::author_w = L"Sean Pesce";
const char*    sp::build    =  __DATE__ "   " __TIME__;
const wchar_t* sp::build_w  = SP_LIB_MAIN_CPP_DATE_W L"   " SP_LIB_MAIN_CPP_TIME_W;
__SP_NAMESPACE_CLOSE
