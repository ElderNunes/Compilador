# Especificação de implementação — GYH repaginada

## Referências e decisões

Referências lidas: `Gramática GYH-Repaginada (1)(1).pdf` e `Especificação 2a Avaliação (Trab 2) (1)(1).pdf`, enviados pela dupla. A primeira define a sintaxe adotada; a segunda define prazo, autoria, implementação manual e apresentação de erros. A especificação contém exemplos da linguagem antiga e não deve ser copiada literalmente para os testes novos.

Decisões propostas para implementação, sem atribuí-las ao professor:

- Analisador descendente recursivo manual, com consumo integral da entrada.
- Remover recursão à esquerda das expressões por laços.
- `*` e `/` têm precedência sobre `+` e `-`; operadores de cada nível associam à esquerda.
- `and` e `or` permanecem no mesmo nível, como no PDF. Não introduzir precedência extra de `and`.
- `else` pertence ao `if` não pareado mais próximo.
- Listas de declarações, de comandos e blocos têm ao menos um elemento.
- A gramática não contém sinal unário, `print` de expressão nem separador `;`.
- Manter comentários `#` e regra de cadeias do lexer existente como convenção herdada; a gramática repaginada não especifica comentários nem escapes.
- Não implementar verificação de declaração prévia ou tipos nesta etapa: isso é análise semântica.
- Parar no primeiro erro; não implementar recuperação silenciosa nem imprimir sucesso após erro.

## Forma operacional das produções

Notação: `{ X }` significa zero ou mais repetições e `[ X ]` representa opcionalidade. Colchetes **entre aspas** são tokens literais.

```ebnf
Programa = "[" "dec" "]" ListaDeclaracoes "[" "prog" "]" ListaComandos EOF ;
ListaDeclaracoes = Declaracao { Declaracao } ;
Declaracao = VAR "[" TipoVar "]" ;
TipoVar = "integer" | "float" ;
ExpressaoAritmetica = TermoAritmetico { ("+" | "-") TermoAritmetico } ;
TermoAritmetico = FatorAritmetico { ("*" | "/") FatorAritmetico } ;
FatorAritmetico = NUMINT | NUMREAL | VAR | "(" ExpressaoAritmetica ")" ;
ExpressaoRelacional = TermoRelacional { ("or" | "and") TermoRelacional } ;
TermoRelacional = ExpressaoAritmetica OP_REL ExpressaoAritmetica
                | "(" ExpressaoRelacional ")" ;
ListaComandos = Comando { Comando } ;
Comando = Atribuicao | Entrada | Saida | Condicao | Repeticao | SubAlgoritmo ;
Atribuicao = VAR "<<" ExpressaoAritmetica ;
Entrada = "read" VAR ;
Saida = "print" (VAR | CADEIA) ;
Condicao = "if" ExpressaoRelacional "then" Comando [ "else" Comando ] ;
Repeticao = "while" ExpressaoRelacional "then" Comando ;
SubAlgoritmo = "start" ListaComandos "end" ;
OP_REL = "<" | "<=" | ">" | ">=" | "==" | "<>" ;
```

A forma opcional de `else` precisa da convenção descrita acima. Esta reescrita **não prova que a gramática completa é LL(1)**.

## Atenção especial aos parênteses

Em `TermoRelacional`, as duas alternativas podem começar por `(`. Escolher sempre uma comparação entre expressões aritméticas rejeitaria `(A < B)`, e escolher sempre agrupamento relacional rejeitaria `(A + 1) < B`.

Casos que devem ser aceitos:

```text
if (A + 1) < B then print A
if (A < B) then print A
if ((A + 1) < B and (C > D)) then print A
if ((A + 1)) < B then print A
if A < (B + C) then print A
```

Para manter reconhecimento sem backtracking, propor um núcleo de expressões por níveis: primário (valores e expressão entre parênteses), multiplicação/divisão, soma/subtração, uma comparação e sequência booleana. Cada resultado carrega uma categoria **sintática** aritmética ou relacional. Exigir operandos aritméticos nas contas/comparações e relacionais nos booleanos; atribuição exige resultado aritmético e condição exige relacional. Assim, os parênteses podem envolver qualquer categoria internamente sem aceitar `(A < B) + C`, `A < B < C` ou `A and B`. Isso é controle de forma gramatical, sem inferir tipos declarados das variáveis.

