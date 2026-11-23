#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define NUM_VERTICES 4

// Nomes dos módulos/bibliotecas correspondentes aos índices (0 a 3)
const char* NOMES_VERTICES[NUM_VERTICES] = {
    "React",                // Índice 0 (Vértice 1)
    "Axios",                // Índice 1 (Vértice 2)
    "Usuarios",             // Índice 2 (Vértice 3)
    "Aplicacao Principal"   // Índice 3 (Vértice 4)
};

// Estrutura de um nó da Lista de Adjacência
typedef struct No {
    int destino;
    struct No* prox;
} No;

// Estrutura do Grafo
typedef struct {
    int numVertices;
    No* listaAdj[NUM_VERTICES];
} Grafo;

// Estrutura de Fila para a Busca em Largura (BFS)
typedef struct {
    int itens[NUM_VERTICES];
    int frente;
    int tras;
} Fila;

// --- FUNÇÕES DE TRATAMENTO DE ENTRADA ---

void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Descarta caracteres restantes na linha até a quebra de linha ou EOF
    }
}

bool lerInteiroSeguro(int* saida) {
    if (saida == NULL) {
        return false;
    }

    if (scanf("%d", saida) != 1) {
        limparBufferEntrada();
        return false;
    }

    limparBufferEntrada();
    return true;
}

// --- FUNÇÕES DE MANIPULAÇÃO DO GRAFO ---

Grafo* criarGrafo(void) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    if (grafo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para o grafo.\n");
        return NULL;
    }

    grafo->numVertices = NUM_VERTICES;
    for (int i = 0; i < NUM_VERTICES; i++) {
        grafo->listaAdj[i] = NULL;
    }
    return grafo;
}

bool adicionarArestaDirecionada(Grafo* grafo, int origem, int destino) {
    if (grafo == NULL || origem < 0 || origem >= NUM_VERTICES || destino < 0 || destino >= NUM_VERTICES) {
        fprintf(stderr, "Erro: parametros invalidos ao adicionar aresta.\n");
        return false;
    }

    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para novo no da aresta.\n");
        return false;
    }

    novoNo->destino = destino;
    novoNo->prox = grafo->listaAdj[origem];
    grafo->listaAdj[origem] = novoNo;
    return true;
}

void exibirGrafo(const Grafo* grafo) {
    if (grafo == NULL) {
        printf("\nErro: grafo nao inicializado.\n");
        return;
    }

    printf("\n--- ESTRUTURA DO GRAFO DE DEPENDENCIAS ---\n");
    for (int i = 0; i < grafo->numVertices; i++) {
        printf("[%d] %s -> ", i + 1, NOMES_VERTICES[i]);
        No* temp = grafo->listaAdj[i];
        if (temp == NULL) {
            printf("(Nenhuma dependencia)");
        }
        while (temp != NULL) {
            printf("%s", NOMES_VERTICES[temp->destino]);
            if (temp->prox != NULL) printf(", ");
            temp = temp->prox;
        }
        printf("\n");
    }
    printf("------------------------------------------\n");
}

// --- FUNÇÕES DE FILA (PARA BFS) ---

Fila* criarFila(void) {
    Fila* f = (Fila*)malloc(sizeof(Fila));
    if (f == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memoria para a fila da BFS.\n");
        return NULL;
    }

    f->frente = -1;
    f->tras = -1;
    return f;
}

bool filaVazia(const Fila* f) {
    return (f == NULL || f->frente == -1);
}

bool enfileirar(Fila* f, int valor) {
    if (f == NULL) {
        return false;
    }

    if (f->tras == NUM_VERTICES - 1) {
        fprintf(stderr, "Aviso: fila atingiu a capacidade maxima.\n");
        return false;
    }

    if (f->frente == -1) {
        f->frente = 0;
    }
    f->tras++;
    f->itens[f->tras] = valor;
    return true;
}

int desenfileirar(Fila* f) {
    if (filaVazia(f)) {
        return -1;
    }

    int item = f->itens[f->frente];
    f->frente++;
    if (f->frente > f->tras) {
        f->frente = f->tras = -1;
    }
    return item;
}

// --- RELATÓRIO GERAL DA BUSCA ---

