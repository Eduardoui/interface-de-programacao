#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//Define o número máximo de relatos e o raio máximo (em km)
#define MAX_RELATOS 100
#define MAX_RAIO 10.0

//Estrutura que representa um relato com iformações pessoais e a localização
typedef struct {
    char nome[60];
    char email[60];
    char telefone[20];
    char descricao[300];
    float latitude;
    float longitude;
} Relato;

//Função para cadastrar novo relato
void CadastrarRelato(Relato relatos[], int *quantidade) {
    if (*quantidade >= MAX_RELATOS) {
        printf("Você atingiu o limite máximo de relatos.\n");
        return;
    }

    Relato r;

    //Entrada de dados do usuário
    printf("Nome: ");
    fgets(r.nome, 60, stdin);
    strtok(r.nome, "\n");  //Remover '\n'

    printf("Email: ");
    fgets(r.email, 60, stdin);
    strtok(r.email, "\n");

    printf("Telefone: ");
    fgets(r.telefone, 20, stdin);
    strtok(r.telefone, "\n");

    printf("Descrição: ");
    fgets(r.descricao, 300, stdin);
    strtok(r.descricao, "\n");

    //Entrada para as coordenadas geográficas
    printf("Latitude: ");
    scanf("%f", &r.latitude);

    printf("Longitude: ");
    scanf("%f", &r.longitude);
    getchar();  //Limpar o '\n'deixando scanf

    //Armazena o relato no vetor
    relatos[*quantidade] = r;
    (*quantidade)++;

    printf("Seu relato foi cadastrado com sucesso!\n");
}

//Função para listar todos os relatos cadastrados
void ListarRelatos(Relato relatos[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nRelato %d:\n", i + 1);
        printf("Nome: %s\n", relatos[i].nome);
        printf("Email: %s\n", relatos[i].email);
        printf("Telefone: %s\n", relatos[i].telefone);
        printf("Descrição: %s\n", relatos[i].descricao);
        printf("Localização: (%.2f, %.2f)\n", relatos[i].latitude, relatos[i].longitude);
    }
}

//Função para calcular a distância entre duas coordenadas em km
float CalcularDistancia(float lat1, float lon1, float lat2, float lon2) {
    float dx = lat2 - lat1;
    float dy = lon2 - lon1;
    return sqrt(dx * dx + dy * dy) * 111;  //111 km ~ 1 grau de distãncia
}

//Consultar relatos dentro de um raio a partir de uma posição (lat, long)
void ConsultarPorRaio(Relato relatos[], int quantidade, float lat, float lon) {
    printf("\nRelatos até %.2f km de distância:\n", RAIO_MAXIMO);
    for (int i = 0; i < quantidade; i++) {
        float dist = CalcularDistancia(lat, lon, relatos[i].latitude, relatos[i].longitude);
        if (dist <= RAIO_MAXIMO) {
            printf("Relato %d: %s - Distância: %.2f km\n", i + 1, relatos[i].descricao, dist);
        }
    }
}

//Ordena os relatos por nome em ordem alfabética
void OrdenarRelatosPorNome(Relato relatos[], int quantidade) {
    for (int i = 0; i < quantidade - 1; i++) {
        for (int j = 0; j < quantidade - i - 1; j++) {
            if (strcmp(relatos[j].nome, relatos[j + 1].nome) > 0) {
                Relato temp = relatos[j];
                relatos[j] = relatos[j + 1];
                relatos[j + 1] = temp;
            }
        }
    }
    printf("Relatos ordenados por nome.\n");
}

//Salvar todos os relatos no arquivo "Relatos.txt"
void SalvarRelatos(Relato relatos[], int quantidade) {
    FILE *f = fopen("relatos.txt", "w");
    if (!f) {
        printf("Erro ao abrir o arquivo para salvar.\n");
        return;
    }

    //Escreve cada relato no arquivo em formato CSV
    for (int i = 0; i < quantidade; i++) {
        fprintf(f, "%s;%s;%s;%s;%.6f;%.6f\n",
                relatos[i].nome, relatos[i].email, relatos[i].telefone, relatos[i].descricao,
                relatos[i].latitude, relatos[i].longitude);
    }
    fclose(f);
    printf("Dados salvos com sucesso.\n");
}

//Carrega os relatos salvos do arquivo "relatos.txt"
void CarregarRelatos(Relato relatos[], int *quantidade) {
    FILE *f = fopen("relatos.txt", "r");
    if (!f) return;

    //Lê o arquivo linha por linha
    while (fscanf(f, "%59[^;];%59[^;];%19[^;];%299[^;];%f;%f\n",
                  relatos[*quantidade].nome,
                  relatos[*quantidade].email,
                  relatos[*quantidade].telefone,
                  relatos[*quantidade].descricao,
                  &relatos[*quantidade].latitude,
                  &relatos[*quantidade].longitude) == 6) {
        (*quantidade)++;
    }
    fclose(f);
}

//Função principal com menu de opções
int main() {
    Relato relatos[MAX_RELATOS];
    int quantidade = 0;
    int opcao;
    float lat, lon;

    //Carrega relatos salvos anteriormente
    CarregarRelatos(relatos, &quantidade);

    do {
        //Menu principal
        printf("\n1. Cadastrar Relato\n2. Listar Relatos\n3. Consultar por Raio\n4. Ordenar por Nome\n5. Sair\nEscolha: ");
        scanf("%d", &opcao);
        getchar();  // Limpar o buffer do teclado
        
        //Escolha da opção
        switch (opcao) {
            case 1:
                CadastrarRelato(relatos, &quantidade);
                break;
            case 2:
                ListarRelatos(relatos, quantidade);
                break;
            case 3:
                printf("Informe sua latitude: ");
                scanf("%f", &lat);
                printf("Informe sua longitude: ");
                scanf("%f", &lon);
                getchar();
                ConsultarPorRaio(relatos, quantidade, lat, lon);
                break;
            case 4:
                OrdenarRelatosPorNome(relatos, quantidade);
                break;
            case 5:
                SalvarRelatos(relatos, quantidade);
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 5); //Continua até quando o usuário escolher sair

    return 0;
}
