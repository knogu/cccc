typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

char *user_input;

Token *token;

typedef struct Var Var;
struct Var {
    Var *next;
    char *name;
    int offset;
};

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_ASSIGN,
    ND_RETURN,
    ND_IF,
    ND_EXPR_STMT,
    ND_VAR,
    ND_NUM,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *next;

    Node *lhs;
    Node *rhs;

    // if
    Node *cond;
    Node *then;
    Node *els;

    Var *var;
    int val;
};

typedef struct {
    Node *node;
    Var *locals;
    int stack_size;
} Program;

Token *consume_ident();

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);

Token *tokenize();
Program *program();
void codegen(Program *program);
char *strndup_(char *p, int len);
