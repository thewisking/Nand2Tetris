// ArrMax01.tst - second test case (no .out in zip)
// Uses Sys.vm to call ArrMax.arrMax with base at 2052, length 5
// Expected: RAM[16] (static 0) ends as 88

load,
output-file ArrMax04.out,
compare-to ArrMax04.cmp,
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
set RAM[17] 2080,  // static 1 = array base
set RAM[18] 7,   // static 2 = array length

set local[0] 10,   // local 0
set local[1] 20,   // local 1
set local[2] 30,   // local 2

set argument[0] 100,  // argument 0
set argument[1] 200,  // argument 1
set argument[2] 300,  // argument 2

// Array: RAM[2080..2086] = [3, 99, 7, 99, 8, 2, 99]
set RAM[2080] 3,
set RAM[2081] 99,
set RAM[2082] 7,
set RAM[2083] 99,
set RAM[2084] 8,
set RAM[2085] 2,
set RAM[2086] 99;
repeat 300 {        // run enough VM steps to complete Sys.init and the call
  vmstep;
}
output;
