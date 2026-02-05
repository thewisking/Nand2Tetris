//    int result = 0;
@0
M=0

//    int SIGN = -1;
@SIGN
M=-1

// check if R1 is negative
//    if (R1 < 0) {
//        R1 = -R1;
//        SIGN++;}

@1
D=M
@R1Skip
D;JGE
@1
M=-M
@SIGN
M=M+1
(R1Skip)

// check if R2 is negative
//    if (R2 < 0) {
//        R2 = -R2;
//        SIGN++;}
@2
D=M
@R2Skip
D;JGE
@2
M=-M
@SIGN
M=M+1
(R2Skip)


// Bitwise multiplication

//    int R1shift = R1;
@1
D=M
@R1shift
M=D
//    int mask = 1;
@mask
M=1


//    while (mask != 0) {
//        if (R2 & mask) {
//            R0 = R0 + R1shift;
//        }
//        R1shift = R1shift << 1;
//        mask = mask << 1;
//    }

(LoopStart)
//        if (R2 & mask) { R0 = R0 + R1shift; }
@mask
D=M
@2
D=D&M
@SKIPR1SHIFT
D;JEQ
@R1shift
D=M
@0
M=M+D
(SKIPR1SHIFT)

// R1shift = R1shift << 1;
@R1shift
D=M
M=D+M

// mask = mask << 1;
@mask
D=M
M=D+M

D=M
@LoopStart
D;JNE


// Apply sign if needed
//    if (SIGN == 0) { R0 = -R0; }
@SIGN
D=M
@SKIPSIGN
D;JNE
@0
M=-M
(SKIPSIGN)

//    return R0;
@EXTRATICKS
M=0
(END)
@EXTRATICKS
M=M+1
@END
0;JMP


// 16 SIGN
// 17 R1shift
// 18 Mask