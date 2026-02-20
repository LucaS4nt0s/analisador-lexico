#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token Types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_OPERATOR,
    TOKEN_DELIMITER,
    TOKEN_ERROR,
    TOKEN_EOF
} TokenType;

// Token Structure
typedef struct {
    TokenType type;
    char *lexeme;
    int line;
    int col;
} Token;

// Token List (Dynamic Array)
typedef struct {
    Token *tokens;
    size_t count;
    size_t capacity;
} TokenList;

// Function Prototypes
void init_token_list(TokenList *list);
void add_token(TokenList *list, TokenType type, const char *lexeme, int line, int col);
void free_token_list(TokenList *list);
const char *token_type_to_string(TokenType type);
void print_tokens(TokenList *list);
char *read_file(const char *filename);
char *read_stdin();
void tokenize(const char *source, TokenList *list);
int is_keyword(const char *str);
char *my_strdup(const char *s);

// List of Keywords
const char *KEYWORDS[] = {
    "programa", "inicio", "fim", "inteiro", "flutuante", "se", "entao",
    "senao", "fimse", "para", "de", "ate", "passo", "faca", "fimpara",
    "enquanto", "fimenquanto", "e", "ou", "nao", "leia", "escreva",
    "escreval", "procedimento", "retorna", "vazio", "div"
};
const int NUM_KEYWORDS = sizeof(KEYWORDS) / sizeof(KEYWORDS[0]);

int is_keyword(const char *str) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(str, KEYWORDS[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char *my_strdup(const char *s) {
    if (!s) return NULL;
    size_t len = strlen(s) + 1;
    char *new_s = (char *)malloc(len);
    if (new_s) memcpy(new_s, s, len);
    return new_s;
}

// Main Function
int main(int argc, char *argv[]) {
    char *source = NULL;

    if (argc > 1) {
        source = read_file(argv[1]);
    } else {
        source = read_stdin();
    }

    if (!source) {
        // Error handling already done in read functions or acceptable empty input
        return 1;
    }

    TokenList list;
    init_token_list(&list);

    tokenize(source, &list);
    print_tokens(&list);

    free_token_list(&list);
    free(source);

    return 0;
}

// Implementations

void init_token_list(TokenList *list) {
    list->count = 0;
    list->capacity = 10;
    list->tokens = (Token *)malloc(list->capacity * sizeof(Token));
    if (!list->tokens) {
        perror("Failed to allocate memory for tokens");
        exit(EXIT_FAILURE);
    }
}

void add_token(TokenList *list, TokenType type, const char *lexeme, int line, int col) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->tokens = (Token *)realloc(list->tokens, list->capacity * sizeof(Token));
        if (!list->tokens) {
            perror("Failed to reallocate memory for tokens");
            exit(EXIT_FAILURE);
        }
    }
    Token *token = &list->tokens[list->count++];
    token->type = type;
    token->lexeme = my_strdup(lexeme);
    token->line = line;
    token->col = col;
    if (!token->lexeme) {
        perror("Failed to allocate memory for lexeme");
        exit(EXIT_FAILURE);
    }
}

void free_token_list(TokenList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].lexeme);
    }
    free(list->tokens);
    list->count = 0;
    list->capacity = 0;
    list->tokens = NULL;
}

const char *token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_INTEGER: return "INTEGER";
        case TOKEN_FLOAT: return "FLOAT";
        case TOKEN_STRING: return "STRING";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_DELIMITER: return "DELIMITER";
        case TOKEN_ERROR: return "ERROR";
        case TOKEN_EOF: return "EOF";
        default: return "UNKNOWN";
    }
}

void print_tokens(TokenList *list) {
    printf("%-6s | %-6s | %-12s | %s\n", "Line", "Col", "Type", "Value");
    printf("-------|--------|--------------|----------------\n");
    for (size_t i = 0; i < list->count; i++) {
        Token *t = &list->tokens[i];
        printf("%-6d | %-6d | %-12s | ", t->line, t->col, token_type_to_string(t->type));

        for (int j = 0; t->lexeme[j] != '\0'; j++) {
            char c = t->lexeme[j];
            if (c == '\n') printf("\\n");
            else if (c == '\t') printf("\\t");
            else if (c == '\r') printf("\\r");
            else if (c == '\\') printf("\\\\");
            else if (isprint(c)) putchar(c);
            else printf("\\x%02x", (unsigned char)c);
        }
        printf("\n");
    }
}

char *read_file(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("Error opening file");
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buffer = (char *)malloc(length + 1);
    if (!buffer) {
        perror("Failed to allocate memory for file buffer");
        fclose(f);
        return NULL;
    }

    fread(buffer, 1, length, f);
    buffer[length] = '\0';
    fclose(f);
    return buffer;
}

