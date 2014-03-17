#ifndef REPLACEWORDFILTER_H
#define REPLACEWORDFILTER_H

#include "Filter.h"

struct ReplaceWordFilter : Filter {

    ReplaceWordFilter(std::string const & source_file, token t, token r, bool is_prefix) :
        _source_file(source_file), _word(t),_replace_word(r), _is_prefix(is_prefix) {}

    virtual void process(std::ifstream &in, std::ofstream &out) {
        std::vector<token> words = get_words(in);
        std::vector<token> words_to_replace;
        std::ifstream source(_source_file.c_str());
        for (std::vector<token>:: iterator iter = words.begin();
             iter != words.end(); ++iter) {

            if (match(in, source, *iter, _word, _is_prefix)) {
                words_to_replace.push_back(token((*iter).begin, (*iter).begin +
                                                 _word.end - _word.begin));
            }
        }
        process_replace(in, out, source, words_to_replace, _replace_word);
        source.close();
    }

    virtual ~ReplaceWordFilter(){}

private:

    std::string const _source_file;
    token _word;
    token _replace_word;
    bool _is_prefix;

};


#endif // REPLACEWORDFILTER_H
