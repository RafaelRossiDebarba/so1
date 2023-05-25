#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>

#define DEBUG 0
#define PRINT_MSG 0
#define TAMANHO_FILA 8
#define TEMPO_RR 30
#define TEMPO 30

typedef struct {
  int * n;
  int * x;
  int * Tmin;
  int * Tmax;
  int * Pmin;
  int * Pmax;
  int * Dmin;
  int * Dmax;
} args_geradora;

typedef struct {
  int * a;
} args_escalonador;

typedef struct process {
  int id;
  int prioridade;
  int tempo;
  int tempo_espera;
} process_t;

process_t * aptos[TAMANHO_FILA];
struct process atual;
sem_t semafaro;
char terminou = 0;

void push(int id, int prioridade, int tempo, int tempo_espera) {
  process_t * novo;
  novo = malloc(sizeof(process_t));

  int i = 0;
  while(aptos[i] != NULL) {
    i++;
  }

  novo->id = id;
  novo->prioridade = prioridade;
  novo->tempo = tempo;
  novo->tempo_espera = tempo_espera;

  aptos[i] = novo;
}

void print_aptos(int id, int oculto) {
  if(id == -1 && oculto == -1) {
    printf("-1\t[]\n");
    return;
  }
  char fila[255] = "";
  char aux[20] = "";
  fila[0] = '[';
  int posicao = 1;
  int i = 0;
  while(aptos[i] != NULL && i < TAMANHO_FILA) {
    if(oculto != aptos[i]->id) {
      fila[posicao] = 'p';
      sprintf(aux, "%d", aptos[i]->id);
      strcat(fila, aux);
      if(aptos[i]->id > 9) {
        posicao += 3;
      }
      else {
        posicao += 2;
      }
      i++;
      if(aptos[i] != NULL && oculto != aptos[i]->id) {
        fila[posicao] = ',';
        posicao ++;
      }
    }
    else {
      i++;
      if(i > 1 && aptos[i] != NULL) {
        fila[posicao] = ',';
        posicao ++;
      }
    }
  }
  fila[posicao] = ']';
  if(id != -1) {
    printf("p%d\t%s\n", id, fila);
  }
  else {
    printf("%d\t%s\n", id, fila);
  }
}

void inicializar_aptos() {
  for(int i = 0; i < TAMANHO_FILA; i++) {
    aptos[i] = NULL;
  }
}

int count_aptos() {
  int i = 0;
  while(aptos[i] != NULL) {
    i++;
  }
  return i;
}

void finaliza_apto(int posicao) {
  int i = posicao + 1;
  process_t * excluir = aptos[posicao];
  atual.tempo = aptos[posicao]->tempo;
  atual.id = aptos[posicao]->id;
  atual.prioridade = aptos[posicao]->prioridade;
  free(excluir);
  for(;i < TAMANHO_FILA; i++) {
    if(aptos[i] != NULL) {
      aptos[i -1] = aptos[i];
    }
    else {
      break;
    }
  }
  aptos[i-1] = NULL;
}

int get_menor_tempo() {
  if(count_aptos() == 0) {
    return -1;
  }
  int posicao = 0;
  int menor = aptos[0]->tempo;
  for(int i = 1; i < TAMANHO_FILA; i++) {
    if(aptos[i] == NULL)
      break;
    if(aptos[i]->tempo < menor) {
      menor = aptos[i]->tempo;
      posicao = i;
    }
  }

  return posicao;
}

int get_posicao_maior_prioridade() {
  if(count_aptos() == 0)
    return -1;
  int posicao = 0;
  int maior = aptos[0]->prioridade;
  for(int i = 1; i < TAMANHO_FILA; i++) {
    if(aptos[i] == NULL)
      break;
    if(aptos[i]->prioridade > maior) {
      maior = aptos[i]->prioridade;
      posicao = i;
    }
  }
  return posicao;
}

int get_maior_prioridade() {
  if(count_aptos() == 0)
    return -1;
  int maior = aptos[0]->prioridade;
  for(int i = 1; i < TAMANHO_FILA; i++) {
    if(aptos[i] == NULL)
      break;
    if(aptos[i]->prioridade > maior) {
      maior = aptos[i]->prioridade;
    }
  }
  return maior;
}

void get_process_prioridade (int prioridade, int * array) {
  int posicao = 0;
  for(int i = 0; i < TAMANHO_FILA; i++) {
    array[i] = -1;
  }
  for(int i = 0; i < TAMANHO_FILA; i++) {
    if(aptos[i] == NULL)
      break;
    if(aptos[i]->prioridade == prioridade) {
      array[posicao] = i;
      posicao ++;
    }
  }
}

