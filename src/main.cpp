#include <cstdio>
#include "lexer.h"
#include "token.h"

static void imprimirToken(const Token *t) {
    printf("<%s, \"%s\">\n", nomeTipoToken(t->tipo), t->lexema);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE *fonte = fopen(argv[1], "r");
    if (!fonte) {
        fprintf(stderr, "Erro ao abrir '%s'\n", argv[1]);
        return 1;
    }

    Lexer lex;
    lexerInicializar(&lex, fonte);

    Token t;
    do {
        t = lexerTesteParte2(&lex);
        imprimirToken(&t);
        if (t.tipo == TOKEN_ERRO) {
            fclose(fonte);
            return 1;
        }
    } while (t.tipo != TOKEN_EOF);

    fclose(fonte);
    return 0;
}