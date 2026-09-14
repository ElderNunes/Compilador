#ifndef LEXER_H
#define LEXER_H

#include <cstdio>

/*
 * Estado do analisador lexico, mantido entre chamadas.
 * - fonte: arquivo aberto que estamos lendo
 * - linha: linha atual (comeca em 1)
 * - caractereAtual: o caractere que ainda nao foi consumido (lookahead de 1)
 * - erroOcorreu: flag pra sinalizar erro lexico mais pra frente
 */
typedef struct {
    FILE *fonte;
    int linha;
    int caractereAtual;
    int erroOcorreu;
} Lexer;

/* Prepara o Lexer para comecar a ler 'fonte' a partir da linha 1. */
void lexerInicializar(Lexer *lex, FILE *fonte);

/* TEMPORARIA (so pra testar a Parte 1) -- remover na Parte 6. */
void lexerDebugImprimirTudo(Lexer *lex);

#endif