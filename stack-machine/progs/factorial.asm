# recursive factorial subroutine

# 'export' makes this jump destination available in the global jump table
export factorial
    loads 1
    pushr 0
    pushlit 1
    sub

    bzero EndFunction

    # 'this.' only looks in this file for the destination and 
    # uses a relative address. this allows for relocatable code
    call this.factorial

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
