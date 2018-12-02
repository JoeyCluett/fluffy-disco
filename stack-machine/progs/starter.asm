# this is where programs start
call local.main
halt

uses standardmath

label main
    pushlit 2
    pushlit 3
    call global.Power
    ptop
    ret
