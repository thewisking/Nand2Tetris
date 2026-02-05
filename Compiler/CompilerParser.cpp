#include "CompilerParser.h"

CompilerParser::CompilerParser(std::list<Token *> tokens) {
    this->tokens = tokens;
    this->it = this->tokens.begin();
}

ParseTree *CompilerParser::compileProgram() {
    if (!have("keyword", "class")) {
        throw ParseException();
    }
    ParseTree *classTree = compileClass();
    if (current() != NULL) {
        throw ParseException();
    }
    return classTree;
}

ParseTree *CompilerParser::compileClass() {
    // class
    Token *classToken = mustBe("keyword", "class");
    ParseTree *classTree = new ParseTree("class", "");
    classTree->addChild(new ParseTree(classToken->getType(), classToken->getValue()));

    // class Name
    Token *nameToken = mustBe("identifier", "");
    classTree->addChild(new ParseTree(nameToken->getType(), nameToken->getValue()));

    // {
    Token *lBrace = mustBe("symbol", "{");
    classTree->addChild(new ParseTree(lBrace->getType(), lBrace->getValue()));

    // classVarDec
    while (have("keyword", "static") || have("keyword", "field")) {
        classTree->addChild(compileClassVarDec());
    }

    // subroutineDec
    while (have("keyword", "constructor") ||
           have("keyword", "function") ||
           have("keyword", "method")) {
        classTree->addChild(compileSubroutine());
    }

    // }
    Token *rBrace = mustBe("symbol", "}");
    classTree->addChild(new ParseTree(rBrace->getType(), rBrace->getValue()));

    return classTree;
}

ParseTree *CompilerParser::compileClassVarDec() {
    ParseTree *node = new ParseTree("classVarDec", "");

    // 'static' or 'field'
    if (have("keyword", "static") || have("keyword", "field")) {
        Token *kw = current();
        next();
        node->addChild(new ParseTree(kw->getType(), kw->getValue()));
    } else {
        throw ParseException();
    }

    // type: 'int' | 'char' | 'boolean' | className (identifier)
    Token *typeTok = nullptr;
    if (have("keyword", "int") || have("keyword", "char") || have("keyword", "boolean") || have("identifier", "")) {
        typeTok = current();
        next();
    } else {
        throw ParseException();
    }
    node->addChild(new ParseTree(typeTok->getType(), typeTok->getValue()));

    // first varName
    Token *varName = mustBe("identifier", "");
    node->addChild(new ParseTree(varName->getType(), varName->getValue()));

    // (',' varName)*
    while (have("symbol", ",")) {
        Token *comma = mustBe("symbol", ",");
        node->addChild(new ParseTree(comma->getType(), comma->getValue()));

        Token *moreName = mustBe("identifier", "");
        node->addChild(new ParseTree(moreName->getType(), moreName->getValue()));
    }

    // ';'
    Token *semi = mustBe("symbol", ";");
    node->addChild(new ParseTree(semi->getType(), semi->getValue()));

    return node;
}

ParseTree *CompilerParser::compileSubroutine() {

    ParseTree *node = new ParseTree("subroutine", "");

    // 'constructor' | 'function' | 'method'
    if (have("keyword", "constructor") || have("keyword", "function") || have("keyword", "method")) {
        Token *subKw = current();
        next();
        node->addChild(new ParseTree(subKw->getType(), subKw->getValue()));
    } else {
        throw ParseException();
    }

    // return type: void | int | char | boolean | className (identifier)
    Token *retType = nullptr;
    if (have("keyword", "void") ||
        have("keyword", "int") ||
        have("keyword", "char") ||
        have("keyword", "boolean") ||
        have("identifier", "")) {
        retType = current();
        next();
        node->addChild(new ParseTree(retType->getType(), retType->getValue()));
    } else {
        throw ParseException();
    }

    // subroutineName
    Token *nameTok = mustBe("identifier", "");
    node->addChild(new ParseTree(nameTok->getType(), nameTok->getValue()));

    // (
    Token *lParen = mustBe("symbol", "(");
    node->addChild(new ParseTree(lParen->getType(), lParen->getValue()));

    // parameterList
    ParseTree *params = compileParameterList();
    node->addChild(params);

    // )
    Token *rParen = mustBe("symbol", ")");
    node->addChild(new ParseTree(rParen->getType(), rParen->getValue()));

    // subroutineBody
    ParseTree *body = compileSubroutineBody();
    node->addChild(body);

    return node;
}

