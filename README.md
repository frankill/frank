[png]: https://github.com/frankill/frank/tree/main/png
[funciton_call]: https://github.com/frankill/frank/blob/main/png/funciton_call.png
[token]: https://github.com/frankill/frank/blob/main/png/token.png
[parser_evaluator]: https://github.com/frankill/frank/blob/main/png/parser_evaluator.png
# frank
 
### 用来学习R内部数据结构的辅助包，包含了表达式提取，函数参数提取，环境内容提取，promise 表达式提取
 
+ get_expr
+ get_env
+ get_fun
+ get_promise

##### get_expr  

``` r
res <- get_expr(lapply)
print(res)
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

``` r
a <- new.env(hash = F)
a$a <- 10
get_env(a)
```
$frame  
$frame$a  
[1] 10  


$parent_env  
 [1] ".GlobalEnv"        "package:frank"     "tools:rstudio"      
 [4] "package:stats"     "package:graphics"  "package:grDevices"  
 [7] "package:utils"     "package:datasets"  "package:methods"    
[10] "Autoloads"         "package:base"      "empty"              

$frame_value  
$frame_value[[1]]  
$frame_value[[1]]$type  
[1] 14  

$frame_value[[1]]$value  
[1] 10  

$frame_value[[1]]$tag  
a


### 对源码的阅读，做了一些记录,如解析器，求值器，字节码还在学习中,更多在[png目录][png]中



![][funciton_call]
![][token]
![][parser_evaluator]


