//Autor   : Guilherme Rodrigues Costa(211039448)
/*Objetivo: Criar um programa para reserva de viagens espaciais
            O programa deve ser capaz de:
                1) Cadastrar voo
                2) Cadastrar reserva
                3) Consultar voo
                4) Consultar reserva
                5) Consultar passageiro
                6) Cancelar voo
                7) Cancelar reserva
                8) Excluir voo
                9) Sair do programa*/

/*Dados dos Voos: número do voo quantidade 
                  máxima de passageiros; 
                  data e hora de partida; 
                  status do voo (ativo ou cancelado). */

/*Dados das Reservas: número da reserva;
                      número do voo;
                      CPF do passageiro; nome completo do passageiro; sexo, data de nascimento;
                      status da reserva (ativa ou cancelada).*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   /*permite a manipulação de caracteres*/
#include <locale.h>  /*permite a utilização de acentuação e "ç"*/
#include <stdbool.h> /*Inclui a variavel logica*/
#include <time.h>    /*Permite pegar a data atual do computador*/
#include <string.h>

#define TAM_MAX 100

//Registros
struct DadosVoo
{
    char numeroV[TAM_MAX];
    int nPassageiros;
    int dia, mes, ano, hora, min;
    char statusVoo;
};

struct DadosReserva
{
    char numeroR[10];
    char numeroV[10];
    char cpf[16];
    char nome[1000];
    char sexo;
    int diaN, mesN, anoN;
    char statusReserva;
};

struct DadosPassageiro
{
    char cpf[16];
    char nome[TAM_MAX];
    char sexo;
    int diaN, mesN, anoN; /*Contem o dia mes e ano*/
    int nReservas;
    char numeroR[TAM_MAX][TAM_MAX];
    char numeroV[TAM_MAX][TAM_MAX];
    char statusReserva[TAM_MAX];
};

//Funções

