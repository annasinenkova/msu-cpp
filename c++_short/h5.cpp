#include <algorithm>
#include <iostream>
#include <string>
#include <map>


class MuseumRooms {
public:
    void set_limit(std::string, size_t);
    void inc_visitors(std::string);
    void dec_visitors(std::string);
    size_t get_cur_size(std::string) const; 
    
    struct room {
        room(size_t l = 0) : limit(l), cur_size(0) {}
        size_t limit;
        size_t cur_size;
    };
private:
    std::map<std::string, room> rooms;
};


void MuseumRooms::set_limit(std::string id_room, size_t limit) {
    this->rooms[id_room] = limit;
}


#define SAFEACCESS\
    do {\
        if (this->rooms.find(id_room) == this->rooms.end()) {\
            throw "room without limit";\
        }\
    } \
    while(0)


void MuseumRooms::inc_visitors(std::string id_room) {
    SAFEACCESS;
    if (this->rooms[id_room].cur_size == this->rooms[id_room].limit) {
        throw "full room";
    }
    ++this->rooms[id_room].cur_size;
}


void MuseumRooms::dec_visitors(std::string id_room) {
    SAFEACCESS;
    if (this->rooms[id_room].cur_size == 0) {
        throw "empty room";
    }
    --this->rooms[id_room].cur_size;
}


size_t MuseumRooms::get_cur_size(std::string id_room) const {
    SAFEACCESS;
    return this->rooms.find(id_room)->second.cur_size;
}


int main() {
    MuseumRooms m;
    m.set_limit("a", 5);
    for (size_t i = 0; i < 5; ++i) {
        try {
            m.inc_visitors("a");
        }
        catch(const char *er) {
            std::cout << er << std::endl;
        }
        try {
            std::cout << m.get_cur_size("a") << std::endl;
        }
        catch(const char *er) {
            std::cout << er << std::endl;
        }
    }
    try {
        m.inc_visitors("a");
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    for (size_t i = 0; i < 5; ++i) {
        try {
            m.dec_visitors("a");
        }
        catch(const char *er) {
            std::cout << er << std::endl;
        }
    }
    try {
        m.dec_visitors("a");
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    try {
        m.inc_visitors("b");
    }
    catch(const char *er) {
        std::cout << er << std::endl;
    }
    return 0;
}

