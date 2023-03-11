#include <cpp11.hpp>
// #include <algorithm>
// #include <numeric>
// #include <execution>
// #include <iostream>
// #include <string>
// #include <ctime>
// #include <future>

// using namespace std;

using namespace cpp11;

int Rlen(SEXP s)
{
    int i = 0;

    for (SEXP cons = s; cons != R_NilValue; cons = CDR(cons))
    {
        i++;
    }

    return i;
}

SEXP getlang(SEXP s)
{
    cpp11::writable::list res(Rlen(s));
    int i = 0;

    for (SEXP d = s; d != R_NilValue; d = CDR(d), i++)
    {
        SEXP a;
        int type = TYPEOF(CAR(d));
        if (LANGSXP == type || DOTSXP == type || LISTSXP == type)
        {
            a = getlang(CAR(d));
        }
        else
        {
            a = CAR(d);
        }
        SEXP t = TAG(d);
        res[i] = list({"type"_nm = integers({TYPEOF(a)}),
                       "value"_nm = a,
                       "tag"_nm = t});
    }

    return res;
}

[[cpp11::register]] SEXP getpromis(SEXP p)
{

    return list({"code"_nm = PRCODE(p),
                 "env"_nm = PRENV(p),
                 "type"_nm = TYPEOF(PRCODE(p)),
                 "value"_nm = PRVALUE(p) == R_UnboundValue ? R_NilValue : PRVALUE(p)});
}

[[cpp11::register]] SEXP getexpr(SEXP e)
{

    if (SYMSXP == TYPEOF(e))
    {
        cpp11::writable::list res;

        res.push_back("type"_nm = integers({TYPEOF(e)}));
        res.push_back("name"_nm = PRINTNAME(e));
        res.push_back("value"_nm = R_UnboundValue == SYMVALUE(e) ? R_NilValue : SYMVALUE(e));
        res.push_back("value_type"_nm = integers({R_UnboundValue == SYMVALUE(e) ? NILSXP : TYPEOF(SYMVALUE(e))}));
        res.push_back("internal"_nm = INTERNAL(e));

        if (PROMSXP == TYPEOF(SYMVALUE(e)))
        {
            res.push_back("value_code"_nm = PRCODE(SYMVALUE(e)));
            res.push_back("value_env"_nm = PRENV(SYMVALUE(e)));
            res.push_back("value_value"_nm = PRVALUE(SYMVALUE(e)));
        }
        return res;
    }

    if (EXPRSXP == TYPEOF(e))
    {

        int n = LENGTH(e);
        cpp11::writable::list res(n);

        for (int i = 0; i < n; i++)
        {
            res[i] = getlang(VECTOR_ELT(e, i));
        }

        return res;
    }

    return getlang(e);
}

[[cpp11::register]] SEXP getfun(SEXP fun)
{
    if (!Rf_isFunction(fun))
    {
        return R_NilValue;
    }

    if (TYPEOF(fun) == BUILTINSXP || TYPEOF(fun) == SPECIALSXP)
    {
        return INTERNAL(fun);
    }

    return list({"formals"_nm = getlang(FORMALS(fun)),
                 "body"_nm = R_BytecodeExpr(BODY(fun)),
                 "closenv"_nm = CLOENV(fun)});
}

SEXP get_env(SEXP e)
{
    if (R_EmptyEnv == e)
    {
        return Rf_list1(Rf_mkString("empty"));
    }

    if (R_BaseEnv == e)
    {
        return Rf_cons(Rf_mkString("package:base"), get_env(ENCLOS(e)));
    }

    if (R_BaseNamespace == e)
    {
        return Rf_cons(Rf_mkString("package:basenamespace"), get_env(ENCLOS(e)));
    }

    if (R_GlobalEnv == e)
    {
        return Rf_cons(Rf_mkString(".GlobalEnv"), get_env(ENCLOS(e)));
    }

    return Rf_cons(Rf_getAttrib(e, R_NameSymbol), get_env(ENCLOS(e)));
}

SEXP tovector(SEXP e)
{

    SEXP res = PROTECT(Rf_allocVector(STRSXP, Rlen(e)));
    int n = 0;

    for (SEXP i = e; i != R_NilValue; i = CDR(i), n++)
    {
        SET_STRING_ELT(res, n, Rf_asChar(CAR(i)));
    }

    return res;
}

[[cpp11::register]] SEXP getenv(SEXP env)
{

    if (!Rf_isEnvironment(env))
    {
        return R_NilValue;
    }

    return list({"hash"_nm = HASHTAB(env),
                 "frame"_nm = FRAME(env),
                 "parent_env"_nm = tovector(get_env(ENCLOS(env))),
                 "frame_value"_nm = getlang(FRAME(env))});
}
