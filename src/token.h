#ifndef TOKEN_H
#define TOKEN_H

struct token {
    size_t begin;
    size_t end;
    token() {}
    token(size_t begin, size_t end) : begin(begin), end(end) {}
};


#endif // TOKEN_H
