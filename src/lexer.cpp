#include <cctype>
#include <cstring>

#include "lexer.h"

// Consome um caractere e mantém a linha atualizada.
static void avancar(Lexer *lex) {
    lex->caractereAtual = fgetc(lex->fonte);
    if (lex->caractereAtual == '\n') {
        lex->linha++;
    }
}

// Prepara o estado do lexer e carrega o primeiro caractere.
void lexerInicializar(Lexer *lex, FILE *fonte) {
    lex->fonte = fonte;
    lex->linha = 1;
    lex->erroOcorreu = 0;

    avancar(lex);
}

// Descarta caracteres sem significado léxico e comentários iniciados por '#'.
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

// Função auxiliar para inspecionar a leitura caractere por caractere.
void lexerDebugImprimirTudo(Lexer *lex) {
    ignorarEspacosEComentarios(lex);
    while (lex->caractereAtual != EOF) {
        printf("linha %d: '%c'\n", lex->linha, lex->caractereAtual);
        avancar(lex);
        ignorarEspacosEComentarios(lex);
    }
    printf("linha %d: EOF\n", lex->linha);
}

// Relaciona cada palavra reservada ao seu tipo de token.
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

// Cria um token e copia seu lexema com limite de tamanho.
static Token montarToken(TipoToken tipo, const char *lexema, int linha) {
    Token t;
    t.tipo = tipo;
    t.linha = linha;
    strncpy(t.lexema, lexema, MAX_LEXEMA - 1);
    t.lexema[MAX_LEXEMA - 1] = '\0';
    return t;
}

// Registra, informa e devolve um erro léxico para a função principal.
static Token tokenErro(Lexer *lex, const char *lexemaInvalido, int linha) {
    lex->erroOcorreu = 1;
    fprintf(stderr, "Erro Léxico na linha %d: Desconhecido \"%s\"\n", linha, lexemaInvalido);
    return montarToken(TOKEN_ERRO, lexemaInvalido, linha);
}

// Procura o lexema na tabela de palavras reservadas.
static TipoToken buscarPalavraReservada(const char *lexema) {
    for (size_t i = 0; i < QTD_PALAVRAS_RESERVADAS; i++) {
        if (strcmp(TABELA_PALAVRAS[i].palavra, lexema) == 0) {
            return TABELA_PALAVRAS[i].tipo;
        }
    }
    return TOKEN_ERRO;
}

// Lê uma palavra iniciada por maiúscula e valida se ela é reservada.
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

// Reconhece delimitadores, parênteses e operadores de um ou dois caracteres.
static Token lerSimbolo(Lexer *lex) {
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
        case '+':
            avancar(lex);
            return montarToken(OP_ARIT_SOMA, "+", linhaInicio);
        case '-':
            avancar(lex);
            return montarToken(OP_ARIT_SUB, "-", linhaInicio);
        case '*':
            avancar(lex);
            return montarToken(OP_ARIT_MULT, "*", linhaInicio);
        case '/':
            avancar(lex);
            return montarToken(OP_ARIT_DIV, "/", linhaInicio);
        case '<':
            avancar(lex);
            if (lex->caractereAtual == '=') {
                avancar(lex);
                return montarToken(OP_REL_MENOR_IGUAL, "<=", linhaInicio);
            }
            return montarToken(OP_REL_MENOR, "<", linhaInicio);
        case '>':
            avancar(lex);
            if (lex->caractereAtual == '=') {
                avancar(lex);
                return montarToken(OP_REL_MAIOR_IGUAL, ">=", linhaInicio);
            }
            return montarToken(OP_REL_MAIOR, ">", linhaInicio);
        case '=':
            avancar(lex);
            if (lex->caractereAtual == '=') {
                avancar(lex);
                return montarToken(OP_REL_IGUAL, "==", linhaInicio);
            }
            return tokenErro(lex, "=", linhaInicio);
        case '!':
            avancar(lex);
            if (lex->caractereAtual == '=') {
                avancar(lex);
                return montarToken(OP_REL_DIF, "!=", linhaInicio);
            }
            return tokenErro(lex, "!", linhaInicio);
        default: {
            char lexemaInvalido[2] = {c, '\0'};
            avancar(lex);
            return tokenErro(lex, lexemaInvalido, linhaInicio);
        }
    }
}

