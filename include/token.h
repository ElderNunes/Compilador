#ifndef TOKEN_H
#define TOKEN_H

#define MAX_LEXEMA 256

enum TipoToken {
    PC_DEC, PC_PROG, PC_INT, PC_REAL, PC_LER, PC_IMPRIMIR,
    PC_SE, PC_SENAO, PC_ENTAO, PC_ENQTO, PC_INI, PC_FIM,

    OP_ARIT_MULT, OP_ARIT_DIV, OP_ARIT_SOMA, OP_ARIT_SUB,

    OP_REL_MENOR, OP_REL_MENOR_IGUAL, OP_REL_MAIOR, OP_REL_MAIOR_IGUAL,
    OP_REL_IGUAL, OP_REL_DIF,

    OP_BOOL_E, OP_BOOL_OU,

    DELIM, ATRIB, ABRE_PAR, FECHA_PAR,

    VAR, NUM_INT, NUM_REAL, CADEIA,

    TOKEN_EOF, TOKEN_ERRO
};

struct Token {
    TipoToken tipo;
    char lexema[MAX_LEXEMA];
    int linha;
};

const char *nomeTipoToken(TipoToken tipo);

#endif
