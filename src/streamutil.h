#ifndef STREAMUTIL_H
#define STREAMUTIL_H

#include <fstream>
#include <vector>
#include <string>
#include "token.h"
#include "constants.h"

void process_remove(std::ifstream & in, std::ofstream & out, std::vector<token> & tokens);

void process_replace(std::ifstream & in, std::ofstream & out,
                     std::ifstream & rin, std::vector<token> & tokens,
                     token const & replace_token);

bool match(std::ifstream & first, std::ifstream & second,
           token const & token1, token const & token2, bool is_prefix);

bool match(std::ifstream & first, std::string const second,
           token const & token1, bool is_prefix);

bool is_word_separator(char c);

bool is_sentence_separator(char c);

std::vector<token> get_words(std::ifstream & in);

std::vector<token> get_sentences(std::ifstream & in);


#endif // STREAMUTIL_H
