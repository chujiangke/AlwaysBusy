
#ifndef DETECTFINAL_TOOLS_H
#define DETECTFINAL_TOOLS_H

#include <iostream>
#include <string>

#define ASSERT(x) assertion(__FILE__, __func__, __LINE__, x)

inline void assertion(std::string filename, std::string function,int line, bool isAllRight){
    if(!isAllRight){
        std::cerr<<filename+" "+function+" "+std::to_string(line)+">>  "+"Assertion Failed"<<std::endl;
        exit(-1);
    }
}

#endif //DETECTFINAL_TOOLS_H
