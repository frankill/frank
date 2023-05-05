#include <cpp11.hpp>

using namespace cpp11;

/**
 * Returns the length of the given SEXP object.
 *
 * @param s The SEXP object whose length is to be determined.
 *
 * @return An integer representing the length of the given SEXP object.
 *
 * @throws None.
 */

int Rlen(SEXP s)
{
    int i = 0;

    for (SEXP cons = s; cons != R_NilValue; cons = CDR(cons))
    {
        i++;
    }

    return i;
}
/**
 * Returns a writable list of language expressions from the input SEXP. Recursively
 * extracts language expressions from nested lists.
 *
 * @param s The input SEXP to extract language expressions from.
 *
 * @return A writable list of language expressions.
 *
 * @throws ErrorType If the input SEXP is not a language expression or list.
 */

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
        res[i] = list({"type"_nm = integers({type}),
                       "value"_nm = a,
                       "tag"_nm = t});
    }

    return res;
}
/**
 * Returns a list containing information about the given promise object.
 *
 * @param p The promise object to retrieve information from.
 *
 * @return A list containing the code, environment, type, and value of the promise.
 *
 * @throws None
 */

[[cpp11::register]] SEXP getpromis(SEXP p)
{
    if (TYPEOF(p) != PROMSXP)
    {
        return list();
    }

    return list({"code"_nm = getlang(PRCODE(p)),
                 "env"_nm = PRENV(p),
                 "type"_nm = TYPEOF(PRCODE(p)),
                 "value"_nm = PRVALUE(p) == R_UnboundValue ? R_NilValue : PRVALUE(p)});
}
/**
 * Returns an R expression as a list with various properties.
 *
 * @param e an R expression
 *
 * @return a list with properties of the expression
 *
 * @throws Error if e is not a valid expression
 */

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
/**
 * Retrieves the formals, bytecode, body, bcexpr, bcexpr_type, and closenv
 * of a given R function.
 *
 * @param fun The R function to extract information from.
 *
 * @return A list containing the extracted information.
 *
 * @throws An error if the given SEXP is not a function.
 */

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
                 "bytecode"_nm = cpp11::writable::list({CAR(BODY(fun)),
                                                        CDR(BODY(fun))}),
                 "body"_nm = BODY(fun),
                 "bcexpr"_nm = R_BytecodeExpr(BODY(fun)),
                 "bcexpr_type"_nm = TYPEOF(R_BytecodeExpr(BODY(fun))),
                 "closenv"_nm = CLOENV(fun)});
}
/**
 * Returns the environment of the given SEXP object.
 *
 * @param e the SEXP object representing the environment
 *
 * @return a SEXP object representing the environment
 *
 * @throws ErrorType if a error occurs while looking up the environment
 */

SEXP get_env(SEXP e)
{
    if (R_EmptyEnv == e)
    {
        return safe[Rf_list1](Rf_mkString("empty"));
    }

    if (R_BaseEnv == e)
    {
        return safe[Rf_cons](Rf_mkString("package:base"), get_env(ENCLOS(e)));
    }

    if (R_BaseNamespace == e)
    {
        return safe[Rf_cons](Rf_mkString("package:basenamespace"), get_env(ENCLOS(e)));
    }

    if (R_GlobalEnv == e)
    {
        return safe[Rf_cons](Rf_mkString(".GlobalEnv"), get_env(ENCLOS(e)));
    }

    return safe[Rf_cons](Rf_getAttrib(e, R_NameSymbol), get_env(ENCLOS(e)));
}
/**
 * Convert an R language list to a character vector.
 *
 * @param e the R language list to be converted.
 *
 * @return a character vector representation of the input list.
 *
 * @throws ErrorType if any error occurs during the conversion.
 */

SEXP tovector(SEXP e)
{
    SEXP res = safe[Rf_allocVector](STRSXP, Rlen(e));
    int n = 0;

    for (SEXP i = e; i != R_NilValue; i = CDR(i), n++)
    {
        SET_STRING_ELT(res, n, safe[Rf_asChar](CAR(i)));
    }

    return res;
}
/**
 * Evaluate a given R expression in a specified environment.
 *
 * @param expr The expression to evaluate.
 * @param env The environment in which to evaluate the expression.
 *
 * @return The result of evaluating the expression in the given environment.
 *
 * @throws ErrorType If there is an error during evaluation.
 */

[[cpp11::register]] SEXP r_eval(SEXP expr, SEXP env)
{
    if (SYMSXP == TYPEOF(expr))
    {

        SEXP s = Rf_findVar(expr, env);
        if (TYPEOF(s) == PROMSXP)
        {
            return PRCODE(s);
            // return getpromis(s) ;
        }

        return s;
    }

    if (EXPRSXP == TYPEOF(expr))
    {
        int n = LENGTH(expr);
        /**
         * Creates a writable list with a given size.
         *
         * @param n the size of the list to create
         *
         * @return the newly created writable list
         *
         * @throws ErrorType description of error
         */

        cpp11::writable::list res(n);
        for (int i = 0; i < n; i++)
        {
            SET_VECTOR_ELT(res, i, r_eval(VECTOR_ELT(expr, i), env));
        }
        return res;
    }

    return safe[Rf_eval](expr, env);
}

//' @export
[[cpp11::register]] SEXP getenv(SEXP env)
{
    if (!Rf_isEnvironment(env))
    {
        return R_NilValue;
    }

    return list({"hash"_nm = env == R_GlobalEnv ? R_NilValue : HASHTAB(env),
                 "frame"_nm = FRAME(env),
                 "frame_type"_nm = TYPEOF(FRAME(env)),
                 "parent_env"_nm = tovector(get_env(ENCLOS(env))),
                 "frame_value"_nm = getlang(FRAME(env))});
}
/**
 * Returns the value of a property with the given name from the input R object.
 *
 * @param object The input R object to search for the property.
 * 
 * @param property_name The name of the property to retrieve from the input object.
 *
 * @return The value of the property with the given name from the input R object.
 * If no property with that name is found, returns R_NilValue.
 *
 * @throws None
 */

SEXP get_property(SEXP object, const char *property_name)
{
    SEXP attrib = ATTRIB(object);
    while (attrib != R_NilValue)
    {
        if (strcmp(CHAR(PRINTNAME(TAG(attrib))), property_name) == 0)
        {
            return CAR(attrib);
        }
        attrib = CDR(attrib);
    }
    return R_NilValue;
}
