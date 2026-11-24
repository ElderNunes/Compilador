#!/usr/bin/env bash
# Executor da suíte léxica e sintática.
# Uso: tests/executar_testes.sh [caminho-do-executavel]   (padrão: ./lexer)
#
# validos/    -> devem terminar com código 0 e sem mensagem em stderr.
# invalidos/  -> devem terminar com código 1, ter uma única linha de erro no
#                formato esperado, a linha correta e o trecho listado em
#                esperados.txt (formato: arquivo|linha|trecho).
# Erro léxico no modo sintático não pode gerar um segundo diagnóstico.

cd "$(dirname "$0")/.." || exit 2
BIN=${1:-./lexer}
[ -x "$BIN" ] || { echo "Executável não encontrado: $BIN (rode make)"; exit 2; }

falhas=0
total=0

falhar() { printf '[FALHOU] %s\n' "$1"; falhas=$((falhas + 1)); }

# executar_valido <arquivo> [flags]
executar_valido() {
    local arq=$1; shift
    local saida erro codigo
    saida=$("$BIN" "$arq" "$@" 2>/tmp/erro_$$); codigo=$?
    erro=$(cat /tmp/erro_$$)
    total=$((total + 1))
    if [ $codigo -ne 0 ]; then falhar "$arq deveria ser aceito (código $codigo): $erro"
    elif [ -n "$erro" ]; then falhar "$arq aceito, mas escreveu em stderr: $erro"
    elif [ "$1" = "--sintatico" ] && [[ "$saida" != *"sintaticamente válido"* ]]; then
        falhar "$arq sem mensagem de sucesso"
    else printf '[OK] %s aceito\n' "$arq"; fi
}

# executar_invalido <arquivo> <tipo-do-erro> <linha> <trecho> [flags]
executar_invalido() {
    local arq=$1 tipo=$2 linha=$3 trecho=$4; shift 4
    local saida erro codigo
    saida=$("$BIN" "$arq" "$@" 2>/tmp/erro_$$); codigo=$?
    erro=$(cat /tmp/erro_$$)
    total=$((total + 1))
    if [ $codigo -ne 1 ]; then falhar "$arq deveria retornar 1 (retornou $codigo)"
    elif [ "$(printf '%s\n' "$erro" | wc -l)" -ne 1 ]; then falhar "$arq: esperado exatamente 1 linha de erro, veio: $erro"
    elif [[ "$erro" != "$tipo na linha $linha:"* ]]; then falhar "$arq: esperado '$tipo na linha $linha:', veio: $erro"
    elif [[ "$erro" != *"$trecho"* ]]; then falhar "$arq: esperado trecho '$trecho', veio: $erro"
    elif [[ "$saida" == *"sintaticamente válido"* ]]; then falhar "$arq: imprimiu sucesso após erro"
    else printf '[OK] %s rejeitado (%s, linha %s)\n' "$arq" "$tipo" "$linha"; fi
}

# executar_grupo <diretorio> <tipo-do-erro> [flags]
executar_grupo() {
    local dir=$1 tipo=$2; shift 2
    local arq nome linha trecho
    for arq in "$dir"/validos/*.gyh; do executar_valido "$arq" "$@"; done
    for arq in "$dir"/invalidos/*.gyh; do
        nome=$(basename "$arq")
        IFS='|' read -r _ linha trecho < <(grep -F "$nome|" "$dir/esperados.txt" | head -n1)
        if [ -z "$linha" ]; then total=$((total + 1)); falhar "$arq sem entrada em $dir/esperados.txt"; continue; fi
        local t=$tipo
        # no modo sintático, entradas marcadas com 'L:' esperam erro léxico
        if [[ "$trecho" == L:* ]]; then t="Erro Léxico"; trecho=${trecho#L:}; fi
        executar_invalido "$arq" "$t" "$linha" "$trecho" "$@"
    done
}

executar_grupo tests/lexico "Erro Léxico"
executar_grupo tests/sintatico "Erro sintático" --sintatico
rm -f /tmp/erro_$$

printf '\n%d testes, %d falha(s)\n' "$total" "$falhas"
[ $falhas -eq 0 ]
