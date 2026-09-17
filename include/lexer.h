#ifndef LEXER_H
#define LEXER_H

#include <cstdio>
#include "token.h"

// Estado usado para percorrer o arquivo com um caractere de antecipação.
struct Lexer {
    FILE *fonte;
    int linha;
    int caractereAtual;
    int erroOcorreu;
};

// Inicializa o lexer associado a um arquivo já aberto.
void lexerInicializar(Lexer *lex, FILE *fonte);

// Utilitário de depuração da infraestrutura de leitura.
void lexerDebugImprimirTudo(Lexer *lex);

// Retorna o próximo token reconhecido no arquivo.
Token proximoToken(Lexer *lex);

#endif
