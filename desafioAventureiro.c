#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- DEFINIÇÕES GLOBAIS ---
#define MAX_ITENS 10 // Capacidade máxima para o Vetor
#define MAX_NOME 30
#define MAX_TIPO 20

// Variáveis para contagem de operações de busca
long long comparacoesBusca = 0;
int totalItensVetor = 0;

// --- 1. CRIAÇÃO DAS STRUCTS ---

/**
 * @brief Struct principal para armazenar dados de um item.
 */
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int ocupado; // 1: ocupado, 0: livre (apenas para o Vetor)
} Item;

/**
 * @brief Struct para representar um NÓ da Lista Encadeada.
 */
typedef struct No {
    Item dados;             // Armazena os dados do Item
    struct No* proximo;     // Ponteiro para o próximo nó da lista
} No;

// Variáveis Globais para as Estruturas
Item mochilaVetor[MAX_ITENS]; // Vetor (Lista Sequencial)
No* headLista = NULL;         // Ponteiro inicial (cabeça) da Lista Encadeada

// --- PROTÓTIPOS DAS FUNÇÕES AUXILIARES E DE MENU ---
void limparBuffer();
int lerStringSegura(char *destino, int tamanho);
void exibirMenuPrincipal();
void exibirMenuOperacoes(int tipoEstrutura);

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
    if (fgets(destino, tamanho, stdin) == NULL) {
        return 0;
    }
    destino[strcspn(destino, "\n")] = '\0';
    return 1;
}

// =======================================================
// === IMPLEMENTAÇÃO DO VETOR (LISTA SEQUENCIAL) - FUNÇÕES 
// =======================================================

/**
 * @brief Inicializa o Vetor marcando todos os slots como livres.
 */
void inicializarVetor() {
    for (int i = 0; i < MAX_ITENS; i++) {
        mochilaVetor[i].ocupado = 0;
    }
    totalItensVetor = 0;
}

/**
 * @brief 3.1 Inserir novo item no Vetor.
 */
void inserirItemVetor() {
    printf("\n--- Cadastro de Novo Item (Vetor) ---\n");

    if (totalItensVetor >= MAX_ITENS) {
        printf("[ERRO] Mochila (Vetor) cheia! Capacidade maxima atingida.\n");
        return;
    }

    int i;
    for (i = 0; i < MAX_ITENS; i++) {
        if (mochilaVetor[i].ocupado == 0) {
            break; // Encontra o primeiro slot livre
        }
    }

    printf("Nome: ");
    if (!lerStringSegura(mochilaVetor[i].nome, MAX_NOME)) return;

    printf("Tipo (ex: arma, cura): ");
    if (!lerStringSegura(mochilaVetor[i].tipo, MAX_TIPO)) return;

    printf("Quantidade: ");
    if (scanf("%d", &mochilaVetor[i].quantidade) != 1 || mochilaVetor[i].quantidade <= 0) {
        printf("[ERRO] Quantidade invalida.\n");
        limparBuffer(); 
        return;
    }
    limparBuffer();

    mochilaVetor[i].ocupado = 1;
    totalItensVetor++;

    printf("\n[SUCESSO] Item '%s' adicionado ao Vetor!\n", mochilaVetor[i].nome);
}

/**
 * @brief 3.2 Remover item por nome no Vetor.
 */
void removerItemVetor() {
    if (totalItensVetor == 0) {
        printf("\n[AVISO] O Vetor esta vazio.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Remocao de Item (Vetor) ---\n");
    printf("Digite o NOME do item a ser removido: ");
    if (!lerStringSegura(nomeBusca, MAX_NOME)) return;

    int encontrado = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochilaVetor[i].ocupado && strcmp(mochilaVetor[i].nome, nomeBusca) == 0) {
            mochilaVetor[i].ocupado = 0; // Marca como livre (removido)
            totalItensVetor--;
            encontrado = 1;
            printf("\n[SUCESSO] Item '%s' removido do Vetor!\n", nomeBusca);
            break;
        }
    }

    if (!encontrado) {
        printf("\n[AVISO] Item '%s' nao encontrado.\n", nomeBusca);
    }
}

/**
 * @brief 3.3 Listar todos os itens do Vetor.
 */
void listarItensVetor() {
    printf("\n============================================\n");
    printf("        ITENS NA MOCHILA (VETOR) (%d/%d)    \n", totalItensVetor, MAX_ITENS);
    printf("============================================\n");

    if (totalItensVetor == 0) {
        printf("\n[AVISO] O Vetor esta vazio.\n");
        printf("--------------------------------------------\n");
        return;
    }

    printf("  # | %-*s | %-*s | Quantidade\n", MAX_NOME - 4, "Nome", MAX_TIPO - 4, "Tipo");
    printf("--------------------------------------------\n");

    int contador = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochilaVetor[i].ocupado) {
            contador++;
            printf(" %2d | %-*s | %-*s | %10d\n",
                   contador,
                   MAX_NOME - 4, mochilaVetor[i].nome,
                   MAX_TIPO - 4, mochilaVetor[i].tipo,
                   mochilaVetor[i].quantidade);
        }
    }
    printf("--------------------------------------------\n");
}

