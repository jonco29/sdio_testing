## to use this, heed the gdb output about putting the safe auto load in your
## ~/.gdbinit
set args untitled.txt
set output-radix 16

## breakpoints
b CCCR::DumpCCCRTable() 

