#include <string>

#include "VMTranslator.h"

using namespace std;

namespace {
inline std::string pushD() { return "@SP\n"
                                    "A=M\n"
                                    "M=D\n"
                                    "@SP\n"
                                    "M=M+1\n"; }

inline std::string popToD() { return "@SP\n"
                                     "M=M-1\n"
                                     "A=M\n"
                                     "D=M\n"; }
} // namespace

/**
 * VMTranslator constructor
 */
VMTranslator::VMTranslator() {
    // Your code here
}

/**
 * VMTranslator destructor
 */
VMTranslator::~VMTranslator() {
    // Your code here
}

/** Generate Hack Assembly code for a VM push operation */
string VMTranslator::vm_push(string segment, int offset) {
    string o = to_string(offset);

    if (segment == "constant") {
        return string("@") + o + "\nD=A\n" + pushD();
    }

    auto pushFromBase = [&](const char *baseSym) {
        return string("@") + o + "\nD=A\n@" + string(baseSym) +
               "\nA=M\nA=A+D\nD=M\n" + pushD();
    };

    if (segment == "local")
        return pushFromBase("LCL");
    if (segment == "argument")
        return pushFromBase("ARG");
    if (segment == "this")
        return pushFromBase("THIS");
    if (segment == "that")
        return pushFromBase("THAT");

    if (segment == "temp") {
        string addr = to_string(5 + offset); // R5..R12
        return string("@") + addr + "\nD=M\n" + pushD();
    }

    if (segment == "pointer") {
        const char *tgt = (offset == 0) ? "THIS" : "THAT";
        return string("@") + string(tgt) + "\nD=M\n" + pushD();
    }

    if (segment == "static") {
        string addr = to_string(16 + offset); // simple static map
        return string("@") + addr + "\nD=M\n" + pushD();
    }

    return "";
}

/** Generate Hack Assembly code for a VM pop operation */
string VMTranslator::vm_pop(string segment, int offset) {
    string o = to_string(offset);

    auto popToBase = [&](const char *baseSym) {
        return string("@") + o + "\n"
                                 "D=A\n"
                                 "@" +
               string(baseSym) + "\n"
                                 "A=M\n"
                                 "D=A+D\n"
                                 "@R13\n"
                                 "M=D\n" +
               popToD() +
               "@R13\n"
               "A=M\n"
               "M=D\n";
    };

    if (segment == "local")
        return popToBase("LCL");
    if (segment == "argument")
        return popToBase("ARG");
    if (segment == "this")
        return popToBase("THIS");
    if (segment == "that")
        return popToBase("THAT");

    if (segment == "temp") {
        string addr = to_string(5 + offset);
        return popToD() +
               string("@") + addr + "\n"
                                    "M=D\n";
    }

    if (segment == "pointer") {
        const char *tgt = (offset == 0) ? "THIS" : "THAT";
        return popToD() +
               string("@") + string(tgt) + "\n"
                                           "M=D\n";
    }

    if (segment == "static") {
        string addr = to_string(16 + offset);
        return popToD() +
               string("@") + addr + "\n"
                                    "M=D\n";
    }

    return "";
}

/** Generate Hack Assembly code for a VM add operation */
string VMTranslator::vm_add() {
    return string("@SP\n") +
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "M=M+D\n"
           "@SP\n"
           "M=M+1\n";
}

/** Generate Hack Assembly code for a VM sub operation */
string VMTranslator::vm_sub() {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "M=M-D\n"
           "@SP\n"
           "M=M+1\n";
}

/** Generate Hack Assembly code for a VM neg operation */
string VMTranslator::vm_neg() {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "M=-M\n"
           "@SP\n"
           "M=M+1\n";
}

/** Generate Hack Assembly code for a VM eq operation */
string VMTranslator::vm_eq() {
    static int c = 0;
    string id = to_string(c++);
    string s;
    s += "@SP\nM=M-1\nA=M\nD=M\n";
    s += "@SP\nM=M-1\nA=M\nD=M-D\n";
    s += "@" + string("EQ_TRUE_") + id + "\n";
    s += "D;JEQ\n";
    s += "D=0\n";
    s += "@" + string("EQ_END_") + id + "\n";
    s += "0;JMP\n";
    s += string("(EQ_TRUE_") + id + ")\n";
    s += "D=-1\n";
    s += string("(EQ_END_") + id + ")\n";
    s += "@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    return s;
}

