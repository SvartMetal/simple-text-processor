#ifndef REPLACESENTENCEFILTER_H
#define REPLACESENTENCEFILTER_H

#include "Filter.h"

struct ReplaceSentenceFilter : Filter {

    ReplaceSentenceFilter(std::string const & source_file, token t, token r, bool is_prefix) :
        _source_file(source_file), _sentence(t),
        _replace_sentence(r), _is_prefix(is_prefix) {}

    virtual void process(std::ifstream &in, std::ofstream &out) {
        std::vector<token> sentences = get_words(in);
        std::vector<token> sentences_to_replace;
        std::ifstream source(_source_file.c_str());
        for (std::vector<token>:: iterator iter = sentences.begin();
             iter != sentences.end(); ++iter) {

            if (match(in, source, *iter, _sentence, _is_prefix)) {
                sentences_to_replace.push_back(*iter);
            }
        }
        process_replace(in, out, source, sentences_to_replace, _replace_sentence);
        source.close();
    }

    virtual ~ReplaceSentenceFilter(){}

private:
    std::string const _source_file;
    token _sentence;
    token _replace_sentence;
    bool _is_prefix;

};


#endif // REPLACESENTENCEFILTER_H