void exibirRelatorioBusca(const char* algoritmo, int origem, int objetivo, bool usarObjetivo,
                          const int* ordemVisita, int qtdVisitados, int arestasPercorridas, bool encontrado) {
    printf("\n==================================================\n");
    printf(" RELATORIO DE EXECUCAO - %s\n", algoritmo);
    printf("==================================================\n");
    printf("-> Vertice de origem: %s\n", NOMES_VERTICES[origem]);

    if (usarObjetivo) {
        printf("-> Vertice objetivo: %s\n", NOMES_VERTICES[objetivo]);
        printf("-> Status do objetivo: %s\n", encontrado ? "ENCONTRADO" : "NAO ENCONTRADO");
    } else {
        printf("-> Modo de busca: Exploracao completa (sem objetivo especifico)\n");
    }

    printf("-> Quantidade de vertices visitados: %d\n", qtdVisitados);
    printf("-> Quantidade de arestas percorridas: %d\n", arestasPercorridas);

    printf("-> Vertices visitados: ");
    for (int i = 0; i < qtdVisitados; i++) {
        printf("%s", NOMES_VERTICES[ordemVisita[i]]);
        if (i < qtdVisitados - 1) printf(", ");
    }
    printf("\n");

    printf("-> Caminho percorrido: ");
    for (int i = 0; i < qtdVisitados; i++) {
        printf("[%s]", NOMES_VERTICES[ordemVisita[i]]);
        if (i < qtdVisitados - 1) printf(" -> ");
    }
    printf("\n==================================================\n");
}

// --- BUSCA EM PROFUNDIDADE - DFS (REQUISITO 2) ---

void dfsRecursivo(const Grafo* grafo, int atual, int objetivo, bool usarObjetivo,
                  bool* visitado, int* ordemVisita, int* qtdVisitados,
                  int* arestasPercorridas, bool* encontrado) {
    if (grafo == NULL || visitado == NULL || ordemVisita == NULL ||
        qtdVisitados == NULL || arestasPercorridas == NULL || encontrado == NULL) {
        return;
    }

    visitado[atual] = true;
    ordemVisita[*qtdVisitados] = atual;
    (*qtdVisitados)++;

    if (usarObjetivo && atual == objetivo) {
        *encontrado = true;
        return;
    }

    No* adj = grafo->listaAdj[atual];
    while (adj != NULL) {
        int vizinho = adj->destino;
        (*arestasPercorridas)++;

        if (!visitado[vizinho]) {
            dfsRecursivo(grafo, vizinho, objetivo, usarObjetivo, visitado,
                         ordemVisita, qtdVisitados, arestasPercorridas, encontrado);

            // Interrompe o backtracking caso o objetivo tenha sido encontrado
            if (usarObjetivo && *encontrado) {
                return;
            }
        }
        adj = adj->prox;
    }
}

void executarDFS(const Grafo* grafo, int origem, int objetivo, bool usarObjetivo) {
    if (grafo == NULL) {
        fprintf(stderr, "Erro: grafo invalido para execucao da DFS.\n");
        return;
    }

    bool visitado[NUM_VERTICES] = {false};
    int ordemVisita[NUM_VERTICES] = {0};
    int qtdVisitados = 0;
    int arestasPercorridas = 0;
    bool encontrado = false;

    dfsRecursivo(grafo, origem, objetivo, usarObjetivo, visitado,
                 ordemVisita, &qtdVisitados, &arestasPercorridas, &encontrado);

    exibirRelatorioBusca("BUSCA EM PROFUNDIDADE (DFS)", origem, objetivo, usarObjetivo,
                         ordemVisita, qtdVisitados, arestasPercorridas, encontrado);
}

// --- BUSCA EM LARGURA - BFS (REQUISITO 3) ---

void executarBFS(const Grafo* grafo, int origem, int objetivo, bool usarObjetivo) {
    if (grafo == NULL) {
        fprintf(stderr, "Erro: grafo invalido para execucao da BFS.\n");
        return;
    }

    bool visitado[NUM_VERTICES] = {false};
    int ordemVisita[NUM_VERTICES] = {0};
    int qtdVisitados = 0;
    int arestasPercorridas = 0;
    bool encontrado = false;

    Fila* fila = criarFila();
    if (fila == NULL) {
        return;
    }

    visitado[origem] = true;
    if (!enfileirar(fila, origem)) {
        free(fila);
        return;
    }

    while (!filaVazia(fila)) {
        int atual = desenfileirar(fila);
        if (atual < 0 || atual >= NUM_VERTICES) {
            break;
        }

        ordemVisita[qtdVisitados] = atual;
        qtdVisitados++;

        if (usarObjetivo && atual == objetivo) {
            encontrado = true;
            break;
        }

        No* adj = grafo->listaAdj[atual];
        while (adj != NULL) {
            int vizinho = adj->destino;
            arestasPercorridas++;

            if (!visitado[vizinho]) {
                visitado[vizinho] = true;
                if (!enfileirar(fila, vizinho)) {
                    free(fila);
                    return;
                }
            }
            adj = adj->prox;
        }
    }

    free(fila);
    exibirRelatorioBusca("BUSCA EM LARGURA (BFS)", origem, objetivo, usarObjetivo,
                         ordemVisita, qtdVisitados, arestasPercorridas, encontrado);
}

