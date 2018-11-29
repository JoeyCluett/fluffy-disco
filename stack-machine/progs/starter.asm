
# this is how functions get 'exported' to other parts of the system
uses progs/factorial

    # we want to know 3!
    pushlit 3
    call global.factorial

    # should be 6
    ptop
    