void adiciona_tempo_espera() {
  for(int i = 0; i < TAMANHO_FILA; i++) {
    if(aptos[i] == NULL)
      break;
    aptos[i]->tempo_espera ++;
  }
}

int get_hrrn() {
  if(count_aptos() == 0)
    return -1;
  int posicao = 0;
  double prioridade = (aptos[0]->tempo_espera + aptos[0]->tempo) / aptos[0]->tempo;
  double aux;
  for(int i = 1; i < TAMANHO_FILA; i++) {
    if(aptos[i] == NULL)
      break;
    aux = (aptos[i]->tempo_espera + aptos[i]->tempo) / aptos[i]->tempo;
    if(aux > prioridade) {
      prioridade = aux;
      posicao = i;
    }
  }
  return posicao;
}

void fifo() {
  while(1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0) {
      sem_post(&semafaro);
      break;
    }

    if(count_aptos() == 0) {
      sem_post(&semafaro);
      print_aptos(-1, -1);
      usleep(10);
      continue;
    }
    finaliza_apto(0);
    print_aptos(atual.id, -1);
    sem_post(&semafaro);
    usleep(atual.tempo);
  }
  if(PRINT_MSG)
    printf("Terminou FIFO\n");
}

void sjf() {
  while(1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0) {
      sem_post(&semafaro);
      break;
    }
    int pos = get_menor_tempo();
    if(DEBUG) {
      printf("Posição SJF: %d\n", pos);
    }
    if(pos > -1) {
      finaliza_apto(pos);
      print_aptos(atual.id, -1);
      sem_post(&semafaro);
      usleep(atual.tempo);
    }
    else {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
    }
  }
  if(PRINT_MSG)
    printf("Terminou SJF\n");
}

void prioridade_nao_preemptivo() {
  while(1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0){
      sem_post(&semafaro);
      break;
    }
    int pos = get_posicao_maior_prioridade();
    if(DEBUG)
      printf("Posição %d\n", pos);
    if(pos > -1) {
      finaliza_apto(pos);
      print_aptos(atual.id, -1);
      sem_post(&semafaro);
      usleep(atual.tempo);
    }
    else {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
    }
  }
  if(PRINT_MSG)
    printf("Terminou Prioridade não preemptivo\n");
}

void hrrn() {
  while(1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0) {
      sem_post(&semafaro);
      break;
    }
    adiciona_tempo_espera();
    int pos = get_hrrn();
    if(pos > -1) {
      finaliza_apto(pos);
      print_aptos(atual.id, -1);
      sem_post(&semafaro);
      usleep(atual.tempo);
    }
    else {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
    }
  }
  if(PRINT_MSG)
    printf("Terminou HRRN\n");
}

void rr() {
  int i = 0;
  while(1) {
    sem_wait(&semafaro);
    int quantidade = count_aptos();
    if(quantidade == 0 && terminou == 1) {
      sem_post(&semafaro);
      break;
    }
    if(quantidade == 0) {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
      continue;
    }
    if(i >= quantidade)
      i = 0;
    if(DEBUG)
      printf("Posição ARRAY: %d\n", i);
    if(aptos[i]->tempo <= TEMPO_RR) {
      finaliza_apto(i);
      print_aptos(atual.id, -1);
      if(DEBUG)
        printf("Consumindo e encerando p%d  tempo execucao: %d\n", atual.id, atual.tempo);
      sem_post(&semafaro);
      usleep(atual.tempo);
    }
    else {
      print_aptos(aptos[i]->id, aptos[i]->id);
      if(DEBUG)
        printf("Consumindo p%d  tempo execução: %d restante: %d\n", aptos[i]->id, aptos[i]->tempo, aptos[i]->tempo - TEMPO_RR);
      aptos[i]->tempo -= TEMPO_RR;
      sem_post(&semafaro);
      usleep(TEMPO_RR);
    }
    i++;
  }
  if(PRINT_MSG)
    printf("Terminou RR\n");
}

