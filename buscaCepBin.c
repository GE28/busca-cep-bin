#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Endereco Endereco;

struct _Endereco {
  char logradouro[72];
  char bairro[72];
  char cidade[72];
  char uf[72];
  char sigla[2];
  char cep[8];
  char lixo[2];
};

int comparaCep(char *s1, char *s2);
int byteEndereco(int i);
Endereco *buscaBinaria(FILE *arquivo, int tamanho, char *cepDesejado);
void exibirCep(Endereco *e);

int main(int argc, char **argv) {
  FILE *entrada;
  entrada = fopen("saida.dat", "rb");
  long posicao, qtd, metade;
  int i;

  fseek(entrada, 0, SEEK_END);
  posicao = ftell(entrada);
  qtd = posicao / sizeof(Endereco);
  metade = qtd / 2;

  rewind(entrada);
  Endereco *e = buscaBinaria(entrada, qtd, argv[1]);
  if (e != NULL) exibirCep(e);
}

Endereco *buscaBinaria(FILE *arquivo, int tamanho, char *cepDesejado) {
  Endereco *e = malloc(sizeof(Endereco));
  long left = 0;
  long right = tamanho;
  do {
    long i = (left + right) / 2;
    fseek(arquivo, byteEndereco(i), SEEK_SET);
    fread(e, sizeof(Endereco), 1, arquivo) != 0;

    if (comparaCep(e->cep, cepDesejado) == 0) {
      printf("Linha %ld\n", i + 1);
      return e;
    }
    comparaCep(e->cep, cepDesejado) > 0 ? (right = i - 1) : (left = i + 1);
  } while (right >= left);
  puts("O cep não foi encontrado");
  return NULL;
}

int byteEndereco(int i) { return i * sizeof(Endereco); }

int comparaCep(char *s1, char *s2) { return strncmp(s1, s2, 8); }

void exibirCep(Endereco *e) {
  printf("CEP ENCONTRADO: %.8s\n", e->cep);
  printf("%.72s\n", e->logradouro);
  printf("%.72s\n", e->bairro);
  printf("%.72s\n", e->cidade);
  printf("%.72s\n", e->uf);
  printf("%.2s\n", e->sigla);
}