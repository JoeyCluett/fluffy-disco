# --------------------------------------
# pretty standard math operations 
# not directly supported in the 
# instruction set
# --------------------------------------

export Square # <base> <return address>
    popr 0
    popr 1
    pushr 1
    pushr 1
    mul
    pushr 0
    ret

export Cube # <base> <return address>
    popr 0
    popr 1
    pushr 1
    pushr 1
    pushr 1
    mul
    mul
    pushr 0
    ret

export Power # <base> <exponent> <return address>
    popr 0 # return address
    popr 1 # exponent
    popr 2 # base
    
    # test for exponent zero
    pushr 1
    bnzero local.prep

    # dont need the tested value anymore
    popr 1

    # exponent is zero, return 1
    pushlit 1
    pushr 0
    ret

label prep

    # store the tested exponent again
    popr 1

    # need to pre-decrement the exponent
    pushr 1
    push1
    sub
    popr 1

    pushr 2

label loop_start
    pushr 2
    mul

    pushr 1
    push1
    sub
    popr 1
    pushr 1

    bzero local.loop_end
    popr 1
    goto local.loop_start

label loop_end
    popr 1
    pushr 0
    ret
    
