#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include "RemoveWordFilter.h"
#include "RemoveSentenceFilter.h"
#include "ReplaceWordFilter.h"
#include "ReplaceSentenceFilter.h"
#include "CompositeFilter.h"

static const char* _WORD = "word";
static const char* _PREFIX = "prefix";
static const char* _REMOVE = "remove";
static const char* _REPLACE = "replace";
static const char* _SENTENCE = "sentence";


struct ConfigParser {

    static CompositeFilter* parse_filter(char const * config_file) {
        std::ifstream config(config_file);
        std::vector<token> words = get_words(config);
        CompositeFilter* compositeFilter = new CompositeFilter();
        for (std::vector<token>::const_iterator iter = words.begin(); iter != words.end();) {
            bool is_prefix;
            Filter* filter = 0;
            if (match(config, _WORD, *iter, false)) {
                is_prefix = false;
            } else if (match(config, _PREFIX, *iter, false)) {
                is_prefix = true;
            }
            ++iter;
            token t = *iter;
            ++iter;
            if (match(config, _REPLACE, *iter, false)) {
                token r = *(++iter);
                filter = new ReplaceWordFilter(config_file, t, r, is_prefix);
            } else if (match(config, _REMOVE, *iter, false)) {
                ++iter;
                if (match(config, _WORD, *iter, false)) {
                    filter = new RemoveWordFilter(config_file, t, is_prefix);
                } else if (match(config, _SENTENCE, *iter, false)) {
                    filter = new RemoveSentenceFilter(config_file, t, is_prefix);
                }
            }
            if (filter != 0) {
                compositeFilter->add_filter(filter);
            }
            ++iter;
        }
        config.close();
        return compositeFilter;
    }

private:

    ConfigParser();

};


#endif // CONFIGPARSER_H