ParseTree *CompilerParser::compileParameterList() {
    ParseTree *node = new ParseTree("parameterList", "");

    // Empty parameter list
    if (!(have("keyword", "int") ||
          have("keyword", "char") ||
          have("keyword", "boolean") ||
          have("identifier", ""))) {
        return node;
    }

    // First (type varName)
    Token *typeTok = current();
    next();
    node->addChild(new ParseTree(typeTok->getType(), typeTok->getValue()));

    Token *nameTok = mustBe("identifier", "");
    node->addChild(new ParseTree(nameTok->getType(), nameTok->getValue()));

    // (',' type varName)*
    while (have("symbol", ",")) {
        Token *comma = mustBe("symbol", ",");
        node->addChild(new ParseTree(comma->getType(), comma->getValue()));

        if (!(have("keyword", "int") ||
              have("keyword", "char") ||
              have("keyword", "boolean") ||
              have("identifier", ""))) {
            throw ParseException();
        }

        Token *moreType = current();
        next();
        node->addChild(new ParseTree(moreType->getType(), moreType->getValue()));

        Token *moreName = mustBe("identifier", "");
        node->addChild(new ParseTree(moreName->getType(), moreName->getValue()));
    }

    return node;
}

ParseTree *CompilerParser::compileSubroutineBody() {
    ParseTree *node = new ParseTree("subroutineBody", "");

    // {
    Token *lBrace = mustBe("symbol", "{");
    node->addChild(new ParseTree(lBrace->getType(), lBrace->getValue()));

    // varDec
    while (have("keyword", "var")) {
        node->addChild(compileVarDec());
    }

    // statements (can be empty, handled inside compileStatements)
    ParseTree *stmts = compileStatements();
    node->addChild(stmts);

    // }
    Token *rBrace = mustBe("symbol", "}");
    node->addChild(new ParseTree(rBrace->getType(), rBrace->getValue()));

    return node;
}

ParseTree *CompilerParser::compileVarDec() {
    ParseTree *node = new ParseTree("varDec", "");

    // var
    Token *varTok = mustBe("keyword", "var");
    node->addChild(new ParseTree(varTok->getType(), varTok->getValue()));

    // type: int | char | boolean | className (identifier)
    Token *typeTok = nullptr;
    if (have("keyword", "int") ||
        have("keyword", "char") ||
        have("keyword", "boolean") ||
        have("identifier", "")) {
        typeTok = current();
        next();
    } else {
        throw ParseException();
    }
    node->addChild(new ParseTree(typeTok->getType(), typeTok->getValue()));

    // first varName
    Token *nameTok = mustBe("identifier", "");
    node->addChild(new ParseTree(nameTok->getType(), nameTok->getValue()));

    // (',' varName)*
    while (have("symbol", ",")) {
        Token *comma = mustBe("symbol", ",");
        node->addChild(new ParseTree(comma->getType(), comma->getValue()));

        Token *moreName = mustBe("identifier", "");
        node->addChild(new ParseTree(moreName->getType(), moreName->getValue()));
    }

    // ;
    Token *semi = mustBe("symbol", ";");
    node->addChild(new ParseTree(semi->getType(), semi->getValue()));

    return node;
}

ParseTree *CompilerParser::compileStatements() {
    ParseTree *node = new ParseTree("statements", "");

    while (true) {
        if (have("keyword", "let")) {
            node->addChild(compileLet());
        } else if (have("keyword", "if")) {
            node->addChild(compileIf());
        } else if (have("keyword", "while")) {
            node->addChild(compileWhile());
        } else if (have("keyword", "do")) {
            node->addChild(compileDo());
        } else if (have("keyword", "return")) {
            node->addChild(compileReturn());
        } else {
            break;
        }
    }

    return node;
}

ParseTree *CompilerParser::compileLet() {
    ParseTree *node = new ParseTree("letStatement", "");

    // let
    Token *letTok = mustBe("keyword", "let");
    node->addChild(new ParseTree(letTok->getType(), letTok->getValue()));

    // varName
    Token *varName = mustBe("identifier", "");
    node->addChild(new ParseTree(varName->getType(), varName->getValue()));

    // optional [ expression ]
    if (have("symbol", "[")) {
        Token *lBracket = mustBe("symbol", "[");
        node->addChild(new ParseTree(lBracket->getType(), lBracket->getValue()));

        ParseTree *indexExpr = compileExpression();
        node->addChild(indexExpr);

        Token *rBracket = mustBe("symbol", "]");
        node->addChild(new ParseTree(rBracket->getType(), rBracket->getValue()));
    }

    // =
    Token *eq = mustBe("symbol", "=");
    node->addChild(new ParseTree(eq->getType(), eq->getValue()));

    // expression
    ParseTree *expr = compileExpression();
    node->addChild(expr);

    // ;
    Token *semi = mustBe("symbol", ";");
    node->addChild(new ParseTree(semi->getType(), semi->getValue()));

    return node;
}

