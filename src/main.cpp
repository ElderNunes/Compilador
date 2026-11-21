// Gabriel Oliveira de Jesus (2614090) e Elder Nunes Gonçalves (2625288)

#include <cstdio>
#include <cstring>

#include "lexer.h"
#include "parser.h"
#include "token.h"

static void imprimirToken(const Token *t) {
    if (t->tipo == TOKEN_EOF) {
        printf("<EOF>\n");
        return;
    }

    printf("<%s,\"%s\">\n", nomeTipoToken(t->tipo), t->lexema);
}

int main(int argc, char *argv[]) {
    bool modoSintatico = false;

    if (argc == 3) {
        if (strcmp(argv[2], "--sintatico") != 0) {
            fprintf(stderr, "Opção desconhecida: %s\n", argv[2]);
            return 1;
        }

        modoSintatico = true;
    } else if (argc != 2) {
        fprintf(stderr, "Uso: %s <arquivo> [--sintatico]\n", argv[0]);
        return 1;
    }

    FILE *fonte = fopen(argv[1], "r");

    if (!fonte) {
        fprintf(stderr, "Erro ao abrir '%s'\n", argv[1]);
        return 1;
    }

    Lexer lex;
    lexerInicializar(&lex, fonte);

    bool sucesso = true;

    if (modoSintatico) {
        Parser parser;
        parserInicializar(&parser, &lex);

        sucesso = parserPrograma(&parser);

        if (sucesso) {
            printf("Programa sintaticamente válido.\n");
        }
    } else {
        Token t;

        do {
            t = proximoToken(&lex);

            if (t.tipo == TOKEN_ERRO) {
                sucesso = false;
                break;
            }

            imprimirToken(&t);
        } while (t.tipo != TOKEN_EOF);
    }

    fclose(fonte);
    return sucesso ? 0 : 1;
}