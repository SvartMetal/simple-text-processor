#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "token.h"
#include "streamutil.h"
#include "TextProcessor.h"
#include "ConfigParser.h"


void run_processor(char const * input_file, char const * output_file, char const * config_file) {

    std::ifstream in(input_file);
    std::ofstream out(output_file);

    TextProcessor textProcessor(ConfigParser::parse_filter(config_file));

    textProcessor.run_process(in, out);

    in.close();
    out.close();
}


void test_process_remove(char const * input, char const * output) {
    token values[] = {*(new token(0,2)),*(new token(10,12)),*(new token(18,27))};
    std::vector <token> tokens(3);

    std::copy(values, values + 3, tokens.begin());
    std::ifstream in(input);
    std::ofstream out(output);
    process_remove(in, out, tokens);
    in.close();
    out.close();
}


void test_iterate_words(char const * input) {
    std::ifstream in(input);
    std::vector<token> words = get_words(in);
    for (size_t i = 0; i < words.size(); ++i) {
        std::cout << words[i].begin << " " << words[i].end << std::endl;
    }
    in.close();
}

void test_iterate_sentences(char const * input) {
    std::ifstream in(input);
    std::vector<token> words = get_sentences(in);
    for (size_t i = 0; i < words.size(); ++i) {
        std::cout << words[i].begin << " " << words[i].end << std::endl;
    }
    in.close();
}

void test_process_replace(char const * input, char const * output, char const * config) {
    token values[] = {token(0,2),*(new token(2,12)),*(new token(16,50))};
    std::vector <token> tokens(3);
    std::copy(values, values + 3, tokens.begin());
    std::ifstream in(input);
    std::ofstream out(output);
    std::ifstream conf(config);
    token replace_token(1,5);
    process_replace(in, out, conf, tokens, replace_token);
    in.close();
    out.close();
    conf.close();
}

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cout << "Not enough arguments." << std::endl;
        std::cout << "Usage: [input_file_name] [output_file_name] [config_file_name]" << std::endl;
        return 0;
    }
    test_process_remove(argv[1], argv[2]);
    test_process_replace(argv[1], argv[2], argv[3]);
    run_processor(argv[1], argv[2], argv[3]);
    test_iterate_words(argv[1]);
    test_iterate_sentences(argv[1]);
    return 0;
}

