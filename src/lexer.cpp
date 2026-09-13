#include "lexer.h"

static void avancar(Lexer *lex) {
    lex->caractereAtual = fgetc(lex->fonte);
    if (lex->caractereAtual == '\n') {
        lex->linha++;
    }
}

void lexerInicializar(Lexer *lex, FILE *fonte) {
    lex->fonte = fonte;
    lex->linha = 1;
    lex->erroOcorreu = 0;

    avancar(lex);
}

static void ignorarEspacosEComentarios(Lexer *lex) {
    for (;;) {
        if (lex->caractereAtual == ' '  || lex->caractereAtual == '\t' ||
            lex->caractereAtual == '\r' || lex->caractereAtual == '\n') {
            avancar(lex);
        } else if (lex->caractereAtual == '#') {
            while (lex->caractereAtual != '\n' && lex->caractereAtual != EOF) {
                avancar(lex);
            }
        } else {
            break;
        }
    }
}

void lexerDebugImprimirTudo(Lexer *lex) {
    ignorarEspacosEComentarios(lex);
    while (lex->caractereAtual != EOF) {
        printf("linha %d: '%c'\n", lex->linha, lex->caractereAtual);
        avancar(lex);
        ignorarEspacosEComentarios(lex);
    }
    printf("linha %d: EOF\n", lex->linha);
}