#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "token.h"

// Estado usado durante a análise sintática.
struct Parser {
    Lexer *lexer;
    Token atual;
    bool erro;
};

// Classifica a estrutura reconhecida durante a análise de expressões.
enum class CategoriaExpressao {
    ARITMETICA,
    RELACIONAL,
    INVALIDA
};

// Prepara o parser e carrega o primeiro token.
void parserInicializar(Parser *parser, Lexer *lexer);

// Solicita o próximo token ao analisador léxico.
bool parserAvancar(Parser *parser);

// Verifica o token esperado e avança quando ele corresponde.
bool parserConsumir(Parser *parser, TipoToken esperado);

// Registra e apresenta um erro sintático.
bool parserErro(Parser *parser, const char *esperado);

// Reconhece um dos tipos permitidos para uma variável.
bool parserTipoVar(Parser *parser);

// Reconhece uma declaração no formato VAR '[' TipoVar ']'.
bool parserDeclaracao(Parser *parser);

// Reconhece uma ou mais declarações consecutivas.
bool parserListaDeclaracoes(Parser *parser);

// Reconhece a lista de comandos. Será implementada na parte de Gabriel.
bool parserListaComandos(Parser *parser);

// Reconhece um programa completo.
bool parserPrograma(Parser *parser);

bool parserExpressaoAritmetica(Parser *parser);
bool parserExpressaoRelacional(Parser *parser);

// Reconhece e consome um operador relacional.
bool parserOperadorRelacional(Parser *parser);

// Reconhece e consome 'and' ou 'or'.
bool parserOperadorBooleano(Parser *parser);

// Reconhece número, variável ou expressão entre parênteses.
CategoriaExpressao parserPrimario(Parser *parser);

// Reconhecerá os níveis de uma expressão completa.
CategoriaExpressao parserExpressaoGeral(Parser *parser);

// Reconhece produtos e divisões entre expressões aritméticas.
CategoriaExpressao parserProduto(Parser *parser);

// Reconhece somas e subtrações entre produtos.
CategoriaExpressao parserSoma(Parser *parser);

// Reconhece uma comparação ou preserva a categoria de uma expressão.
CategoriaExpressao parserComparacao(Parser *parser);

#endif