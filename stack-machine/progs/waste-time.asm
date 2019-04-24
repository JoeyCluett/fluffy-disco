export Delay # decrement argument until zero, then return
    
    popr 0 # save return address
    bzero local.end    

label loop
    push1
    sub
    bnzero local.loop

label end
    # push return address and return to caller
    pushr 0
    ret