char *read_stdin() {
    size_t capacity = 1024;
    size_t length = 0;
    char *buffer = (char *)malloc(capacity);
    if (!buffer) return NULL;

    int c;
    while ((c = getchar()) != EOF) {
        if (length + 1 >= capacity) {
            capacity *= 2;
            char *new_buffer = (char *)realloc(buffer, capacity);
            if (!new_buffer) {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
        buffer[length++] = (char)c;
    }
    buffer[length] = '\0';
    return buffer;
}

void tokenize(const char *source, TokenList *list) {
    int line = 1;
    int col = 1;
    int i = 0;

    while (source[i] != '\0') {
        char c = source[i];

        // Skip Whitespace
        if (isspace(c)) {
            if (c == '\n') {
                line++;
                col = 1;
            } else {
                col++;
            }
            i++;
            continue;
        }

        // Comments
        if (c == '/' && source[i+1] == '/') {
            // Single-line comment
            i += 2;
            col += 2;
            while (source[i] != '\0' && source[i] != '\n') {
                i++;
                // Newline will be handled in next iteration
            }
            continue;
        }
        if (c == '/' && source[i+1] == '*') {
            // Multi-line comment
            i += 2;
            col += 2;
            while (source[i] != '\0') {
                if (source[i] == '*' && source[i+1] == '/') {
                    i += 2;
                    col += 2;
                    break;
                }
                if (source[i] == '\n') {
                    line++;
                    col = 1;
                } else {
                    col++;
                }
                i++;
            }
            continue;
        }

        // Strings
        if (c == '"') {
            int start_line = line;
            int start_col = col;
            i++;
            col++; // skip opening quote

            int str_cap = 64;
            int str_len = 0;
            char *str_val = (char *)malloc(str_cap);
            if (!str_val) exit(EXIT_FAILURE);

            int closed = 0;
            while (source[i] != '\0') {
                if (source[i] == '"') {
                    i++;
                    col++;
                    closed = 1;
                    break;
                }

                if (source[i] == '\n') {
                    break;
                }

                char char_to_add = source[i];
                if (source[i] == '\\') {
                    i++;
                    col++;
                    if (source[i] == 'n') char_to_add = '\n';
                    else if (source[i] == 't') char_to_add = '\t';
                    else if (source[i] == '\\') char_to_add = '\\';
                    else if (source[i] == '"') char_to_add = '"';
                    else char_to_add = source[i];
                }

                if (str_len + 1 >= str_cap) {
                    str_cap *= 2;
                    str_val = (char *)realloc(str_val, str_cap);
                    if (!str_val) exit(EXIT_FAILURE);
                }
                str_val[str_len++] = char_to_add;

                i++;
                col++;
            }
            str_val[str_len] = '\0';

            if (!closed) {
                add_token(list, TOKEN_ERROR, "Unclosed String", start_line, start_col);
            } else {
                add_token(list, TOKEN_STRING, str_val, start_line, start_col);
            }
            free(str_val);
            continue;
        }

        // Identifiers and Keywords
        if (isalpha(c) || c == '_') {
            int start_col = col;
            int start_i = i;
            while (isalnum(source[i]) || source[i] == '_') {
                i++;
                col++;
            }
            int len = i - start_i;
            char *lexeme = (char *)malloc(len + 1);
            strncpy(lexeme, source + start_i, len);
            lexeme[len] = '\0';

            if (is_keyword(lexeme)) {
                add_token(list, TOKEN_KEYWORD, lexeme, line, start_col);
            } else {
                add_token(list, TOKEN_IDENTIFIER, lexeme, line, start_col);
            }
            free(lexeme);
            continue;
        }

        // Numbers (Float and Integer)
        if (isdigit(c) || (c == '.' && isdigit(source[i+1]))) {
            int start_col = col;
            int start_i = i;
            int is_float = 0;

            if (c == '.') {
                is_float = 1;
                i++; col++;
            }

            while (isdigit(source[i])) {
                i++; col++;
            }

            if (!is_float && source[i] == '.') {
                is_float = 1;
                i++; col++;
                while (isdigit(source[i])) {
                    i++; col++;
                }
            }

            if (source[i] == 'e' || source[i] == 'E') {
                int save_i = i;
                int save_col = col;
                i++; col++;
                if (source[i] == '+' || source[i] == '-') {
                    i++; col++;
                }
                if (isdigit(source[i])) {
                    is_float = 1;
                    while (isdigit(source[i])) {
                        i++; col++;
                    }
                } else {
                    i = save_i;
                    col = save_col;
                }
            }

            int len = i - start_i;
            char *lexeme = (char *)malloc(len + 1);
            strncpy(lexeme, source + start_i, len);
            lexeme[len] = '\0';

            add_token(list, is_float ? TOKEN_FLOAT : TOKEN_INTEGER, lexeme, line, start_col);
            free(lexeme);
            continue;
        }

        // Operators
        // Check for double-char operators: ==, !=, <=, >=, &&, ||
        if (c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|') {
            char next = source[i+1];
            int is_double = 0;

            if (c == '=' && next == '=') is_double = 1;
            else if (c == '!' && next == '=') is_double = 1;
            else if (c == '<' && next == '=') is_double = 1;
            else if (c == '>' && next == '=') is_double = 1;
            else if (c == '&' && next == '&') is_double = 1;
            else if (c == '|' && next == '|') is_double = 1;

            if (is_double) {
                char op[3] = {c, next, '\0'};
                add_token(list, TOKEN_OPERATOR, op, line, col);
                i += 2;
                col += 2;
                continue;
            }
        }

        // Single char operators: +, -, *, /, =, <, >
        if (strchr("+-*/=<>", c)) {
             char op[2] = {c, '\0'};
             add_token(list, TOKEN_OPERATOR, op, line, col);
             i++;
             col++;
             continue;
        }

        // Delimiters
        if (strchr("(){},;", c)) {
            char del[2] = {c, '\0'};
            add_token(list, TOKEN_DELIMITER, del, line, col);
            i++;
            col++;
            continue;
        }

        // Unknown / Error
        // NOTE: !, &, | alone are handled here because they are not valid single operators in this spec.
        char unknown[2] = {c, '\0'};
        add_token(list, TOKEN_ERROR, unknown, line, col);
        i++;
        col++;
    }
}
