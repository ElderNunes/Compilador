#include <cstdio>
#include "lexer.h"

// Main teste

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
    lexerDebugImprimirTudo(&lex);

    fclose(fonte);
    return 0;
}