/* Номер студенческого билета положителен. 
   Если оценка не выставлена, то возвращается 0. */

#include <iostream>
#include <cstring>


class Statement {
    struct Data {
        unsigned long number;
        int mark; 
    };
    size_t size;
    Data *buf;

public:
    Statement(size_t );
    Statement(const Statement & );
    void add_number(size_t ,unsigned long );
    Statement & operator =(const Statement & );
    ~Statement();  
    class Mark {
        friend class Statement;
        int & mark;
        Mark(int & m) : mark(m) {};
    public:
        operator int();
        int operator =(int );
    };   
    Mark operator [](unsigned long );
    int operator [](unsigned long ) const;
};


Statement::Statement(size_t s) {
    this->size = s;
    this->buf = new Data[s];
    for (size_t i = 0; i < this->size; ++i) {
        this->buf[i].number = 0;
    }
}


Statement::Statement(const Statement & stm) {
    this->size = stm.size;
    this->buf = new Data[stm.size];
    memcpy(this->buf, stm.buf, stm.size * sizeof(*this->buf));
}


Statement & Statement::operator =(const Statement & stm) {
    if (this == &stm) {
        return *this;
    }
    delete [] this->buf;
    this->size = stm.size;
    this->buf = new Data[stm.size];
    memcpy(this->buf, stm.buf, stm.size * sizeof(*this->buf));
    return *this;
}


void Statement::add_number(size_t pos, unsigned long num) {
    if (pos >= this->size) {
        throw "out of range";
    }
    if (this->buf[pos].number != 0) {
        throw "occupied position";
    }
    this->buf[pos].number = num;
    this->buf[pos].mark = 0;
}


Statement::Mark Statement::operator [](unsigned long num) {
    for (size_t i = 0; i < this->size; ++i) {
        if (this->buf[i].number == num) {
            return Mark(this->buf[i].mark);
        }
    }
    throw "invalid number";
}


int Statement::operator [](unsigned long num) const {
    for (size_t i = 0; i < this->size; ++i) {
        if (this->buf[i].number == num) {
            return this->buf[i].mark;
        }
    }
    throw "invalid number";
}


int Statement::Mark::operator =(int m) {
    if (m < 2 || m > 5) {
        throw "invalid mark";
    }
    this->mark = m;
    return m;
}


Statement::Mark::operator int() {
    return this->mark;
}


Statement::~Statement() {
    delete [] this->buf;
}


int main() {
    Statement grades(10);
    try {
        grades.add_number(2, 1234521); //добавляет номер 1234521 на 2 позицию
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        grades.add_number(4, 3283942); //добавляет номер 3283942 на 4 позицию
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        grades.add_number(2, 7863453); //добавляет номер 7863453 на 2 позицию
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        grades[3283942] = 5; //устанавливает оценку "отлично" у студента с номером 3283942
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        grades[1234521] = 6; //устанавливает оценку 6 у студента с номером 7863453
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        grades[5847361] = 5; //устанавливает оценку "отлично" у студента с номером 5847361
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        std::cout << grades[3283942] << std::endl; //печатает оценку у студента с номером 3283942
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    const Statement constgrades = grades; //копия ведомости
    try {
        std::cout << constgrades[3283942] << std::endl; //печатает оценку у студента с номером 3283942
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        std::cout << constgrades[2736343] << std::endl; //печатает оценку у студента с номером 2736343
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        std::cout << constgrades[1234521] << std::endl; //печатает оценку у студента с номером 1234521
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    return 0;
}

