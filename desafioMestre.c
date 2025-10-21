#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Necessário para clock()

// --- DEFINIÇÕES GLOBAIS ---
#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20

// Define o componente-chave que a Busca Binária deve encontrar
#define CHAVE_BUSCA "chip central"

// Variáveis Globais para Contagem e Tempo
long long comparacoesOrdenacao = 0;
long long comparacoesBusca = 0;
clock_t tempoExecucao;

// --- 1. CRIAÇÃO DA STRUCT ---

/**
 * @brief Struct que representa um Componente da torre de fuga.
 */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1 (mais baixa) a 10 (mais alta)
} Componente;

// Variáveis Globais para a Estrutura e Controle
Componente torreComponentes[MAX_COMPONENTES];
int totalComponentes = 0;
int ordenadoPorNome = 0; // Flag para indicar se o array está ordenado por nome

// --- PROTÓTIPOS E AUXILIARES ---
void limparBuffer();
int lerStringSegura(char *destino, int tamanho);
void trocarComponente(Componente *a, Componente *b);
void cadastrarComponentes();
void mostrarComponentes(Componente array[], int tamanho);

// --- FUNÇÕES AUXILIARES ---

/**
 * @brief Limpa o buffer de entrada (stdin).
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Obtém uma string do usuário e limpa o \n.
 * @return 1 se a leitura foi bem-sucedida, 0 caso contrário.
 */
int lerStringSegura(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) return 0;
    destino[strcspn(destino, "\n")] = '\0';
    return 1;
}

/**
 * @brief Troca dois elementos do tipo Componente.
 */
void trocarComponente(Componente *a, Componente *b) {
    Componente temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * @brief 2. Entrada dos dados: Permite o cadastro dos componentes.
 */
void cadastrarComponentes() {
    printf("\n--- Cadastro de Componentes (Max %d) ---\n", MAX_COMPONENTES);
    totalComponentes = 0;

    int num;
    printf("Quantos componentes deseja cadastrar (1-%d)? ", MAX_COMPONENTES);
    if (scanf("%d", &num) != 1 || num <= 0 || num > MAX_COMPONENTES) {
        printf("[ERRO] Numero de componentes invalido. Cadastramento cancelado.\n");
        limparBuffer();
        return;
    }
    limparBuffer();

    for (int i = 0; i < num; i++) {
        Componente *novo = &torreComponentes[i];
        
        printf("\nComponente #%d:\n", i + 1);
        
        printf("  Nome (max %d): ", MAX_NOME - 1);
        if (!lerStringSegura(novo->nome, MAX_NOME)) continue;

        printf("  Tipo (ex: controle, suporte): ");
        if (!lerStringSegura(novo->tipo, MAX_TIPO)) continue;

        printf("  Prioridade (1 a 10): ");
        if (scanf("%d", &novo->prioridade) != 1 || novo->prioridade < 1 || novo->prioridade > 10) {
            printf("[ERRO] Prioridade invalida. Componente nao cadastrado.\n");
            limparBuffer();
            i--; // Repete a iteração atual
            continue;
        }
        limparBuffer();
        
        totalComponentes++;
    }

    ordenadoPorNome = 0; // Reinicia o flag
    printf("\n[SUCESSO] %d componentes cadastrados.\n", totalComponentes);
}


/**
 * @brief 6. Exibir todos os componentes de forma formatada.
 */
void mostrarComponentes(Componente array[], int tamanho) {
    printf("\n=================================================================\n");
    printf("  COMPONENTES DA TORRE (Total: %d) \n", tamanho);
    printf("=================================================================\n");

    if (tamanho == 0) {
        printf("\n[AVISO] Nao ha componentes cadastrados.\n");
        return;
    }

    printf("  # | %-*s | %-*s | Prioridade\n", MAX_NOME - 4, "Nome", MAX_TIPO - 4, "Tipo");
    printf("-----------------------------------------------------------------\n");

    for (int i = 0; i < tamanho; i++) {
        printf(" %2d | %-*s | %-*s | %10d\n",
               i + 1,
               MAX_NOME - 4, array[i].nome,
               MAX_TIPO - 4, array[i].tipo,
               array[i].prioridade);
    }
    printf("-----------------------------------------------------------------\n");
}


// =======================================================
// === 3. ALGORITMOS DE ORDENAÇÃO E 5. MEDIÇÃO
// =======================================================

/**
 * @brief 3.1 Bubble Sort: Ordena por Nome (String).
 */
void bubbleSortNome(Componente array[], int n) {
    comparacoesOrdenacao = 0;
    ordenadoPorNome = 1; // Marca como ordenado por nome

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            comparacoesOrdenacao++; // Contabiliza a comparação de strings
            if (strcmp(array[j].nome, array[j+1].nome) > 0) {
                trocarComponente(&array[j], &array[j+1]);
            }
        }
    }
}

