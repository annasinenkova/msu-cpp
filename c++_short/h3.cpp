#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

class Logger {
public:
    virtual ~Logger();
    virtual void log(const std::string & message) = 0;
};


Logger::~Logger() {}


class FileLogger : public Logger {
public:
    FileLogger();
    FileLogger(const char *n);
    FileLogger(const FileLogger & fl);
    FileLogger & operator=(const FileLogger & fl);
     ~FileLogger();
    void log(const std::string & message);
private:
    char *name;
};


FileLogger::FileLogger() {
    this->name = new char[strlen("file_log") + 1];
    strcpy(this->name, "file_log");
}


FileLogger::FileLogger(const char *n) {
    this->name = new char[strlen(n) + 1];
    strcpy(this->name, n);
}


FileLogger::FileLogger(const FileLogger & fl) {
    this->name = new char[strlen(fl.name) + 1];
    strcpy(this->name, fl.name);
}


FileLogger::~FileLogger() {
    delete [] this->name;
}


FileLogger & FileLogger::operator=(const FileLogger & fl) {
    if (this == &fl) {
        return *this;
    }
    delete [] this->name;
    this->name = new char[strlen(fl.name) + 1];
    strcpy(this->name, fl.name);
    return *this;
}


void FileLogger::log(const std::string & message) {
    std::ofstream fout(this->name, std::ios_base::app);
    fout << message << std::endl;
    fout.close();
}



class StderrLogger : public Logger {
public:
    void log(const std::string & message);
};


void StderrLogger::log(const std::string & message) {
    std::cerr << message << std::endl;
}



class CompositeLogger : public Logger {
public:
    void add(Logger *logger);
    void log(const std::string & message);
private:
    std::vector<Logger*> buf;
};


void CompositeLogger::add(Logger *logger) {
    buf.push_back(logger);
}


void CompositeLogger::log(const std::string & message) {
    for (size_t i = 0; i < buf.size(); ++i) {
        buf[i]->log(message);
    }
}


int main() {
    CompositeLogger clog;
    Logger *buf[4] = {new StderrLogger, new FileLogger, new FileLogger("log"), NULL};
    Logger **ptr = buf;
    while (*ptr != NULL) {
        clog.add(*(ptr++));
    }
    clog.log("log message");
    ptr = buf;
    while (*ptr != NULL) {
        delete *(ptr++);
    }
    return 0;
}

