#include <cstdio>
#include "parser.h"

void parserInicializar(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    parser->erro = false;

    parserAvancar(parser);
}

bool parserAvancar(Parser *parser) {
    if (parser->erro) {
        return false;
    }

    parser->atual = proximoToken(parser->lexer);

    if (parser->atual.tipo == TOKEN_ERRO) {
        parser->erro = true;
        return false;
    }

    return true;
}

bool parserErro(Parser *parser, const char *esperado) {
    if (!parser->erro) {
        fprintf(
            stderr,
            "Erro sintático na linha %d: esperado %s, "
            "encontrado %s (\"%s\")\n",
            parser->atual.linha,
            esperado,
            nomeTipoToken(parser->atual.tipo),
            parser->atual.lexema
        );
    }

    parser->erro = true;
    return false;
}

bool parserConsumir(Parser *parser, TipoToken esperado) {
    if (parser->erro) {
        return false;
    }

    if (parser->atual.tipo != esperado) {
        return parserErro(parser, nomeTipoToken(esperado));
    }

    if (esperado == TOKEN_EOF) {
        return true;
    }

    return parserAvancar(parser);
}

bool parserTipoVar(Parser *parser) {
    if (parser->erro) {
        return false;
    }

    switch (parser->atual.tipo) {
        case PC_INT:
        case PC_REAL:
            return parserAvancar(parser);

        default:
            return parserErro(parser, "'integer' ou 'float'");
    }
}

bool parserDeclaracao(Parser *parser) {
    if (!parserConsumir(parser, VAR)) {
        return false;
    }

    if (!parserConsumir(parser, INI_DELIM)) {
        return false;
    }

    if (!parserTipoVar(parser)) {
        return false;
    }

    return parserConsumir(parser, FIM_DELIM);
}

bool parserListaDeclaracoes(Parser *parser) {
    if (!parserDeclaracao(parser)) {
        return false;
    }

    while (parser->atual.tipo == VAR) {
        if (!parserDeclaracao(parser)) {
            return false;
        }
    }

    return true;
}

bool parserPrograma(Parser *parser) {
    if (!parserConsumir(parser, INI_DELIM)) {
        return false;
    }

    if (!parserConsumir(parser, PC_DEC)) {
        return false;
    }

    if (!parserConsumir(parser, FIM_DELIM)) {
        return false;
    }

    if (!parserListaDeclaracoes(parser)) {
        return false;
    }

    if (!parserConsumir(parser, INI_DELIM)) {
        return false;
    }

    if (!parserConsumir(parser, PC_PROG)) {
        return false;
    }

    if (!parserConsumir(parser, FIM_DELIM)) {
        return false;
    }

    if (!parserListaComandos(parser)) {
        return false;
    }

    return parserConsumir(parser, TOKEN_EOF);
}

bool parserExpressaoAritmetica(Parser *parser) {
    CategoriaExpressao categoria = parserExpressaoGeral(parser);

    if (categoria == CategoriaExpressao::INVALIDA) {
        return false;
    }

    if (categoria != CategoriaExpressao::ARITMETICA) {
        return parserErro(parser, "expressão aritmética");
    }

    return true;
}

bool parserExpressaoRelacional(Parser *parser) {
    CategoriaExpressao categoria = parserExpressaoGeral(parser);

    if (categoria == CategoriaExpressao::INVALIDA) {
        return false;
    }

    if (categoria != CategoriaExpressao::RELACIONAL) {
        return parserErro(parser, "expressão relacional");
    }

    return true;
}

bool parserOperadorRelacional(Parser *parser) {
    if (parser->erro) {
        return false;
    }

    switch (parser->atual.tipo) {
        case OP_REL_MENOR:
        case OP_REL_MENOR_IGUAL:
        case OP_REL_MAIOR:
        case OP_REL_MAIOR_IGUAL:
        case OP_REL_IGUAL:
        case OP_REL_DIF:
            return parserAvancar(parser);

        default:
            return parserErro(
                parser,
                "operador relacional (<, <=, >, >=, == ou <>)"
            );
    }
}

bool parserOperadorBooleano(Parser *parser) {
    if (parser->erro) {
        return false;
    }

    switch (parser->atual.tipo) {
        case OP_BOOL_E:
        case OP_BOOL_OU:
            return parserAvancar(parser);

        default:
            return parserErro(parser, "'and' ou 'or'");
    }
}

CategoriaExpressao parserPrimario(Parser *parser) {
    if (parser->erro) {
        return CategoriaExpressao::INVALIDA;
    }

    switch (parser->atual.tipo) {
        case NUM_INT:
        case NUM_REAL:
        case VAR:
            if (!parserAvancar(parser)) {
                return CategoriaExpressao::INVALIDA;
            }

            return CategoriaExpressao::ARITMETICA;

        case ABRE_PAR: {
            if (!parserConsumir(parser, ABRE_PAR)) {
                return CategoriaExpressao::INVALIDA;
            }

            CategoriaExpressao categoria =
                parserExpressaoGeral(parser);

            if (categoria == CategoriaExpressao::INVALIDA) {
                return CategoriaExpressao::INVALIDA;
            }

            if (!parserConsumir(parser, FECHA_PAR)) {
                return CategoriaExpressao::INVALIDA;
            }

            return categoria;
        }

        default:
            parserErro(parser, "número, variável ou '('");
            return CategoriaExpressao::INVALIDA;
    }
}

