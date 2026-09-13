#include "token.h"

const char *nomeTipoToken(TipoToken tipo) {
    switch (tipo) {
        case PC_DEC:              return "PCDec";
        case PC_PROG:              return "PCProg";
        case PC_INT:               return "PCInt";
        case PC_REAL:              return "PCReal";
        case PC_LER:               return "PCLer";
        case PC_IMPRIMIR:          return "PCImprimir";
        case PC_SE:                return "PCSe";
        case PC_SENAO:             return "PCSenao";
        case PC_ENTAO:             return "PCEntao";
        case PC_ENQTO:             return "PCEnqto";
        case PC_INI:               return "PCIni";
        case PC_FIM:               return "PCFim";

        case OP_ARIT_MULT:         return "OpAritMult";
        case OP_ARIT_DIV:          return "OpAritDiv";
        case OP_ARIT_SOMA:         return "OpAritSoma";
        case OP_ARIT_SUB:          return "OpAritSub";

        case OP_REL_MENOR:         return "OpRelMenor";
        case OP_REL_MENOR_IGUAL:   return "OpRelMenorIgual";
        case OP_REL_MAIOR:         return "OpRelMaior";
        case OP_REL_MAIOR_IGUAL:   return "OpRelMaiorIgual";
        case OP_REL_IGUAL:         return "OpRelIgual";
        case OP_REL_DIF:           return "OpRelDif";

        case OP_BOOL_E:            return "OpBoolE";
        case OP_BOOL_OU:           return "OpBoolOu";

        case DELIM:                return "Delim";
        case ATRIB:                return "Atrib";
        case ABRE_PAR:             return "AbrePar";
        case FECHA_PAR:            return "FechaPar";

        case VAR:                  return "Var";
        case NUM_INT:              return "NumInt";
        case NUM_REAL:             return "NumReal";
        case CADEIA:               return "Cadeia";

        case TOKEN_EOF:            return "EOF";
        case TOKEN_ERRO:           return "Erro";
        default:                   return "Desconhecido";
    }
}
