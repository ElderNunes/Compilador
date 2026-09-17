#include <cstdio>
#include "lexer.h"
#include "token.h"

// Exibe o token no formato pedido pelo trabalho.
static void imprimirToken(const Token *t) {
    printf("<%s, \"%s\">\n", nomeTipoToken(t->tipo), t->lexema);
}

int main(int argc, char *argv[]) {
    // O primeiro argumento deve ser o arquivo-fonte da linguagem GYH.
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

    // Solicita tokens até o fim do arquivo ou até o primeiro erro.
    Token t;
    do {
        t = proximoToken(&lex);
        if (t.tipo == TOKEN_ERRO) {
            fclose(fonte);
            return 1;
        }
        imprimirToken(&t);
    } while (t.tipo != TOKEN_EOF);

    fclose(fonte);
    return 0;
}