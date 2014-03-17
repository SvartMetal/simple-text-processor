#include "streamutil.h"

static void copy_to_out_buf(std::ofstream &, char *, char*, size_t, size_t &);
static size_t get_file_length(std::ifstream &);
static void replace_and_write(std::ofstream &, std::ifstream &, token const &, char *, size_t &);


static void copy_to_out_buf(std::ofstream & out, char * output_buffer,
                   char* input_buffer, size_t index, size_t & bytes_copied) {
    output_buffer[bytes_copied] = input_buffer[index];
    ++bytes_copied;
    if (bytes_copied == BUF_SIZE) {
        out.write(output_buffer, bytes_copied);
        bytes_copied = 0;
    }
}

static size_t get_file_length(std::ifstream & file) {
    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);
    return length;
}

void process_remove(std::ifstream & in, std::ofstream & out, std::vector<token> & tokens) {
    char input_buffer[BUF_SIZE];
    char output_buffer[BUF_SIZE];
    size_t bytes_copied = 0;
    size_t length = get_file_length(in);
    std::vector<token>::iterator iter = tokens.begin();
    size_t bytes_read = 0;
    size_t position = 0;
    size_t count = std::min(length, BUF_SIZE);
    while ((bytes_read = in.read(input_buffer, count).gcount()) != 0) {
        size_t i = 0;
        while (i < bytes_read) {
            if (iter != tokens.end()) {
                if (i + position >= (*iter).begin && i + position < (*iter).end) {
                    ++i;
                    continue;
                } else if (i + position >= (*iter).end) {
                    ++iter;
                    continue;
                }
            }
            copy_to_out_buf(out, output_buffer, input_buffer, i, bytes_copied);
            ++i;
        }
        position += bytes_read;
        count = std::min(length - position, BUF_SIZE);
    }
    out.write(output_buffer, bytes_copied);
}


static void replace_and_write(std::ofstream & out, std::ifstream & rin, token const & replace_token,
                   char * output_buffer, size_t & bytes_copied) {
    size_t bytes_replaced = 0;
    size_t need_to_replace = replace_token.end - replace_token.begin;
    rin.seekg(replace_token.begin);
    if (bytes_copied != 0) {
        out.write(output_buffer, bytes_copied);
        bytes_copied = 0;
    }
    size_t count = std::min(need_to_replace - bytes_replaced, BUF_SIZE);
    while ((count = rin.read(output_buffer, count).gcount()) != 0 &&
           bytes_replaced <= need_to_replace) {
        out.write(output_buffer, count);
        bytes_replaced += count;
        count = std::min(need_to_replace - bytes_replaced, BUF_SIZE);
    }
}

void process_replace(std::ifstream & in, std::ofstream & out,
                     std::ifstream & rin, std::vector<token> & tokens,
                     token const & replace_token) {
    char input_buffer[BUF_SIZE];
    char output_buffer[BUF_SIZE];
    size_t length = get_file_length(in);
    size_t position = 0;
    size_t bytes_copied = 0;
    size_t bytes_read = 0;
    size_t count = std::min(length, BUF_SIZE);
    std::vector<token>::iterator iter = tokens.begin();
    while ((bytes_read = in.read(input_buffer, count).gcount()) != 0) {
        size_t i = 0;
        size_t cur_position = 1;
        while (i < bytes_read) {
            if (iter != tokens.end() && i + position == (*iter).begin) {
                replace_and_write(out, rin, replace_token, output_buffer, bytes_copied);
                if ((*iter).end > position + bytes_read) {
                    position = (*iter).end;
                    in.seekg((*iter).end);
                    ++iter;
                    cur_position = 0;
                    break;
                }
                i = (*iter).end - position;
                ++iter;
            } else {
                copy_to_out_buf(out, output_buffer, input_buffer, i, bytes_copied);
                ++i;
            }
        }
        position += cur_position * bytes_read;
        count = std::min(length - position, BUF_SIZE);
    }
    out.write(output_buffer, bytes_copied);
}

