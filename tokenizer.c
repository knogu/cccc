#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include <stdbool.h>

bool starts_with(char *p, char *pre) {
    return strncmp(p, pre, strlen(pre)) == 0;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

bool is_alpha(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

bool is_alnum(char c) {
    return is_alpha(c) || ('0' <= c && c <= '9');;
}

char *strndup_(char *p, int len) {
    char *buf = malloc(len + 1);
    strncpy(buf, p, len);
    buf[len] = '\0';
    return buf;
}

Token *tokenize() {
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        // keywords
        if (starts_with(p, "return") && !is_alnum(p[6])) {
            cur = new_token(TK_RESERVED, cur, p, 6);
            p += 6;
            continue;
        }

        if (starts_with(p, "==") || starts_with(p, "!=") || starts_with(p, "<=") || starts_with(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>;=", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (is_alpha(*p)) {
            char *q = p++;
            while (is_alnum(*p)) p++;
            cur = new_token(TK_IDENT, cur, q, p - q);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        error_at(p, "expected a number");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}