CategoriaExpressao parserProduto(Parser *parser) {
    CategoriaExpressao esquerda = parserPrimario(parser);

    if (esquerda == CategoriaExpressao::INVALIDA) {
        return CategoriaExpressao::INVALIDA;
    }

    while (parser->atual.tipo == OP_ARIT_MULT ||
           parser->atual.tipo == OP_ARIT_DIV) {

        if (esquerda != CategoriaExpressao::ARITMETICA) {
            parserErro(parser, "expressão aritmética antes de '*' ou '/'");
            return CategoriaExpressao::INVALIDA;
        }

        if (!parserAvancar(parser)) {
            return CategoriaExpressao::INVALIDA;
        }

        CategoriaExpressao direita = parserPrimario(parser);

        if (direita == CategoriaExpressao::INVALIDA) {
            return CategoriaExpressao::INVALIDA;
        }

        if (direita != CategoriaExpressao::ARITMETICA) {
            parserErro(parser, "expressão aritmética após '*' ou '/'");
            return CategoriaExpressao::INVALIDA;
        }

        esquerda = CategoriaExpressao::ARITMETICA;
    }

    return esquerda;
}

CategoriaExpressao parserSoma(Parser *parser) {
    CategoriaExpressao esquerda = parserProduto(parser);

    if (esquerda == CategoriaExpressao::INVALIDA) {
        return CategoriaExpressao::INVALIDA;
    }

    while (parser->atual.tipo == OP_ARIT_SOMA ||
           parser->atual.tipo == OP_ARIT_SUB) {

        if (esquerda != CategoriaExpressao::ARITMETICA) {
            parserErro(parser, "expressão aritmética antes de '+' ou '-'");
            return CategoriaExpressao::INVALIDA;
        }

        if (!parserAvancar(parser)) {
            return CategoriaExpressao::INVALIDA;
        }

        CategoriaExpressao direita = parserProduto(parser);

        if (direita == CategoriaExpressao::INVALIDA) {
            return CategoriaExpressao::INVALIDA;
        }

        if (direita != CategoriaExpressao::ARITMETICA) {
            parserErro(parser, "expressão aritmética após '+' ou '-'");
            return CategoriaExpressao::INVALIDA;
        }
    }

    return esquerda;
}

CategoriaExpressao parserComparacao(Parser *parser) {
    CategoriaExpressao esquerda = parserSoma(parser);

    if (esquerda == CategoriaExpressao::INVALIDA) {
        return CategoriaExpressao::INVALIDA;
    }

    switch (parser->atual.tipo) {
        case OP_REL_MENOR:
        case OP_REL_MENOR_IGUAL:
        case OP_REL_MAIOR:
        case OP_REL_MAIOR_IGUAL:
        case OP_REL_IGUAL:
        case OP_REL_DIF:
            break;

        default:
            return esquerda;
    }

    if (esquerda != CategoriaExpressao::ARITMETICA) {
        parserErro(parser, "expressão aritmética antes da comparação");
        return CategoriaExpressao::INVALIDA;
    }

    if (!parserOperadorRelacional(parser)) {
        return CategoriaExpressao::INVALIDA;
    }

    CategoriaExpressao direita = parserSoma(parser);

    if (direita == CategoriaExpressao::INVALIDA) {
        return CategoriaExpressao::INVALIDA;
    }

    if (direita != CategoriaExpressao::ARITMETICA) {
        parserErro(parser, "expressão aritmética após a comparação");
        return CategoriaExpressao::INVALIDA;
    }

    return CategoriaExpressao::RELACIONAL;
}

CategoriaExpressao parserExpressaoGeral(Parser *parser) {
    CategoriaExpressao esquerda = parserComparacao(parser);

    if (esquerda == CategoriaExpressao::INVALIDA) {
        return CategoriaExpressao::INVALIDA;
    }

    while (parser->atual.tipo == OP_BOOL_E ||
           parser->atual.tipo == OP_BOOL_OU) {

        if (esquerda != CategoriaExpressao::RELACIONAL) {
            parserErro(parser, "condição antes de 'and' ou 'or'");
            return CategoriaExpressao::INVALIDA;
        }

        if (!parserOperadorBooleano(parser)) {
            return CategoriaExpressao::INVALIDA;
        }

        CategoriaExpressao direita = parserComparacao(parser);

        if (direita == CategoriaExpressao::INVALIDA) {
            return CategoriaExpressao::INVALIDA;
        }

        if (direita != CategoriaExpressao::RELACIONAL) {
            parserErro(parser, "condição após 'and' ou 'or'");
            return CategoriaExpressao::INVALIDA;
        }

        esquerda = CategoriaExpressao::RELACIONAL;
    }

    return esquerda;
}