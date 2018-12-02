# --------------------------------------
# pretty standard math operations 
# not supported in the instruction set
# --------------------------------------

export Square
    popr 0
    popr 1
    pushr 1
    pushr 1
    mul
    pushr 0
    ret

export Cube
    popr 0
    popr 1
    pushr 1
    pushr 1
    pushr 1
    mul
    mul
    pushr 0
    ret

export Power
    popr 0 # return address
    bnzero local.full_function

    # exponent is zero, return one
    popr 1
    popr 1
    push1
    pushr 0
    ret

label full_function
    popr 1 # exponent
    popr 2 # base

    # get the ball rolling
    pushr 2

    pushr 2
    mul

    # decrement the exponent
    
