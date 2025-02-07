#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TABLE_SIZE 100 // Tamanho das tabelas hash

// Estrutura para representar um livro
typedef struct Livro {
    int id;
    char titulo[100];
    char autor[100];
    int ano;
    char genero[100];
    int emprestado; // 0 = não emprestado, 1 = emprestado
    char emprestadoPara[100]; // Nome da pessoa que pegou emprestado
    struct Livro *proximo;
} Livro;

// Variáveis globais
Livro *lista = NULL; // Lista encadeada principal
Livro *hashTitulo[TABLE_SIZE]; // Tabela hash para títulos
Livro *hashAutor[TABLE_SIZE]; // Tabela hash para autores
Livro *hashAno[TABLE_SIZE]; // Tabela hash para anos

// Função hash para strings (usada para título e autor)
unsigned int hashFunction(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash * 31) + *str++;
    }
    return hash % TABLE_SIZE;
}

// Função hash para inteiros (usada para ano)
unsigned int hashFunctionInt(int num) {
    return num % TABLE_SIZE;
}

// Função para criar um novo livro
Livro *criar(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro *novo = (Livro *)malloc(sizeof(Livro));
    novo->id = id;
    strcpy(novo->titulo, titulo);
    strcpy(novo->autor, autor);
    novo->ano = ano;
    strcpy(novo->genero, genero);
    novo->emprestado = 0;
    strcpy(novo->emprestadoPara, "");
    novo->proximo = NULL;
    return novo;
}

// Função para verificar se um ID já existe
int idExiste(int id) {
    Livro *atual = lista;
    while (atual != NULL) {
        if (atual->id == id) {
            return 1; // ID já existe
        }
        atual = atual->proximo;
    }
    return 0; // ID não existe
}

// Função para inserir um livro na lista encadeada
void inserirNaLista(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro *novo = criar(id, titulo, autor, ano, genero);
    if (lista == NULL) {
        lista = novo;
    } else {
        Livro *atual = lista;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novo;
    }
}

// Função para inserir um livro nas tabelas hash
void inserirNasHash(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    // Criar um novo livro para cada tabela hash
    Livro *novoTitulo = criar(id, titulo, autor, ano, genero);
    Livro *novoAutor = criar(id, titulo, autor, ano, genero);
    Livro *novoAno = criar(id, titulo, autor, ano, genero);

    // Inserir na tabela hash de títulos
    unsigned int indexTitulo = hashFunction(titulo);
    novoTitulo->proximo = hashTitulo[indexTitulo];
    hashTitulo[indexTitulo] = novoTitulo;

    // Inserir na tabela hash de autores
    unsigned int indexAutor = hashFunction(autor);
    novoAutor->proximo = hashAutor[indexAutor];
    hashAutor[indexAutor] = novoAutor;

    // Inserir na tabela hash de anos
    unsigned int indexAno = hashFunctionInt(ano);
    novoAno->proximo = hashAno[indexAno];
    hashAno[indexAno] = novoAno;
}

// Função para inserir um livro na lista e nas tabelas hash
void inserir(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    if (idExiste(id)) {
        printf("Erro: ID %d já existe.\n", id);
        return;
    }
    inserirNaLista(id, titulo, autor, ano, genero);
    inserirNasHash(id, titulo, autor, ano, genero);
    printf("Livro inserido com sucesso!\n");
}

// Função para exibir todos os livros
void exibir(Livro *lista) {
    Livro *atual = lista;
    while (atual != NULL) {
        printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
               atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
               atual->emprestado ? "Sim" : "Não", atual->emprestadoPara);
        atual = atual->proximo;
    }
}

// Função para buscar todos os livros por título
void buscarPorTitulo(const char *titulo) {
    unsigned int index = hashFunction(titulo);
    Livro *atual = hashTitulo[index];
    int encontrados = 0;

    printf("\n--- Resultados da busca por título: '%s' ---\n", titulo);
    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
                   atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
                   atual->emprestado ? "Sim" : "Não", atual->emprestadoPara);
            encontrados++;
        }
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Nenhum livro encontrado com o título '%s'.\n", titulo);
    }
}

// Função para converter uma string para minúsculas
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Função para buscar todos os livros por autor
void buscarPorAutor(const char *autor) {
    unsigned int index = hashFunction(autor);
    Livro *atual = hashAutor[index];
    int encontrados = 0;

    // Converter o autor inserido para minúsculas para comparação
    char autorLower[100];
    strcpy(autorLower, autor);
    toLowerCase(autorLower);

    printf("\n--- Resultados da busca por autor: '%s' ---\n", autor);

    // Itera sobre todos os livros na posição da tabela hash
    while (atual != NULL) {
        // Converter o autor do livro para minúsculas para comparação
        char atualAutorLower[100];
        strcpy(atualAutorLower, atual->autor);
        toLowerCase(atualAutorLower);

        // Verifica se o autor é igual (ignorando maiúsculas/minúsculas)
        if (strcmp(atualAutorLower, autorLower) == 0) {
            printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
                   atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
                   atual->emprestado ? "Sim" : "Não", atual->emprestadoPara);
            encontrados++;
        }
        // Move para o próximo livro na lista encadeada
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Nenhum livro encontrado com o autor '%s'.\n", autor);
    }
}

