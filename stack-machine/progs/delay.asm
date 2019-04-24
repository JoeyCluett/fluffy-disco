# ----------------------------------------------
# decrement argument until zero, then return
# ----------------------------------------------

export Delay # <(TOP-1) integer> <(TOP) return address>
    
    popr r0 # save return address in register 0
    bzero local.end    

label loop
    # push1
    # sub
    # bnzero local.loop
    x.decs
    bnzero local.loop

label end
    x.popnone   # dont need the zero anymore, get rid of it
    x.pushret r0 # return to address saved earlier

    # old way:
    #       popr >1   # because we have to choose something to overwrite
    #       pushr 0  # returning now requires 2 instructions
    #       ret