ParseTree *CompilerParser::compileIf() {
    ParseTree *node = new ParseTree("ifStatement", "");

    // if
    Token *ifTok = mustBe("keyword", "if");
    node->addChild(new ParseTree(ifTok->getType(), ifTok->getValue()));

    // (
    Token *lParen = mustBe("symbol", "(");
    node->addChild(new ParseTree(lParen->getType(), lParen->getValue()));

    // expression
    ParseTree *condExpr = compileExpression();
    node->addChild(condExpr);

    // )
    Token *rParen = mustBe("symbol", ")");
    node->addChild(new ParseTree(rParen->getType(), rParen->getValue()));

    // {
    Token *lBrace = mustBe("symbol", "{");
    node->addChild(new ParseTree(lBrace->getType(), lBrace->getValue()));

    // statements
    ParseTree *ifStmts = compileStatements();
    node->addChild(ifStmts);

    // }
    Token *rBrace = mustBe("symbol", "}");
    node->addChild(new ParseTree(rBrace->getType(), rBrace->getValue()));

    // optional else
    if (have("keyword", "else")) {
        Token *elseTok = mustBe("keyword", "else");
        node->addChild(new ParseTree(elseTok->getType(), elseTok->getValue()));

        Token *elseLBrace = mustBe("symbol", "{");
        node->addChild(new ParseTree(elseLBrace->getType(), elseLBrace->getValue()));

        ParseTree *elseStmts = compileStatements();
        node->addChild(elseStmts);

        Token *elseRBrace = mustBe("symbol", "}");
        node->addChild(new ParseTree(elseRBrace->getType(), elseRBrace->getValue()));
    }

    return node;
}

ParseTree *CompilerParser::compileWhile() {
    ParseTree *node = new ParseTree("whileStatement", "");

    // while
    Token *whileTok = mustBe("keyword", "while");
    node->addChild(new ParseTree(whileTok->getType(), whileTok->getValue()));

    // (
    Token *lParen = mustBe("symbol", "(");
    node->addChild(new ParseTree(lParen->getType(), lParen->getValue()));

    // expression
    ParseTree *cond = compileExpression();
    node->addChild(cond);

    // )
    Token *rParen = mustBe("symbol", ")");
    node->addChild(new ParseTree(rParen->getType(), rParen->getValue()));

    // {
    Token *lBrace = mustBe("symbol", "{");
    node->addChild(new ParseTree(lBrace->getType(), lBrace->getValue()));

    // statments
    ParseTree *stmts = compileStatements();
    node->addChild(stmts);

    // }
    Token *rBrace = mustBe("symbol", "}");
    node->addChild(new ParseTree(rBrace->getType(), rBrace->getValue()));

    return node;
}

ParseTree *CompilerParser::compileDo() {
    ParseTree *node = new ParseTree("doStatement", "");

    // do
    Token *doTok = mustBe("keyword", "do");
    node->addChild(new ParseTree(doTok->getType(), doTok->getValue()));

    // expression
    ParseTree *expr = compileExpression();
    node->addChild(expr);

    // ;
    Token *semi = mustBe("symbol", ";");
    node->addChild(new ParseTree(semi->getType(), semi->getValue()));

    return node;
}

ParseTree *CompilerParser::compileReturn() {
    ParseTree *node = new ParseTree("returnStatement", "");

    // return
    Token *retTok = mustBe("keyword", "return");
    node->addChild(new ParseTree(retTok->getType(), retTok->getValue()));

    // optional expression?
    if (!have("symbol", ";")) {
        ParseTree *expr = compileExpression();
        node->addChild(expr);
    }

    // ;
    Token *semi = mustBe("symbol", ";");
    node->addChild(new ParseTree(semi->getType(), semi->getValue()));

    return node;
}

ParseTree *CompilerParser::compileExpression() {
    ParseTree *node = new ParseTree("expression", "");

    // expression
    ParseTree *firstTerm = compileTerm();
    node->addChild(firstTerm);

    // op: + - * / & | < > =
    while (have("symbol", "+") ||
           have("symbol", "-") ||
           have("symbol", "*") ||
           have("symbol", "/") ||
           have("symbol", "&") ||
           have("symbol", "|") ||
           have("symbol", "<") ||
           have("symbol", ">") ||
           have("symbol", "=")) {

        Token *opTok = current();
        next();
        node->addChild(new ParseTree(opTok->getType(), opTok->getValue()));

        ParseTree *nextTerm = compileTerm();
        node->addChild(nextTerm);
    }

    return node;
}

