#ifndef TOKEN_H
#define TOKEN_H

#define MAX_LEXEMA 256

// Todos os tipos de token reconhecidos pela linguagem GYH.
enum TipoToken {
    // Palavras-chave.
    PC_DEC, PC_PROG, PC_INT, PC_REAL, PC_LER, PC_IMPRIMIR,
    PC_SE, PC_SENAO, PC_ENTAO, PC_ENQTO, PC_INI, PC_FIM,

    // Operadores aritméticos e relacionais.
    OP_ARIT_MULT, OP_ARIT_DIV, OP_ARIT_SOMA, OP_ARIT_SUB,

    OP_REL_MENOR, OP_REL_MENOR_IGUAL, OP_REL_MAIOR, OP_REL_MAIOR_IGUAL,
    OP_REL_IGUAL, OP_REL_DIF,

    // Operadores booleanos.
    OP_BOOL_E, OP_BOOL_OU,

    // Símbolos e categorias de valores.
    DELIM, ATRIB, ABRE_PAR, FECHA_PAR,

    VAR, NUM_INT, NUM_REAL, CADEIA,

    TOKEN_EOF, TOKEN_ERRO
};

// Resultado produzido pelo lexer para cada lexema da entrada.
struct Token {
    TipoToken tipo;
    char lexema[MAX_LEXEMA];
    int linha;
};

const char *nomeTipoToken(TipoToken tipo);

#endif
