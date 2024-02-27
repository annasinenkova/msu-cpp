/*   -std=c++11

S -> CTYPE VARS ";"
CTYPE -> CONST TYPE CONST
TYPE -> "int" | "long" | "long" "int"
CONST -> "const" CONST | eps
VARS -> VAR | VARS "," VAR
VAR -> "*" CONST VAR | V
V -> F MAS
F -> "id" | "(" VAR ")"
MAS -> "[" "num" "]" MAS | "(" ARGS ")" MAS | "(" ")" MAS | eps
ARGS -> CTYPE VAR "," ARGS | CTYPE VAR

------------------------------------------------------------------
проверка на пересечение first
------------------------------------------------------------------
1. TYPE -> "int" | "long" | "long" "int"
    first(int) = { i }
    first(long) = { l }
    first(long int) = { l }  //пересечение
        TYPE -> "int" | "long" NEWTYPE
        NEWTYPE -> eps | "int"
    
2. VARS -> VAR | VARS "," VAR  //леворекурсивна
        VARS -> VAR NEWVARS
        NEWVARS ->  "," VAR NEWVARS | eps
    
3. VAR -> "*" CONST VAR | V
    first("*" CONST VAR) = { * }
    first(V) = { i | (}
    
4. F -> "id" | "(" VAR ")"
    first("id") = { i }
    first("(" VAR ")") = { ( }
    
5. MAS -> "[" "num" "]" MAS | "(" ARGS ")" MAS | "(" ")" MAS | eps
    first("[" "num" "]" MAS) = { [ }
    first("(" ARGS ")" MAS) = { ( }
    first("(" ")" MAS) = { ( }  //пересечение
        MAS -> "[" "num" "]" MAS | "(" NEWMAS | eps
        NEWMAS ->  ARGS ")" MAS | ")" MAS
    first(ARGS ")" MAS) = { c | i | l }
    first(")" MAS) = { ) }
    
6. ARGS -> CTYPE VAR "," ARGS | CTYPE VAR
    first(CTYPE VAR "," ARGS) = { c | i | l }
    first(CTYPE VAR) = { c | i | l }  //пересечение
        ARGS -> CTYPE VAR NEWARGS
        NEWARGS -> "," ARGS | eps
------------------------------------------------------------------   
    
S -> CTYPE VARS ";"
CTYPE -> CONST TYPE CONST
TYPE -> "int" | "long" NEWTYPE
NEWTYPE -> "int" | eps
CONST -> "const" CONST | eps
VARS -> VAR NEWVARS
NEWVARS ->  "," VAR NEWVARS | eps
VAR -> "*" CONST VAR | V
V -> F MAS
F -> "id" | "(" VAR ")"
MAS -> "[" "num" "]" MAS | "(" NEWMAS | eps
NEWMAS ->  ARGS ")" MAS | ")" MAS
ARGS -> CTYPE VAR NEWARGS
NEWARGS -> "," ARGS | eps

------------------------------------------------------------------
проверка на пересечение first-follow
------------------------------------------------------------------
1. NEWTYPE -> "int" | eps
    first(NEWTYPE) = { i }
    follow(NEWTYPE) = { c }
    
2. CONST -> "const" CONST | eps
    first(CONST) = { c }
    follow(CONST) = { i | l | * | ( }
    
3. NEWVARS ->  "," VAR NEWVARS | eps
    first(NEWVARS) = { , }
    follow(NEWVARS) = { ; }
    
4. MAS -> "[" "num" "]" MAS | "(" NEWMAS | eps
    first(MAS) = { [ | ( }
    follow(MAS) = { , | ) }
    
5. NEWMAS ->  ARGS ")" MAS | ")" MAS
    first(ARGS ")" MAS) = { c | i | l }
    first(")" MAS) = { ) }
    
    
6. NEWARGS -> "," ARGS | eps
    first(NEWARGS) = { , }
    follow(NEWARGS) = { ) }
----------------------------------------------------------------
*/


