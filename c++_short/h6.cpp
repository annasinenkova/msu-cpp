/* @ - символ конца строки */

#include <iostream> 
#include <utility>
#include <vector>
#include <iomanip>


enum lex_type {
    LEX_NULL,
    LEX_FIN,
    LEX_NUM,
    LEX_DEL,
    LEX_WRD,
    LEX_ID 
};


class Lex {
    lex_type t_lex;
    size_t v_lex;
    size_t str_lex;
    size_t pos_lex;
public:
    Lex(lex_type t = LEX_NULL, size_t v = 0, size_t s = 0, size_t p = 0) : t_lex(t), v_lex(v), str_lex(s), pos_lex(p) {}
    lex_type get_type();
    int get_value();
    size_t get_str();
    size_t get_pos();
    friend std::ostream & operator <<(std::ostream & , Lex);
};


class Ident {
    std::string name;
    bool declare;
    lex_type type;
    bool assign;
    int value;
public:
    Ident(std::string n) : name(n), declare(false), assign(false) {}
    Ident() : declare(false), assign(false) {}
    std::string get_name();
    void put_name(std::string);
    bool get_declare();
    void put_declare();
    lex_type get_type();
    void put_type(lex_type t);
    bool get_assign();
    void put_assign();
    int get_value();
    void put_value(int);
};


class Scanner {
    enum state { H, IDENT, NUMB, DELIM };
    static std::vector<std::string> TD;
    static std::vector<std::string> TW;   
    static std::vector<Ident> TID;
    state CS;
    char c;
    size_t str, pos;
    size_t put_ident(std::string);
    size_t look(std::string, std::vector<std::string>);
    void gc();
public:
    Scanner();
    Lex get_lex();
    friend std::ostream & operator <<(std::ostream & , Lex);
};


lex_type Lex::get_type() { 
    return t_lex; 
}


int Lex::get_value() {
    return v_lex;
}


size_t Lex::get_str() {
    return str_lex;
}


size_t Lex::get_pos() {
    return pos_lex;
}


std::string Ident::get_name() {
    return name;
}


void Ident::put_name(std::string n) {
    name = n;
}


bool Ident::get_declare() {
    return declare;
}


void Ident::put_declare() {
    declare = true;
}


lex_type Ident::get_type() {
    return type;
}


void Ident::put_type(lex_type t) {
    type = t;
}


bool Ident::get_assign() {
    return assign;
}


void Ident::put_assign() {
    assign = true;
}


int Ident::get_value() {
    return value;
}


void Ident::put_value(int v) {
    value = v;
}


std::vector<Ident> Scanner::TID;
std::vector<std::string> Scanner::TW = { "", "div" };
std::vector<std::string> Scanner::TD = { "", "(", ")", "+", "-", "*" };


Scanner::Scanner() {
    CS = H;
    str = 1;
    pos = 0;
    gc();
}


size_t Scanner::put_ident(std::string name) {
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


size_t Scanner::look(std::string s, std::vector<std::string> s_buf) {
    size_t i = 0;
    for (i = 0; i < s_buf.size(); ++i) {
        if (s == s_buf[i]) {
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
    size_t num = 0;
    size_t pos_in_t = 0;
    size_t p_lex = pos; 
    size_t s_lex = str;
    CS = H;
    do {
        switch (CS) {
            case H:
                if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
                    if (c == '\n') {
                        ++str;
                        pos = 0;
                    }
                    gc();
                    p_lex = pos;
                    s_lex = str;
                }
                else if (isalpha(c)) {
                    buf.push_back(c);
                    gc();
                    CS = IDENT;
                }
                else if (c == '0') {
                    gc();
                    return Lex(LEX_NUM, 0, s_lex, p_lex);
                }
                else if (isdigit(c)) {
                    num = c - '0';
                    gc();
                    CS = NUMB;
                }
                else if (c == '@') {
                    return Lex(LEX_FIN, 0, s_lex, p_lex);
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
                        return Lex(LEX_WRD, pos_in_t, s_lex, p_lex);
                    }
                    pos_in_t = put_ident(buf);
                    return Lex(LEX_ID, pos_in_t, s_lex, p_lex);
                }
                break;
            case NUMB:
                if (isdigit(c)) {
                    num = num * 10 + (c - '0');
                    gc();
                }
                else {
                    return Lex(LEX_NUM, num, s_lex, p_lex);
                }
                break;
            case DELIM:
                buf.push_back(c);
                pos_in_t = look(buf, TD);
                if (pos_in_t) {
                    gc();
                    return Lex(LEX_DEL, pos_in_t, s_lex, p_lex);
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


std::ostream & operator <<(std::ostream & out, Lex l) {
    if (l.t_lex == LEX_NUM) {
        out << std::setw(10) << "const" << std::setw(10) << l.v_lex;
    }
    else if (l.t_lex == LEX_DEL) {
        out << std::setw(10) << "delimiter" << std::setw(10) << Scanner::TD[l.v_lex];
    }
    else if (l.t_lex == LEX_WRD) {
        out << std::setw(10) << "serv word" << std::setw(10) << Scanner::TW[l.v_lex];
    }
    else if (l.t_lex == LEX_ID) {
        out << std::setw(10) << "identifier" << std::setw(10) << Scanner::TID[l.v_lex].get_name();
    }
    else if (l.t_lex == LEX_FIN) {
        out << std::setw(10) << "final" << std::setw(10) << "@";
    }
    out << std::setw(5) << l.str_lex << std::setw(5) << l.pos_lex;
    return out;
}


int main() {
    Scanner s;
    Lex l;
    try {
        while (true) {
            l = s.get_lex();
            std::cout << l << std::endl;
        }
    }
    catch(char c) {
        std::cout << "error " << c << std::endl;
    }   
    return 0;    
}
