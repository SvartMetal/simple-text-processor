#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>
#include <fstream>

#include "CompositeFilter.h"

struct TextProcessor {

    TextProcessor (CompositeFilter* filter) : _filter(filter) {}

    void run_process (std::ifstream & in, std::ofstream & out) {
        _filter->process(in, out);
    }

    ~TextProcessor() {
        delete _filter;
    }

private:

    std::ifstream _istream;
    std::ofstream _ostream;
    CompositeFilter* _filter;

    TextProcessor(TextProcessor const &);
    TextProcessor & operator = (TextProcessor const &);

};


#endif // TEXTPROCESSOR_H
