# Compilador GYH — Análise léxica e sintática

Disciplina: **Compiladores — UTFPR Apucarana**
Dupla: **Elder Nunes Gonçalves e Gabriel Oliveira de Jesus**
Etapa atual: **planejamento da 2ª avaliação — analisador sintático**
Entrega indicada no enunciado: **13/11/2026**.

## Estado real do projeto

- O analisador léxico da AP1 está implementado em C++17.
- A suíte atual possui 13 entradas e verifica somente aceitação/rejeição léxica da linguagem antiga.
- O analisador sintático **ainda não está implementado**.
- O lexer ainda precisa ser adaptado à gramática repaginada antes da integração do parser.
- Este planejamento adota a gramática repaginada enviada pela dupla como referência da sintaxe. O enunciado da avaliação contém exemplos da versão antiga; as diferenças estão documentadas abaixo.

## Documentação da nova etapa

- [Divisão das tarefas e critérios de conclusão](docs/TAREFAS-SINTATICO.md)
- [Gramática, decisões e contrato entre módulos](docs/SINTATICO.md)
- [Documentação histórica da AP1](docs/LEXICO-AP1.md)
- [Exemplo de fatorial na sintaxe repaginada](examples/sintatico/fatorial.gyh)

## Compilar e executar o que já existe

Requisitos: `g++` com C++17 e GNU Make.

```bash
make
./lexer tests/teste_leitura.txt
make run ARQUIVO=tests/testeParte2Valido.txt
make test
make clean
```

`./lexer` apenas lista tokens; sua aprovação não significa que um programa é sintaticamente válido. O exemplo em `examples/sintatico/` é uma especificação para a próxima etapa e **ainda não é aceito pelo lexer atual**. Não existe alvo de testes sintáticos neste momento.

## Mudanças da linguagem

| Elemento | AP1 / implementação atual | Gramática repaginada |
|---|---|---|
| Seções | `:DEC`, `:PROG` | `[dec]`, `[prog]` |
| Declaração | `x:INT`, `y:REAL` | `X[integer]`, `Y[float]` |
| Variável | começa com minúscula | começa com maiúscula |
| Atribuição | `:=` | `<<` |
| Diferença | `!=` | `<>` |
| Booleanos | `E`, `OU` | `and`, `or` |
| Comandos | `LER`, `IMPRIMIR`, `SE`, `SENAO`, `ENTAO`, `ENQTO` | `read`, `print`, `if`, `else`, `then`, `while` |
| Bloco | `INI ... FIM` | `start ... end` |
| Repetição | exemplos antigos sem `ENTAO` | `while ExpressaoRelacional then Comando` |

Não misturar as versões nos testes. Os `;` usados para terminar produções no PDF não são tokens da linguagem.

## Fluxo de trabalho da dupla

Criar as branches a partir da `main` atual; a branch `develop` não foi encontrada no remoto durante esta revisão.

1. Integrar primeiro o contrato do parser e a adaptação léxica (tarefas S01/S02).
2. Elder e Gabriel implementam seus módulos em branches separadas conforme a tabela de tarefas.
3. Cada PR deve explicar o que mudou e incluir os resultados dos testes correspondentes. O colega revisa.
4. Antes da entrega, integrar na `main`, compilar e executar todos os testes sintáticos.

```bash
git switch main
git pull --ff-only
git switch -c feature/s01-lexer-repaginado
# implementar, testar e registrar as alterações
git add include/ src/ tests/ Makefile
git commit -m "feat: adapta lexer para gramatica repaginada"
git push -u origin feature/s01-lexer-repaginado
```

## Requisitos da entrega

- Parser escrito manualmente: o enunciado proíbe geradores como ANTLR e yacc.
- Proposta da dupla: descendente recursivo; ver as ressalvas de previsão de parênteses em `docs/SINTATICO.md`.
- Primeira linha do arquivo com `main`: comentário com os nomes da dupla. O cabeçalho atual começa com `/*`; ajustar para conter os nomes já na primeira linha.
- Mensagem de erro deve explicar o esperado e o encontrado e indicar a linha.
- Revisar os arquivos finais e garantir que ambos entendam a implementação para a apresentação.
