# Autômatos do analisador léxico

Os arquivos `.jff` desta pasta representam os principais reconhecedores usados
no código do lexer:

- `automato_numeros.jff`: números inteiros e reais.
- `automato_variaveis.jff`: variáveis iniciadas por letra minúscula.
- `automato_cadeias.jff`: cadeias ASCII delimitadas por aspas.
- `automato_operadores_simbolos.jff`: operadores, delimitador, atribuição e
  parênteses.
- `automato_palavras_reservadas.jff`: palavras-chave, `E` e `OU`.

Os nomes dos estados finais indicam o token reconhecido. O JFLAP apenas informa
se a entrada foi aceita; o nome do estado final ajuda a relacionar o diagrama
ao `TipoToken` usado pelo programa.

## Números

O arquivo `automato_numeros.jff` representa o mesmo reconhecimento feito por
`lerNumero` no analisador léxico.

- `q0`: estado inicial.
- `q1_NumInt`: estado final para números inteiros.
- `q2_NumReal`: estado final para números reais.
- Um dígito leva de `q0` a `q1_NumInt`.
- Outros dígitos mantêm o autômato em `q1_NumInt`.
- Um ponto leva de `q1_NumInt` a `q2_NumReal`.
- Dígitos mantêm o autômato em `q2_NumReal`.

Como `q2_NumReal` é final, entradas como `2.` são aceitas, em conformidade com
os programas de teste fornecidos.

Para visualizar, abra o JFLAP 7.1 e use **File > Open** para selecionar um dos
arquivos `.jff`.

Entradas aceitas: `10`, `42`, `3.14`, `0.5` e `2.`.

Entradas rejeitadas: `.5`, `abc` e a cadeia vazia.

## Variáveis

O primeiro caractere deve estar entre `a` e `z`. Depois dele, o autômato aceita
letras maiúsculas, letras minúsculas e dígitos, seguindo `lerVariavel`.

Exemplos aceitos: `x`, `parametro`, `num1` e `valorTotal2`.

## Cadeias

Uma cadeia começa e termina com aspas. O estado de conteúdo possui transições
para os caracteres ASCII imprimíveis, exceto as próprias aspas, que levam ao
estado final.

Exemplos aceitos: `"Oi"`, `"Oi tudo bem?"` e `""`.

## Operadores e símbolos

Esse autômato reconhece `+`, `-`, `*`, `/`, `<`, `<=`, `>`, `>=`, `==`, `!=`,
`:`, `:=`, `(` e `)`. Os estados alcançados por `=` e `!` isolados não são
finais, por isso essas entradas são rejeitadas.

## Palavras reservadas

O autômato forma uma árvore de prefixos. Caminhos podem compartilhar letras,
como `E`, `ENTAO` e `ENQTO`, mas apenas as palavras completas terminam em
estados finais.
