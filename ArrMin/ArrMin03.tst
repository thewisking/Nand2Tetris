// Overflow/sign edge case for ArrMin.asm
// Tests that min(32767, -32768, 123, -1) == -32768
// This catches naive "a-b < 0" compares that overflow.

load ArrMin.asm,
output-file ArrMin03.out,
compare-to ArrMin03.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[20]%D2.6.2 RAM[21]%D2.6.2 RAM[22]%D2.6.2 RAM[23]%D2.6.2;

set PC 0,
set RAM[0]  0,   // R0 (ignored by correct impl when R2>0)
set RAM[1]  20,  // R1 = base addr
set RAM[2]  4,   // R2 = size

// Arrange so first element is +32767 and a later element is -32768
// Naive (a-b) compare will overflow on -32768 - 32767 and fail this test.
set RAM[20]  32767,   // arr[0]
set RAM[21] -32768,   // arr[1]  (true min)
set RAM[22]     123,  // arr[2]
set RAM[23]      -1;  // arr[3]

repeat 300 {
  ticktock;
}

// Restore args (in case the student's program modified them)
set RAM[1] 20,
set RAM[2] 4,

output;
