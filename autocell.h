#ifndef AUTOCELL_H_INCLUDED
#define AUTOCELL_H_INCLUDED

#include <string>
#include <iostream>

using namespace std;

class AutomateException {
public:
AutomateException(const std::string& message):info(message) {}
std::string getInfo() const { return info; }
private:
std::string info;
};




#endif // AUTOCELL_H_INCLUDED
