// ArrMax01.tst - second test case (no .out in zip)
// Uses Sys.vm to call ArrMax.arrMax with base at 2052, length 5
// Expected: RAM[16] (static 0) ends as 88

load,
output-file ArrMax02.out,
compare-to ArrMax02.cmp,
output-list sp%D1.6.1 local%D1.6.1 argument%D1.8.1 this%D1.6.1 that%D1.6.1
            RAM[16]%D1.6.1 RAM[17]%D1.6.1 RAM[18]%D1.6.1
            local[0]%D1.8.1 local[1]%D1.8.1 local[2]%D1.8.1
            argument[0]%D1.11.1 argument[1]%D1.11.1 argument[2]%D1.11.1;

set sp 256,        // stack pointer
set local 300,     // base address of the local segment
set argument 400,  // base address of the argument segment
set this 3000,     // base address of the this segment
set that 3010,     // base address of the that segment

set RAM[16] 0,     // static 0 (result gets stored here by Sys.vm)
set RAM[17] 2060,  // static 1 = array base
set RAM[18] 1,     // static 2 = array length

set local[0] 10,   // local 0
set local[1] 20,   // local 1
set local[2] 30,   // local 2

set argument[0] 100,  // argument 0
set argument[1] 200,  // argument 1
set argument[2] 300,  // argument 2

// Array: RAM[2060..2060] = [999]
set RAM[2060] 999;

repeat 300 {        // run enough VM steps to complete Sys.init and the call
  vmstep;
}
output;