void prioridade_rr() {
  int * array_exec = (int *) malloc(sizeof(int[TAMANHO_FILA]));
  int maior_prioridade = -1;
  while (1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0) {
      sem_post(&semafaro);
      break;
    }
    maior_prioridade = get_maior_prioridade();
    if(maior_prioridade > -1) {
      get_process_prioridade(maior_prioridade, array_exec);
      sem_post(&semafaro);
      for(int i = 0; i < TAMANHO_FILA; i++) {
        if(array_exec[i] == -1)
          break;
        sem_wait(&semafaro);
        if(DEBUG) {
          printf("Posicao array_exec: %d  posicao aptos: %d\n", i, array_exec[i]);
        }
        if(aptos[array_exec[i]]->tempo <= TEMPO_RR) {
          finaliza_apto(array_exec[i]);
          print_aptos(atual.id, -1);
          get_process_prioridade(maior_prioridade, array_exec);
          sem_post(&semafaro);
          usleep(atual.tempo);
        }
        else {
          print_aptos(aptos[array_exec[i]]->id, aptos[array_exec[i]]->id);
          aptos[array_exec[i]]->tempo -= TEMPO_RR;
          sem_post(&semafaro);
          usleep(TEMPO_RR);
        }
      }
    }
    else {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
    }
  }
  free(array_exec);
  if(PRINT_MSG)
    printf("Terminou Prioridades RR\n");
}

void srt() {
  while(1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0) {
      sem_post(&semafaro);
      break;
    }
    if(count_aptos() == 0) {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
      continue;
    }
    int pos = get_menor_tempo();
    if(DEBUG) 
      printf("Posição SRT: %d\n", pos);
    if(aptos[pos]->tempo <= TEMPO) {
      finaliza_apto(pos);
      print_aptos(atual.id, -1);
      sem_post(&semafaro);
      usleep(atual.tempo);
    }
    else {
      print_aptos(aptos[pos]->id, aptos[pos]->id);
      aptos[pos]->tempo -= TEMPO;
      sem_post(&semafaro);
      usleep(TEMPO);
      while(1) {
        sem_wait(&semafaro);
        int verifica = get_menor_tempo();
        if(verifica != pos) {
          sem_post(&semafaro);
          break;
        }
        if(aptos[pos]->tempo <= TEMPO) {
          finaliza_apto(pos);
          sem_post(&semafaro);
          usleep(atual.tempo);
          break;
        }
        else {
          aptos[pos]->tempo -= TEMPO;
          sem_post(&semafaro);
          usleep(TEMPO);
        }
      }
    }
  }
  if(PRINT_MSG)
    printf("Terminou SRT\n");
}

void prioridade_fifo() {
  while(1) {
    sem_wait(&semafaro);
    if(terminou == 1 && count_aptos() == 0) {
      sem_post(&semafaro);
      break;
    }
    if(count_aptos() == 0) {
      print_aptos(-1, -1);
      sem_post(&semafaro);
      usleep(1);
      continue;
    }
    int pos = get_posicao_maior_prioridade();
    if(DEBUG)
      printf("Posição: %d\n", pos);
    if(aptos[pos]->tempo <= TEMPO) {
      finaliza_apto(pos);
      print_aptos(atual.id, -1);
      sem_post(&semafaro);
      usleep(atual.tempo);
    }
    else {
      print_aptos(aptos[pos]->id, aptos[pos]->id);
      aptos[pos]->tempo -= TEMPO;
      sem_post(&semafaro);
      usleep(TEMPO);
      while(1) {
        sem_wait(&semafaro);
        int verifica = get_posicao_maior_prioridade();
        if(verifica != pos) {
          sem_post(&semafaro);
          break;
        }
        if(aptos[pos]->tempo <= TEMPO) {
          finaliza_apto(pos);
          sem_post(&semafaro);
          usleep(atual.tempo);
          break;
        }
        else {
          aptos[pos]->tempo -= TEMPO;
          sem_post(&semafaro);
          usleep(TEMPO);
        }
      } 
    }
  }
  if(PRINT_MSG)
    printf("Terminou Prioridades FIFO\n");
}

void *thread_geradora(void *args) {
  args_geradora *parametros = args;
  srand(time(NULL));
  char msg = 1;
  int tempo, prioridade, tempo_geracao, numero_processo = 0;

  while(numero_processo < *parametros->n) {

    sem_wait(&semafaro);

    if(count_aptos() >= *parametros->x) {
      sem_post(&semafaro);
      //usleep(100);
      if(msg == 1) {
        printf("Geradora Suspença\n");
        msg = 0;
      }
      continue;
    }

    msg = 1;

    tempo = *parametros->Tmax - *parametros->Tmin;
    tempo = *parametros->Tmin + rand() % tempo;
    
    prioridade = *parametros->Pmax - *parametros->Pmin;
    prioridade = *parametros->Pmin + rand() % prioridade;

    tempo_geracao = *parametros->Dmax - *parametros->Dmin;
    tempo_geracao = *parametros->Dmin + rand() % tempo_geracao;
    push(numero_processo, prioridade, tempo, 0);
    if(DEBUG == 1) {
      printf("Novo p%d  prioridade: %d  tempo: %d\n", numero_processo, prioridade, tempo);
    }
    sem_post(&semafaro);
    usleep(tempo_geracao);
    numero_processo ++;
  }
  terminou = 1;
  if(PRINT_MSG)
    printf("Todos os processos Gerados\n");
  return NULL;
}

