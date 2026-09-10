# Analisador Léxico — Linguagem GYH

Disciplina: **Compiladores** (CPCO6A EC-AP — UTFPR-AP)
Entrega parcial 1 (AP1): Analisador Léxico

**Dupla:** Elder Nunes & Gabriel Jesus

---

## 1. O que precisa ser feito (resumo do enunciado)

- Ler um arquivo-fonte na linguagem GYH e devolver a sequência de tokens no formato `<Tipo, "lexema">`.
- Quando encontrar algo inválido, parar e imprimir `Erro Léxico na linha N: Desconhecido "X"`.
- Desenvolver em C.
- No arquivo onde estiver a `main`, colocar nome completo + RA dos dois integrantes.
- Entregar todos os `.c`/`.h` do analisador.

### Categorias de token que o analisador precisa reconhecer

| Categoria | Lexemas | Token |
|---|---|---|
| Palavras-chave | `DEC PROG INT REAL LER IMPRIMIR SE SENAO ENTAO ENQTO INI FIM` | `PCDec, PCProg, PCInt, PCReal, PCLer, PCImprimir, PCSe, PCSenao, PCEntao, PCEnqto, PCIni, PCFim` |
| Booleanos | `E`, `OU` | `OpBoolE`, `OpBoolOu` |
| Aritméticos | `* / + -` | `OpAritMult, OpAritDiv, OpAritSoma, OpAritSub` |
| Relacionais | `< <= > >= == !=` | `OpRelMenor, OpRelMenorIgual, OpRelMaior, OpRelMaiorIgual, OpRelIgual, OpRelDif` |
| Delimitador | `:` | `Delim` |
| Atribuição | `:=` | `Atrib` |
| Parênteses | `( )` | `AbrePar, FechaPar` |
| Variável | letra minúscula + letras/dígitos | `Var` |
| Número inteiro | dígitos sem ponto | `NumInt` |
| Número real | dígitos com ponto | `NumReal` |
| Cadeia | texto entre `"aspas"` | `Cadeia` |
| Comentário | `# até o fim da linha` | (ignorado) |

Detalhe que pega muita gente: um identificador que começa com **maiúscula** só é válido se for
exatamente uma palavra reservada; se não for, é erro léxico (é o exemplo `Parametro` do enunciado,
que dá `Erro Léxico na linha 8: Desconhecido "Parametro"`).

## 2. Divisão do trabalho por partes

| Parte | O que envolve | Sugestão |
|---|---|---|
| **1. Infraestrutura do lexer** | Abrir arquivo, controlar posição atual, implementar `peek()`, `advance()` e `match()`, controlar número da linha, ignorar espaços, tabs, quebras de linha e comentários `#`, além de reconhecer EOF | Nunes |
| **2. Palavras e classificação textual** | Reconhecer `DEC`, `PROG`, `INT`, `REAL`, `LER`, `IMPRIMIR`, `SE`, `SENAO`, `ENTAO`, `ENQTO`, `INI`, `FIM`, operadores booleanos `E`/`OU` e variáveis `Var` | Nunes |
| **3. Operadores e símbolos** | Reconhecer operadores aritméticos `+`, `-`, `*`, `/`, relacionais `<`, `<=`, `>`, `>=`, `==`, `!=`, delimitador `:`, atribuição `:=` e parênteses `(`, `)` | Gabriel |
| **4. Números e cadeias** | Reconhecer `NumInt`, `NumReal` e `Cadeia` entre aspas | Gabriel |
| **5. Erro léxico** | Detectar caractere ou lexema inválido, identificar a linha do erro e emitir a mensagem no formato exigido pelo enunciado | os dois juntos |
| **6. Testes e integração** | Juntar todos os módulos em um único programa, testar com o exemplo do fatorial válido e com erros léxicos, comparar a saída com a especificação e revisar o cabeçalho do código | os dois juntos |

Ajustem entre vocês quem pega o quê — o importante é que cada parte vire uma branch e um PR
separado, pra dar pra revisar o código um do outro antes de juntar.

## 3. Como organizar no Git

### Branches

- **`main`** — versão estável, é o que vai ser entregue. Só recebe merge vindo de `develop`.
- **`develop`** — branch de integração, onde as partes vão se juntando.
- **`feature/<nome-da-parte>`** — uma branch por parte da tabela acima. Nasce de `develop`, é fundida de volta em `develop` via PR.

Fluxo pra cada parte:
```bash
git checkout develop
git pull
git checkout -b feature/nome-da-parte
# ...codam...
git push -u origin feature/nome-da-parte
# abrir PR feature/nome-da-parte -> develop, o outro revisa e aprova
```

Antes da entrega: PR `develop -> main`.

### Convenção de commit

- `feat: reconhecimento das palavras-chave`
- `fix: erro ao tratar operador de dois caracteres`
- `test: casos de teste para numeros reais`
- `docs: atualiza README`

## 4. Por onde começar (passo a passo)

1. Criar o repositório no GitHub (vazio, sem README/gitignore automáticos) e os dois clonarem.
2. Criar as branches `main` e `develop`.
3. Alguém pega a **Parte 1 (Infraestrutura)** numa branch `feature/infraestrutura` — sem isso as outras partes não têm o que consumir (função que lê caractere por caractere e controla linha).
4. Com a Parte 1 mergeada em `develop`, dividir Partes 2, 3 e 4 entre vocês dois em paralelo, cada um na sua branch.
5. Juntar tudo num `switch`/dispatcher central que decide, pro caractere atual, qual das partes chamar (é a função principal do analisador — só faz sentido escrever depois que 2, 3 e 4 existem).
6. Fazer a Parte 5 (erro léxico) por cima do que já existe.
7. Escrever pelo menos dois arquivos de teste: um válido (pode usar o exemplo do fatorial do enunciado) e um com erro proposital.
8. Revisar cabeçalho (nomes + RA), rodar os testes, PR final `develop -> main`.
9. Registrar a dupla na planilha oficial de homologação do AP1.

## 5. Checklist antes de entregar

- [ ] Todos os `.c`/`.h` estão no repositório
- [ ] Cabeçalho da `main` com nome completo e RA dos dois
- [ ] Compila sem warnings
- [ ] Testado com entrada válida e com entrada de erro
- [ ] Dupla registrada na planilha oficial
