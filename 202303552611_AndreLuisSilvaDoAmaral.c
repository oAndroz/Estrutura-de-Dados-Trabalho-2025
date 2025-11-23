/*
   Trabalho de Estrutura de Dados 
   Grupo (individual):
   - André Luis Silva do Amaral – Matrícula: 202303552611

   O programa implementa cadastro, ordenação e busca de componentes
   utilizados na montagem da torre de fuga. Inclui contagem de comparações,
   medição de tempo e operações de salvar/carregar em binário.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 20

typedef struct {
    char nome[30];
    char tipo[20];
    int prioridade;
} Componente;

/* HUD do sistema com estilo ASCII */
void mostrarHUD() {
    printf("\n");
    printf("┌────────────────────────────────────────────────┐\n");
    printf("│                PAINEL DE COMANDO               │\n");
    printf("│      Sistema de Prioridade – Torre de Fuga     │\n");
    printf("├────────────────────────────────────────────────┤\n");
    printf("│ 1 ▸ Inserir componentes                        │\n");
    printf("│ 2 ▸ Exibir componentes                         │\n");
    printf("│ 3 ▸ Ordenar componentes                        │\n");
    printf("│ 4 ▸ Buscar componente (binária)                │\n");
    printf("│ 5 ▸ Salvar progresso (binário)                 │\n");
    printf("│ 6 ▸ Carregar progresso (binário)               │\n");
    printf("│ 7 ▸ Encerrar missão                            │\n");
    printf("└────────────────────────────────────────────────┘\n");
}

/* Ordena por nome usando Bubble Sort (case-insensitive) */
int bubbleSortNome(Componente v[], int n) {
    int comp = 0;
    int trocou;

    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - i - 1; j++) {
            comp++;
            if (strcasecmp(v[j].nome, v[j+1].nome) > 0) {
                Componente tmp = v[j];
                v[j] = v[j+1];
                v[j+1] = tmp;
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
    return comp;
}

/* Ordena por tipo usando Insertion Sort */
int insertionSortTipo(Componente v[], int n) {
    int comp = 0;

    for (int i = 1; i < n; i++) {
        Componente atual = v[i];
        int j = i - 1;

        while (j >= 0 && strcasecmp(v[j].tipo, atual.tipo) > 0) {
            comp++;
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = atual;
    }

    return comp;
}

/* Ordena por prioridade usando Selection Sort */
int selectionSortPrioridade(Componente v[], int n) {
    int comp = 0;

    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            comp++;
            if (v[j].prioridade < v[min].prioridade) {
                min = j;
            }
        }
        if (min != i) {
            Componente tmp = v[i];
            v[i] = v[min];
            v[min] = tmp;
        }
    }

    return comp;
}

/* Busca binária pelo nome (funciona apos ordenar por nome) */
int buscaBinariaPorNome(Componente v[], int n, char chave[]) {
    int inicio = 0, fim = n - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int r = strcasecmp(chave, v[meio].nome);

        if (r == 0) return meio;
        if (r > 0) inicio = meio + 1;
        else fim = meio - 1;
    }

    return -1;
}

/* Exibe lista de componentes */
void mostrarComponentes(Componente v[], int n) {
    printf("\nLista de componentes:\n");

    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Tipo: %s | Prioridade: %d\n",
               i+1, v[i].nome, v[i].tipo, v[i].prioridade);
    }
}

/* Salva vetor em arquivo binário */
void salvarBinario(Componente v[], int n) {
    FILE *f = fopen("progresso.bin", "wb");
    if (!f) {
        printf("Erro ao salvar arquivo.\n");
        return;
    }

    fwrite(&n, sizeof(int), 1, f);
    fwrite(v, sizeof(Componente), n, f);

    fclose(f);
    printf("Progresso salvo com sucesso.\n");
}

/* Carrega vetor do arquivo binário */
int carregarBinario(Componente v[]) {
    FILE *f = fopen("progresso.bin", "rb");
    if (!f) {
        printf("Nenhum arquivo encontrado.\n");
        return 0;
    }

    int n;
    fread(&n, sizeof(int), 1, f);
    fread(v, sizeof(Componente), n, f);
    fclose(f);

    printf("Progresso carregado.\n");
    return n;
}

int main() {
    Componente lista[MAX];
    int qtd = 0;
    int opcao;

    do {
        mostrarHUD();

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        /* Inserção de novo componente */
        if (opcao == 1) {
            if (qtd >= MAX) {
                printf("Limite atingido.\n");
                continue;
            }

            printf("Nome do componente: ");
            fgets(lista[qtd].nome, 30, stdin);
            lista[qtd].nome[strcspn(lista[qtd].nome, "\n")] = 0;

            printf("Tipo do componente: ");
            fgets(lista[qtd].tipo, 20, stdin);
            lista[qtd].tipo[strcspn(lista[qtd].tipo, "\n")] = 0;

            printf("Prioridade (1-10): ");
            scanf("%d", &lista[qtd].prioridade);
            getchar();

            qtd++;
        }

        /* Exibe vetor */
        else if (opcao == 2) {
            mostrarComponentes(lista, qtd);
        }

        /* Menu de ordenação */
        else if (opcao == 3) {
            int escolha;

            printf("\n1 - Ordenar por nome (Bubble)\n");
            printf("2 - Ordenar por tipo (Insertion)\n");
            printf("3 - Ordenar por prioridade (Selection)\n");
            printf("Escolha: ");
            scanf("%d", &escolha);
            getchar();

            clock_t t1 = clock();
            int comp = 0;

            if (escolha == 1) comp = bubbleSortNome(lista, qtd);
            else if (escolha == 2) comp = insertionSortTipo(lista, qtd);
            else if (escolha == 3) comp = selectionSortPrioridade(lista, qtd);

            clock_t t2 = clock();
            double tempo = (double)(t2 - t1) / CLOCKS_PER_SEC;

            printf("Comparacoes: %d | Tempo: %.6f s\n", comp, tempo);
        }

        /* Busca binária */
        else if (opcao == 4) {
            char chave[30];
            printf("Nome a buscar: ");
            fgets(chave, 30, stdin);
            chave[strcspn(chave, "\n")] = 0;

            int pos = buscaBinariaPorNome(lista, qtd, chave);

            if (pos == -1) printf("Componente nao encontrado.\n");
            else printf("Encontrado na posicao %d.\n", pos + 1);
        }

        /* Salvar arquivo */
        else if (opcao == 5) {
            salvarBinario(lista, qtd);
        }

        /* Carregar arquivo */
        else if (opcao == 6) {
            qtd = carregarBinario(lista);
        }

    } while (opcao != 7);

    printf("Missao encerrada.\n");
    return 0;
}
