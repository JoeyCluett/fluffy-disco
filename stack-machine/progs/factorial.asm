export factorial
    loads 1 # get the passed argument
    pushr 0 # place it on the top of the stack

    # test if we should return now
    bzero ReturnOne

    # decrement value and call again
    loads 0
    pushr 0
    pushlit 1
    sub
    call this.factorial

label ReturnOne
    loads 1 # get the orginal return address
    
