#ifndef COMPOSITEFILTER_H
#define COMPOSITEFILTER_H

#include "Filter.h"
#include <vector>

struct CompositeFilter : Filter {

    CompositeFilter() {}

    void add_filter(Filter* filter) {
        _filters.push_back(filter);
    }

    virtual void process(std::ifstream &in, std::ofstream &out) {
        if (_filters.empty()) return;

        std::string temp1 = ".temp1"; // there should be unique file names
        std::string temp2 = ".temp2";

        for (size_t i = 0; i < _filters.size(); ++i) {
            if (i == 0) {
                std::ofstream temp_out(temp1.c_str());
                _filters[i]->process(in, temp_out);
                temp_out.close();
            } else {
                if (i % 2 == 1) {
                    process_filter(temp1, temp2, _filters[i]);
                } else {
                    process_filter(temp2, temp1, _filters[i]);
                }
            }
        }
        if (_filters.size() % 2 == 0) {
            std::ifstream temp_in(temp2.c_str());
            write_to_file(temp_in, out);
            temp_in.close();
        } else {
            std::ifstream temp_in(temp1.c_str());
            write_to_file(temp_in, out);
            temp_in.close();
        }
    }

    virtual ~CompositeFilter(){
        for (std::vector<Filter*>::iterator iter = _filters.begin(); iter != _filters.end(); ++iter) {
            delete *iter;
        }
    }

private:

    void write_to_file(std::ifstream & in, std::ofstream & out) {
        char buf[BUF_SIZE];
        while (!in.eof()) {
            size_t bytes_read = in.read(buf, BUF_SIZE).gcount();
            out.write(buf, bytes_read);
        }
    }

    void process_filter(std::string const & in_file, std::string const & out_file, Filter* filter) {
        std::ifstream in(in_file.c_str());
        std::ofstream out(out_file.c_str());
        filter->process(in, out);
        in.close();
        out.close();
    }

    std::vector<Filter*> _filters;

    CompositeFilter(CompositeFilter const &);
    CompositeFilter & operator = (CompositeFilter const &);
};


#endif // COMPOSITEFILTER_H