#include <iostream> 
#include <utility>
#include <vector>
#include <iomanip>


enum lex_type {
    LEX_NULL,
    LEX_FIN,
    LEX_INT,
    LEX_LONG,
    LEX_CONST,
    LEX_NUM,
    LEX_ID,
    LEX_TIMES,
    LEX_COMMA,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_LBRACK,
    LEX_RBRACK,
    LEX_SEMICOLON
};


class Lex {
    lex_type t_lex;
    size_t v_lex;
public:
    Lex(lex_type t = LEX_NULL, size_t v = 0) : t_lex(t), v_lex(v) {}
    lex_type get_type();
    size_t get_value();
};


class Ident {
    std::string name;
    bool declare;
    lex_type type;
    bool assign;
    int value;
public:
    Ident(const std::string & n) : name(n), declare(false), assign(false) {}
    Ident() : declare(false), assign(false) {}
    std::string get_name();
};


class Scanner {
    enum state { H, IDENT, NUMB, DELIM };
    static std::vector<std::pair<std::string, lex_type> > TD;
    static std::vector<std::pair<std::string, lex_type> > TW;   
    static std::vector<Ident> TID;
    state CS;
    char c;
    size_t str, pos;
    size_t put_ident(const std::string &);
    size_t look(const std::string &, std::vector<std::pair<std::string, lex_type> >);
    void gc();
public:
    Scanner();
    Lex get_lex();
};


lex_type Lex::get_type() { 
    return t_lex; 
}


size_t Lex::get_value() {
    return v_lex;
}


std::string Ident::get_name() {
    return name;
}


std::vector<Ident> Scanner::TID;
std::vector<std::pair<std::string, lex_type> > Scanner::TW = { {"", LEX_NULL}, {"int", LEX_INT}, {"long", LEX_LONG}, {"const", LEX_CONST} };
std::vector<std::pair<std::string, lex_type> > Scanner::TD = { {"", LEX_NULL}, {"(", LEX_LPAREN}, {")", LEX_RPAREN}, {"[", LEX_LBRACK}, {"]", LEX_RBRACK}, {"*", LEX_TIMES}, {",", LEX_COMMA}, {";", LEX_SEMICOLON} };


Scanner::Scanner() {
    CS = H;
    gc();
}


size_t Scanner::put_ident(const std::string & name) {
    size_t i;
    for (i = 0; i < TID.size(); ++i) {
        if (name == TID[i].get_name()) {
            return i;
        }
    }
    Ident id(name);
    TID.push_back(id);
    return i; 
} 


size_t Scanner::look(const std::string & s, std::vector<std::pair<std::string, lex_type> > s_buf) {
    size_t i = 0;
    for (i = 0; i < s_buf.size(); ++i) {
        if (s == s_buf[i].first) {
            return i;
        }
    }
    return 0;
}


void Scanner::gc() {
    c = getchar();
    ++pos;
}


Lex Scanner::get_lex() {
    std::string buf;
    size_t pos_in_t = 0;
    size_t num = 0;
    CS = H;
    do {
        switch (CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                    gc();
                }
                else if (isalpha(c)) {
                    buf.push_back(c);
                    gc();
                    CS = IDENT;
                }
                else if (c == '0') {
                    gc();
                    return Lex(LEX_NUM, 0);
                }
                else if (isdigit(c)) {
                    num = c - '0';
                    gc();
                    CS = NUMB;
                }
                else {
                    CS = DELIM;
                }
                break;
            case IDENT:
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c);
                    gc();
                }
                else {
                    pos_in_t = look(buf, TW);
                    if (pos_in_t) {
                        return Lex(TW[pos_in_t].second, pos_in_t);
                    }
                    pos_in_t = put_ident(buf);
                    return Lex(LEX_ID, pos_in_t);
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    num = num * 10 + (c - '0');
                    gc();
                }
                else {
                    return Lex(LEX_NUM, num);
                }
                break;
            case DELIM:
                buf.push_back(c);
                pos_in_t = look(buf, TD);
                if (pos_in_t) {
                    gc();
                    return Lex(TD[pos_in_t].second, pos_in_t);
                }
                else {
                    throw c;
                }
                break;
            default:
                throw c;
        }
    } while(true);
}


