    pushlit 1

label loop_top
    pushlit -1
    add
    ptop
    bnzero local.loop_top # branch wont be taken
    ptop

