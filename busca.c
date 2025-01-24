#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 100 //Define o tamanho das tabelas hash

//Estrutura para representar um livro 
typedef struct  Livro 
{
    int id;     //Indentificador único do Livro 
    char titulo[100];   //Título do Livro
    char autor[100];    //Autor do Livro
    int ano;    //Ano da publicação
    char genero[100];   //Gênero do Livro
    struct Livro *proximo;      //Ponteiro para o próximo nó (caso de colisões na tabela hash)     
} Livro;

//Ponteiros para estruturas usadas no programa
Livro *lista = NULL; //Lista encadeada principal para armazenar os livros
Livro *hashTitulo[TABLE_SIZE]; //Tabela hash para buscar por título
Livro *hashAutor[TABLE_SIZE]; //Tabela hash para buscar por autor
Livro *hashAno[TABLE_SIZE]; //Tabela hash para buscar por ano

//Função hash para strings (usada para título e autor)
unsigned int hashFunction(const char *str){
    unsigned int hash = 0;
    while (*str)    //Percorrer cada caractere da string
    {
        hash = (hash * 31) + *str++;    //Calcula o hash usando uma fórmula de potência de 31
    }
    return hash % TABLE_SIZE; //Garante que o índice estará dentro do tamanho da tabela
}

//Função hash para inteiros (usada para ano)
unsigned int hashFunctionInt(int num) {
    return num % TABLE_SIZE;    //Retorna o índice diretamente como o resto da divisão
}    

//Função para criar um novo livro
Livro * criar (int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro * novo = (Livro *)malloc(sizeof(Livro));  //Aloca memória para o novo livro
    novo->id = id;  //Atribui o ID
    strcpy(novo->titulo, titulo);   //Copia o título para a estrutura
    strcpy(novo->autor, autor);    //Copia o autor para a estrutura
    novo->ano = ano;    //Define o ano
    strcpy(novo->genero, genero);   //Copia o gênero
    novo->proximo = NULL;   //Inicializa o próximo como NULL
    return novo;    //Retorna o ponteiro para o novo livro
}

//Inserir livro na lista encadeada original
void inserirNaLista(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro *novo = criar(id, titulo, autor, ano, genero); //Cria um novo Livro
    if (lista == NULL) {    //Caso a lista esteja vazia
        lista = novo;   //O Novo livro será o primeiro elemento
    }   else {
        Livro *atual = lista; //Começa no início da lista
        while (atual->proximo != NULL) { //Percorre até o final da lista
            atual = atual->proximo;
        }
        atual->proximo = novo; //Adiciona o novo  livro no final da lista
    }
}

//Inserir livro nas tabelas hash
void inserirNasHash(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    Livro *novo = criar(id, titulo, autor, ano, genero); //Cria um novo livro

    //Inserir na tabela hash de títulos
    unsigned int indexTitulo = hashFunction(titulo);    //Calcula o índice com base no título
    novo->proximo =hashTitulo[indexTitulo];     //Adiciona na frente da lista (resolvendo colisões)
    hashTitulo[indexTitulo] = novo;

    //Inserir na tabela hash de autores
    unsigned int indexAutor = hashFunction(autor);    //Calcula o índice com base no autor
    novo->proximo =hashAutor[indexAutor];     //Adiciona na frente da lista (resolvendo colisões)
    hashAutor[indexAutor] = novo;

    //Inserir na tabela hash de anos
    unsigned int indexAno = hashFunctionInt(ano);    //Calcula o índice com base no ano
    novo->proximo =hashAno[indexAno];     //Adiciona na frente da lista (resolvendo colisões)
    hashAno[indexAno] = novo;
}

//Combina a inserção na lista encadeada e nas tabekas hash
void inserir(int id, const char *titulo, const char *autor, int ano, const char *genero) {
    inserirNaLista(id, titulo, autor, ano, genero);     //Insere na lista encadeada
    inserirNasHash(id, titulo, autor, ano, genero);     //Insere nas tabelas hash
}

//Exibir todos os livros na lista encadeada
void exibir(Livro *lista) {
    Livro *atual = lista;
    while (atual != NULL) {     //Percorre toda a lista
        printf("ID: %d \nTítulo: %s \nAutor: %s \nAno: %d \nGênero: %s \n\n", atual->id, atual->titulo, atual->autor, atual->ano, atual->genero);
        atual = atual->proximo;    //Avança para o próximo  livro
    }
}

//Buscar livro por título usando a tabela hash
Livro * buscarPorTitulo(const char *titulo) {
    unsigned int index = hashFunction(titulo);  //Calcula o índice baseado no título
    Livro *atual = hashTitulo[index];
    while (atual != NULL)   //Percorre a lista de colisões
    {
        if (strcmp(atual->titulo, titulo) == 0)     //Compara o título
        {
            return atual;   //Retorna o livro encontrado
        }
        atual = atual->proximo;
    }
    return NULL;    //Retorna NULL se não encontrado
}

