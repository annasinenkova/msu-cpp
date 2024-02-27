/* При сложении векторов разных типов результат будет иметь тип первого параметра */

#include <iostream>
#include <cstdlib>

template <typename T>
class MyVector {
public:
    MyVector(size_t);
    MyVector(size_t, T);
    MyVector(const MyVector<T> &);
    template <typename S> MyVector(const MyVector<S> &);
    MyVector<T> & operator=(const MyVector<T> &);
    template <typename S> MyVector<T> & operator=(const MyVector<S> &);
    T & operator[](size_t);
    T operator[](size_t) const;
    size_t size() const;
    ~MyVector();  
    
    class iterator {
    public:
        iterator(T v, T *c, T *e) : value(v), cur(c), end(e) {}
        bool operator==(const iterator &) const;
        bool operator!=(const iterator &) const;
        iterator & operator++();
        iterator operator++(int);
        T & operator*();
    private:
        T value;
        T *cur;
        T *end;
    };
    
    iterator iterate_value(T);
    iterator end_value(T);
   
private:
    T *buf;
    size_t buf_size;
};


//......................................................................................


template <typename T>
MyVector<T>::MyVector(size_t s) {
    this->buf_size = s;
    this->buf = new T[this->buf_size];
}


template <typename T>
MyVector<T>::MyVector(size_t s, T value) {
    this->buf_size = s;
    this->buf = new T[this->buf_size];
    for (size_t i = 0; i < this->buf_size; ++i) {
        this->buf[i] = value;
    }
}


template <typename T>
MyVector<T>::MyVector(const MyVector<T> & vec) {
    this->buf_size = vec.buf_size;
    this->buf = new T[this->buf_size];
    for (size_t i = 0; i < this->buf_size; ++i) {
        this->buf[i] = vec[i];
    }
}


template <typename T>
template <typename S>
MyVector<T>::MyVector(const MyVector<S> & vec) {
    this->buf_size = vec.size();
    this->buf = new T[this->buf_size];
    for (size_t i = 0; i < this->buf_size; ++i) {
        this->buf[i] = vec[i];
    }
}


template <typename T>
MyVector<T> & MyVector<T>::operator=(const MyVector<T> & vec) {
    if (this == &vec) {
        return *this;
    }
    delete [] this->buf;
    this->buf_size = vec.size();
    this->buf = new T[this->buf_size];
    for (size_t i = 0; i < this->buf_size; ++i) {
        this->buf[i] = vec[i];
    }
    return *this;
}


template <typename T>
template <typename S>
MyVector<T> & MyVector<T>::operator=(const MyVector<S> & vec) {
    delete [] this->buf;
    this->buf_size = vec.size();
    this->buf = new T[this->buf_size];
    for (size_t i = 0; i < this->buf_size; ++i) {
        this->buf[i] = vec[i];
    }
    return *this;
}


template <typename T>
T & MyVector<T>::operator[](size_t index) {
    return this->buf[index];
}


template <typename T>
T MyVector<T>::operator[](size_t index) const {
    return this->buf[index];
}


template <typename T>
size_t MyVector<T>::size() const {
    return this->buf_size;
}


template <typename T>
MyVector<T>::~MyVector() {
    delete [] this->buf;
}


template <typename T>
typename MyVector<T>::iterator MyVector<T>::iterate_value(T value) {
    size_t i = 0;
    for (i = 0; i < this->buf_size && this->buf[i] != value; ++i);
    return iterator(value, &this->buf[i], &this->buf[this->buf_size]);
}


template <typename T>
typename MyVector<T>::iterator MyVector<T>::end_value(T value) {
    return iterator(value, &this->buf[this->buf_size], &this->buf[this->buf_size]);
}


//......................................................................................


template <typename T>
bool MyVector<T>::iterator::operator==(const iterator & itr) const {
    return (this->value == itr.value) && (this->end == itr.end) && (this->cur == itr.cur);
}


template <typename T>
bool MyVector<T>::iterator::operator!=(const iterator & itr) const {
    return !(*this == itr);
}


template <typename T>
typename MyVector<T>::iterator & MyVector<T>::iterator::operator++() {
    while (++(this->cur) != this->end && *(this->cur) != value);
    return *this;
}


template <typename T>
typename MyVector<T>::iterator MyVector<T>::iterator::operator++(int) {
    iterator itr = *this;
    while (++(this->cur) != this->end && *(this->cur) != value);
    return itr;
}


template <typename T>
T & MyVector<T>::iterator::operator*() {
    return *this->cur;
}


//......................................................................................


template <typename T, typename S>
MyVector<T> operator+(const MyVector<T> & vecT, const MyVector<S> & vecS) {
    if (vecT.size() != vecS.size()) {
        throw "invalid size";
    }
    MyVector<T> vec = vecT;
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i] += vecS[i];
    }
    return vec;
}


template <typename T>
std::ostream & operator<<(std::ostream & out, const MyVector<T> & vec) {
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] <<  " ";
    }
    return out;
}



int main() {
    MyVector<int> vec_i(10, 2); // вектор <int> размера 10 со значениями элементов 2
    MyVector<int> vec_ic(2);
    vec_ic = vec_i; // <int> копия vec_i
    MyVector<long> vec_l = vec_i; // <long> копия vec_i
    for (size_t i = 0; i < vec_i.size(); ++i) {
        vec_i[i] = i;
    }
    vec_l[1] = 5;
    vec_l[9] = 5;
    MyVector<long>::iterator itr = vec_l.iterate_value(5); // итератор по элементам vec_l равным 5
    while (itr != vec_l.end_value(5)) { // увеличение этих элементов на 5
        *itr = *itr + 5;
        ++itr;
    }
    MyVector<unsigned> vec_sum(10);
    try {
        vec_sum = vec_i + vec_l + vec_ic;
    }
    catch(const char *er) {
        std::cout << er << std::endl;
        exit(1);
    }   
    std::cout << vec_i << std::endl;
    std::cout << vec_l << std::endl;
    std::cout << vec_ic << std::endl;
    std::cout << vec_sum << std::endl;
    return 0;
}

