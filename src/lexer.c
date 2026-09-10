#include "lexer.h"

/*
 * Le UM caractere de lex->fonte e joga em lex->caractereAtual.
 * Se o caractere lido for '\n', incrementa lex->linha -- assim a contagem
 * de linha fica sempre correta pra qualquer token que a gente emitir depois.
 *
 * 'static' porque essa funcao e um detalhe interno do lexer; nada fora
 * deste arquivo precisa chamar ela diretamente.
 */
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

    /*
     * Le o primeiro caractere ja na inicializacao. Isso implementa o
     * padrao "lookahead de 1": em qualquer momento, lex->caractereAtual
     * e o proximo caractere ainda NAO processado. Toda funcao de leitura
     * (que a dupla vai escrever nas proximas partes) vai OLHAR esse
     * caractere pra decidir o que fazer, e so DEPOIS chamar avancar().
     */
    avancar(lex);
}

/*
 * Consome espacos, tabs, quebras de linha e comentarios de linha (# ate \n),
 * ate o caractere atual ser algo "de verdade" (inicio de token) ou EOF.
 *
 * Fica num loop porque pode ter espaco, depois comentario, depois mais
 * espaco de novo -- precisa continuar pulando ate sobrar algo relevante.
 */
static void ignorarEspacosEComentarios(Lexer *lex) {
    for (;;) {
        if (lex->caractereAtual == ' '  || lex->caractereAtual == '\t' ||
            lex->caractereAtual == '\r' || lex->caractereAtual == '\n') {
            avancar(lex);
        } else if (lex->caractereAtual == '#') {
            while (lex->caractereAtual != '\n' && lex->caractereAtual != EOF) {
                avancar(lex);
            }
            /* nao precisa dar avancar() extra aqui: o proximo loop do 'for'
               vai cair no caso '\n' acima (se nao for EOF) e consumir ele */
        } else {
            break;
        }
    }
}

/*
 * FUNCAO TEMPORARIA, so pra testar a Parte 1 isoladamente.
 * Le o arquivo inteiro pulando espaco/comentario e imprime cada caractere
 * "de verdade" com a linha onde ele esta. Apaga isso (e o header em
 * lexer.h) quando a Parte 6 (integracao) comecar a escrever proximoToken --
 * ele que vai assumir esse papel de "olhar o caractere atual".
 */
void lexerDebugImprimirTudo(Lexer *lex) {
    ignorarEspacosEComentarios(lex);
    while (lex->caractereAtual != EOF) {
        printf("linha %d: '%c'\n", lex->linha, lex->caractereAtual);
        avancar(lex);
        ignorarEspacosEComentarios(lex);
    }
    printf("linha %d: EOF\n", lex->linha);
}