/**
 * @brief 3.4 Busca Sequencial no Vetor.
 * @return 1 se encontrado, 0 se não.
 */
int buscarSequencialVetor(const char* nomeBusca) {
    comparacoesBusca = 0;
    int encontrado = 0;

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochilaVetor[i].ocupado) {
            comparacoesBusca++; // Contabiliza a comparação de string
            if (strcmp(mochilaVetor[i].nome, nomeBusca) == 0) {
                printf("\n[RESULTADO BUSCA SEQUENCIAL] Item encontrado:\n");
                printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                       mochilaVetor[i].nome, mochilaVetor[i].tipo, mochilaVetor[i].quantidade);
                encontrado = 1;
                break;
            }
        }
    }
    return encontrado;
}

/**
 * @brief 4. Ordena o Vetor usando Selection Sort (por nome).
 */
void ordenarVetor() {
    if (totalItensVetor <= 1) {
        printf("\n[AVISO] Vetor com 0 ou 1 item. Nao e necessaria a ordenacao.\n");
        return;
    }
    
    // Cria um vetor temporário apenas com os itens ocupados para facilitar a ordenação
    Item tempArray[MAX_ITENS];
    int j = 0;
    for(int i = 0; i < MAX_ITENS; i++) {
        if(mochilaVetor[i].ocupado) {
            tempArray[j++] = mochilaVetor[i];
        }
    }
    
    int n = totalItensVetor;
    Item chave;

    // Selection Sort no tempArray
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int k = i + 1; k < n; k++) {
            if (strcmp(tempArray[k].nome, tempArray[min_idx].nome) < 0) {
                min_idx = k;
            }
        }
        // Troca o elemento mínimo com o primeiro elemento não ordenado
        chave = tempArray[min_idx];
        tempArray[min_idx] = tempArray[i];
        tempArray[i] = chave;
    }
    
    // Copia de volta (simples e rápido para 10 itens)
    inicializarVetor(); // Limpa a estrutura principal
    for(int i = 0; i < n; i++) {
        mochilaVetor[i] = tempArray[i];
        mochilaVetor[i].ocupado = 1;
        totalItensVetor++;
    }

    printf("\n[SUCESSO] Vetor ordenado por Nome (Selection Sort).\n");
}

/**
 * @brief 5. Busca Binária no Vetor (apenas após ordenação).
 * @return 1 se encontrado, 0 se não.
 */
int buscarBinariaVetor(const char* nomeBusca) {
    comparacoesBusca = 0;
    int encontrado = 0;
    int n = totalItensVetor;
    
    // A busca binária deve ocorrer apenas nos slots ocupados (0 a n-1)
    int baixo = 0;
    int alto = n - 1;

    while (baixo <= alto) {
        int meio = baixo + (alto - baixo) / 2;
        int resultado;

        // Verifica se o slot do meio está ocupado (deve estar se o vetor foi compactado/ordenado corretamente)
        if (!mochilaVetor[meio].ocupado) {
            // Em um vetor "compactado", isso não deveria ocorrer.
            return 0; 
        }

        comparacoesBusca++; // Contabiliza a comparação de string
        resultado = strcmp(mochilaVetor[meio].nome, nomeBusca);

        if (resultado == 0) {
            printf("\n[RESULTADO BUSCA BINARIA] Item encontrado:\n");
            printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                   mochilaVetor[meio].nome, mochilaVetor[meio].tipo, mochilaVetor[meio].quantidade);
            encontrado = 1;
            break;
        } else if (resultado < 0) {
            baixo = meio + 1;
        } else {
            alto = meio - 1;
        }
    }

    return encontrado;
}

// =======================================================
// === IMPLEMENTAÇÃO DA LISTA ENCEADA - FUNÇÕES
// =======================================================

/**
 * @brief Inicializa a Lista Encadeada.
 */
void inicializarLista() {
    // A lista é inicializada quando headLista = NULL
    headLista = NULL;
}

/**
 * @brief 3.1 Inserir novo item na Lista Encadeada (no início para simplificar).
 */