// --- FUNÇÕES AUXILIARES DE MENU ---

int selecionarVertice(const char* mensagem) {
    int escolha = 0;

    while (true) {
        printf("\n%s\n", mensagem);
        for (int i = 0; i < NUM_VERTICES; i++) {
            printf("  %d - %s\n", i + 1, NOMES_VERTICES[i]);
        }
        printf("Opcao (1 a %d): ", NUM_VERTICES);

        if (!lerInteiroSeguro(&escolha)) {
            printf("Entrada invalida! Digite um numero inteiro entre 1 e %d.\n", NUM_VERTICES);
            continue;
        }

        if (escolha < 1 || escolha > NUM_VERTICES) {
            printf("Opcao fora do intervalo! Escolha um valor entre 1 e %d.\n", NUM_VERTICES);
            continue;
        }

        return escolha - 1;
    }
}

bool perguntarUsarObjetivo(void) {
    int resposta = 0;

    while (true) {
        printf("\nDeseja definir um vertice OBJETIVO (destino)? (1 - Sim / 0 - Nao): ");
        if (!lerInteiroSeguro(&resposta)) {
            printf("Entrada invalida! Digite 1 para Sim ou 0 para Nao.\n");
            continue;
        }

        if (resposta == 1) {
            return true;
        }
        if (resposta == 0) {
            return false;
        }

        printf("Opcao invalida! Digite apenas 1 (Sim) ou 0 (Nao).\n");
    }
}

void liberarGrafo(Grafo* grafo) {
    if (grafo == NULL) {
        return;
    }

    for (int i = 0; i < grafo->numVertices; i++) {
        No* atual = grafo->listaAdj[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafo);
}

// --- PROGRAMA PRINCIPAL ---

int main(void) {
    Grafo* grafo = criarGrafo();
    if (grafo == NULL) {
        return 1;
    }

    // Mapeamento:
    // 0: React | 1: Axios | 2: Usuarios | 3: Aplicacao Principal
    if (!adicionarArestaDirecionada(grafo, 2, 1) || // Usuarios -> Axios
        !adicionarArestaDirecionada(grafo, 2, 0) || // Usuarios -> React
        !adicionarArestaDirecionada(grafo, 3, 2)) { // Aplicacao Principal -> Usuarios
        fprintf(stderr, "Erro fatal ao inicializar as arestas do grafo.\n");
        liberarGrafo(grafo);
        return 1;
    }

    int opcao = 0;
    do {
        printf("\n================ MENU INTERATIVO ================\n");
        printf("1. Exibir estrutura do grafo (Dependencias)\n");
        printf("2. Executar Busca em Profundidade (DFS)\n");
        printf("3. Executar Busca em Largura (BFS)\n");
        printf("0. Sair\n");
        printf("=================================================\n");
        printf("Escolha uma opcao: ");

        if (!lerInteiroSeguro(&opcao)) {
            printf("Entrada invalida! Por favor, digite um numero do menu.\n");
            opcao = -1; // Garante que o loop continue
            continue;
        }

        switch (opcao) {
            case 1:
                exibirGrafo(grafo);
                break;

            case 2:
            case 3: {
                int origem = selecionarVertice("Selecione o vertice de ORIGEM:");
                bool usarObjetivo = perguntarUsarObjetivo();
                int objetivo = -1;

                if (usarObjetivo) {
                    objetivo = selecionarVertice("Selecione o vertice OBJETIVO:");
                }

                if (opcao == 2) {
                    executarDFS(grafo, origem, objetivo, usarObjetivo);
                } else {
                    executarBFS(grafo, origem, objetivo, usarObjetivo);
                }
                break;
            }

            case 0:
                printf("Encerrando o programa...\n");
                break;

            default:
                printf("Opcao invalida! Escolha entre 0 e 3.\n");
                break;
        }
    } while (opcao != 0);

    liberarGrafo(grafo);
    return 0;
}