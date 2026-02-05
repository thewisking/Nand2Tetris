// Sample Test file for AddSub.asm
// Follows the Test Scripting Language format described in 
// Appendix B of the book "The Elements of Computing Systems"

load AddSub.asm,
output-file AddSub04.out,
compare-to AddSub04.cmp,
output-list RAM[0]%D2.6.2 RAM[1]%D2.6.2 RAM[2]%D2.6.2 RAM[3]%D2.6.2;

set PC 0,
set RAM[0] 0,
set RAM[1] 5,
set RAM[2] -3,
set RAM[3] -2;
repeat 100 {
  ticktock;    // Run for 100 clock cycles
}
set RAM[1] 5,
set RAM[2] -3,
set RAM[3] -2,
output;        // Output to file

