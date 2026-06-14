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

char *starts_with_reserved(char *p) {
    // Keyword
    static char *kw[] = {"return", "if", "else", "while", "for"};

    for (int i = 0; i < sizeof(kw) / sizeof(*kw); i++) {
        int len = strlen(kw[i]);
        if (starts_with(p, kw[i]) && !is_alnum(p[len]))
            return kw[i];
    }

    // Multi-letter punctuator
    static char *ops[] = {"==", "!=", "<=", ">="};

    for (int i = 0; i < sizeof(ops) / sizeof(*ops); i++)
        if (starts_with(p, ops[i]))
            return ops[i];

    return NULL;
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

        char *kw = starts_with_reserved(p);
        if (kw) {
            int len = strlen(kw);
            cur = new_token(TK_RESERVED, cur, p, len);
            p += len;
            continue;
        }

        if (strchr("+-*/()<>;={}", *p)) {
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