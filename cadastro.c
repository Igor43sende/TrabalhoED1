#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura para o livro
typedef struct Livro{
    int id;
    char titulo[100];
    char autor[100];
    int ano;
    char genero[100];
    struct Livro *proximo;
}Livro;
     
// criar um novo nó Livro
Livro* criar(int id, char titulo, char *autor, int ano, char genero){
    Livro *novo = (Livro *)malloc(sizeof(Livro));
    novo ->id = id;
    strcpy(novo ->titulo, titulo);
    strcpy(novo ->autor, autor);
    novo->ano = ano;
    strcpy(novo->genero, genero);
    novo->proximo = NULL;
    return novo;
}

void inserir(Livro **lista, int id, char *titulo, char *autor, int ano, char *genero){
    Livro *novo = criar(id, titulo, autor, ano, genero);
    if(*lista == NULL){
        *lista = novo;
    } else {
        Livro *atual = *lista;
        while (atual->proximo != NULL){
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
}

void exibir(Livro *lista){
    Livro *atual = lista;
    while(atual != NULL){
        printf("ID: %d \n Título: %s \n Autor: %s \n Ano: %d \n Gênero: %s \n \n",atual->id, atual->titulo, atual->autor, atual->ano, atual->genero);
        atual = atual->proximo;
    }
}

Livro* busca(Livro *lista, int id){
    Livro *atual = lista;
    while(atual!= NULL){
        if(atual->id == id){
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

void remover(Livro **lista, int id){
    Livro *atual = *lista, *anterior = NULL;
    
    while(atual != NULL && atual->id != id){
        anterior = atual;
        atual = atual->proximo;
    }
    if(atual == NULL){
        printf("Livro com o ID (%d) não encontrado \n",id);
        return;
    }
    if(anterior == NULL){
        *lista = atual->proximo;
    } else {
        anterior->proximo = atual ->proximo;
    }
    free(atual);
    printf("Livro com o ID (%d) removido com sucesso \n",id);
}

void carregar(Livro **lista, const char *livrosED){
    FILE *arquivo = fopen(livrosED, "r");
    if(arquivo == NULL){
        printf("Erro \n");
        return;
    }
    int id, ano;
    char titulo[100],autor[100],genero[100];
    
    while(fscanf(arquivo,"%d;%99[^;];%99[^;];%d;%49[^\n]\n", &id, titulo, autor, &ano, genero)==5){
        inserir(lista, id, titulo, autor, ano, genero);
    }
    fclose(arquivo);
    printf("Livros carregados \n");
}
int main(int argc, char** argv) {
    Livro *lista = NULL;
    int opcao, id, ano;
    char titulo[100], autor[100], genero[100];
    
    carregar(&lista, "C:\\Users\\User\\OneDrive\\Área de Trabalho\\livrosED.txt");
    
    do{
        printf("\nMenu:\n");
        printf("1. Exibir livros\n");
        printf("2. Inserir livro\n");
        printf("3. Buscar livro por ID\n");
        printf("4. Remover livro por ID\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch(opcao){
            case 1:
                exibir(lista);
                break;
            case 2: 
                printf("Digite o ID: ");
                scanf("%d", &id);
                getchar(); // Limpar buffer
                printf("Digite o título: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0'; // Remover nova linha
                printf("Digite o autor: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';
                printf("Digite o ano: ");
                scanf("%d", &ano);
                getchar();
                printf("Digite o gênero: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';
                inserir(&lista, id, titulo, autor, ano, genero);
                printf("Livro inserido com sucesso!\n");
                break;
            case 3:
                printf("Digite o ID do livro a buscar: ");
                scanf("%d", &id);
                Livro *encontrado = busca(lista, id);
                if (encontrado) {
                    printf("Livro encontrado:\n");
                    printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\n",
                           encontrado->id, encontrado->titulo, encontrado->autor, encontrado->ano, encontrado->genero);
                } else {
                    printf("Livro com ID %d não encontrado.\n", id);
                }
                break;
            case 4:
                printf("Digite o ID do livro a remover: ");
                scanf("%d", &id);
                remover(&lista, id);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    }while (opcao != 0);
    return (EXIT_SUCCESS);
}