ParseTree *CompilerParser::compileTerm() {
    ParseTree *node = new ParseTree("term", "");
    Token *tok = current();

    if (tok == NULL) {
        throw ParseException();
    }

    // integerConstant or stringConstant
    if (tok->getType() == "integerConstant" || tok->getType() == "stringConstant") {
        next();
        node->addChild(new ParseTree(tok->getType(), tok->getValue()));
        return node;
    }

    // keywordConstant
    if (tok->getType() == "keyword" &&
        (tok->getValue() == "true" ||
         tok->getValue() == "false" ||
         tok->getValue() == "null" ||
         tok->getValue() == "this" ||
         tok->getValue() == "skip")) {
        next();
        node->addChild(new ParseTree(tok->getType(), tok->getValue()));
        return node;
    }

    // ( expression )
    if (have("symbol", "(")) {
        Token *lParen = mustBe("symbol", "(");
        node->addChild(new ParseTree(lParen->getType(), lParen->getValue()));

        ParseTree *expr = compileExpression();
        node->addChild(expr);

        Token *rParen = mustBe("symbol", ")");
        node->addChild(new ParseTree(rParen->getType(), rParen->getValue()));

        return node;
    }

    // unaryOp term  - or ~
    if (have("symbol", "-") || have("symbol", "~")) {
        Token *unaryTok = current();
        next();
        node->addChild(new ParseTree(unaryTok->getType(), unaryTok->getValue()));

        ParseTree *subTerm = compileTerm();
        node->addChild(subTerm);

        return node;
    }

    // identifier, array access, or subroutineCall
    if (have("identifier", "")) {
        Token *idTok = current();
        next();
        node->addChild(new ParseTree(idTok->getType(), idTok->getValue()));

        // varName [ expression ]
        if (have("symbol", "[")) {
            Token *lBracket = mustBe("symbol", "[");
            node->addChild(new ParseTree(lBracket->getType(), lBracket->getValue()));

            ParseTree *indexExpr = compileExpression();
            node->addChild(indexExpr);

            Token *rBracket = mustBe("symbol", "]");
            node->addChild(new ParseTree(rBracket->getType(), rBracket->getValue()));

            return node;
        }

        // subroutineCall  identifier ( expressionList )
        if (have("symbol", "(")) {
            Token *lParen = mustBe("symbol", "(");
            node->addChild(new ParseTree(lParen->getType(), lParen->getValue()));

            ParseTree *args = compileExpressionList();
            node->addChild(args);

            Token *rParen = mustBe("symbol", ")");
            node->addChild(new ParseTree(rParen->getType(), rParen->getValue()));

            return node;
        }

        // subroutineCall  identifier . identifier ( expressionList )
        if (have("symbol", ".")) {
            Token *dot = mustBe("symbol", ".");
            node->addChild(new ParseTree(dot->getType(), dot->getValue()));

            Token *subName = mustBe("identifier", "");
            node->addChild(new ParseTree(subName->getType(), subName->getValue()));

            Token *lParen = mustBe("symbol", "(");
            node->addChild(new ParseTree(lParen->getType(), lParen->getValue()));

            ParseTree *args = compileExpressionList();
            node->addChild(args);

            Token *rParen = mustBe("symbol", ")");
            node->addChild(new ParseTree(rParen->getType(), rParen->getValue()));

            return node;
        }

        // varName
        return node;
    }

    // nothing matched
    throw ParseException();
}

ParseTree *CompilerParser::compileExpressionList() {
    ParseTree *node = new ParseTree("expressionList", "");

    // Empty list: next token is ')' or no tokens left
    if (have("symbol", ")") || current() == NULL) {
        return node;
    }

    // First expression
    ParseTree *expr = compileExpression();
    node->addChild(expr);

    // (',' expression)*
    while (have("symbol", ",")) {
        Token *comma = mustBe("symbol", ",");
        node->addChild(new ParseTree(comma->getType(), comma->getValue()));

        ParseTree *nextExpr = compileExpression();
        node->addChild(nextExpr);
    }

    return node;
}

void CompilerParser::next() {
    if (it != tokens.end()) {
        it++;
    }
    return;
}

Token *CompilerParser::current() {
    if (it == tokens.end()) {
        return NULL;
    }
    return *it;
}

bool CompilerParser::have(std::string expectedType, std::string expectedValue) {
    Token *token = current();
    if (token == NULL ||
        (!expectedType.empty() && token->getType() != expectedType) ||
        (!expectedValue.empty() && token->getValue() != expectedValue)) {
        return false;
    }
    return true;
}

Token *CompilerParser::mustBe(std::string expectedType, std::string expectedValue) {
    if (!have(expectedType, expectedValue)) {
        throw ParseException();
    }

    Token *token = current();
    next();
    return token;
}

const char *ParseException::what() {
    return "An Exception occurred while parsing!";
}