void inserirItemLista() {
    printf("\n--- Cadastro de Novo Item (Lista Encadeada) ---\n");

    // Aloca memória para o novo nó
    No* novoNo = (No*)malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("[ERRO] Falha na alocacao de memoria.\n");
        return;
    }

    // Leitura dos dados
    printf("Nome: ");
    if (!lerStringSegura(novoNo->dados.nome, MAX_NOME)) {
        free(novoNo);
        return;
    }

    printf("Tipo (ex: arma, cura): ");
    if (!lerStringSegura(novoNo->dados.tipo, MAX_TIPO)) {
        free(novoNo);
        return;
    }

    printf("Quantidade: ");
    if (scanf("%d", &novoNo->dados.quantidade) != 1 || novoNo->dados.quantidade <= 0) {
        printf("[ERRO] Quantidade invalida.\n");
        limparBuffer();
        free(novoNo);
        return;
    }
    limparBuffer();

    // Inserção no início da lista
    novoNo->proximo = headLista;
    headLista = novoNo;

    printf("\n[SUCESSO] Item '%s' adicionado a Lista Encadeada!\n", novoNo->dados.nome);
}

/**
 * @brief 3.2 Remover item por nome na Lista Encadeada.
 */
void removerItemLista() {
    if (headLista == NULL) {
        printf("\n[AVISO] A Lista Encadeada esta vazia.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Remocao de Item (Lista Encadeada) ---\n");
    printf("Digite o NOME do item a ser removido: ");
    if (!lerStringSegura(nomeBusca, MAX_NOME)) return;

    No* atual = headLista;
    No* anterior = NULL;
    int encontrado = 0;

    // Percorre a lista
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            if (anterior == NULL) {
                // Remoção do primeiro nó (head)
                headLista = atual->proximo;
            } else {
                // Remoção de um nó no meio ou fim
                anterior->proximo = atual->proximo;
            }
            free(atual); // Libera a memória
            encontrado = 1;
            printf("\n[SUCESSO] Item '%s' removido da Lista Encadeada!\n", nomeBusca);
            break;
        }
        anterior = atual;
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("\n[AVISO] Item '%s' nao encontrado na Lista Encadeada.\n", nomeBusca);
    }
}

/**
 * @brief 3.3 Listar todos os itens da Lista Encadeada.
 */
void listarItensLista() {
    printf("\n============================================\n");
    printf("      ITENS NA MOCHILA (LISTA ENCEADA)      \n");
    printf("============================================\n");

    if (headLista == NULL) {
        printf("\n[AVISO] A Lista Encadeada esta vazia.\n");
        printf("--------------------------------------------\n");
        return;
    }

    printf("  # | %-*s | %-*s | Quantidade\n", MAX_NOME - 4, "Nome", MAX_TIPO - 4, "Tipo");
    printf("--------------------------------------------\n");

    No* atual = headLista;
    int contador = 0;
    while (atual != NULL) {
        contador++;
        printf(" %2d | %-*s | %-*s | %10d\n",
               contador,
               MAX_NOME - 4, atual->dados.nome,
               MAX_TIPO - 4, atual->dados.tipo,
               atual->dados.quantidade);
        atual = atual->proximo;
    }

    printf("--------------------------------------------\n");
}

/**
 * @brief 3.4 e 6. Busca Sequencial na Lista Encadeada e Contador.
 */
