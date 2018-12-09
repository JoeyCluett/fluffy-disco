stamp # get a startime
pushlit 100000000
goto local.main

# ===============================
# see how fast we can count down
# ===============================
label main
    pushlit 1
    sub
    bnzero local.main

    popr 0
    popr 1
    stamp
    pushr 1
    sub
    halt
