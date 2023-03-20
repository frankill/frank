#' @description expression symbol promise lang bytecode
#' @export 
get_function <- function(f) {
    getfun(f)
}

#' @export 
get_expr <- function(e) {
    getexpr(substitute(e))
}

#' @export 
get_env <- function(e) {
    getenv(e)
}

#' @export 
get_promise <- function(p) {
    getpromis(p)
}

#' @export  
enexpr <- function(e) {
    r_eval(substitute(e), parent.frame())
}

#' @export  
expr <- function(e) {
    enexpr(e)
}