void buscarSequencialLista() {
    if (headLista == NULL) {
        printf("\n[AVISO] A Lista Encadeada esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Busca Sequencial (Lista Encadeada) ---\n");
    printf("Digite o NOME do item a ser buscado: ");
    if (!lerStringSegura(nomeBusca, MAX_NOME)) return;

    No* atual = headLista;
    comparacoesBusca = 0;
    int encontrado = 0;

    // Percorre a lista (Busca Sequencial)
    while (atual != NULL) {
        comparacoesBusca++; // Contabiliza a comparação de string
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            printf("\n[RESULTADO BUSCA SEQUENCIAL] Item encontrado:\n");
            printf("Nome: %s | Tipo: %s | Qtd: %d\n",
                   atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            encontrado = 1;
            break;
        }
        atual = atual->proximo;
    }

    if (!encontrado) {
        printf("\n[AVISO] Item '%s' nao encontrado.\n", nomeBusca);
    }

    printf("\n[CONTADOR] Comparações realizadas: %lld\n", comparacoesBusca);
}

// =======================================================
// === FUNÇÕES DE MENU E FLUXO PRINCIPAL
// =======================================================

/**
 * @brief Exibe o menu principal de escolha de estrutura.
 */
void exibirMenuPrincipal() {
    printf("\n=========================================\n");
    printf("   SISTEMA DE MOCHILA: ESCOLHA A ESTRUTURA \n");
    printf("=========================================\n");
    printf("1. Gerenciar Mochila (Vetor/Lista Sequencial)\n");
    printf("2. Gerenciar Mochila (Lista Encadeada)\n");
    printf("0. Sair do programa\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Exibe o menu de operações para a estrutura escolhida.
 * @param tipoEstrutura 1 para Vetor, 2 para Lista Encadeada.
 */
void exibirMenuOperacoes(int tipoEstrutura) {
    const char* nomeEstrutura = (tipoEstrutura == 1) ? "VETOR" : "LISTA ENCEADA";
    printf("\n=========================================\n");
    printf("  OPERACOES NA MOCHILA (%s)  \n", nomeEstrutura);
    printf("=========================================\n");
    printf("1. Inserir novo item\n");
    printf("2. Remover item (por nome)\n");
    printf("3. Listar todos os itens\n");
    printf("4. Buscar item (Busca Sequencial)\n");
    
    if (tipoEstrutura == 1) {
        printf("5. Ordenar itens (Selection Sort por Nome)\n");
        printf("6. Buscar item (Busca Binaria - Requer Ordenacao)\n");
        printf("7. Voltar ao Menu Principal\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");
    } else {
        printf("5. Voltar ao Menu Principal\n");
        printf("-----------------------------------------\n");
        printf("Escolha uma opcao: ");
    }
}

/**
 * @brief Gerencia as operações na estrutura de Vetor.
 */
void gerenciarVetor() {
    int opcao;
    char nomeBusca[MAX_NOME];
    
    do {
        exibirMenuOperacoes(1);
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opcao invalida.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        comparacoesBusca = 0; // Reseta o contador
        int encontrado = 0;

        switch (opcao) {
            case 1: 
                inserirItemVetor();
                listarItensVetor(); 
                break;
            case 2: 
                removerItemVetor();
                listarItensVetor(); 
                break;
            case 3: 
                listarItensVetor(); 
                break;
            case 4: 
                // Busca Sequencial
                if (totalItensVetor == 0) {
                     printf("\n[AVISO] O Vetor esta vazio.\n");
                } else {
                    printf("Digite o NOME para a Busca Sequencial: ");
                    if (!lerStringSegura(nomeBusca, MAX_NOME)) break;
                    encontrado = buscarSequencialVetor(nomeBusca);
                    if (!encontrado) printf("\n[AVISO] Item '%s' nao encontrado.\n", nomeBusca);
                    printf("\n[CONTADOR] Comparações realizadas (Sequencial): %lld\n", comparacoesBusca);
                }
                break;
            case 5: 
                ordenarVetor(); 
                listarItensVetor(); 
                break;
            case 6:
                // Busca Binária
                if (totalItensVetor == 0) {
                     printf("\n[AVISO] O Vetor esta vazio.\n");
                } else {
                    printf("Digite o NOME para a Busca Binaria: ");
                    if (!lerStringSegura(nomeBusca, MAX_NOME)) break;
                    encontrado = buscarBinariaVetor(nomeBusca);
                    if (!encontrado) printf("\n[AVISO] Item '%s' nao encontrado.\n", nomeBusca);
                    printf("\n[CONTADOR] Comparações realizadas (Binária): %lld\n", comparacoesBusca);
                }
                break;
            case 7:
                printf("\n[INFO] Voltando ao Menu Principal.\n");
                break;
            default:
                printf("\n[AVISO] Opcao nao reconhecida.\n");
                break;
        }

        if (opcao != 7 && opcao != 0) {
             printf("\nPressione ENTER para continuar...");
             getchar(); 
        }

    } while (opcao != 7);
}

/**
 * @brief Gerencia as operações na estrutura de Lista Encadeada.
 */
void gerenciarListaEncadeada() {
    int opcao;
    
    do {
        exibirMenuOperacoes(2);
        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opcao invalida.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: 
                inserirItemLista();
                listarItensLista(); 
                break;
            case 2: 
                removerItemLista();
                listarItensLista(); 
                break;
            case 3: 
                listarItensLista(); 
                break;
            case 4: 
                buscarSequencialLista(); // Inclui o contador
                break;
            case 5:
                printf("\n[INFO] Voltando ao Menu Principal.\n");
                break;
            default:
                printf("\n[AVISO] Opcao nao reconhecida.\n");
                break;
        }

        if (opcao != 5 && opcao != 0) {
             printf("\nPressione ENTER para continuar...");
             getchar(); 
        }

    } while (opcao != 5);
}

/**
 * @brief Função principal (main) do programa.
 */
int main() {
    int opcaoPrincipal;

    // Inicialização das estruturas
    inicializarVetor();
    inicializarLista();

    do {
        exibirMenuPrincipal();

        if (scanf("%d", &opcaoPrincipal) != 1) {
            printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcaoPrincipal) {
            case 1: 
                gerenciarVetor();
                break;
            case 2: 
                gerenciarListaEncadeada();
                break;
            case 0:
                printf("\n[INFO] Encerrando o sistema. Adeus!\n");
                break;
            default:
                printf("\n[AVISO] Opcao nao reconhecida. Por favor, escolha uma opcao valida.\n");
                break;
        }

    } while (opcaoPrincipal != 0);

    return 0;
}