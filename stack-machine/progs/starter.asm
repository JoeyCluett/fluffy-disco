# this is where programs start
goto local.main

uses standardmath

label main
    pushlit 2
    pushlit 3
    call global.Power
    
    # call with exponent zero
    pushlit -3
    pushlit 0
    call global.Power

    halt
