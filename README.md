# Grafo de Dependências em C — Atividade Avaliativa 3

Implementação em linguagem **C** de um grafo direcionado de dependências entre módulos e bibliotecas de um sistema de software, desenvolvido para a **Atividade Avaliativa 3** (tema: *Implementação de grafos em C*, metodologia: *Aprendizagem por projeto*)[cite: 1].

---

## Estrutura do Grafo

O grafo representa dependências direcionadas (onde `A -> B` significa que `A` depende de `B`), composto por **4 vértices** e **3 arestas**:

### Vértices
1. `React`
2. `Axios`
3. `Usuários`
4. `Aplicação Principal`

### Arestas (Dependências)
* `Usuários` → `React`
* `Usuários` → `Axios`
* `Aplicação Principal` → `Usuários`

---

## Requisitos Atendidos

1. **Menu interativo:** permite exibir a estrutura do grafo, executar buscas e encerrar o programa com segurança[cite: 1].
2. **Busca em profundidade (DFS):** implementada de forma recursiva (`dfsRecursivo`) com *backtracking* no retorno das chamadas e interrupção antecipada quando um objetivo é encontrado (quando aplicável)[cite: 1].
3. **Busca em largura (BFS):** implementada com uma estrutura de fila (`Fila`, `enfileirar` e `desenfileirar`) para exploração por níveis.
4. **Caminho percorrido:** exibe de forma organizada a sequência de vértices visitados durante cada busca.
5. **Dados gerais da busca:** apresenta um relatório ao final de cada execução contendo[cite: 1]:
   * Vértice de origem.
   * Vértice objetivo e indicação se foi encontrado ou não (quando um objetivo é definido).
   * Vértices visitados[cite: 1]
   * Quantidade de vértices visitados.
   * Quantidade de arestas percorridas.
   * Caminho percorrido.
6. **Validação de entradas e memória:** leitura segura de inteiros com limpeza do buffer de entrada (`stdin`), validação de intervalos de opções e verificação de alocação dinâmica (`malloc`).

---

## Como Compilar e Executar

### Pré-requisitos
* Compilador `gcc` instalado.

### Compilação
```bash
gcc main.c -o grafo_dependencias -Wall -Wextra
```

### Execução
```bash
./grafo_dependencias
```

---

## Organização do Código (`main.c`)

* **Estruturas de dados:** `No`, `Grafo` (lista de adjacência) e `Fila` (para a BFS).
* **Validação e memória:** `limparBufferEntrada`, `lerInteiroSeguro`, verificação de ponteiros `NULL` e `liberarGrafo`.
* **Algoritmos de busca:** `executarDFS` / `dfsRecursivo` e `executarBFS`.
* **Relatório e interface:** `exibirGrafo`, `exibirRelatorioBusca`, `selecionarVertice`, `perguntarUsarObjetivo` e `main`.

---

### Participantes

* **Conrado Henrique Lima da Mata**
* **Giovane Soldera Ribeiro**
* **João Felipe Aleixo de Melo**
* **Luis Gustavo Roque Desiderio da Silva**