bool vooPossivel(char codV[])
{
    //Confere se o codigo do voo esta dentro dos padrões
    int tamanhoCod;

    tamanhoCod = strlen(codV);

    if (tamanhoCod != 8)
    {
        //Codigo menor/maior doq deveria
        return false;
    }
    else if (codV[0] != 'J' || codV[1] != 'E' || codV[2] != 'B' || codV[3] != '-' || (isdigit(codV[4])) == 0 || (isdigit(codV[5])) == 0 || (isdigit(codV[6])) == 0 || (isdigit(codV[7])) == 0)
    {
        //Codigo fora do padrão pre estabelecido(JEB-XXXX)
        return false;
    }
    else
    {
        return true;
    }
}
bool vooExiste(FILE *arqVoo, struct DadosVoo *voo, int nV)
{
    //Retorna verdadeiro caso encontre o voo e falso caso nao encontre
    int i;

    for (i = 0; i < nV; i++)
    {
        if (strcmp(voo[nV].numeroV, voo[i].numeroV) == 0) //confere se o codigo do voo ja foi informado anteriormente
        {
            return true;
        }
    }

    return false;
}
bool vooExisteRes(struct DadosVoo *voo, char codVoo[], int nV)
{
    int i = 0;
    //Confere se o Voo existe no arquivo e se ele ainda consta como ativo

    for (i = 0; i < nV; i++)
    {
        if (strcmp(codVoo, voo[i].numeroV) == 0 && voo[i].statusVoo == 'A')
        {
            return true;
        }
    }
    return false;
}
bool validaNascimento(int dia, int mes, int ano)
{
    //Confere se data de nascimento existe

    //seta o tempo do computado pro formato DD/MM/AAAA
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    if (mes > 12 || mes < 1)
    {
        return false;
    }
    else if (dia > 31 || dia < 1)
    {
        return false;
    }
    else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) //Meses com no max 30 dias
    {
        return false;
    }
    else if (ano <= 1900)
    {
        return false;
    }
    else
    {                       /*Analise de casos de ano bissexto*/
        if (ano % 100 == 0) /*Bissexto multiplo de 100(caso especial)*/
        {
            if (ano % 400 == 0)
            {
                if (mes == 2 && dia > 29)
                {
                    return false;
                }
                else
                {
                    if (ano > (tm.tm_year + 1900)) //Ano maior que o atual
                    {
                        return false;
                    }
                    else if (ano == (tm.tm_year + 1900)) //ano igual ao atual
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            else /*Bissexto Padrão*/
            {
                if (mes == 2 && dia > 28)
                {
                    return false;
                }
                else if (ano > (tm.tm_year + 1900)) //Ano maior que o atual
                {
                    return false;
                }
                else if (ano == (tm.tm_year + 1900)) //ano igual ao atual
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
        else
        {
            if (ano % 4 == 0)
            {
                if (mes == 2 && dia > 29)
                {
                    return false;
                }
                else if (ano > (tm.tm_year + 1900)) //Ano maior que o atual
                {
                    return false;
                }
                else if (ano == (tm.tm_year + 1900)) //ano igual ao atual
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            else
            {
                if (mes == 2 && dia > 28)
                {
                    return false;
                }
                else if (ano > (tm.tm_year + 1900)) //Ano maior que o atual
                {
                    return false;
                }
                else if (ano == (tm.tm_year + 1900)) //ano igual ao atual
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
        }
    }
}
bool validaData(struct DadosVoo *voo, int nV)
{
    //Condições para que a data possa ser cadastrada

    //seta o tempo do computado pro formato DD/MM/AAAA
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    if (voo[nV].min < 0 || voo[nV].min > 59)
    {
        return false;
    }
    else if (voo[nV].hora < 0 || voo[nV].hora > 23)
    {
        return false;
    }
    else if (voo[nV].mes > 12 || voo[nV].mes < 1)
    {
        return false;
    }
    else if (voo[nV].dia > 31 || voo[nV].dia < 1)
    {
        return false;
    }
    else if ((voo[nV].mes == 4 || voo[nV].mes == 6 || voo[nV].mes == 9 || voo[nV].mes == 11) && voo[nV].dia > 30) //Meses com no max 30 dias
    {
        return false;
    }
    else
    { /*Analise de casos de ano bissexto*/
        if (voo[nV].ano % 100 == 0)
        {
            if (voo[nV].ano % 400 == 0)
            {
                if (voo[nV].mes == 2 && voo[nV].dia > 29)
                {
                    return false;
                }
                else
                {
                    if (voo[nV].ano < (tm.tm_year + 1900)) //Ano menor que o atual
                    {
                        return false;
                    }
                    else if (voo[nV].ano == (tm.tm_year + 1900)) //Ano igual o atual
                    {
                        if (voo[nV].mes < (tm.tm_mon + 1)) //Mes menor que o atual no msm ano
                        {
                            return false;
                        }
                        else if (voo[nV].mes == (tm.tm_mon + 1) && voo[nV].dia <= tm.tm_mday) //dia menor que o dia atual no mesmo mes e ano
                        {
                            return false;
                        }
                        else
                        {
                            return true;
                        }
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (voo[nV].mes == 2 && voo[nV].dia > 28)
                {
                    return false;
                }
                else
                {
                    if (voo[nV].ano < (tm.tm_year + 1900)) //Analise se a data é menor q a atual
                    {
                        return false;
                    }
                    else if (voo[nV].ano == (tm.tm_year + 1900))
                    {
                        if (voo[nV].mes <= (tm.tm_mon + 1))
                        {
                            return false;
                        }
                        else if (voo[nV].mes == (tm.tm_mon + 1) && voo[nV].dia <= tm.tm_mday)
                        {
                            return false;
                        }
                        else
                        {
                            return true;
                        }
                    }
                    else
                    {
                        return true;
                    }
                }
            }
        }
        else
        {
            if (voo[nV].ano % 4 == 0)
            {
                if (voo[nV].mes == 2 && voo[nV].dia > 29)
                {
                    return false;
                }
                else
                {
                    if (voo[nV].ano < (tm.tm_year + 1900)) //Analise se a data é menor q a atual
                    {
                        return false;
                    }
                    else if (voo[nV].ano == (tm.tm_year + 1900))
                    {
                        if (voo[nV].mes <= (tm.tm_mon + 1))
                        {
                            return false;
                        }
                        else if (voo[nV].mes == (tm.tm_mon + 1) && voo[nV].dia <= tm.tm_mday)
                        {
                            return false;
                        }
                        else
                        {
                            return true;
                        }
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            else
            {
                if (voo[nV].mes == 2 && voo[nV].dia > 28)
                {
                    return false;
                }
                else
                {
                    if (voo[nV].ano < (tm.tm_year + 1900)) //Analise se a data é menor q a atual
                    {
                        return false;
                    }
                    else if (voo[nV].ano == (tm.tm_year + 1900))
                    {
                        if (voo[nV].mes < (tm.tm_mon + 1))
                        {
                            return false;
                        }
                        else if (voo[nV].mes == (tm.tm_mon + 1) && voo[nV].dia <= tm.tm_mday)
                        {
                            return false;
                        }
                        else
                        {
                            return true;
                        }
                    }
                    else
                    {
                        return true;
                    }
                }
            }
        }
    }
}
bool reservaExist(FILE *arqReserva, struct DadosReserva *res, int nR)
{   //confere se o codigo da Reserva já foi informada anteriormente e retorna verdadeiro caso encontre
    int i;

    for (i = 0; i < nR; i++)
    {
        if (strcmp(res[nR].numeroR, res[i].numeroR) == 0) 
        {
            return true;
        }
    }
    return false;
}
bool reservaPossivel(char codR[])
{
    //Confere se o codigo da reserva informado está dentro dos padrões pre estabelecidos
    int tamanhoCod;

    tamanhoCod = strlen(codR);

    if (tamanhoCod != 9)
    {
        //Código menor doq deveria
        return false;
    }
    else if (codR[0] != 'G' || codR[1] != 'B' || codR[2] != '-' || (isdigit(codR[3])) == 0 || (isdigit(codR[4])) == 0 || (isdigit(codR[5])) == 0 || (isdigit(codR[6])) == 0 || (isdigit(codR[7])) == 0 || (isdigit(codR[8])) == 0)
    {
        //Código fora do padrão pre estabelecido(GB-XXXXXX)
        return false;
    }
    else
    {
        return true;
    }
}
bool validaCPF(char cpf[])
{   //Realiza o calculo de validacao do CPF informado e retorna verdadeiro caso seja validado 

    int tamanhocpf, i, j = 0;
    int cpfNumeros[12], penultDig = 0, ultDig = 0;
    //calculo dos  ultimos 2 digitos do cpf

    tamanhocpf = strlen(cpf);
    if (tamanhocpf != 14) //Confere o tamnho do CPF digitado
    {
        return false;
    }
    else if (isdigit(cpf[0]) == 0 || isdigit(cpf[1]) == 0 || isdigit(cpf[2]) == 0 || cpf[3] != '.' || isdigit(cpf[4]) == 0 || isdigit(cpf[5]) == 0 || isdigit(cpf[6]) == 0 || cpf[7] != '.' || isdigit(cpf[8]) == 0 || isdigit(cpf[9]) == 0 || isdigit(cpf[10]) == 0 || cpf[11] != '-' || isdigit(cpf[12]) == 0 || isdigit(cpf[13]) == 0)
    {
        return false;
    }
    else
    {
        //Converção dos digitos pra inteiros
        for (i = 0; i < 14; i++)
        {
            if (i != 3 && i != 7 && i != 11)
            {
                cpfNumeros[j] = cpf[i] - '0'; //converte o caracteres do CPF para números inteiros
                j = j + 1;                    //Posiciona o vetor na próxima casa
            }
        }
        //Em Caso de todos os números do CPF serem iguais
        if (cpfNumeros[0] == cpfNumeros[1] && cpfNumeros[1] == cpfNumeros[2] && cpfNumeros[2] == cpfNumeros[3] && cpfNumeros[3] == cpfNumeros[4] && cpfNumeros[4] == cpfNumeros[5] && cpfNumeros[5] == cpfNumeros[6] && cpfNumeros[6] == cpfNumeros[7] && cpfNumeros[7] == cpfNumeros[8] && cpfNumeros[8] == cpfNumeros[9] && cpfNumeros[9] == cpfNumeros[10])
        {
            return false;
        }
        //Calculo  de validação do penultimo digito do CPF
        for (i = 0; i < 9; i++)
        {
            penultDig = penultDig + cpfNumeros[i] * (10 - i);
        }

        penultDig = (penultDig * 10) % 11; //Resto da divisão po resultado por 11

        if (penultDig == 10)
        {
            penultDig = 0;
        }
        if (penultDig != cpfNumeros[9])
        {
            return false;
        }
        else
        {
            //Calculo do ultimo digito do CPF
            for (i = 0; i < 10; i++)
            {
                ultDig = ultDig + cpfNumeros[i] * (11 - i);
            }

            ultDig = (ultDig * 10) % 11; //Resto da divisão po resultado por 11

            if (ultDig == 10)
            {
                ultDig = 0;
            }
            if (ultDig == cpfNumeros[10])
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}
float assentosOcupados(char nVoo[], FILE *arqReserva)//retorna o numero de assentos ocupados 
{   

    char linha1[TAM_MAX];
    float ocupados = 0;
    int nR = 0, i;
    struct DadosReserva res[TAM_MAX];
    rewind(arqReserva);

    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {

        fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }
    nR = nR - 1; //Correção do ponteiro do vetor pra próxima casa a ser ocupado

    for (i = 0; i < nR; i++)
    {
        if (strcmp(res[i].numeroV, nVoo) == 0 && (res[i].statusReserva == 'A')) //Compara se o numero de voo bate com o numero do voo cadastrado na reserva e se a reserva está ativa
        {
            ocupados = ocupados + 1;
        }
    }

    return ocupados;
}
bool reservaANoVoo(struct DadosReserva *res, int nR)
{
    //Confere se o CPF informado já possui alguma reserva Ativa no Voo Informado
    int i;
    for (i = 0; i < nR; i++)
    {
        if (strcmp(res[nR].numeroV, res[i].numeroV) == 0 && strcmp(res[nR].cpf, res[i].cpf) == 0 && res[i].statusReserva == 'A') //Confere se os números de Voos e os CPFs são iguais e se a reserva está ativa
        {
            return true;
        }
    }
    return false;
}
bool vooPartiu(struct DadosVoo *voo, char codVoo[], int nV)
{ /*Confere se o voo informado já partiu e retorna verdadeiro em caso ja tenha partido*/

    int i = 0, n;

    //seta o tempo do computado pro formato DD/MM/AAAA
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);

    for (i = 0; i < nV; i++)
    {
        if (strcmp(codVoo, voo[i].numeroV) == 0) //confere se as strings são iguais
        {
            n = i;
        }
    }

    if (voo[n].ano < (tm.tm_year + 1900)) //Ano menor que o atual
    {
        return true;
    }
    else if (voo[n].ano == (tm.tm_year + 1900)) //Ano igual o atual
    {
        if (voo[n].mes < (tm.tm_mon + 1)) //Mes menor que o atual no msm ano
        {
            return true;
        }
        else if (voo[n].mes == (tm.tm_mon + 1) && voo[n].dia <= tm.tm_mday) //dia menor que o dia atual no mesmo mes e ano
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
bool lotacaoMax(char codVoo[], struct DadosReserva *res, int nR, int nV, struct DadosVoo *voo)
{   
    //confere se o Voo ja atingiu sua lotacao maxima
    int i, ocupados = 0, p;

    for (i = 0; i < nV; i++)
    {
        if (strcmp(codVoo, voo[i].numeroV) == 0) //Procura a posição do vetor em que está armazenado o numero do voo informado
        {
            p = i;
        }
    }

    for (i = 0; i < nR; i++)
    {
        if (strcmp(res[i].numeroV, codVoo) == 0 && (res[i].statusReserva == 'A')) //Confere 
        {
            ocupados = ocupados + 1; 
        }
    }
    if (ocupados >= voo[p].nPassageiros)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void dadosVoo(char codVoo[], FILE *arqVoo)
{   //printa os dados do voo na tela do usuario

    char linha1[TAM_MAX];
    int nV = 0, i = 0, n;
    struct DadosVoo voo[TAM_MAX];

    rewind(arqVoo);

    //Instruções
    fgets(linha1, TAM_MAX, arqVoo);                                                                                                                                                  //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;
    while (!feof(arqVoo)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1; //Posicionamento do vetor na proxima casa
    }
    nV = nV - 1; //Correção da casa do vetor

    for (i = 0; i < nV; i++)
    {
        if (strcmp(codVoo, voo[i].numeroV) == 0) //confere se as strings são iguais
        {
            n = i;
        }
    }

    //Dados do voo
    printf("Dados do Voo: \n");
    printf("  Numero do Voo: %s\n", voo[n].numeroV);
    printf("  Horario de Partida: %02d/%02d/%04d %02d:%02d\n", voo[n].dia, voo[n].mes, voo[n].ano, voo[n].hora, voo[n].min);
    if (voo[n].statusVoo == 'A') //Confere se o voo se encontra ativo
    {
        printf("  VOO ATIVO\n");
    }
    else
    {
        printf("  VOO CANCELADO\n");
    }
    
}
bool cpfCadastrado(char cpf[], struct DadosReserva *res, int nR)
{
    int i;
    //Confere se o mesmo CPF aparece em algum momento dentro do Arquivo
    for (i = 0; i < nR; i++)
    {
        if (strcmp(cpf, res[i].cpf) == 0)
        {
            return true;
        }
    }

    return false;
}
int encontraPass(char cpf[], struct DadosReserva *res, int nR)
{
    //Procura a posição de um vetor que possui o msm cpf informado e a retorna
    int i;

    for (i = 0; i < nR; i++)
    {
        if (strcmp(cpf, res[i].cpf) == 0)
        {
            return i;
        }
    }

    return -1;
}

//Interfaces dos proceedimentos
//Printa a interface da opção escolhida no menu
void interfaceCadVoo()
{

    system("cls || clean");
    printf("=================================================\n");
    printf("                  CADASTRO DE VOO               \n");
    printf("=================================================\n");
    printf("Informe os dados do VOO:\n");
    printf("-------------------------------------------------\n");
}
void interfaceCadRes()
{

    system("cls || clean");
    printf("=================================================\n");
    printf("                CADASTRO DA RESERVA               \n");
    printf("=================================================\n");
    printf("Informe os dados da Reserva:\n");
    printf("-------------------------------------------------\n");
}

//Procedimentos

//Confere se os arquivo txt possuem cabeçalho e caso n encontre escreve o cabeçalho no arquivo
void procuraCabecalho(){
    FILE *arqVoo, *arqReserva;
    char linha1[TAM_MAX];

    //Abrir arquivos
    arqVoo = fopen("Voos.txt", "a+t"); //abre ele de forma a ler/escrever ou criar o arquivo caso não exista
    //abre ele de forma a ler/escrever ou criar o arquivo caso não exista
    arqReserva = fopen("Reservas.txt", "a+t"); //abre ele de forma a ler/escrever ou criar o arquivo caso não exista

    if (arqVoo == NULL || arqReserva == NULL)
    { //Se não for possivel abrir o arquivo

        system("cls || clean");
        printf("Nao foi possivel abrir seus arquivos\n");
        printf("Fechando o Programa\n");
        system("pause");
        exit(0);
    }

    fgets(linha1, TAM_MAX, arqVoo); //Leitura da primeira linha do arquivo

    rewind(arqVoo);
    if (strcmp(linha1, "JEB-XXXX,NPass,DD/MM/AAAA,hh:mm,Status[A/C]\n") != 0) //Confere se a primeira do arquivo é igual ao cabeçalho do arquivo
    {
        fprintf(arqVoo, "JEB-XXXX,NPass,DD/MM/AAAA,hh:mm,Status[A/C]\n");
        printf("Cabecalho do voo nao encontrado\n");
        system("pause");
    }

    fgets(linha1, TAM_MAX, arqReserva); //Leitura da primeira linha do arquivo

    rewind(arqReserva);
    if (strcmp(linha1, "GB-XXXXXX,JEB-XXXX,CPF,nome,sexo,DataDeNascimento,StatusReserva\n") != 0) //Confere se a primeira do arquivo é igual ao cabeçalho do arquivo
    {
        fprintf(arqReserva, "GB-XXXXXX,JEB-XXXX,CPF,nome,sexo,DataDeNascimento,StatusReserva\n");
        printf("Cabecalho da reserva nao encontrado\n");
        system("pause");
    }

    //Fechando os arquivos
    fclose(arqVoo);
    fclose(arqReserva);
}

void menu()
{
    printf("================================================\n");
    printf("                  GALATIC BIRDS          \n");
    printf("                 VIAGENS ESPACIAIS        \n");
    printf("================================================\n");
    printf("Informe o numero correspondente a sua opcao: \n");
    printf("\n");
    printf("CADASTRAR VOO             [1] \n");
    printf("CADASTRAR RESERVA         [2] \n");
    printf("CONSULTAR VOO             [3] \n");
    printf("CONSULTAR RESERVA         [4] \n");
    printf("CONSULTAR PASSAGEIRO      [5] \n");
    printf("CANCELAR VOO              [6] \n");
    printf("CANCELAR RESERVA          [7] \n");
    printf("EXCLUIR VOO               [8] \n");
    printf("SAIR DO PROGRAMA          [9] \n");
    printf("------------------------------------------------\n");
}

void cadastra_voo(FILE *arqVoo)
{
    //Declarações
    char linha1[TAM_MAX];
    int nV = 0, i = 0;
    bool possivelContinuar = true, vooExistente = false;
    struct DadosVoo voo[TAM_MAX];
    rewind(arqVoo);

    //Instruções
    fgets(linha1, TAM_MAX, arqVoo);                                                                                                                                                  //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;
    while (!feof(arqVoo)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1; //Posicionamento do vetor na proxima casa
    }
    nV = nV - 1; //Correção da casa do vetor
    system("cls || clean");

    do
    {
        interfaceCadVoo();
        //Entradas de Dados
        printf("Informe o numero do Voo: [JEB-XXXX]  ");
        scanf(" %s", voo[nV].numeroV);

        possivelContinuar = vooPossivel(voo[nV].numeroV);
        vooExistente = vooExiste(arqVoo, voo, nV);
        if (possivelContinuar == false) /*confere se o codigo do voo está no padrão estabelecido*/
        {
            printf("codigo informado fora do padrão JEB-XXXX \n");
            system("pause");
        }
        else if (vooExistente == true)
        { /*Confere se o voo já foi cadastrado anteriormente*/
            printf("Voo informado ja cadastrado anteriormente \n");
            system("pause");
            possivelContinuar = false;
        }
        if (possivelContinuar == true)
        {
            printf("Numero de Voo cadastrado com sucesso\n");
            system("pause");
        }

    } while (possivelContinuar == false);

    possivelContinuar = false;

    do
    {
        printf("Informe o Numero total de passageiros(Apenas numeros): ");
        fflush(stdin);
        scanf(" %d", &voo[nV].nPassageiros);

        if (voo[nV].nPassageiros <= 0)
        {
            printf("Numero de passageiros precisa pertencer ao conjunto dos numeros Naturais \n");
            system("pause");
            interfaceCadVoo();
        }

    } while (voo[nV].nPassageiros <= 0);

    do
    {
        printf("Informe a data e hora do Voo [DD/MM/AA hh:mm]: ");
        fflush(stdin);
        scanf(" %d/%d/%d %d:%d", &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min);

        possivelContinuar = validaData(voo, nV);

        if (possivelContinuar == false)
        {
            printf("Data ou hora informada sao invalidas \n");
            printf(" Por Favor Tente outra data \n");
            system("pause");
            interfaceCadVoo();
        }
        else
        {
            printf("Data cadastrada com sucesso\n");
            system("pause");
        }

    } while (possivelContinuar == false);

    fprintf(arqVoo, "%s,%d,%02d/%02d/%04d,%02d:%02d,A\n", voo[nV].numeroV, voo[nV].nPassageiros, voo[nV].dia, voo[nV].mes, voo[nV].ano, voo[nV].hora, voo[nV].min); //Arquivação dos dados do Voo

    if (!ferror(arqVoo)) //Confere se ocorreu algum erro durante a escrita no arquivo
    {
        printf("VOO ARQUIVADO COM SUCESSO\n");
        printf("Dados casdastrados: %s,%d,%02d/%02d/%04d,%02d:%02d,A\n", voo[nV].numeroV, voo[nV].nPassageiros, voo[nV].dia, voo[nV].mes, voo[nV].ano, voo[nV].hora, voo[nV].min);
        system("pause");
    }
    else
    {
        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
        printf("FECHANDO O PROGRAMA\n");
        fclose(arqVoo);
        exit(0);
    }
}

void casdastra_reserva(FILE *arqReserva, FILE *arqVoo)
{
    //Declarações
    char linha1[TAM_MAX], confNome = 'g';
    int statusV;
    int nR = 0, i = 0, nV = 0, n = -1;
    bool possivelContinuar = true, vooExistente = false, reservaExistente = false, vooSaiu = true, maxPassageiros, cpfEncontrado;
    struct DadosReserva res[TAM_MAX];
    struct DadosVoo voo[TAM_MAX];

    rewind(arqVoo);
    rewind(arqReserva);

    system("cls || clean");
    //Extração dos DaDos no arquivo Reservas
    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
    fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
    nR = nR + 1;              //Posicionamento do vetor na proxima casa
    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }
    nR = nR - 1; //Correção da casa do vetor

    //Extração dos dados no arquivo Voos
    fgets(linha1, TAM_MAX, arqVoo);                                                                                                                                                  //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;
    while (!feof(arqVoo)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1;
    }

    nV = nV - 1;

    do
    {
        do
        {
            interfaceCadRes();
            system("cls || clean");
            printf("Informe os dados do Reserva:\n");
            printf("Informe o numero do Voo: [JEB-XXXX]  ");
            scanf(" %s", res[nR].numeroV);
            possivelContinuar = vooPossivel(res[nR].numeroV);

            if (possivelContinuar == false) /*confere se o codigo do voo está no padrão estabelecido*/
            {
                printf("codigo informado fora do padrão JEB-XXXX \n");
                system("pause");
            }
            else
            {
                vooExistente = vooExisteRes(voo, res[nR].numeroV, nV); //Confere se o voo informado foi cadastrado

                if (vooExistente == true)
                {
                    vooSaiu = vooPartiu(voo, res[nR].numeroV, nV);

                    maxPassageiros = lotacaoMax(res[nR].numeroV, res, nR, nV, voo);
                    if (vooSaiu == true)
                    {
                        printf("Voo Informado ja partiu\n");
                        system("pause");
                        possivelContinuar = false;
                    }
                    else if (maxPassageiros == true)
                    {
                        printf("Voo Informado ja esta em sua lotacao maxima\n");
                        system("pause");
                        possivelContinuar = false;
                    }
                    else
                    {
                        printf("Numero de Voo cadastrado com sucesso\n");
                        system("pause");
                        possivelContinuar = true;
                    }
                }
                else
                {
                    printf("Nao existe voo ativo no sistema com esse numero\n");
                    system("pause");
                    possivelContinuar = false;
                }
            }
        } while (possivelContinuar == false);

        possivelContinuar = false;

        do
        {
            printf("Informe o numero da Reserva[GB-XXXXXX]: ");
            scanf(" %s", res[nR].numeroR);

            possivelContinuar = reservaPossivel(res[nR].numeroR);

            reservaExistente = reservaExist(arqReserva, res, nR);

            if (possivelContinuar == false) /*confere se o codigo do voo está no padrão estabelecido*/
            {
                printf("codigo informado fora do padrão GB-XXXXXX \n");
                system("pause");
                interfaceCadRes();
            }
            else if (reservaExistente == true)
            { /*Confere se o voo já foi cadastrado anteriormente*/
                printf("Reserva informada ja cadastrada anteriormente \n");
                system("pause");
                interfaceCadRes();
                possivelContinuar = false;
            }
            if (possivelContinuar == true)
            {
                printf("Numero da Reserva cadastrada com sucesso\n");
                system("pause");
            }

        } while (possivelContinuar == false);
        do
        {
            do
            {
                printf("Informe o CPF do Passageiro [###.###.###-##]: ");
                scanf(" %s", res[nR].cpf);

                possivelContinuar = validaCPF(res[nR].cpf); //Confere se o CPF digitado é válido

                if (possivelContinuar == false)
                {
                    printf("CPF  INVALIDO\n");
                    system("pause");
                    interfaceCadRes();
                }
                else
                {
                    printf("CPF VALIDO \n");
                }

            } while (possivelContinuar == false);

            possivelContinuar = !(reservaANoVoo(res, nR)); //Confere se o passageiro já existe uma reserva ativa nesse voo

            if (possivelContinuar == false) //Em caso de existir reserva ativo no mesmo voo
            {
                printf("Passageiro de CPF: %s já possui uma reserva ativa no VOO %s\n", res[nR].cpf, res[nR].numeroV);
                system("pause");
                interfaceCadRes();
            }
            else
            {

                cpfEncontrado = cpfCadastrado(res[nR].cpf, res, nR);

                if (cpfEncontrado == true)
                {
                    printf("Passageiro com o msm cpf ja cadastrado\n");
                    n = encontraPass(res[nR].cpf, res, nR);

                    if (n < 0) //Em caso de erro na função
                    {
                        printf("Erro ao encontrar o passageiro \n");
                        printf("Fechando o programa \n");
                        fclose(arqReserva);
                        fclose(arqVoo);
                        exit(0);
                    }
                    else
                    { //Em caso de encontrar o passageiro

                        do
                        {
                            printf("Nome: %s\n", res[n].nome);
                            printf("Este eh voce ? [N/S]\n");
                            scanf(" %c", &confNome);
                            confNome = toupper(confNome);

                            if (confNome != 'S' && confNome != 'N')
                            {
                                printf("Opcao invalida \n");
                                system("pause");
                            }

                        } while (confNome != 'S' && confNome != 'N');

                        if (confNome == 'S')
                        {
                            //Atribui os dados do passageiro encontrado ao passageiro atual
                            strcpy(res[nR].nome, res[n].nome);
                            res[nR].sexo = res[n].sexo;
                            res[nR].diaN = res[n].diaN;
                            res[nR].mesN = res[n].mesN;
                            res[nR].anoN = res[n].anoN;
                            possivelContinuar == true;
                        }
                        else
                        {
                            printf("Por favor informe seu CPF\n");
                            system("pause");
                            possivelContinuar == false;
                        }
                    }
                }
                else
                {
                    printf("Informe o nome completo do cliente [apenas letras]:  \n");
                    scanf(" %[^\n]s", res[nR].nome);

                    res[nR].nome[0] = toupper(res[nR].nome[0]); //Primeira letra de Nome próprio sempre maiuscula
                }
            }
        } while (confNome == 'N'); //Em caso de do usuário informar que o nome não é o mesmo do dele

    } while (possivelContinuar == false); //em caso de problemas com CPF

    if (cpfEncontrado == false)
    {
        do
        {
            printf("Informe o sexo do cliente [M/F]:  \n");
            fflush(stdin);
            scanf(" %c", &res[nR].sexo);
            res[nR].sexo = toupper(res[nR].sexo);

            if (res[nR].sexo != 'F' && res[nR].sexo != 'M')
            {
                printf("Opcao invalida \n");
                system("pause");
                interfaceCadRes();
            }
            else
            {
                printf("Sexo cadastrado com sucesso\n");
            }

        } while (res[nR].sexo != 'F' && res[nR].sexo != 'M');

        do
        {
            printf("Informe a data de nascimento do cliente [DD/MM/AAAA]:  \n");
            fflush(stdin);
            scanf(" %d/%d/%d", &res[nR].diaN, &res[nR].mesN, &res[nR].anoN);

            possivelContinuar = validaNascimento(res[nR].diaN, res[nR].mesN, res[nR].anoN);

            if (possivelContinuar == false)
            {
                printf("Data invalida \n");
                system("pause");
                interfaceCadRes();
            }
            else
            {
                printf("Data de Nascimento cadastrada com sucesso\n");
                system("pause");
            }

        } while (possivelContinuar == false);
    }
    fprintf(arqReserva, "%s,%s,%s,%s,%c,%d/%d/%d,A\n", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, res[nR].sexo, res[nR].diaN, res[nR].mesN, res[nR].anoN); //Arquivamento dos dados da Reserva

    if (!ferror(arqReserva)) //Confere se ocorreu algum erro durante a escrita no arquivo eretorna uma resposta ao usuario
    {
        printf("RESERVA ARQUIVADA COM SUCESSO\n");
        printf("Dados casdastrados: %s,%s,%s,%s,%c,%02d/%02d/%04d,A\n", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, res[nR].sexo, res[nR].diaN, res[nR].mesN, res[nR].anoN);
        system("pause");
    }
    else
    {
        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
        printf("FECHANDO O PROGRAMA\n");
        fclose(arqVoo);
        fclose(arqReserva);
        exit(0);
    }
}

void consulta_voo(FILE *arqVoo, FILE *arqReserva)
{
    //Declarações
    char linha1[TAM_MAX], continua;
    float assentosOcup;
    float percentOcup;
    int nV = 0, i = 0, n, nR = 0;
    struct DadosVoo voo[TAM_MAX];
    struct DadosReserva res[TAM_MAX];

    rewind(arqVoo);
    rewind(arqReserva);
    //Instruções
    fgets(linha1, TAM_MAX, arqVoo); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;                                                                                                                                                                     //Posicionamento do vetor na proxima casa
    while (!feof(arqVoo))                                                                                                                                                            //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1; //Posicionamento do vetor na proxima casa
    }

    nV = nV - 2; //Coreção da casa do vetor

    //Extração dos DaDos no arquivo Reservas
    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
    nR = nR + 1; //Posicionamento do vetor na proxima casa

    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }
    nR = nR - 1; //Correção da casa do vetor

    //Menu
    if (nV == -1) //Confere se o arquivo possui cadastros
    {
        printf("Nenhum voo encontrado\n");
    }else
    {
        nV++;
        do
        {
            system("cls || clean");
            printf("=============================================\n");
            printf("               CONSULTA VOO\n");
            printf("=============================================\n");
            printf("              VOOs CADASTRADOS\n");

            for (i = 0; i < nV; i++)
            {
                printf("%s [%d]\n", voo[i].numeroV, i);
            }

            printf("---------------------------------------------\n");
            printf("Informe a opcao do voo que deseja consultar: \n");
            fflush(stdin);
            scanf(" %d", &n);

            if (n < 0 || n >= nV) //Caso o usuário informe uma opção que não está no menu
            {
                printf("OPCAO INVALIDA\n");
                system("pause");
            }
        } while (n < 0 || n >= nV); //Caso o usuário informe uma opção que não está no menu o programa retorna ao menu

        system("cls || clean");

        assentosOcup = assentosOcupados(voo[n].numeroV, arqReserva); //retorna o numero de assentos ocupados 
        percentOcup = (float)assentosOcup * 100 / (float)voo[n].nPassageiros; //Calculo do percentual de ocupação do voo

        //Dados do Voo escolhido
        printf("Numero do Voo:                    %s\n", voo[n].numeroV);
        printf("Horario de Partida:               %02d/%02d/%04d %02d:%02d\n", voo[n].dia, voo[n].mes, voo[n].ano, voo[n].hora, voo[n].min);
        printf("Total de assentos:                %d\n", voo[n].nPassageiros);
        printf("Percentual de assentos ocupoados: %.3f%%\n", percentOcup);
        printf("--------------------------------------------------------\n");
        if (voo[n].statusVoo == 'A')
        {
            printf("VOO ATIVO\n");
        }
        else
        {
            printf("VOO CANCELADO\n");
        }

        printf("\n");
        printf("--------------------------------------------------------\n");

        printf("Listas de passageiros Ativos:\n");
        for (i = 0; i < nR; i++)
        {
            if (strcmp(voo[n].numeroV, res[i].numeroV) == 0 && res[i].statusReserva == 'A')
            {
                printf("  Nome:%s   CPF: %s\n", res[i].nome, res[i].cpf);
            }
        }

        printf("\n");
        printf("--------------------------------------------------------\n");

        printf("Listas de passageiros Cancelados:\n");
        for (i = 0; i < nR; i++)
        {
            if (strcmp(voo[n].numeroV, res[i].numeroV) == 0 && res[i].statusReserva == 'C')
            {
                printf("  Nome:%s   CPF: %s\n", res[i].nome, res[i].cpf);
            }
        }
    }
    printf("\n");
    system("pause");
}

void consulta_reserva(FILE *arqReserva, FILE *arqVoo)
{
    char linha1[TAM_MAX];
    int nR = 0, i = 0, n;
    struct DadosReserva res[TAM_MAX];

    rewind(arqReserva);

    //Instruções

    //Extração dos DaDos no arquivo Reservas
    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
    fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
    nR = nR + 1;              //Posicionamento do vetor na proxima casa
    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }
    nR = nR - 2; //Correção da casa do vetor

    if (nR == -1)//Confere se o arquivo possui cadastros
    {
        printf("Nenhuma Reserva encontrada\n");
    }else
    {
        //Menu
        do
        {
            system("cls || clean");
            printf("================================================\n");
            printf("        Reservas Cadastradas\n");

            for (i = 0; i <= nR; i++)
            {
                printf("%s [%d]\n", res[i].numeroR, i);
            }
            printf("------------------------------------------------\n");
            printf("Informe a opcao da Reserva que deseja consultar: \n");
            fflush(stdin);
            scanf(" %d", &n);

            if (n < 0 || n > nR) //Caso o usuário informe uma opção que não está no menu
            {
                printf("OPCAO INVALIDA\n");
                system("pause");
            }
        } while (n < 0 || n > nR);
        system("cls || clean");

        //Dados do Voo escolhido
        printf("----------------------------------------: \n");
        printf("Numero da Reserva:  %s\n", res[n].numeroR);
        printf("Numero do Voo:      %s\n", res[n].numeroV);
        if (res[n].statusReserva == 'A')
        {
            printf("Reserva    ATIVA\n");
        }
        else
        {
            printf("Reserva    Cancelada\n");
        }
        
        printf("\n----------------------------------------: \n");
        dadosVoo(res[n].numeroV, arqVoo); //Imprime os dados do Voo cadastrado na reserva
        printf("\n----------------------------------------: \n");
        printf("Dados do passageiro: \n");
        printf("  Nome: %s\n", res[n].nome);
        printf("  CPF: %s\n", res[n].cpf);
        if (res[n].sexo == 'M') //Sexo considerados pelo sistema, apenas masculino e feminino
        {
            printf("  Sexo: Masculino\n");
        }
        else
        {
            printf("  Sexo: Feminino\n");
        }
        printf("  Data de Nascimento: %02d/%02d/%04d\n", res[n].diaN, res[n].mesN, res[n].anoN);
    }
    printf("\n");
    system("pause");
}

void consulta_passageiro(FILE *arqReserva)
{
    //Declarações
    char linha1[TAM_MAX];
    int statusV;
    int nR = 0, i = 0, nV = 0, j, nP = 0;
    bool cpfCad = false;
    struct DadosReserva res[TAM_MAX];
    struct DadosPassageiro pass;

    pass.nReservas = 0;

    rewind(arqReserva);

    //Extração dos DaDos no arquivo Reservas
    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
    fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
    nR = nR + 1;              //Posicionamento do vetor na proxima casa
    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }
    nR = nR - 1; //Correção da casa do vetor

    system("cls || clean");
    printf("=================================================\n");
    printf("              CONSULTA DE PASSAGEIRO             \n");
    printf("=================================================\n");
    printf("Informe o CPF do passageiro que deseja consultar: \n");
    printf("[ XXX.XXX.XXX-XX ] ");
    scanf(" %[^\n]", pass.cpf);

    
    cpfCad = cpfCadastrado(pass.cpf, res, nR); //Confere se o cpf informado já esta no arquivo de reservas

    if (cpfCad == false)
    {
        printf("CPF nao cadastrado no sistema\n");
        system("pause");
    }
    else
    {
        //Captura dos dados do passageiro
        for (i = 0; i < nR; i++)
        {
            if (strcmp(pass.cpf, res[i].cpf) == 0)
            {
                //atribui os dados das reservas ao CPF do passageiro
                strcpy(pass.nome, res[i].nome);
                strcpy(pass.numeroR[pass.nReservas], res[i].numeroR);
                strcpy(pass.numeroV[pass.nReservas], res[i].numeroV);
                pass.statusReserva[pass.nReservas] = res[i].statusReserva;
                pass.diaN = res[i].diaN;
                pass.mesN = res[i].mesN;
                pass.anoN = res[i].anoN;
                pass.nReservas++;
            }
        }

        //Dados do passageiro
        system("cls || clean");
        printf("=================================================\n");
        printf("              CONSULTA DE PASSAGEIRO             \n");
        printf("=================================================\n");
        printf("Nome:                %s\n", pass.nome);
        printf("Data de nascimento:  %d/%d/%d\n", pass.diaN, pass.mesN, pass.anoN);
        printf("CPF:                 %s\n", pass.cpf);
        printf("-------------------------------------------------\n");
        printf("Lista de Reservas: \n");
        printf("  RESERVA        VOO       STATUS\n");
        printf("\n");
        for (i = 0; i < pass.nReservas; i++)
        {
            printf(" %s     %s   ", pass.numeroR[i], pass.numeroV[i]);
            if (pass.statusReserva[i] == 'A')
            {
                printf("ATIVO\n");
            }
            else
            {
                printf("CANCELADA\n");
            }
        }
        system("pause");
    }

}

void cancela_voo(FILE *arqVoo, FILE *arqReserva)
{
    //Declarações
    char linha1[TAM_MAX], continua;
    bool vooSaiu;
    int nV = 0, i = 0, n, nR = 0;
    struct DadosVoo voo[TAM_MAX];
    struct DadosReserva res[TAM_MAX];

    rewind(arqVoo);
    rewind(arqReserva);

    //Instruções
    fgets(linha1, TAM_MAX, arqVoo); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;                                                                                                                                                                     //Posicionamento do vetor na proxima casa

    while (!feof(arqVoo)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1; //Posicionamento do vetor na proxima casa
    }

    nV = nV - 2;
    if (nV == -1)//Confere se o arquivo possui cadastros
    {
        printf("Nenhum voo encontrado\n");
        system("pause");
    }else
    {
        //Menu
        system("cls || clean");
        printf("=========================================\n");
        printf("           VOOs CADASTRADOS\n");
        printf("-----------------------------------------\n");

        for (i = 0; i < nV; i++)
        {
            if (voo[i].statusVoo == 'A')
            {
                printf("%s    ATIVO    [%d]\n", voo[i].numeroV, i);
            }else
            {
                printf("%s   CANCELADO [%d]\n", voo[i].numeroV, i);
            }
        }

        printf("-----------------------------------------\n");
        printf("Informe o voo que deseja cancelar: \n");
        fflush(stdin);
        scanf(" %d", &n);
        printf("-----------------------------------------\n");

        if (n < 0 || n >= nV )//Confere se a opção escolhida esta entre as informadas
        {
            printf("Opcao inexistente\n");
            system("pause");
            
        }else{
            //nV++;
            vooSaiu = vooPartiu(voo, voo[n].numeroV, nV); //Não é possivel cancelar um voo que já decolou

            if (voo[n].statusVoo != 'A') //Confere se o voo está ativo
            {
                printf("Voo informado ja consta como cancelado\n");
                system("pause");
            }else if (vooSaiu == true)
            {
                printf("Voo informado ja partiu\n");
                system("pause");
            }else
            {
                do
                {
                    printf("Voo %s e todas as suas reservas serao cancelados\n", voo[n].numeroV);
                    printf("Esta decisao NAO PODE SER REVERTIDA\n");
                    printf("Deseja continuar?\n"); //confirmação da desição do Usuário
                    scanf(" %c", &continua);

                    continua = toupper(continua); //Torna a variável em letra maiuscula pra facilitar as operações

                    if (continua != 'S' && continua != 'N')
                    {
                        printf("Opcao invalida \n");
                        system("pause");
                    }

                } while (continua != 'S' && continua != 'N');

                //Extração dos DaDos no arquivo Reservas
                fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis
                fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
                nR = nR + 1;              //Posicionamento do vetor na proxima casa
                while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
                {
                    fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
                    nR = nR + 1; //Posicionamento do vetor na proxima casa
                }
                nR = nR - 1; //Correção da casa do vetor

                if (continua == 'S')
                {
                    fclose(arqVoo);
                    arqVoo = fopen("Voos.txt", "w+t"); //Abre o arquivo Reservas.txt de modo a reescrever-lo por inteiro

                    if (arqVoo == NULL)
                    { //Se não for possivel abrir o arquivo
                        system("cls || clean");
                        printf("Não foi possivel abrir seu arquivo\n");
                        printf("Fechando o Programa\n");
                        system("pause");
                        exit(0);
                    }
                    fprintf(arqVoo, "JEB-XXXX,NPass,DD/MM/AAAA,hh:mm,Status[A/C]\n"); //Escreve o cabeçalho do

                    for (i = 0; i < nV; i++)
                    {
                        if (i != n)
                        {
                            fprintf(arqVoo, "%s,%d,%02d/%02d/%04d,%02d:%02d,%c\n", voo[i].numeroV, voo[i].nPassageiros, voo[i].dia, voo[i].mes, voo[i].ano, voo[i].hora, voo[i].min, voo[i].statusVoo);
                        }else
                        {
                            fprintf(arqVoo, "%s,%d,%02d/%02d/%04d,%02d:%02d,C\n", voo[n].numeroV, voo[n].nPassageiros, voo[n].dia, voo[n].mes, voo[n].ano, voo[n].hora, voo[n].min);
                        }
                    }

                    if (!ferror(arqVoo)) //Confere se ocorreu algum erro durante a escrita no arquivo
                    {
                        printf("VOO CANCELADA COM SUCESSO\n");
                        printf("Dados Alterados p/: %s,%d,%02d/%02d/%04d,%02d:%02d,C\n", voo[n].numeroV, voo[n].nPassageiros, voo[n].dia, voo[n].mes, voo[n].ano, voo[n].hora, voo[n].min);
                        system("pause");
                        fclose(arqVoo);                        //Fecha novamente o arquivo
                        arqVoo = fopen("Reservas.txt", "a+t"); //Reabre ele de forma a ler/escrever ou criar o arquivo caso não exista
                    }else
                    {
                        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
                        printf("FECHANDO O PROGRAMA\n");
                        fclose(arqVoo);
                        exit(0);
                    }

                    fclose(arqReserva);
                    arqReserva = fopen("Reservas.txt", "w+t"); //Abre o arquivo Reservas.txt de modo a reescrever-lo por inteiro

                    if (arqReserva == NULL)
                    { //Se não for possivel abrir o arquivo
                        system("cls || clean");
                        printf("Não foi possivel abrir seu arquivo\n");
                        printf("Fechando o Programa\n");
                        system("pause");
                        exit(0);
                    }

                    fprintf(arqReserva, "GB-XXXXXX,JEB-XXXX,CPF,nome,sexo,DataDeNascimento,StatusReserva\n"); //Escreve o cabeçalho do arquivo

                    for (i = 0; i < nR; i++)
                    {
                        if (strcmp(res[i].numeroV, voo[n].numeroV) == 0) //Confere se a reserva está vinculada ao voo que se deseja excluir
                        {
                            fprintf(arqReserva, "%s,%s,%s,%s,%c,%d/%d/%d,C\n", res[i].numeroR, res[i].numeroV, res[i].cpf, res[i].nome, res[i].sexo, res[i].diaN, res[i].mesN, res[i].anoN);
                        }else
                        {
                            fprintf(arqReserva, "%s,%s,%s,%s,%c,%d/%d/%d,%c\n", res[i].numeroR, res[i].numeroV, res[i].cpf, res[i].nome, res[i].sexo, res[i].diaN, res[i].mesN, res[i].anoN, res[i].statusReserva);
                        }
                    }

                    if (!ferror(arqReserva)) //Confere se ocorreu algum erro durante a escrita no arquivo
                    {
                        printf("RESERVAS CANCELADAS COM SUCESSO\n");
                        system("pause");
                        fclose(arqReserva);                        //Fecha novamente o arquivo
                        arqReserva = fopen("Reservas.txt", "a+t"); //Reabre ele de forma a ler/escrever ou criar o arquivo caso não exista
                    }else
                    {
                        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
                        printf("FECHANDO O PROGRAMA\n");
                        fclose(arqReserva);
                        exit(0);
                    }
                }
            }
        }
    }
}

void cancela_reserva(FILE *arqReserva, FILE *arqVoo)
{
    char linha1[TAM_MAX], continua;
    int nR = 0, i = 0, n, nV = 0;
    bool vooSaiu;
    struct DadosReserva res[TAM_MAX];
    struct DadosVoo voo[TAM_MAX];
    rewind(arqReserva);
    rewind(arqVoo);
    //Instruções

    //Extração dos DaDos no arquivo Reservas
    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
    nR = nR + 1; //Posicionamento do vetor na proxima casa

    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }
    nR = nR - 2; //Correção da casa do vetor

    //Extração dos DaDos no arquivo Voos
    fgets(linha1, TAM_MAX, arqVoo); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;                                                                                                                                                                     //Posicionamento do vetor na proxima casa

    while (!feof(arqVoo)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1; //Posicionamento do vetor na proxima casa
    }
    nV = nV - 1; // correção da casa do Vetor

    //Menu
    if (nR == -1) //Confere se o arquivo possui cadastros
    {
        printf("Nenhuma Reserva encontrada\n");
        system("pause");
    }else
    {
        //Menu
        system("cls || clean");
        printf("==================================================\n");
        printf("            Reservas Cadastradas\n");
        printf("--------------------------------------------------\n");

        for (i = 0; i <= nR; i++)
        {
            if (res[i].statusReserva == 'A')
            {
                printf("Reserva: %s  ATIVA     Voo: %s  [%d]\n", res[i].numeroR, res[i].numeroV, i);
            }else
            {
                printf("Reserva: %s  CANCELADA Voo: %s  [%d]\n", res[i].numeroR, res[i].numeroV, i);
            }
        }

        printf("-------------------------------------------------\n");
        printf("Informe a opcao da Reserva que deseja cancelar: \n");
        scanf(" %d", &n);
        printf("-------------------------------------------------\n");

        if (n <0 || n > nR )//Confere se a opção escolhida esta entre as informadas
        {
            printf("Opcao inexistente\n");
            system("pause");

        }else
        {
            vooSaiu = vooPartiu(voo, res[n].numeroV, nV);

            if (res[n].statusReserva != 'A') //Confere se a reserva ainda está ativa
            {
                printf("A reserva Informada ja consta como cancelada\n");
                system("pause");
                
            }else if (vooSaiu == true)
            {
                printf("Voo no qual a reserva está cadastrada já partiu\n");
                system("pause");

            }else
            {
                do
                {
                    printf("Reserva %s sera cancelada\n", res[n].numeroR);
                    printf("Esta decisao NAO PODE SER REVERTIDA\n"); //confirmação da desição do Usuário
                    printf("Deseja continuar?\n");
                    scanf(" %c", &continua);

                    continua = toupper(continua); //Torna a variável em letra maiuscula pra facilitar as operações

                    if (continua != 'S' && continua != 'N')
                    {
                        printf("Opcao invalida \n");
                        system("pause");
                    }

                } while (continua != 'S' && continua != 'N');

                if (continua == 'S') //Caso o usuário confirme a decisão
                {
                    fclose(arqReserva);
                    arqReserva = fopen("Reservas.txt", "w+t"); //Abre o arquivo Reservas.txt de modo a reescrever-lo por inteiro

                    if (arqReserva == NULL)
                    { //Se não for possivel abrir o arquivo
                        system("cls || clean");
                        printf("Não foi possivel abrir seu arquivo\n");
                        printf("Fechando o Programa\n");
                        system("pause");
                        exit(0);
                    }

                    fprintf(arqReserva, "GB-XXXXXX,JEB-XXXX,CPF,nome,sexo,DataDeNascimento,StatusReserva\n");

                    for (i = 0; i <= nR; i++)
                    {
                        if (i != n)
                        {
                            fprintf(arqReserva, "%s,%s,%s,%s,%c,%02d/%02d/%004d,%c\n", res[i].numeroR, res[i].numeroV, res[i].cpf, res[i].nome, res[i].sexo, res[i].diaN, res[i].mesN, res[i].anoN, res[i].statusReserva);
                        }
                        else
                        {
                            fprintf(arqReserva, "%s,%s,%s,%s,%c,%02d/%02d/%004d,C\n", res[i].numeroR, res[i].numeroV, res[i].cpf, res[i].nome, res[i].sexo, res[i].diaN, res[i].mesN, res[i].anoN);
                        }
                    }
                    if (!ferror(arqReserva)) //Confere se ocorreu algum erro durante a escrita no arquivo
                    {
                        printf("RESERVA CANCELADA COM SUCESSO\n");
                        printf("Dados Alterados: %s,%s,%s,%s,%c,%02d/%02d/%04d,C\n", res[n].numeroR, res[n].numeroV, res[n].cpf, res[n].nome, res[n].sexo, res[n].diaN, res[n].mesN, res[n].anoN);
                        system("pause");
                        fclose(arqReserva);                        //Fecha novamente o arquivo
                        arqReserva = fopen("Reservas.txt", "a+t"); //Reabre ele de forma a ler/escrever ou criar o arquivo caso não exista
                    }
                    else
                    {
                        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
                        printf("FECHANDO O PROGRAMA\n");
                        fclose(arqReserva);
                        exit(0);
                    }
                }
            }
        }
        //Caso o usuário cancele a decisão o programa volta ao menu
    }
}

void excluir_voo(FILE *arqVoo, FILE *arqReserva)
{
    //Declarações
    char linha1[TAM_MAX], continua;
    bool vooSaiu;
    int nV = 0, i = 0, n, nR = 0;
    struct DadosVoo voo[TAM_MAX];
    struct DadosReserva res[TAM_MAX];

    rewind(arqVoo);
    rewind(arqReserva);

    //Extração dos dados do arquivo do Voo
    fgets(linha1, TAM_MAX, arqVoo); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqVoo, "%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo); //Primereira linha não possui o '\n' em seu inicio
    nV = nV + 1;                                                                                                                                                                     //Posicionamento do vetor na proxima casa

    while (!feof(arqVoo)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqVoo, "\n%[^,],%d,%d/%d/%d,%d:%d,%c", voo[nV].numeroV, &voo[nV].nPassageiros, &voo[nV].dia, &voo[nV].mes, &voo[nV].ano, &voo[nV].hora, &voo[nV].min, &voo[nV].statusVoo);
        nV = nV + 1; //Posicionamento do vetor na proxima casa
    }
    nV = nV - 2; //Correção da casa do vetor para o ultimo escrito

    //Extração dos DaDos no arquivo Reservas

    fgets(linha1, TAM_MAX, arqReserva); //Leitura da Primeira linha do texto, que contem apenas a sinalização da disposição das variaveis

    fscanf(arqReserva, "%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
    nR = nR + 1;              //Posicionamento do vetor na proxima casa
    while (!feof(arqReserva)) //atribuição dos dados do arquivo nas variáveis temporarias
    {
        fscanf(arqReserva, "\n%[^,],%[^,],%[^,],%[^,],%c,%d/%d/%d,%c", res[nR].numeroR, res[nR].numeroV, res[nR].cpf, res[nR].nome, &res[nR].sexo, &res[nR].diaN, &res[nR].mesN, &res[nR].anoN, &res[nR].statusReserva);
        nR = nR + 1; //Posicionamento do vetor na proxima casa
    }

    nR = nR - 1; //Correção da casa do vetor
    if (nV == -1)//Confere se o arquivo possui cadastros
    {
        printf("Nenhum voo encontrado\n");
        system("pause");
    }else
    {
        nV++;
        //Menu
        system("cls || clean");
        printf("===========================================\n");
        printf("              EXCLUIR VOO\n");
        printf("===========================================\n");
        printf("            VOOs CADASTRADOS\n");
        printf("--------------------------------------------\n");

        for (i = 0; i < nV; i++)
        {
            if (voo[i].statusVoo == 'A')
            {
                printf("%s    ATIVO    [%d]\n", voo[i].numeroV, i);
            }else
            {
                printf("%s   CANCELADO [%d]\n", voo[i].numeroV, i);
            }
        }

        printf("--------------------------------------------\n");
        printf("Informe a opcao do voo que deseja cancelar: \n");
        fflush(stdin);
        scanf(" %d", &n);
        printf("--------------------------------------------\n");

        if (n <0 || n >= nV )//Confere se a opcao escolhida esta entre as informadas
        {
            printf("Opcao inexistente\n");
            system("pause");
        }else
        {

            vooSaiu = vooPartiu(voo, voo[n].numeroV, nV); //Não é possivel cancelar um voo que já decolou

            if (vooSaiu == true)
            {
                printf("Voo informado ja partiu\n");
                system("pause");
            }
            else
            {
                do
                {
                    printf("Voo %s e todas as suas reservas serao EXCLUIDOS\n", voo[n].numeroV);
                    printf("Esta decisao NAO PODE SER REVERTIDA\n");
                    printf("Deseja continuar?\n"); //confirmação da desição do Usuário
                    scanf(" %c", &continua);

                    continua = toupper(continua); //Torna a variável em letra maiuscula pra facilitar as operações

                    if (continua != 'S' && continua != 'N')
                    {
                        printf("Opcao invalida \n");
                        system("pause");
                    }

                } while (continua != 'S' && continua != 'N');

                if (continua == 'S')
                {
                    fclose(arqVoo);
                    arqVoo = fopen("Voos.txt", "w+t"); //Abre o arquivo Reservas.txt de modo a reescrever-lo por inteiro

                    if (arqVoo == NULL)
                    { //Se não for possivel abrir o arquivo
                        system("cls || clean");
                        printf("Não foi possivel abrir seu arquivo\n");
                        printf("Fechando o Programa\n");
                        system("pause");
                        exit(0);
                    }
                    fprintf(arqVoo, "JEB-XXXX,NPass,DD/MM/AAAA,hh:mm,Status[A/C]\n"); //Cabeçalho do arquivo

                    for (i = 0; i < nV; i++)
                    {
                        if (i != n)
                        {
                            fprintf(arqVoo, "%s,%d,%02d/%02d/%04d,%02d:%02d,%c\n", voo[i].numeroV, voo[i].nPassageiros, voo[i].dia, voo[i].mes, voo[i].ano, voo[i].hora, voo[i].min, voo[i].statusVoo);
                        }
                    }

                    if (!ferror(arqVoo)) //Confere se ocorreu algum erro durante a escrita no arquivo
                    {
                        printf("VOO %s EXCLUIDO COM SUCESSO\n", voo[n].numeroV);
                        system("pause");
                        fclose(arqVoo);                        //Fecha novamente o arquivo
                        arqVoo = fopen("Reservas.txt", "a+t"); //Reabre ele de forma a ler/escrever ou criar o arquivo caso não exista
                    }
                    else
                    {
                        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
                        printf("FECHANDO O PROGRAMA\n");
                        fclose(arqVoo);
                        exit(0);
                    }

                    fclose(arqReserva);
                    arqReserva = fopen("Reservas.txt", "w+t"); //Abre o arquivo Reservas.txt de modo a reescrever-lo por inteiro

                    if (arqReserva == NULL)
                    { //Se não for possivel abrir o arquivo
                        system("cls || clean");
                        printf("Não foi possivel abrir seu arquivo\n");
                        printf("Fechando o Programa\n");
                        system("pause");
                        exit(0);
                    }

                    fprintf(arqReserva, "GB-XXXXXX,JEB-XXXX,CPF,nome,sexo,DataDeNascimento,StatusReserva\n"); //Escreve o cabeçalho do arquivo

                    for (i = 0; i < nR; i++)
                    {
                        if (strcmp(res[i].numeroV, voo[n].numeroV) == 0) //Confere se a reserva está vinculada ao voo que se deseja excluir
                        {
                            /*Em caso de de exclusão do voo todas as reservas vinculadas a ele devem ser excluidas*/
                        }
                        else
                        {
                            fprintf(arqReserva, "%s,%s,%s,%s,%c,%d/%d/%d,%c\n", res[i].numeroR, res[i].numeroV, res[i].cpf, res[i].nome, res[i].sexo, res[i].diaN, res[i].mesN, res[i].anoN, res[i].statusReserva);
                        }
                    }

                    if (!ferror(arqReserva)) //Confere se ocorreu algum erro durante a escrita no arquivo
                    {
                        printf("RESERVAS APAGADAS COM SUCESSO\n");
                        system("pause");
                        fclose(arqReserva);                        //Fecha novamente o arquivo
                        arqReserva = fopen("Reservas.txt", "a+t"); //Reabre ele de forma a ler/escrever ou criar o arquivo caso não exista
                    }
                    else
                    {
                        printf("OCORREU ALGUM ERRO DURANTE O ARQUIVAMENTO DOS DADOS\n");
                        printf("FECHANDO O PROGRAMA\n");
                        fclose(arqReserva);
                        exit(0);
                    }
                }
            }
        }
    }
}

int main()
{
    //Declarações
    char respMenu[TAM_MAX], continua = 'N';
    FILE *arqReserva, *arqVoo;

    //Instruções
    procuraCabecalho();

    do
    {
        //Abrir arquivos
        arqVoo = fopen("Voos.txt", "a+t"); //abre ele de forma a ler/escrever ou criar o arquivo caso não exista
        //abre ele de forma a ler/escrever ou criar o arquivo caso não exista
        arqReserva = fopen("Reservas.txt", "a+t"); //abre ele de forma a ler/escrever ou criar o arquivo caso não exista

        if (arqVoo == NULL || arqReserva == NULL)
        { //Se não for possivel abrir o arquivo

            system("cls || clean");
            printf("Não foi possivel abrir seus arquivos\n");
            printf("Fechando o Programa\n");
            system("pause");
            exit(0);
        }
        //Retorna a leitura dos arquivos para a primeira linha
        system("cls || clean");
        rewind(arqVoo);
        rewind(arqReserva);
        rewind(arqVoo);

        setlocale(LC_ALL, "Portuguese");
        menu();
        fflush(stdin);
        scanf(" %[^\n]", respMenu); //le a opção a ser executada
        if (strlen(respMenu) != 1)
        {
            printf("Opcao invalida \n");
            system("pause");
        }else
        {

            switch (respMenu[0])
            {
            case '1':
                cadastra_voo(arqVoo);

                break;
            case '2':
                casdastra_reserva(arqReserva, arqVoo);
                break;
            case '3':
                consulta_voo(arqVoo, arqReserva);
                break;
            case '4':
                consulta_reserva(arqReserva, arqVoo);
                break;
            case '5':
                consulta_passageiro(arqReserva);
                break;
            case '6':
                cancela_voo(arqVoo, arqReserva);
                break;
            case '7':
                cancela_reserva(arqReserva, arqVoo);
                break;
            case '8':
                excluir_voo(arqVoo, arqReserva);
                break;
            case '9':
                fclose(arqVoo);
                fclose(arqReserva);
                exit(0);
                break;

            default: //em caso de respotas diferentes das esperadas baseado no menu
                printf("Opcao invalida \n");
                system("pause");
                break;
            }
        }
        //Fechando os arquivos
        fclose(arqVoo);
        fclose(arqReserva);
    } while (1);

    return 0;
}