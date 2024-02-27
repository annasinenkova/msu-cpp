#include <iostream>
#include <cstring>

class Queue {
    int *buf;
    size_t last;
    size_t first;
    size_t maxsize;
    size_t elemnum;

public:
    Queue(size_t size);
    Queue(const Queue & q);
    size_t size() const;
    size_t capacity() const;
    Queue & operator =(const Queue & q);
    Queue & operator <<(int value);
    Queue & operator >>(int & value);
    ~Queue();
};


Queue::Queue(size_t size) {
    this->last = size;
    this->first = 0;
    this->maxsize = size;
    this->elemnum = 0;
    this->buf = new int[size];
};


Queue::Queue(const Queue & q) {
    this->last = q.last;
    this->first = q.first;
    this->maxsize = q.maxsize;
    this->elemnum = q.elemnum;
    this->buf = new int[q.maxsize];
    memcpy(this->buf, q.buf, q.maxsize);
};


size_t Queue::size() const {
    return this->elemnum;
}


size_t Queue::capacity() const {
    return this->maxsize;
}


Queue & Queue::operator <<(int value) {
    ++this->elemnum;
    if (++this->last >= this->maxsize) {
        this->last = 0;
    }
    this->buf[this->last] = value;
    return *this;
}


Queue & Queue::operator >>(int & value) {
    --this->elemnum;
    if (this->first >= this->maxsize) {
        this->first = 0;
    }
    value = this->buf[this->first++];
    return *this;
}


Queue & Queue::operator =(const Queue & q) {
    if (this == &q) {
        return *this;
    }
    delete [] this->buf;
    this->last = q.last;
    this->first = q.first;
    this->maxsize = q.maxsize;
    this->elemnum = q.elemnum;
    this->buf = new int[q.maxsize];
    memcpy(this->buf, q.buf, q.maxsize);
    return *this;
}


Queue::~Queue() {
    delete [] this->buf;
}


int main() {
    Queue q1(5); // создание пустой очереди с максимальным размером 5
    int num = 0;
    q1 << num << 25; // добавление чисел в очередь (num, затем 25)
    int num1, num2;
    q1 >> num1 >> num2; // извлечение чисел из очереди
    std::cout << "num1 = " << num1 << " num2 = " << num2 << std::endl;
    q1 << num << 25;
    const Queue q2 = q1; // создание копии
    int s = q2.size(); // получение текущего размера очереди
    std::cout << "size = " << s << std::endl;
    int z = q2.capacity(); // получение максимального размера очереди
    std::cout << "capacity = " << z << std::endl;
    return 0;
}