void *thread_escalonador(void *args) {
  printf("CPU\tFila de aptos\n");
  args_escalonador * parametros = args;
  if(*parametros->a == 1) { // FIFO
    fifo();
  }
  else if(*parametros->a == 2) { // SJF
    sjf();
  }
  else if(*parametros->a == 3) { // Prioridade não preemptivo
    prioridade_nao_preemptivo();
  }
  else if(*parametros->a == 4) { // HRRN
    hrrn();
  }
  else if(*parametros->a == 5) { // RR
    rr();
  }
  else if(*parametros->a == 6) { // Prioridade preemptivo com FIFO
    prioridade_fifo();
  }
  else if(*parametros->a == 7) { // Prioridade preemptivo com RR
    prioridade_rr();
  }
  else { // SRT
    srt();
  }
  return NULL;
}

int main (int argc, char *argv[]) {
  if( argc != 10) {
    printf("Verifique os parametros enviados\n");
    printf("Argumentos ./escalonamento N X Tmin Tmax Pmin Pmax Dmin Dmax A\n");
    printf("Exemplo\n./escalonamento 5 2 50 60 2 3 38 50 1\n");
    exit(1);
  }
  int n, x, Tmin, Tmax, Pmin, Pmax, Dmin, Dmax, a;

  inicializar_aptos();

  args_geradora geradora;
  args_escalonador escalonador;

  n = atoi(argv[1]);
  x = atoi(argv[2]);
  Tmin = atoi(argv[3]);
  Tmax = atoi(argv[4]);
  Pmin = atoi(argv[5]);
  Pmax = atoi(argv[6]);
  Dmin = atoi(argv[7]);
  Dmax = atoi(argv[8]);
  a = atoi(argv[9]);

  if(n <= 0 || n >= 50) {
    printf("O valor de N deve ser maior que 0 e menor que 50\n");
    exit(1);
  }
  if(x <= 1 || x >= TAMANHO_FILA) {
    printf("O valor do X deve ser maior que 1 e menor que %d\n", TAMANHO_FILA);
    exit(1);
  }
  if(Tmin <= 9 || Tmin > 200) {
    printf("O valor de Tmin deve ser maior que 9 e menor ou igual a 200\n");
    exit(1);
  }
  if(Tmax <= Tmin || Tmax > 500) {
    printf("O valor de Tmax deve ser maior que Tmin e menor ou igual a 500\n");
    exit(1);
  }
  if(Pmin <= 0 || Pmin >= 3) {
    printf("O valor de Pmin dever ser maior que 0 e menor que 3\n");
    exit(1);
  }
  if(Pmax <= Pmin || Pmax >= 6) {
    printf("O valor de Pmax deve ser maior que Pmin e menor que 6\n");
    exit(1);
  }
  if(Dmin <= 9 || Dmin > 100) {
    printf("O valor de Dmin deve ser maio que 9 e menor ou igual a 100\n");
    exit(1);
  }
  if(Dmax <= Dmin || Dmax > 400) {
    printf("O valor de Dmax deve ser maior que Dmin e menor ou igual a 400\n");
    exit(1);
  }
  if(a < 1 || a >= 9) {
    printf("Algoritmos de 1 a 8\n");
    exit(1);
  }

  if(Tmin >= Tmax) {
    printf("Tmin não pode ser maior ou igual a Tmax\n");
    exit(1);
  }
  if(Pmin >= Pmax) {
    printf("Pmin não pode ser maior ou igual a Pmax\n");
    exit(1);
  }
  if(Dmin >= Dmax) {
    printf("Dmin não pode ser maior ou igual a Dmax\n");
    exit(1);
  }

  sem_init(&semafaro, 0, 1);

  geradora.n = &n;
  geradora.x = &x;
  geradora.Tmin = &Tmin;
  geradora.Tmax = &Tmax;
  geradora.Pmin = &Pmin;
  geradora.Pmax = &Pmax;
  geradora.Dmin = &Dmin;
  geradora.Dmax = &Dmax;

  escalonador.a = &a;


  pthread_t t_gerador;
  pthread_t t_escalonador;

  pthread_create(&t_gerador, NULL, thread_geradora, &geradora);
  pthread_create(&t_escalonador, NULL, thread_escalonador, &escalonador);

  pthread_join(t_gerador, NULL);
  pthread_join(t_escalonador, NULL);

  return 0;
}
