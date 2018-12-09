# this is where programs start
call local.main

label main
    pushlit 3
    call global.Factorial
    pushlit 4
    call global.Factorial

    # testing the movr instructions
    popr 0
    movr 1 0
    movr 2 1
    movr 3 2
    movr 4 3

    halt

uses factorial