// Função para buscar todos os livros por ano
void buscarPorAno(int ano) {
    unsigned int index = hashFunctionInt(ano);
    Livro *atual = hashAno[index];
    int encontrados = 0;

    printf("\n--- Resultados da busca por ano: %d ---\n", ano);

    // Itera sobre todos os livros na posição da tabela hash
    while (atual != NULL) {
        // Verifica se o ano é igual
        if (atual->ano == ano) {
            printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
                   atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
                   atual->emprestado ? "Sim" : "Não", atual->emprestadoPara);
            encontrados++;
        }
        // Move para o próximo livro na lista encadeada
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Nenhum livro encontrado com o ano %d.\n", ano);
    }
}


// Função para registrar empréstimo de um livro
void registrarEmprestimo(const char *titulo, const char *nomePessoa) {
    unsigned int index = hashFunction(titulo);
    Livro *atual = hashTitulo[index];
    int encontrados = 0;

    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            if (atual->emprestado) {
                printf("O livro '%s' já está emprestado para %s.\n", titulo, atual->emprestadoPara);
            } else {
                atual->emprestado = 1;
                strcpy(atual->emprestadoPara, nomePessoa);
                printf("Empréstimo registrado: '%s' emprestado para %s.\n", titulo, nomePessoa);
            }
            encontrados++;
        }
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Livro com título '%s' não encontrado.\n", titulo);
    }
}

// Função para registrar devolução de um livro
void registrarDevolucao(const char *titulo) {
    unsigned int index = hashFunction(titulo);
    Livro *atual = hashTitulo[index];
    int encontrados = 0;

    while (atual != NULL) {
        if (strcmp(atual->titulo, titulo) == 0) {
            if (atual->emprestado) {
                atual->emprestado = 0;
                strcpy(atual->emprestadoPara, "");
                printf("Devolução registrada: '%s' está disponível novamente.\n", titulo);
            } else {
                printf("O livro '%s' já está disponível.\n", titulo);
            }
            encontrados++;
        }
        atual = atual->proximo;
    }

    if (encontrados == 0) {
        printf("Livro com título '%s' não encontrado.\n", titulo);
    }
}

// Função para carregar livros de um arquivo
void carregar(const char *arquivoNome) {
    FILE *arquivo = fopen(arquivoNome, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    int id, ano;
    char titulo[100], autor[100], genero[100];
    while (fscanf(arquivo, "%d;%99[^;];%99[^;];%d;%99[^\n]\n", &id, titulo, autor, &ano, genero) == 5) {
        inserir(id, titulo, autor, ano, genero);
    }
    fclose(arquivo);
    printf("Livros carregados com sucesso.\n");
}

// Função para gerar relatório de livros cadastrados
void relatorioLivrosCadastrados() {
    Livro *atual = lista;
    printf("\n--- Relatório de Livros Cadastrados ---\n");
    while (atual != NULL) {
        printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\nEmprestado: %s\nPara: %s\n\n",
               atual->id, atual->titulo, atual->autor, atual->ano, atual->genero,
               atual->emprestado ? "Sim" : "Não", atual->emprestadoPara);
        atual = atual->proximo;
    }
}

// Função para liberar a memória alocada
void liberarMemoria() {
    Livro *atual = lista;
    Livro *proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

// Função principal
int main() {
    // Inicializar tabelas hash
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTitulo[i] = NULL;
        hashAutor[i] = NULL;
        hashAno[i] = NULL;
    }

    carregar("livrosED.txt"); // Carregar livros do arquivo

    int opcao, id, ano;
    char titulo[100], autor[100], genero[100], nomePessoa[100];

    do {
        printf("\nMENU\n");
        printf("1. Exibir Livros\n");
        printf("2. Inserir Livro\n");
        printf("3. Buscar por Título\n");
        printf("4. Buscar por Autor\n");
        printf("5. Buscar por Ano\n");
        printf("6. Registrar Empréstimo\n");
        printf("7. Registrar Devolução\n");
        printf("8. Relatório de Livros Cadastrados\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do teclado após o scanf

        switch (opcao) {
            case 1:
                exibir(lista);
                break;
            case 2:
                printf("Digite o ID: ");
                scanf("%d", &id);
                getchar();
                printf("Digite o título: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                printf("Digite o autor: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';
                printf("Digite o ano: ");
                scanf("%d", &ano);
                getchar();
                printf("Digite o gênero: ");
                fgets(genero, sizeof(genero), stdin);
                genero[strcspn(genero, "\n")] = '\0';
                inserir(id, titulo, autor, ano, genero);
                break;
            case 3:
                printf("Digite o título: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                buscarPorTitulo(titulo);
                break;
            case 4:
                printf("Digite o autor: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';
                buscarPorAutor(autor);
                break;
            case 5:
                printf("Digite o ano: ");
                scanf("%d", &ano);
                buscarPorAno(ano);
                break;
            case 6:
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                printf("Digite o nome da pessoa: ");
                fgets(nomePessoa, sizeof(nomePessoa), stdin);
                nomePessoa[strcspn(nomePessoa, "\n")] = '\0';
                registrarEmprestimo(titulo, nomePessoa);
                break;
            case 7:
                printf("Digite o título do livro: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                registrarDevolucao(titulo);
                break;
            case 8:
                relatorioLivrosCadastrados();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    liberarMemoria(); // Liberar memória alocada

    return 0;
}