/**
 * @brief 3.2 Insertion Sort: Ordena por Tipo (String).
 */
void insertionSortTipo(Componente array[], int n) {
    comparacoesOrdenacao = 0;
    ordenadoPorNome = 0; // Desmarca o flag

    for (int i = 1; i < n; i++) {
        Componente chave = array[i];
        int j = i - 1;

        // Compara o tipo da chave com os elementos anteriores
        while (j >= 0) {
            comparacoesOrdenacao++; // Contabiliza a comparação de strings
            if (strcmp(array[j].tipo, chave.tipo) > 0) {
                array[j + 1] = array[j];
                j = j - 1;
            } else {
                break;
            }
        }
        array[j + 1] = chave;
    }
}

/**
 * @brief 3.3 Selection Sort: Ordena por Prioridade (int).
 */
void selectionSortPrioridade(Componente array[], int n) {
    comparacoesOrdenacao = 0;
    ordenadoPorNome = 0; // Desmarca o flag

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            comparacoesOrdenacao++; // Contabiliza a comparação de inteiros
            if (array[j].prioridade > array[min_idx].prioridade) {
                // A prioridade mais alta deve vir primeiro (ordem decrescente)
                min_idx = j;
            }
        }
        if (min_idx != i) {
            trocarComponente(&array[min_idx], &array[i]);
        }
    }
}


// =======================================================
// === 4. BUSCA BINÁRIA OTIMIZADA
// =======================================================

/**
 * @brief 4. Busca Binária: Localiza o componente-chave por nome.
 * @return índice do componente se encontrado, -1 caso contrário.
 */
int buscaBinariaPorNome(Componente array[], int n, const char* nomeBusca) {
    comparacoesBusca = 0;
    int baixo = 0;
    int alto = n - 1;

    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        
        comparacoesBusca++; // Contabiliza a comparação de strings
        int resultado = strcmp(array[meio].nome, nomeBusca);

        if (resultado == 0) {
            return meio; // Componente encontrado
        } else if (resultado < 0) {
            baixo = meio + 1; // Busca na metade superior
        } else {
            alto = meio - 1; // Busca na metade inferior
        }
    }

    return -1; // Componente não encontrado
}


// =======================================================
// === FUNÇÕES DE MENU E FLUXO PRINCIPAL
// =======================================================

void exibirMenuPrincipal() {
    printf("\n=======================================================\n");
    printf("   SISTEMA DE MONTAGEM DA TORRE DE FUGA (COMP %d/%d)   \n", totalComponentes, MAX_COMPONENTES);
    printf("=======================================================\n");
    printf("1. Cadastrar/Recadastrar Componentes\n");
    printf("2. Escolher Estrategia de Ordenacao (Medir Desempenho)\n");
    printf("3. Iniciar Busca Binaria (Ativar Torre)\n");
    printf("4. Listar Componentes Atuais\n");
    printf("0. Sair\n");
    printf("-------------------------------------------------------\n");
    printf("Escolha uma opcao: ");
}

