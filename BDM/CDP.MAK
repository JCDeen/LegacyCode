.c.obj:
  cl -c /AL /FPc /W3 /Od /Zi $*.c >> cdp.err

nothome.xxx:          
    echo cdp.err > cdp.err

async.obj:      async.h async.c

cdp.obj:        cdp.h cdp.c

cdp00.obj:      cdp.h cdp00.c async.h async.c

cdp01.obj:      cdp.h cdp01.c

cdp02.obj:      cdp.h cdp02.c


# ckdisk.obj: EXAMPLE OF COMMENT TO REMIND THYSELF ...

cdp.exe: cdp.obj cdp00.obj cdp01.obj cdp02.obj async.obj
        type cdp.err | more
        link /CO/ST:0x1000 $**,,,ct5_m5l+graphics,,


