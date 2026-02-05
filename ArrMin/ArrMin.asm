// Finds the smallest element in the array of length R2 whose first element is at RAM[R1] and stores the result in R0.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

//    R2 is size
//    R1 is array (address obv)
//
//    int @R0 = R1[0];
//    for (int i = 1; i < size; ++i) {
//        if (R1[i] < @R0) { @R0 = R1[i]; }
//    }

//    int @R0 = R1[0];
@R2
D=M
@END
D;JEQ
@R1
A=M
D=M
@R0
M=D

//    for (int i = 1; i < size; ++i) {
@R2
M=M-1
(LOOP)
@R2
D=M
@END
D;JEQ           // if remaining==0 -> end

@R1
M=M+1           // advance pointer to next element (i++)
A=M
D=M             // D = a = *R1

//        if (R1[i] < @R0) {
@A_IS_NEG
D;JLT

@R0
D=M
@B_IS_NEG
D;JLT
@R1
A=M
D=M
@R0
D=D-M
@DO_UPDATE
D;JLT
@SKIPIF
0;JMP

(A_IS_NEG)
@R0
D=M
@B_NONNEG
D;JGE
@R1
A=M
D=M
@R0
D=D-M
@DO_UPDATE
D;JLT
@SKIPIF
0;JMP

(B_NONNEG)
@DO_UPDATE
0;JMP

(B_IS_NEG)
@SKIPIF
0;JMP

//  @R0 = R1[i];
(DO_UPDATE)
@R1
A=M
D=M
@R0
M=D
(SKIPIF) // }
@R2
M=M-1
@LOOP
0;JMP
(END)
@END
0;JMP
