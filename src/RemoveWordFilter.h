#ifndef REMOVEWORDFILTER_H
#define REMOVEWORDFILTER_H

#include "Filter.h"

struct RemoveWordFilter : Filter {

    RemoveWordFilter(std::string const & source_file, token t, bool is_prefix) :
        _source_file(source_file), _remove_word(t), _is_prefix(is_prefix) {}

    virtual void process(std::ifstream & in, std::ofstream & out) {
        std::vector<token> words = get_words(in);
        std::vector<token> words_to_remove;
        std::ifstream source(_source_file.c_str());
        for (std::vector<token>:: iterator iter = words.begin();
             iter != words.end(); ++iter) {

            if (match(in, source, *iter, _remove_word, _is_prefix)) {
                words_to_remove.push_back(*iter);
            }
        }
        process_remove(in, out, words_to_remove);
        source.close();
    }

    virtual ~RemoveWordFilter(){}

private:
    std::string const _source_file;
    token _remove_word;
    bool _is_prefix;
};


#endif // REMOVEWORDFILTER_H