// Lê uma variável iniciada por minúscula, seguida por letras ou números.
static Token lerVariavel(Lexer *lex) {
    char buffer[MAX_LEXEMA];
    int i = 0;
    int linhaInicio = lex->linha;

    while(isalnum(static_cast<unsigned char>(lex->caractereAtual)) && i < MAX_LEXEMA - 1) {
        buffer[i++] = static_cast<char>(lex->caractereAtual);
        avancar(lex);
    }

    buffer[i] = '\0';
    return montarToken(VAR, buffer, linhaInicio);
}

// Lê a parte inteira e, se houver ponto, classifica o número como real.
static Token lerNumero(Lexer *lex) {
    char buffer[MAX_LEXEMA];
    int i = 0;
    int linhaInicio = lex->linha;
    bool numeroReal = false;

    while (isdigit(static_cast<unsigned char>(lex->caractereAtual)) && i < MAX_LEXEMA - 1) {
        buffer[i++] = static_cast<char>(lex->caractereAtual);
        avancar(lex);
    }

    // O ponto faz a transição do estado inteiro para o estado real.
    if (lex->caractereAtual == '.' && i < MAX_LEXEMA - 1) {
        numeroReal = true;
        buffer[i++] = '.';
        avancar(lex);

        while(isdigit(static_cast<unsigned char>(lex->caractereAtual)) && i < MAX_LEXEMA - 1) {
            buffer[i++] = static_cast<char>(lex->caractereAtual);
            avancar(lex);
        }
    }

    buffer[i] = '\0';

    if (numeroReal) {
        return montarToken(NUM_REAL, buffer, linhaInicio);
    }

    return montarToken(NUM_INT, buffer, linhaInicio);
}

// Lê o conteúdo entre aspas; quebra de linha ou EOF antes do fechamento é erro.
static Token lerCadeia(Lexer *lex) {
    char buffer[MAX_LEXEMA];
    int i = 0;
    int linhaInicio = lex->linha;

    avancar(lex); // Consome as aspas iniciais.

    while (lex->caractereAtual != '"' &&
           lex->caractereAtual != '\n' &&
           lex->caractereAtual != '\r' &&
           lex->caractereAtual != EOF &&
           i < MAX_LEXEMA - 1) {
        buffer[i++] = static_cast<char>(lex->caractereAtual);
        avancar(lex);
    }

    buffer[i] = '\0';

    if (lex->caractereAtual != '"') {
        return tokenErro(lex, buffer, linhaInicio);
    }

    avancar(lex); // Consome as aspas finais.
    return montarToken(CADEIA, buffer, linhaInicio);
}

// Decide qual autômato deve reconhecer o próximo token da entrada.
Token proximoToken(Lexer *lex) {
    ignorarEspacosEComentarios(lex);
 
    int linhaAtual = lex->linha;
 
    if (lex->caractereAtual == EOF) {
        return montarToken(TOKEN_EOF, "EOF", linhaAtual);
    }
 
    if (isupper(static_cast<unsigned char>(lex->caractereAtual))) {
        return lerPalavraMaiuscula(lex);
    }

    if (islower(static_cast<unsigned char>(lex->caractereAtual))) {
        return lerVariavel(lex);
    }
 
    if (lex->caractereAtual == '(' || lex->caractereAtual == ')' ||
        lex->caractereAtual == ':' || lex->caractereAtual == '+' ||
        lex->caractereAtual == '-' || lex->caractereAtual == '*' ||
        lex->caractereAtual == '/' || lex->caractereAtual == '<' ||
        lex->caractereAtual == '>' || lex->caractereAtual == '=' ||
        lex->caractereAtual == '!') {
        return lerSimbolo(lex);
    }
 
    if (isdigit(static_cast<unsigned char>(lex->caractereAtual))) {
        return lerNumero(lex);
    }

    if (lex->caractereAtual == '"') {
        return lerCadeia(lex);
    }

    // Um caractere que não iniciou um token conhecido é inválido.
    char lexemaInvalido[2] = {static_cast<char>(lex->caractereAtual), '\0'};
    avancar(lex);
    return tokenErro(lex, lexemaInvalido, linhaAtual);
}