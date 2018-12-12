#include <stdio.h> /* Standard I/O functions */
#include <stdlib.h> /* Miscellaneous functions (rand, malloc, srand)*/
#include <getopt.h> /* get options from system argc/argv */
#include <string.h>

#define SBUFF 256

typedef struct st_lista
{
    int estado;
    int qinic,qprox;
    char *letra;
    struct st_lista *prox;
} lista;

void help(void); /* print some help */
void copyr(void); /* print version and copyright information */
void ex8_init(void); /* global initialization function */
void inserir_finais(lista **cabeca,int x);
void inserir_deltas(lista **cabeca,int estini,int estprox,char *letra);
int busca_proximo(lista  *cabeca,int atual,char alfb);
int busca_final(lista  *cabeca,int atual);
void entradatxt(FILE **respostas);
void afd(int estinicial,lista *deltas,lista *definais,char *nova);
int main(int argc, char *argv[])
{
    int opt; /* return from getopt() */
    FILE *respostas;

    opterr = 0;

    while((opt = getopt(argc, argv, "vhf:V")) != EOF)
        switch(opt)
        {
            case 'h':
                break;
            case 'V':
                break;
            case 'v':
                break;
            case 'f':
                respostas=fopen(optarg,"r");
                entradatxt(&respostas);
                break;
            default:
                printf("Type\n\t$man %s\nor\n\t$%s -h\nfor help.\n\n", argv[0], argv[0]);
                return EXIT_FAILURE;
        }

    return EXIT_SUCCESS;
}
void entradatxt(FILE **respostas)
{
    char *delt;
    char *estftok;
    char *arq;
    char *tokens[4],*trans[2],*p;
    char nova[SBUFF];
    int c = 0,estinicial;
    unsigned int t=0;
    lista *deltas=NULL,*definais=NULL;
    opterr = 0;
    char resp[SBUFF];
    int def;

    fgets(resp, sizeof(resp), *respostas);
    arq = strtok(resp, ";");
    while(arq != NULL)
    {
        tokens[c] = arq;
        arq = strtok(NULL, ";");
        c++;
    }
    estinicial=atoi(tokens[2]);

    estftok = strtok(tokens[3], ",");
    while(estftok != NULL)
    {
        def=atoi(estftok);
        inserir_finais(&definais,def);
        estftok = strtok(NULL, ",");
    }

    delt = strtok(tokens[4], ",");
    while(delt!=NULL)
    {
        trans[t]=delt;
        delt = strtok(NULL, ",");
        if((t+1)%3==0 && t!=0)
        {
            inserir_deltas(&deltas,atoi(trans[0]),atoi(trans[2]),trans[1]);
            t=-1;
            trans[0]=" ";
            trans[1]=" ";
            trans[2]=" ";
        }
        t++;
    }

    fgets(nova,SBUFF,stdin);
    if((p=strchr(nova, '\n'))!=NULL)
        *p='\0';

    afd(estinicial,deltas,definais,nova);

    return;
}
void afd(int estinicial,lista *deltas,lista *definais,char *nova)
{
    int proxest=0;
    unsigned int i=0;

    while(proxest!=-1)
    {
        proxest = busca_proximo(deltas,estinicial,nova[i]); /* Checa se eh possivel mudar de estado,caso nao seja retorn -1 */
        i++;
        if(busca_final(definais,proxest)) /*Checa se o proximo estado eh final*/
        {
            if(i==strlen(nova)) /*Depois checa se ta no final da string*/
            {
                printf("Valida!\n");
                return;
            }
        }
        estinicial=proxest; /*Muda o estado*/
    }
    printf("Invalida!\n");
    return;
}
void inserir_finais(lista **cabeca,int x)
{
    lista *pl=*cabeca;
    lista *plant=NULL;

    while(pl!=NULL)
    {
        plant=pl;
        pl=pl->prox;
    }
    pl=malloc(sizeof(lista));
    pl->estado=x;
    pl->prox=NULL;
    if(plant!=NULL)
        plant->prox=pl;
    else
        *cabeca=pl;
}
void inserir_deltas(lista **cabeca,int estini,int estprox,char *letra)
{
    lista *pl=*cabeca;
    lista *plant=NULL;

    while(pl!=NULL)
    {
        plant=pl;
        pl=pl->prox;
    }
    pl=malloc(sizeof(lista));
    pl->qinic=estini;
    pl->qprox=estprox;
    pl->letra=letra;
    pl->prox=NULL;
    if(plant!=NULL)
        plant->prox=pl;
    else
        *cabeca=pl;
}
int busca_proximo(lista  *cabeca,int atual,char alfb)
{
    lista  *pl=cabeca;
    while(pl!=NULL)
    {
        if(pl->qinic==atual && pl->letra[0]==alfb)
            return pl->qprox;
        pl=pl->prox;
    }
    return  -1;
}
int busca_final(lista  *cabeca,int atual)
{
    lista  *pl=cabeca;
    while(pl!=NULL)
    {
        if(pl->estado==atual)
            return 1;
        pl=pl->prox;
    }
    return  0;
}
