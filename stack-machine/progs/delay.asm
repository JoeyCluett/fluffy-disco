# ----------------------------------------------
# decrement argument until zero, then return
# ----------------------------------------------

# test program for Delay
pushlit 5
call global.Delay
halt

export Delay # <(TOP-1) integer> <(TOP) return address>
    
    popr r0 # save return address in register 0
    bzero local.end    

label loop
    x.decs
    bnzero local.loop

label end
    x.popnone    # dont need the zero anymore, get rid of it
    x.pushret r0 # return to address saved earlier
