# Tarefas — 2ª avaliação

Responsáveis sugeridos com base na dupla já identificada no código. Todas as tarefas abaixo estão **pendentes**; este documento não indica que o parser já funciona.

| ID | Responsável | Entrega e arquivos | Dependência | Critério de conclusão |
|---|---|---|---|---|
| S01 | Gabriel | Adaptar `include/token.h`, `src/token.cpp`, `src/lexer.cpp` e testes léxicos | Nenhuma | Reconhece palavras minúsculas, variáveis maiúsculas, `[`/`]`, `<<`, `<>`; rejeita sintaxe antiga conforme contrato |
| S02 | Elder | Criar `include/parser.h` e `src/parser.cpp`: estado, consumo, diagnóstico, programa e declarações | Contrato em SINTATICO.md; integração depende de S01 | Valida `[dec]`, ao menos uma declaração, `[prog]`, lista não vazia e EOF; erro léxico não vira erro sintático duplicado |
| S03 | Elder | Criar `src/parser_expressoes.cpp`: expressões aritméticas, relacionais e booleanas | S02 | Precedência aritmética correta; `and`/`or` no mesmo nível; trata parênteses aritméticos e relacionais sem rejeitar entradas válidas |
| S04 | Gabriel | Criar `src/parser_comandos.cpp`: listas, atribuição, entrada, saída, if/else, while, blocos | Interface de S02 e funções de S03 | `else` associa ao `if` mais próximo; bloco não vazio; `while` exige `then`; `print` aceita somente variável ou cadeia |
| S05 | Gabriel | Criar testes sintáticos positivos e negativos em `tests/sintatico/` e executor | S01–S04 para rodar; escrever entradas desde já | Código de saída e diagnóstico conferidos; cobre todas as produções e casos da matriz em SINTATICO.md |
| S06 | Elder | Integrar `src/main.cpp`, `Makefile`, README e testes automatizados | S01–S05 | `make`, testes léxicos novos e sintáticos passam; CLI e cabeçalho corretos; rejeita tokens extras após programa |
| S07 | Ambos | Revisão cruzada e preparação da entrega | S06 | Cada integrante explica os módulos do colega; conferir requisitos e arquivos exigidos |

## Organização para evitar conflitos

- Gabriel concentra alterações do lexer e escreve os comandos em arquivo próprio.
- Elder define primeiro o header do parser e escreve núcleo/expressões em arquivos próprios.
- Elder centraliza alterações no Makefile e na main; Gabriel entrega o executor de testes para integração.
- Não deixar `static` em funções que outro `.cpp` precisa chamar; declarar a interface compartilhada no header.
- S01 e o header de S02 podem avançar juntos. S03 e S04 avançam em paralelo após fechar a interface, mesmo antes da implementação de todas as funções chamadas.

## Ordem sugerida de PRs

1. `feature/s01-lexer-repaginado` — Gabriel; revisão de Elder.
2. `feature/s02-parser-base` — Elder; revisão de Gabriel.
3. `feature/s03-expressoes` — Elder; revisão de Gabriel.
4. `feature/s04-comandos` — Gabriel; revisão de Elder.
5. `feature/s05-testes` — Gabriel; revisão de Elder.
6. `feature/s06-integracao` — Elder; revisão de Gabriel.

Este arquivo é o quadro inicial de tarefas. Issues no GitHub ainda não foram criadas; os IDs S01–S07 permitem referenciar cada tarefa em commits e PRs.
