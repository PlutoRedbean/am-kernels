_start:
    lui     t0, 0x30000
    jalr    x0, 0(x5)
/*
_start:
    lui     t0, 0x10000
    addi    t1, zero, 65
    sb      t1, 0(t0)
    addi    t1, zero, 10
    sb      t1, 0(t0)
halt_loop:
    j       halt_loop
*/