class Parser {
    Lex curr_lex;
    lex_type c_type;
    int c_val;
    Scanner scan;
    void S();
    void CTYPE();
    void TYPE();
    void NEWTYPE();
    void CONST();
    void VARS();
    void NEWVARS();
    void VAR();
    void V();
    void F();
    void MAS();
    void NEWMAS();
    void ARGS();
    void NEWARGS();
    void gl() {
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = curr_lex.get_value();
    }

public:
    void analyze();
};


void Parser::analyze() {
    gl();
    S();
    std::cout << "YES" << std::endl;
}


void Parser::S() { //S -> CTYPE VARS ";"
    CTYPE();
    VARS();
    if (c_type != LEX_SEMICOLON) {
        throw curr_lex;
    }
}


void Parser::CTYPE() { //CTYPE -> CONST TYPE CONST
    CONST();
    TYPE();
    CONST();
}


void Parser::TYPE() { //TYPE -> "int" | "long" NEWTYPE
    if (c_type == LEX_INT) {
        gl();
    }
    else if (c_type == LEX_LONG) {
        gl();
        NEWTYPE();
    }
    else {
        throw curr_lex;
    }
}


void Parser::NEWTYPE() { //NEWTYPE -> "int" | eps
    if (c_type == LEX_INT) {
        gl();
    }
}


void Parser::CONST() { //CONST -> "const" CONST | eps
    if (c_type == LEX_CONST) {
        gl();
        CONST();
    }
}


void Parser::VARS() { //VARS -> VAR NEWVARS
    VAR();
    NEWVARS();
}


void Parser::NEWVARS() { //NEWVARS ->  "," VAR NEWVARS | eps
    if (c_type == LEX_COMMA) {
        gl();
        VAR();
        NEWVARS();
    }
}


void Parser::VAR() { //VAR -> "*" CONST VAR | V
    if (c_type == LEX_TIMES) {
        gl();
        CONST();
        VAR();
    }
    else {
        V();
    }
}


void Parser::V() { //V -> F MAS
    F();
    MAS();
}


void Parser::F() { //F -> "id" | "(" VAR ")"
    if (c_type != LEX_ID && c_type != LEX_LPAREN) {
        throw curr_lex;
    }

    if (c_type == LEX_LPAREN) {
        gl();
        VAR();
        if (c_type != LEX_RPAREN) {
            throw curr_lex;
        }
    }
    gl();
}


void Parser::MAS() { //MAS -> "[" "num" "]" MAS | "(" NEWMAS | eps
    if (c_type == LEX_LBRACK) {
        gl();
        if (c_type != LEX_NUM) {
            throw curr_lex;
        }
        gl();
        if (c_type != LEX_RBRACK) {
            throw curr_lex;
        }
        gl();
        MAS();
    }
    else if (c_type == LEX_LPAREN) {
        gl();
        NEWMAS();
    }
}


void Parser::NEWMAS() { //NEWMAS ->  ARGS ")" MAS | ")" MAS
    if (c_type != LEX_RPAREN) {
        ARGS();
    }
    if (c_type != LEX_RPAREN) {
        throw curr_lex;
    }
    gl();
    MAS();
}


void Parser::ARGS() { //ARGS -> CTYPE VAR NEWARGS
    CTYPE();
    VAR();
    NEWARGS();
}


void Parser::NEWARGS() { //NEWARGS -> "," ARGS | eps
    if (c_type == LEX_COMMA) {
        gl();
        ARGS();
    }
}


int main() {
    Parser p;
    try {
        p.analyze();
    }
    catch(...) {
        std::cout << "NO" << std::endl;
    } 
    return 0;
}
