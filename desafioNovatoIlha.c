#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definição do tamanho máximo da mochila e dos campos
#define MAX_ITENS 10
#define MAX_NOME 30
#define MAX_TIPO 20

// Simplificação: Manter a struct é essencial.
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int ocupado; // Essencial para gerenciar remoções sem "compactar" o array
} Item;

// Variáveis Globais (Essenciais para a estrutura)
Item mochila[MAX_ITENS]; 
int totalItens = 0;      

// Protótipos das Funções
void inicializarMochila();
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void exibirMenu();
void limparBuffer();

/**
 * @brief Limpa o buffer de entrada (stdin).
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Inicializa o vetor da mochila (marcando como livre).
 */
void inicializarMochila() {
    for (int i = 0; i < MAX_ITENS; i++) {
        mochila[i].ocupado = 0;
    }
}

/**
 * @brief Obtém uma string do usuário e limpa o \n.
 * @param destino O array de caracteres para armazenar a string.
 * @param tamanho O tamanho máximo do array.
 * @return 1 se a leitura foi bem-sucedida, 0 caso contrário.
 *
 * Simplificação: Extrai a lógica de fgets e limpeza do \n para reuso.
 */
int lerStringSegura(char *destino, int tamanho) {
    if (fgets(destino, tamanho, stdin) == NULL) {
        return 0;
    }
    // Remove a quebra de linha ('\n') se houver
    destino[strcspn(destino, "\n")] = '\0';
    return 1;
}

/**
 * @brief Adiciona um novo item à mochila.
 */
void inserirItem() {
    printf("\n--- Cadastro de Novo Item ---\n");

    if (totalItens >= MAX_ITENS) {
        printf("[ERRO] Mochila cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    int i;
    // Simplificação: Não precisa de 'indiceLivre = -1', pois 'totalItens' garante espaço
    for (i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ocupado == 0) {
            break; // Encontrou o primeiro slot livre
        }
    }

    // Leitura dos dados
    printf("Nome do item (max %d caracteres): ", MAX_NOME - 1);
    if (!lerStringSegura(mochila[i].nome, MAX_NOME)) {
        printf("[ERRO] Falha na leitura do nome.\n");
        return;
    }

    printf("Tipo do item (ex: arma, municao, cura) (max %d caracteres): ", MAX_TIPO - 1);
    if (!lerStringSegura(mochila[i].tipo, MAX_TIPO)) {
        printf("[ERRO] Falha na leitura do tipo.\n");
        return;
    }

    printf("Quantidade: ");
    if (scanf("%d", &mochila[i].quantidade) != 1 || mochila[i].quantidade <= 0) {
        printf("[ERRO] Quantidade invalida. O item nao foi cadastrado.\n");
        limparBuffer(); 
        return;
    }
    limparBuffer(); 

    // Finaliza o cadastro
    mochila[i].ocupado = 1;
    totalItens++;

    printf("\n[SUCESSO] Item '%s' adicionado a mochila!\n", mochila[i].nome);
}

/**
 * @brief Remove um item da mochila pelo nome.
 */
void removerItem() {
    if (totalItens == 0) {
        printf("\n[AVISO] A mochila esta vazia. Nao ha itens para remover.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Remocao de Item ---\n");
    printf("Digite o NOME do item a ser removido: ");
    if (!lerStringSegura(nomeBusca, MAX_NOME)) {
        printf("[ERRO] Falha na leitura do nome.\n");
        return;
    }

    int encontrado = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ocupado && strcmp(mochila[i].nome, nomeBusca) == 0) {
            // Remoção: marca a posição como livre
            mochila[i].ocupado = 0;
            totalItens--;
            encontrado = 1;
            printf("\n[SUCESSO] Item '%s' removido da mochila!\n", nomeBusca);
            break;
        }
    }

    if (!encontrado) {
        printf("\n[AVISO] Item '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}

/**
 * @brief Implementa a busca sequencial.
 */
void buscarItem() {
    if (totalItens == 0) {
        printf("\n[AVISO] A mochila esta vazia. Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[MAX_NOME];
    printf("\n--- Busca de Item ---\n");
    printf("Digite o NOME do item a ser buscado: ");
    if (!lerStringSegura(nomeBusca, MAX_NOME)) {
        printf("[ERRO] Falha na leitura do nome.\n");
        return;
    }

    int encontrado = 0;

    // Busca sequencial
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ocupado && strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[RESULTADO] Item encontrado:\n");
            printf("-------------------------------\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Qtd.: %d\n", mochila[i].quantidade);
            printf("-------------------------------\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\n[AVISO] Item '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}

/**
 * @brief Lista todos os itens atualmente registrados na mochila.
 *
 * Simplificação: Melhor formatação da tabela.
 */
void listarItens() {
    printf("\n============================================\n");
    printf("          ITENS NA MOCHILA (%d/%d)          \n", totalItens, MAX_ITENS);
    printf("============================================\n");

    if (totalItens == 0) {
        printf("\n[AVISO] A mochila esta vazia.\n");
        printf("--------------------------------------------\n");
        return;
    }

    // Usando formatação fixa para alinhamento
    printf("  # | %-*s | %-*s | Quantidade\n", MAX_NOME - 4, "Nome", MAX_TIPO - 4, "Tipo");
    printf("--------------------------------------------\n");

    int contador = 0;
    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ocupado) {
            contador++;
            printf(" %2d | %-*s | %-*s | %10d\n",
                   contador,
                   MAX_NOME - 4, mochila[i].nome,
                   MAX_TIPO - 4, mochila[i].tipo,
                   mochila[i].quantidade);
        }
    }

    printf("--------------------------------------------\n");
}

/**
 * @brief Exibe o menu principal.
 */
void exibirMenu() {
    printf("\n=========================================\n");
    printf("   SISTEMA DE GERENCIAMENTO DE MOCHILA   \n");
    printf("=========================================\n");
    printf("1. Cadastrar novo item\n");
    printf("2. Remover item (pelo nome)\n");
    printf("3. Buscar item (pelo nome)\n");
    printf("4. Listar todos os itens\n");
    printf("0. Sair do programa\n");
    printf("-----------------------------------------\n");
    printf("Escolha uma opcao: ");
}

/**
 * @brief Função principal (main) do programa.
 */
int main() {
    int opcao;
    inicializarMochila();

    do {
        exibirMenu();

        if (scanf("%d", &opcao) != 1) {
            printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
            limparBuffer(); 
            continue;
        }
        limparBuffer(); 

        switch (opcao) {
            case 1: inserirItem();    break;
            case 2: removerItem();    break;
            case 3: buscarItem();     break;
            case 4: listarItens();    break;
            case 0: printf("\n[INFO] Encerrando o sistema. Adeus!\n"); break;
            default: printf("\n[AVISO] Opcao nao reconhecida.\n"); break;
        }

        // Simplificação: Se a operação for 1, 2 ou 3, listar, se não for 0 nem 4.
        if (opcao > 0 && opcao < 4) {
            listarItens(); // Listagem após operações de modificação/busca
        }
        
        // Espera para continuar (mantém a usabilidade)
        if (opcao != 0) {
             printf("\nPressione ENTER para continuar...");
             getchar();
        }

    } while (opcao != 0);

    return 0;
}