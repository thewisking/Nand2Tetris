// Sorts the array of length R2 whose first element is at RAM[R1] in ascending order in place. Sets R0 to True (-1) when complete.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

// Check if R2 is less than two. one is alr sorted 0 and negatives not allowed so we assume it's sorted for that too
@R2
D=M-1
@END
D;JLE

//    for (int j = 0; j < n - 1; ++j) {
//  int j = 0
@j
M=0
(STARTOUTERLOOP)
//  if (j >= n - 1) goto ENDOUTERLOOP
@R2
D=M-1
@j
D=M-D
@ENDOUTERLOOP
D;JGE

//        for (int i = 0; i < (n - j - 1); ++i) {
//  int i = 0
@i
M=0
(STARTINNERLOOP)

//  if (i >= n - j - 1) goto ENDINNERLOOP
@R2
D=M
@j
D=D-M
D=D-1
@i
D=M-D
@ENDINNERLOOP
D;JGE

// R3 = array + i
@i
D=M
@R1
D=D+M
@R3
M=D

//            if (array[i] > array[i + 1]) {
// Check sign
@R13
M=1
@R14
M=1

@R3
A=M
D=M
@SKIPLEFT
D;JGE
@R13
M=M-1
(SKIPLEFT)

@R3
A=M
A=A+1
D=M
@SKIPRIGHT
D;JGE
@R14
M=M-1
(SKIPRIGHT)

@R14
D=M
@R13
D=D-M
@IFSAMESIGN
D;JEQ

(IFDIFFERENTSIGN) // this label is unused but i keep it for clarity
@R13
D=M
@IFNOTCASE
D;JEQ
@IFCASE
0;JMP

(IFSAMESIGN)
@R3
A=M
D=M
A=A+1
D=D-M
@IFNOTCASE
D;JLE

(IFCASE)
// R4 = array[i]
@R3
A=M
D=M
@R4
M=D

// array[i] = array[i+1]
@R3
A=M
A=A+1
D=M
@R3
A=M
M=D

// array[i+1] = R4
@R4
D=M
@R3
A=M
A=A+1
M=D

//            }
(IFNOTCASE)

//        }
//  ++i
@i
M=M+1
@STARTINNERLOOP
0;JMP
(ENDINNERLOOP)

//    }
//  ++j
@j
M=M+1
@STARTOUTERLOOP
0;JMP

(ENDOUTERLOOP)
(SKIP)
@R0
M=-1

(END)
@END
0;JMP