// Buscar livro por autor usando a tabela hash
Livro *buscarPorAutor(const char *autor) {
    unsigned int index = hashFunction(autor); // Calcula o índice baseado no autor
    Livro *atual = hashAutor[index];
    while (atual != NULL) {
        if (strcmp(atual->autor, autor) == 0) { // Compara o autor
            return atual;
        }
        atual = atual->proximo;
    }
    return NULL;
}

//Buscar livro por ano usando a tabela hash
Livro * buscarPorAno(int ano) {
    unsigned int index = hashFunctionInt(ano);  //Calcula o índice baseado no ano
    Livro *atual = hashAno[index];
    while (atual != NULL)   //Percorre a lista de colisões
    {
        if (atual->ano == ano)     //Compara o ano
        {
            return atual;   //Retorna o livro encontrado
        }
        atual = atual->proximo;
    }
    return NULL;    //Retorna NULL se não encontrado
}

//Carregar livros de um arquivo
void carregar(const char * arquivoNome) {
    FILE *arquivo =  fopen(arquivoNome, "r");   //Abre o arquivo para leitura
    if (arquivo == NULL)    //Verifica se o arquivo foi aberto com sucesso
    {
        printf("Ërro ao executar o arquivo");
        return;
    }
    int id, ano;
    char titulo[100], autor[1000], genero[100];
    //Lê os dados do arquivo e insere na lista e tabelas hash
    while (fscanf(arquivo, "%d;%99[^;];%99[^;];%d;%49[^\n]\n", &id, titulo, autor, &ano, genero) == 5) 
    {
        inserir(id, titulo, autor, ano, genero);
    }
    fclose(arquivo);    //fecha o arquivo
    printf("Livros carregados com sucesso \n");
}

int main() {
    //Inicializar tabelas hash (garantir que estão vazias no início)
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        hashTitulo[i] = NULL;
        hashAutor[i] = NULL;
        hashAno[i] = NULL;
    }

    int opcao, id, ano;
    char titulo[100], autor[100], genero[100];
    Livro *encontrado;

    carregar("livrosED.txt");   //Carregar os livros de um arquivo externo

    do
    {
        //Menu de opções
        printf("\nMENU\n");
        printf("1. Exibir Livros\n");
        printf("2.Inserir Lirvros\n");
        printf("3. Buscar livro por título\n");
        printf("4. Buscar livro por autor\n");
        printf("5. Buscar livro por ano\n");
        printf("0.Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            exibir(lista);  //Exibir lista dos livros
            break;
        case 2:
            //Entrada dos dados do novo livro
            printf ("Digite o ID: ");
            scanf("%d", &id);
            getchar();  //Limpa o buffer do teclado
            printf("Digite o título: ");
            fgets(titulo,sizeof(titulo), stdin);
            titulo[strcspn(titulo, "\n")] = '\0';   //Remove o '\n'
            printf("Digite o autor: ");
            fgets(autor, sizeof(autor), stdin);
            autor[strcspn(autor, "\n")] = '\0';
            printf("Digite o ano: ");
            scanf("%d", &ano);
            getchar();
            printf("Digite o gênero: ");
            fgets(genero, sizeof(genero), stdin);
            genero[strspn(genero, "\n")] = '\0';
            inserir(id, titulo, autor, ano, genero);    //Insere um novo livro
            printf("Livro inserido com sucesso!!!\n");
            break;
        case 3:
                // Busca por título
                printf("Digite o título do livro a buscar: ");
                getchar();
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = '\0';
                encontrado = buscarPorTitulo(titulo);
                if (encontrado) {
                    printf("Livro encontrado:\n");
                    printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\n",
                           encontrado->id, encontrado->titulo, encontrado->autor, encontrado->ano, encontrado->genero);
                } else {
                    printf("Livro com título '%s' não encontrado.\n", titulo);
                }
                break;
            case 4:
                // Busca por autor
                printf("Digite o autor do livro a buscar: ");
                getchar();
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = '\0';
                encontrado = buscarPorAutor(autor);
                if (encontrado) {
                    printf("Livro encontrado:\n");
                    printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\n",
                           encontrado->id, encontrado->titulo, encontrado->autor, encontrado->ano, encontrado->genero);
                } else {
                    printf("Nenhum livro do autor '%s' foi encontrado.\n", autor);
                }
                break;
            case 5:
                // Busca por ano
                printf("Digite o ano do livro a buscar: ");
                scanf("%d", &ano);
                encontrado = buscarPorAno(ano);
                if (encontrado) {
                    printf("Livro encontrado:\n");
                    printf("ID: %d\nTítulo: %s\nAutor: %s\nAno: %d\nGênero: %s\n",
                           encontrado->id, encontrado->titulo, encontrado->autor, encontrado->ano, encontrado->genero);
                } else {
                    printf("Nenhum livro do ano '%d' foi encontrado.\n", ano);
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
    
    return 0;
}