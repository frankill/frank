# frank
### 用来学习R内部数据结构的辅助包，包含了表达式提取，函数参数提取，环境内容提取，promise 表达式提取

+ get_expr
+ get_env
+ get_fun
+ get_promise

##### get_expr  

``` r
get_expr(lapply)
```

$type
[1] 1

$name
<CHARSXP: "lapply">

$value
<promise: 0x7f9c44092820>

$value_type
[1] 5

$internal
.Primitive("lapply")

$value_code
lazyLoadDBfetch(c(533118L, 481L), datafile, compressed, envhook)

$value_env
NULL

$value_value
function (X, FUN, ...) 
{
    FUN <- match.fun(FUN)
    if (!is.vector(X) || is.object(X)) 
        X <- as.list(X)
    .Internal(lapply(X, FUN))
}
<bytecode: 0x7f9c440f35a0>
<environment: namespace:base>

### 对源码的阅读，做了一些学习记录，更多在png目录中

![][avatar]

[avatar]: https://github.com/frankill/frank/blob/main/png/package.png