Se a dupla preferir outra técnica, documentar e testar a decisão antes de integrar; não anunciar LL(1) ou ausência de backtracking sem garantir isso na implementação.

## Contrato entre arquivos (a implementar)

Reaproveitar os tipos `Token`, `TipoToken`, `Lexer` e a função `proximoToken(Lexer*)`. Os nomes internos existentes podem continuar (`PC_LER` para `read`, por exemplo), desde que a tabela de lexemas seja alterada. Substituir `DELIM` por `INI_DELIM` e `FIM_DELIM` nos tokens e na conversão dos nomes.

Interface proposta em `include/parser.h`:

```cpp
struct Parser {
    Lexer *lexer;
    Token atual;
    bool erro;
};
void parserInicializar(Parser *parser, Lexer *lexer);
bool parserAvancar(Parser *parser);
bool parserConsumir(Parser *parser, TipoToken esperado);
bool parserErro(Parser *parser, const char *esperado);
bool parserPrograma(Parser *parser);
bool parserListaDeclaracoes(Parser *parser);
bool parserDeclaracao(Parser *parser);
bool parserExpressaoAritmetica(Parser *parser);
bool parserExpressaoRelacional(Parser *parser);
bool parserListaComandos(Parser *parser);
bool parserComando(Parser *parser);
```

- Inicializar carrega o primeiro token; `TOKEN_ERRO` marca falha imediatamente.
- Toda função retorna `false` ao falhar, sem consumir novos tokens após erro.
- `parserConsumir` compara tipo e avança apenas no sucesso; EOF aceito sem solicitar tokens indefinidamente.
- Funções de expressão deixam em `atual` o primeiro token não pertencente à expressão, permitindo `then`, `)`, outro comando ou EOF.
- Núcleo e declarações: `parser.cpp`; expressão: `parser_expressoes.cpp`; comandos: `parser_comandos.cpp`.
- `parserListaComandos` exige primeiro comando e repete enquanto o próximo token inicia comando (`VAR`, `read`, `print`, `if`, `while`, `start`). O chamador valida o fechamento adequado: `end` no bloco ou EOF no programa.
- `if` consome apenas um comando em cada ramo; múltiplos comandos exigem `start ... end`.
- Formato proposto: `Erro sintático na linha N: esperado <descrição>, encontrado <tipo> ("lexema")`.
- `TOKEN_ERRO` já vem diagnosticado pelo lexer; propagar falha sem segunda mensagem enganosa.
- CLI proposta: `./sintatico arquivo.gyh`, sucesso com retorno 0, falha com retorno diferente de 0. Manter modo léxico separado e documentado.

## Matriz mínima de validação

| Grupo | Casos |
|---|---|
| Estrutura | programa mínimo, múltiplas declarações, inteiro e real, ausência de `[dec]`/`[prog]`, declaração/comando vazio, token após fim |
| Aritmética | todos os operadores, precedência, parênteses aninhados, operando ausente, parêntese aberto, unário inválido |
| Relações | os seis operadores, `and`/`or` misturados, ambos os usos de parênteses, comparação encadeada inválida |
| Atribuição | inteiro, real, variável, expressão; ausência de `<<`; tentativa de atribuir comparação |
| Entrada/saída | `read VAR`, `print VAR`, `print CADEIA`; rejeitar `read 1`, `print 1` e `print A + B` |
| Controle | if simples, if/else, else mais próximo, while com then, blocos aninhados; then/end ausente, else solto, bloco vazio |
| Léxico | palavras novas, variáveis maiúsculas, símbolos novos, caractere inválido e cadeia sem fechamento; sem diagnóstico sintático duplicado |
| Diagnóstico | código de saída e linha correta, incluindo erro no fim de uma linha e EOF |

A rotina atual de leitura incrementa a linha ao carregar a quebra de linha; revisar a contagem antes da integração para evitar erro deslocado em tokens imediatamente antes de `\n`.

## Checklist final

- [ ] Lexer migrado e testes antigos reclassificados como históricos.
- [ ] Todos os módulos do parser implementados manualmente.
- [ ] Matriz de casos executada com verificação de retornos e mensagens.
- [ ] Makefile compila módulos novos e executa testes sem mascarar falhas.
- [ ] Primeira linha da main contém nomes da dupla; conferir RAs existentes.
- [ ] README reflete os comandos realmente disponíveis após integração.
- [ ] Entrega revisada pelos dois até 13/11/2026.
