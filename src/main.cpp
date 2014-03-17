#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cout << "Not enough arguments." << std::endl;
        std::cout << "Usage: [input_file_name] [output_file_name] [config_file_name]" << std::endl;
        return 0;
    }
//    test_process_remove(argv[1], argv[2]);
//    test_process_replace(argv[1], argv[2], argv[3]);
    run_processor(argv[1], argv[2], argv[3]);
//    test_iterate_words(argv[1]);
//    test_iterate_sentences(argv[1]);
    return 0;
}
