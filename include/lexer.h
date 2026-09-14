#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include "token.h"

struct Lexer {
    FILE *fonte;
    int linha;
    int caractereAtual;
    int erroOcorreu;
};

void lexerInicializar(Lexer *lex, FILE *fonte);

void lexerDebugImprimirTudo(Lexer *lex);

Token lexerTesteParte2(Lexer *lex);

#endif