bool match(std::ifstream & first, std::ifstream & second,
           token const & token1, token const & token2, bool is_prefix) {
    size_t length1 = token1.end - token1.begin;
    size_t length2 = token2.end - token2.begin;
    if (length1 < length2) {
        return false;
    }
    size_t need_to_compare = is_prefix ? length2 : length1;
    first.seekg(token1.begin), second.seekg(token2.begin);
    size_t bytes_read = 0;
    size_t block_begin = 0;
    size_t sec_block_begin = 0;
    size_t not_whitespaces = 0;
    size_t sec_count = std::min(length2, BUF_SIZE);
    size_t count = std::min(need_to_compare, BUF_SIZE);
    bool is_first = true;
    char first_buffer[BUF_SIZE], second_buffer[BUF_SIZE];
    bool next_read = true;
    size_t k = 0;
    while ((bytes_read = first.read(first_buffer, count).gcount()) != 0) {
        if (next_read) second.read(second_buffer, sec_count);
        for (size_t i = 0; i < bytes_read; ++i) {
            if (is_first && is_word_separator(first_buffer[i])) continue;
            if (first_buffer[i] != second_buffer[k]) return false;
            if (is_first) {
                is_first = false;
            }
            ++not_whitespaces;
            ++k;
        }
        if (k == sec_count) {
            sec_block_begin += sec_count;
            sec_count = std::min(length2 - sec_block_begin, BUF_SIZE);
            next_read = true;
            k = 0;
        } else {
            next_read = false;
        }
        block_begin += count;
        count = std::min(need_to_compare - block_begin, BUF_SIZE);
    }
    if (!is_prefix && not_whitespaces != length2) return false;
    return true;
}

bool match(std::ifstream & first, std::string const second,
           token const & token1, bool is_prefix) {
    size_t length1 = token1.end - token1.begin;
    size_t length2 = second.size();
    if ( (!is_prefix && length1 != length2) || length1 < length2) {
        return false;
    }
    size_t need_to_compare = is_prefix ? length2 : length1;
    first.seekg(token1.begin);
    size_t bytes_read = 0;
    size_t block_begin = 0;
    size_t count = std::min(need_to_compare - bytes_read, BUF_SIZE);
    char first_buffer[BUF_SIZE];
    while ((bytes_read = first.read(first_buffer, count).gcount()) != 0) {
        for (size_t i = 0; i < bytes_read; ++i) {
            if (first_buffer[i] != second[i + block_begin]) return false;
        }
        block_begin += count;
        count = std::min(need_to_compare - block_begin, BUF_SIZE);
    }
    return true;
}


bool is_word_separator(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == ',' || c == ':' || c == ';';
}

bool is_sentence_separator(char c) {
    return c == '.' || c == '!' || c == '?';
}

std::vector<token> get_words(std::ifstream & in) {
    std::vector<token> result;
    size_t length = get_file_length(in);
    char buf[BUF_SIZE];
    size_t bytes_read = 0;
    size_t block_begin = 0;
    size_t token_begin = 0;
    size_t count = std::min(length, BUF_SIZE);
    while ((bytes_read = in.read(buf, count).gcount()) != 0) {
        for (size_t i = 0; i < bytes_read; ++i) {
            if (is_word_separator(buf[i]) || is_sentence_separator(buf[i])) {
                if (token_begin == i + block_begin) {
                    token_begin = i + block_begin + 1;
                    continue;
                }
                result.push_back(token(token_begin, block_begin + i));
                token_begin = i + block_begin + 1;
            }
        }
        block_begin += count;
        count = std::min(length - block_begin, BUF_SIZE);
    }
    if (token_begin != length) {
        result.push_back(token(token_begin, length));
    }
    return result;
}

std::vector<token> get_sentences(std::ifstream & in) {
    std::vector<token> result;
    size_t length = get_file_length(in);
    char buf[BUF_SIZE];
    size_t bytes_read = 0;
    size_t block_begin = 0;
    size_t token_begin = 0;
    size_t count = std::min(length, BUF_SIZE);
    while ((bytes_read = in.read(buf, count).gcount()) != 0) {
        for (size_t i = 0; i < bytes_read; ++i) {
            if (is_sentence_separator(buf[i])) {
                if (token_begin == i + block_begin) {
                    token_begin = i + block_begin + 1;
                    continue;
                }
                result.push_back(token(token_begin, block_begin + i));
                token_begin = i + block_begin + 1;
            }
        }
        block_begin += count;
        count = std::min(length - block_begin, BUF_SIZE);
    }
    return result;
}

