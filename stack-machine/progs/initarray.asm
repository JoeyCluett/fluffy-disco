export InitArray

    # save the return address
    popr 0
    popr 3 # this is what array will get intialized with

    # size of array is passed on the stack
    bnzero local.FunctionStart
    halt # error condition, cant make array of size zero

label FunctionStart

    # verified that the array size is nonzero
    popr 1

    # first array element
    # pushlit 0
    pushr 3

    # decrement needed elements
    pushr 1
    pushlit 1
    sub

    bnzero local.FunctionStart

    # function end, return to caller
    popr 2
    pushr 0
    ret
