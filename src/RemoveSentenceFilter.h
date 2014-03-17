#ifndef REMOVESENTENCEFILTER_H
#define REMOVESENTENCEFILTER_H

#include "Filter.h"

struct RemoveSentenceFilter : Filter {

    RemoveSentenceFilter(std::string const & source_file, token t, bool is_prefix) :
        _source_file(source_file), _remove_sentence(t), _is_prefix(is_prefix) {}

    virtual void process(std::ifstream &in, std::ofstream &out) {
        std::vector<token> sentences = get_sentences(in);
        std::vector<token> sentences_to_remove;
        std::ifstream source(_source_file.c_str());
        for (std::vector<token>:: iterator iter = sentences.begin();
             iter != sentences.end(); ++iter) {

            if (match(in, source, *iter, _remove_sentence, _is_prefix)) {
                token s((*iter).begin, (*iter).end);
                if (s.begin != 0) {
                    --s.begin;
                }
                ++s.end;
                sentences_to_remove.push_back(s);
            }
        }
        process_remove(in, out, sentences_to_remove);
        source.close();
    }

    virtual ~RemoveSentenceFilter(){}

private:
    std::string const _source_file;
    token _remove_sentence;
    bool _is_prefix;
};


#endif // REMOVESENTENCEFILTER_H
