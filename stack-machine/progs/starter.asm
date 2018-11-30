call local.main
halt

# this is how functions get 'exported' to other parts of the system
uses factorial

label main
    # we want to know 4(factorial)
    pushlit 4
    call global.factorial

    # should be 6
    ptop