void exibirMenuOrdenacao() {
    printf("\n--- Escolha a Estrategia de Ordenacao ---\n");
    printf("1. Bubble Sort (Ordenar por NOME - Necessario para Busca Binaria)\n");
    printf("2. Insertion Sort (Ordenar por TIPO)\n");
    printf("3. Selection Sort (Ordenar por PRIORIDADE - Decrescente)\n");
    printf("4. Voltar ao Menu Principal\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Função principal para gerenciar as operações de ordenação.
 */
void gerenciarOrdenacao() {
    int opcao;
    
    if (totalComponentes == 0) {
        printf("\n[AVISO] Cadastre os componentes antes de ordenar (Opcao 1).\n");
        return;
    }
    
    do {
        exibirMenuOrdenacao();
        if (scanf("%d", &opcao) != 1) { limparBuffer(); continue; }
        limparBuffer();

        if (opcao < 1 || opcao > 3) {
            if (opcao == 4) break;
            printf("\n[AVISO] Opcao invalida.\n");
            continue;
        }

        // Inicia a medição de tempo
        tempoExecucao = clock(); 

        switch (opcao) {
            case 1: 
                bubbleSortNome(torreComponentes, totalComponentes);
                break;
            case 2: 
                insertionSortTipo(torreComponentes, totalComponentes);
                break;
            case 3: 
                selectionSortPrioridade(torreComponentes, totalComponentes);
                break;
        }

        // Finaliza a medição de tempo
        tempoExecucao = clock() - tempoExecucao;
        double tempoEmSegundos = ((double)tempoExecucao) / CLOCKS_PER_SEC;

        // 5. Exibe os resultados
        printf("\n[DESEMPENHO] Ordenacao Concluida.\n");
        printf("Comparações: %lld\n", comparacoesOrdenacao);
        printf("Tempo de Execucao: %.6f segundos (Requisito: < 2s)\n", tempoEmSegundos);

        // 6. Montagem final: Exibe o resultado e confirma a estratégia
        mostrarComponentes(torreComponentes, totalComponentes);
        
        printf("\nPressione ENTER para continuar...");
        getchar(); 

    } while (opcao != 4);
}

/**
 * @brief Gerencia a busca binária.
 */
void gerenciarBuscaBinaria() {
    if (totalComponentes == 0) {
        printf("\n[AVISO] Cadastre os componentes antes de buscar (Opcao 1).\n");
        return;
    }

    if (!ordenadoPorNome) {
        printf("\n[ERRO] A Busca Binaria requer que os componentes estejam ordenados por NOME.\n");
        printf("[INSTRUCAO] Escolha a opcao 2, e depois 1 (Bubble Sort) para ordenar.\n");
        return;
    }

    printf("\n--- Iniciar Ativacao da Torre ---\n");
    printf("Buscando componente-chave: '%s'\n", CHAVE_BUSCA);

    // Inicia a medição de tempo
    tempoExecucao = clock();
    
    int indice = buscaBinariaPorNome(torreComponentes, totalComponentes, CHAVE_BUSCA);
    
    // Finaliza a medição de tempo
    tempoExecucao = clock() - tempoExecucao;
    double tempoEmSegundos = ((double)tempoExecucao) / CLOCKS_PER_SEC;

    if (indice != -1) {
        printf("\n[SUCESSO] Componente-Chave '%s' encontrado no indice %d!\n", CHAVE_BUSCA, indice + 1);
        printf("[RESULTADO] Torre de Fuga ATIVADA com sucesso!\n");
        // 6. Confirmação visual
        printf("Detalhes do Componente:\n");
        printf("Nome: %s | Tipo: %s | Prioridade: %d\n", 
               torreComponentes[indice].nome, torreComponentes[indice].tipo, torreComponentes[indice].prioridade);
    } else {
        printf("\n[FALHA] Componente-Chave '%s' nao encontrado.\n", CHAVE_BUSCA);
        printf("[RESULTADO] A torre nao pode ser ativada.\n");
    }

    // 5. Exibe o desempenho da busca
    printf("\n[DESEMPENHO BUSCA BINARIA]\n");
    printf("Comparações: %lld\n", comparacoesBusca);
    printf("Tempo de Execucao: %.6f segundos\n", tempoEmSegundos);
}


/**
 * @brief Função principal (main) do programa.
 */
int main() {
    int opcao;

    do {
        exibirMenuPrincipal();

        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opcao invalida.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarComponentes();
                break;
            case 2:
                gerenciarOrdenacao();
                break;
            case 3:
                gerenciarBuscaBinaria();
                break;
            case 4:
                mostrarComponentes(torreComponentes, totalComponentes);
                break;
            case 0:
                printf("\n[INFO] Encerrando o sistema de montagem. Adeus!\n");
                break;
            default:
                printf("\n[AVISO] Opcao nao reconhecida.\n");
                break;
        }

        if (opcao != 0) {
             printf("\nPressione ENTER para continuar...");
             getchar();
        }

    } while (opcao != 0);

    return 0;
}