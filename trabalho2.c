#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
  int total_caixas;
  int espacos;
  int max_caixas_pedido;
  int intervalo_caminhao;
} args_estoque;

typedef struct {
  int numero;
  int tempo;
} args_funcionario;

sem_t sem_estoque;
int deposito = 0;
char encerado = 1;
char entrega_concluida = 1;

void *thread_funcionario(void *arg) {
  args_funcionario *parametros = arg;
  while (1) {
    sem_wait(&sem_estoque);
    if(deposito > 0) {
      int tempo = rand() % parametros -> tempo;
      usleep(tempo);
      deposito -= 1;
      printf("Funcionario %d, levou uma caixa ao estoque\n", parametros -> numero);
    }
    if(deposito <= 0 && entrega_concluida == 0) {
      sem_post(&sem_estoque);
      break;
    }
    sem_post(&sem_estoque);
  };
}

void *thread_estoque(void *arg) {
  args_estoque *parametros = arg;
  int continuar = 1;
  int numero_caminhao = 1;
  int caixas_entreges = 0;
  do {
    sem_wait(&sem_estoque);
    int total_caixas_caminhao = rand() % parametros -> max_caixas_pedido;
    if(total_caixas_caminhao == 0) {
      total_caixas_caminhao = 1;
    }
    if(caixas_entreges + total_caixas_caminhao > parametros -> total_caixas) {
      total_caixas_caminhao = parametros -> total_caixas - caixas_entreges;
    }
    if(total_caixas_caminhao <= (parametros -> espacos - deposito)) {
      deposito += total_caixas_caminhao;
      sem_post(&sem_estoque);
    }
    else {
      int quantidade_inserida = parametros -> espacos - deposito;
      int caixas_caminhao = total_caixas_caminhao;
      char mais_carga = 1;
      deposito += quantidade_inserida;
      caixas_caminhao -= quantidade_inserida;
      sem_post(&sem_estoque);
      do {
        usleep(20);
        sem_wait(&sem_estoque);
        quantidade_inserida = parametros -> espacos - deposito;
        if(quantidade_inserida > 0) {
          if(quantidade_inserida >= caixas_caminhao) {
            deposito += caixas_caminhao;
            mais_carga = 0;
          }
          else {
            deposito += quantidade_inserida;
            caixas_caminhao -= quantidade_inserida;
          }
          sem_post(&sem_estoque);
        }
        else {
          if(caixas_entreges >= parametros -> total_caixas) {
            mais_carga = 0;
          }
          sem_post(&sem_estoque);
        }
      } while (mais_carga == 1);
    }
    caixas_entreges += total_caixas_caminhao;
    printf("Caminhão %d, entregou %d caixas na área de carga\n", numero_caminhao, total_caixas_caminhao);
    numero_caminhao ++;
    if(caixas_entreges >= parametros -> total_caixas) {
      continuar = 0;
    }
    else {
      int tempo = rand() % parametros -> intervalo_caminhao;
      usleep(tempo);
    }
  } while (continuar == 1);
  printf("Não ha mais caminhões\n");
  entrega_concluida = 0;
}

int main(int argc, char *argv[]) {
  if(argc != 7) {
    printf("Quantidade de parametros incorreta\n");
    exit(1);
  }
  args_estoque arg_estoque1;
  int caixas_caminhao = atoi(argv[1]);
  int espacos = atoi(argv[2]);
  int max_caixas_pedidos = atoi(argv[3]);
  int max_funcionarios = atoi(argv[4]);
  int intervalo_caminhao = atoi(argv[5]);
  int max_temp_func = atoi(argv[6]);

  arg_estoque1.total_caixas = caixas_caminhao;
  arg_estoque1.espacos = espacos;
  arg_estoque1.max_caixas_pedido = max_caixas_pedidos;
  arg_estoque1.intervalo_caminhao = intervalo_caminhao;

  sem_init(&sem_estoque, 0, 1);
  pthread_t funcionarios[3];
  pthread_t estoque;

  if(max_funcionarios > 3 || max_funcionarios < 1) {
    printf("Número de funcionarios pode ser entre 1 e 3\n");
    exit(1);
  }

  pthread_create(&estoque, NULL, thread_estoque, &arg_estoque1);
  args_funcionario arg[3];
  for(int i = 0; i < max_funcionarios; i++) {
    arg[i].numero = i+1;
    arg[i].tempo = max_temp_func;
    pthread_create(&funcionarios[i], NULL, thread_funcionario, &arg[i]);
  }
  
  pthread_join(estoque, NULL);
  for(int i = 0; i < max_funcionarios; i++) {
    pthread_join(funcionarios[i], NULL);
  }
  return 0;
}
