// Generated by cpp11: do not edit by hand
// clang-format off


#include "cpp11/declarations.hpp"
#include <R_ext/Visibility.h>

// util.cpp
SEXP getpromis(SEXP p);
extern "C" SEXP _frank_getpromis(SEXP p) {
  BEGIN_CPP11
    return cpp11::as_sexp(getpromis(cpp11::as_cpp<cpp11::decay_t<SEXP>>(p)));
  END_CPP11
}
// util.cpp
SEXP getexpr(SEXP e);
extern "C" SEXP _frank_getexpr(SEXP e) {
  BEGIN_CPP11
    return cpp11::as_sexp(getexpr(cpp11::as_cpp<cpp11::decay_t<SEXP>>(e)));
  END_CPP11
}
// util.cpp
SEXP getfun(SEXP fun);
extern "C" SEXP _frank_getfun(SEXP fun) {
  BEGIN_CPP11
    return cpp11::as_sexp(getfun(cpp11::as_cpp<cpp11::decay_t<SEXP>>(fun)));
  END_CPP11
}
// util.cpp
SEXP getenv(SEXP env);
extern "C" SEXP _frank_getenv(SEXP env) {
  BEGIN_CPP11
    return cpp11::as_sexp(getenv(cpp11::as_cpp<cpp11::decay_t<SEXP>>(env)));
  END_CPP11
}

extern "C" {
static const R_CallMethodDef CallEntries[] = {
    {"_frank_getenv",    (DL_FUNC) &_frank_getenv,    1},
    {"_frank_getexpr",   (DL_FUNC) &_frank_getexpr,   1},
    {"_frank_getfun",    (DL_FUNC) &_frank_getfun,    1},
    {"_frank_getpromis", (DL_FUNC) &_frank_getpromis, 1},
    {NULL, NULL, 0}
};
}

extern "C" attribute_visible void R_init_frank(DllInfo* dll){
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
  R_forceSymbols(dll, TRUE);
}