/** Generate Hack Assembly code for a VM gt operation */
string VMTranslator::vm_gt() {
    static int c = 0;
    string id = to_string(c++);
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M-D\n"
           "@GT_TRUE_" +
           id + "\n"
                "D;JGT\n"
                "D=0\n"
                "@GT_END_" +
           id + "\n"
                "0;JMP\n"
                "(GT_TRUE_" +
           id + ")\n"
                "D=-1\n"
                "(GT_END_" +
           id + ")\n"
                "@SP\n"
                "A=M\n"
                "M=D\n"
                "@SP\n"
                "M=M+1\n";
}

/** Generate Hack Assembly code for a VM lt operation */
string VMTranslator::vm_lt() {
    static int c = 0;
    string id = to_string(c++);
    string s;
    s += "@SP\nM=M-1\nA=M\nD=M\n";
    s += "@SP\nM=M-1\nA=M\nD=M-D\n";
    s += "@LT_TRUE_" + id + "\nD;JLT\n";
    s += "D=0\n@LT_END_" + id + "\n0;JMP\n";
    s += "(LT_TRUE_" + id + ")\nD=-1\n";
    s += "(LT_END_" + id + ")\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    return s;
}

/** Generate Hack Assembly code for a VM and operation */
string VMTranslator::vm_and() {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "M=D&M\n"
           "@SP\n"
           "M=M+1\n";
}

/** Generate Hack Assembly code for a VM or operation */
string VMTranslator::vm_or() {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "M=D|M\n"
           "@SP\n"
           "M=M+1\n";
}

/** Generate Hack Assembly code for a VM not operation */
string VMTranslator::vm_not() {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "M=!M\n"
           "@SP\n"
           "M=M+1\n";
}

/** Generate Hack Assembly code for a VM label operation */
string VMTranslator::vm_label(string label) {
    return "(" + label + ")\n";
}

/** Generate Hack Assembly code for a VM goto operation */
string VMTranslator::vm_goto(string label) {
    return "@" + label + "\n0;JMP\n";
}

/** Generate Hack Assembly code for a VM if-goto operation */
string VMTranslator::vm_if(string label) {
    return "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@" +
           label + "\n"
                   "D;JNE\n";
}

/** Generate Hack Assembly code for a VM function operation */
string VMTranslator::vm_function(string function_name, int n_vars) {
    string s;
    s += "(" + function_name + ")\n";
    for (int i = 0; i < n_vars; ++i) {
        s += "@0\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    }
    return s;
}

/** Generate Hack Assembly code for a VM call operation */
string VMTranslator::vm_call(string function_name, int n_args) {
    static int call_id = 0;
    string id = to_string(call_id++);
    string ret = function_name + "$ret." + id;
    string na = to_string(n_args);
    string s;

    s += "@" + ret + "\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    s += "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    s += "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    s += "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    s += "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    s += "@SP\nD=M\n@5\nD=D-A\n@" + na + "\nD=D-A\n@ARG\nM=D\n";
    s += "@SP\nD=M\n@LCL\nM=D\n";
    s += "@" + function_name + "\n0;JMP\n";
    s += "(" + ret + ")\n";

    return s;
}

/** Generate Hack Assembly code for a VM return operation */
string VMTranslator::vm_return() {
    return "@LCL\n"
           "D=M\n"
           "@R13\n"
           "M=D\n"
           "@5\n"
           "A=D-A\n"
           "D=M\n"
           "@R14\n"
           "M=D\n"
           "@SP\n"
           "M=M-1\n"
           "A=M\n"
           "D=M\n"
           "@ARG\n"
           "A=M\n"
           "M=D\n"
           "@ARG\n"
           "D=M\n"
           "@SP\n"
           "M=D+1\n"
           "@R13\n"
           "AM=M-1\n"
           "D=M\n"
           "@THAT\n"
           "M=D\n"
           "@R13\n"
           "AM=M-1\n"
           "D=M\n"
           "@THIS\n"
           "M=D\n"
           "@R13\n"
           "AM=M-1\n"
           "D=M\n"
           "@ARG\n"
           "M=D\n"
           "@R13\n"
           "AM=M-1\n"
           "D=M\n"
           "@LCL\n"
           "M=D\n"
           "@R14\n"
           "A=M\n"
           "0;JMP\n";
}