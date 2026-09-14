#include <cctype>
#include <cstring>

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

struct PalavraReservada {
    const char *palavra;
    TipoToken tipo;
};

static const PalavraReservada TABELA_PALAVRAS[] = {
    {"DEC",      PC_DEC},
    {"PROG",     PC_PROG},
    {"INT",      PC_INT},
    {"REAL",     PC_REAL},
    {"LER",      PC_LER},
    {"IMPRIMIR", PC_IMPRIMIR},
    {"SE",       PC_SE},
    {"SENAO",    PC_SENAO},
    {"ENTAO",    PC_ENTAO},
    {"ENQTO",    PC_ENQTO},
    {"INI",      PC_INI},
    {"FIM",      PC_FIM},
    {"E",        OP_BOOL_E},
    {"OU",       OP_BOOL_OU},
};

#define QTD_PALAVRAS_RESERVADAS (sizeof(TABELA_PALAVRAS) / sizeof(PalavraReservada))

static Token montarToken(TipoToken tipo, const char *lexema, int linha) {
    Token t;
    t.tipo = tipo;
    t.linha = linha;
    strncpy(t.lexema, lexema, MAX_LEXEMA - 1);
    t.lexema[MAX_LEXEMA - 1] = '\0';
    return t;
}

static Token tokenErro(Lexer *lex, const char *lexemaInvalido, int linha) {
    lex->erroOcorreu = 1;
    fprintf(stderr, "Erro Lexico na linha %d: Desconhecido \"%s\"\n", linha, lexemaInvalido);
    return montarToken(TOKEN_ERRO, lexemaInvalido, linha);
}


// Procura o 'Lexema' da palavra reservada
static TipoToken buscarPalavraReservada(const char *lexema) {
    for (size_t i = 0; i < QTD_PALAVRAS_RESERVADAS; i++) {
        if (strcmp(TABELA_PALAVRAS[i].palavra, lexema) == 0) {
            return TABELA_PALAVRAS[i].tipo;
        }
    }
    return TOKEN_ERRO;
}

static Token lerPalavraMaiuscula(Lexer *lex) {
    char buffer[MAX_LEXEMA];
    int i = 0;
    int linhaInicio = lex->linha;
 
    while (isalnum(static_cast<unsigned char>(lex->caractereAtual)) && i < MAX_LEXEMA - 1) {
        buffer[i++] = static_cast<char>(lex->caractereAtual);
        avancar(lex);
    }
    buffer[i] = '\0';
 
    TipoToken tipo = buscarPalavraReservada(buffer);
    if (tipo != TOKEN_ERRO) {
        return montarToken(tipo, buffer, linhaInicio);
    }
    return tokenErro(lex, buffer, linhaInicio);
}

static Token lerSimboloParte2(Lexer *lex) {
    int linhaInicio = lex->linha;
    char c = static_cast<char>(lex->caractereAtual);
 
    switch (c) {
        case '(':
            avancar(lex);
            return montarToken(ABRE_PAR, "(", linhaInicio);
        case ')':
            avancar(lex);
            return montarToken(FECHA_PAR, ")", linhaInicio);
        case ':':
            avancar(lex);
            if (lex->caractereAtual == '=') {
                avancar(lex);
                return montarToken(ATRIB, ":=", linhaInicio);
            }
            return montarToken(DELIM, ":", linhaInicio);
        default: {
            char lexemaInvalido[2] = {c, '\0'};
            avancar(lex);
            return tokenErro(lex, lexemaInvalido, linhaInicio);
        }
    }
}

// TEMPORARIA - Teste pra Parte 2 
Token lexerTesteParte2(Lexer *lex) {
    ignorarEspacosEComentarios(lex);
 
    int linhaAtual = lex->linha;
 
    if (lex->caractereAtual == EOF) {
        return montarToken(TOKEN_EOF, "EOF", linhaAtual);
    }
 
    if (isupper(static_cast<unsigned char>(lex->caractereAtual))) {
        return lerPalavraMaiuscula(lex);
    }
 
    if (lex->caractereAtual == '(' || lex->caractereAtual == ')' || lex->caractereAtual == ':') {
        return lerSimboloParte2(lex);
    }
 
    char lexemaInvalido[2] = {static_cast<char>(lex->caractereAtual), '\0'};
    avancar(lex);
    return tokenErro(lex, lexemaInvalido, linhaAtual);
}