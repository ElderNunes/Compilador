#include "parser.h"

// Indica se o token atual pode iniciar um comando.
static bool iniciaComando(const Parser *parser) {
    switch (parser->atual.tipo) {
        case VAR:
        case PC_LER:
        case PC_IMPRIMIR:
        case PC_SE:
        case PC_ENQTO:
        case PC_INI:
            return true;

        default:
            return false;
    }
}

// ListaComandos = Comando { Comando } ;
// Para quando o próximo token não inicia comando; quem chamou valida
// o fechamento (EOF no programa, 'end' no bloco).
bool parserListaComandos(Parser *parser) {
    if (!parserComando(parser)) {
        return false;
    }

    while (iniciaComando(parser)) {
        if (!parserComando(parser)) {
            return false;
        }
    }

    return true;
}

// Comando = Atribuicao | Entrada | Saida | Condicao | Repeticao | SubAlgoritmo ;
bool parserComando(Parser *parser) {
    if (parser->erro) {
        return false;
    }

    switch (parser->atual.tipo) {
        case VAR:         return parserAtribuicao(parser);
        case PC_LER:      return parserEntrada(parser);
        case PC_IMPRIMIR: return parserSaida(parser);
        case PC_SE:       return parserCondicao(parser);
        case PC_ENQTO:    return parserRepeticao(parser);
        case PC_INI:      return parserSubAlgoritmo(parser);

        default:
            return parserErro(
                parser,
                "comando (variável, 'read', 'print', 'if', 'while' ou 'start')"
            );
    }
}

// Atribuicao = VAR "<<" ExpressaoAritmetica ;
bool parserAtribuicao(Parser *parser) {
    if (!parserConsumir(parser, VAR)) {
        return false;
    }

    if (!parserConsumir(parser, ATRIB)) {
        return false;
    }

    return parserExpressaoAritmetica(parser);
}

// Entrada = "read" VAR ;
bool parserEntrada(Parser *parser) {
    if (!parserConsumir(parser, PC_LER)) {
        return false;
    }

    return parserConsumir(parser, VAR);
}

// Saida = "print" ( VAR | CADEIA ) ;
bool parserSaida(Parser *parser) {
    if (!parserConsumir(parser, PC_IMPRIMIR)) {
        return false;
    }

    if (parser->atual.tipo == VAR || parser->atual.tipo == CADEIA) {
        return parserAvancar(parser);
    }

    return parserErro(parser, "variável ou cadeia");
}

// Condicao = "if" ExpressaoRelacional "then" Comando [ "else" Comando ] ;
// O 'else' é consumido assim que aparece, então pertence ao 'if' mais próximo.
bool parserCondicao(Parser *parser) {
    if (!parserConsumir(parser, PC_SE)) {
        return false;
    }

    if (!parserExpressaoRelacional(parser)) {
        return false;
    }

    if (!parserConsumir(parser, PC_ENTAO)) {
        return false;
    }

    if (!parserComando(parser)) {
        return false;
    }

    if (parser->atual.tipo == PC_SENAO) {
        if (!parserConsumir(parser, PC_SENAO)) {
            return false;
        }

        return parserComando(parser);
    }

    return true;
}

// Repeticao = "while" ExpressaoRelacional "then" Comando ;
bool parserRepeticao(Parser *parser) {
    if (!parserConsumir(parser, PC_ENQTO)) {
        return false;
    }

    if (!parserExpressaoRelacional(parser)) {
        return false;
    }

    if (!parserConsumir(parser, PC_ENTAO)) {
        return false;
    }

    return parserComando(parser);
}

// SubAlgoritmo = "start" ListaComandos "end" ;
bool parserSubAlgoritmo(Parser *parser) {
    if (!parserConsumir(parser, PC_INI)) {
        return false;
    }

    if (!parserListaComandos(parser)) {
        return false;
    }

    return parserConsumir(parser, PC_FIM);
}
