#ifndef FILTER_H
#define FILTER_H

#include <string>
#include <fstream>

#include "token.h"
#include "streamutil.h"

struct Filter {

    virtual void process(std::ifstream & in, std::ofstream & out) = 0;

    virtual ~Filter() {}
};


#endif // FILTER_H
