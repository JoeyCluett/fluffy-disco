# ---------------------------------
# recursive factorial subroutine
# ---------------------------------
export Factorial # <number> <return address>
    loads 1 # number
    pushr 0
    pushlit 1
    sub

    bzero local.EndFunction

    # 'local.' only looks in this file for the destination
    call local.Factorial

    # get the next multiplicand off of the stack and multiply
    loads 2
    pushr 0  
    mul

    # store the new result in a place the caller 
    # can find it and return
    popr 0
    stores 1
    ret

label EndFunction  
    # remove the zero and return
    popr 0
    ret

# ----------------------------------
# iterative factorial subroutine
# ----------------------------------
export FactorialIterative # <number> <return address>
    popr 0 # return address
    bnzero local.start_iterative

    # zero factorial is just zero
    pushr 0
    ret

label start_iterative
    popr 1
    pushr 1 # copy <number> into register 1

label iterative_loop
    # decrement <number> and multiply
    pushr 1
    pushlit 1
    sub

    # save new value of <number>
    popr 1
    pushr 1

    # multiply through
    mul
    
    pushr 1
    

    halt

label iterative_end
