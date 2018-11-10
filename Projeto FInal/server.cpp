
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>

#include "serializable.hpp"
#include "oo_model.hpp"

//#define MAX_BUFFER 150
#define MAX_CONEXOES 5

/* Estou usando variaveis globais para me referir a variaveis que sao usadas
 * tanto pela trhead principal quanto pela auxiliar. Mas, criancas, nao facam
 * isso em casa. A abordagem correta eh colocar todas as variaveis numa struct e
 * passa-la como parametro na chamada da thread */

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


struct sockaddr_in myself, client;
socklen_t client_size;
int socket_fd;
int connection_fd[MAX_CONEXOES];
int conexao_usada[MAX_CONEXOES];
int running;


int adicionar_conexao(int new_connection_fd) {
  int i;
  for (i=0; i<MAX_CONEXOES; i++) {
    if (conexao_usada[i] == 0) {
      conexao_usada[i] = 1;
      connection_fd[i] = new_connection_fd;
      return i;
    }
  }
  return -1;
}

int remover_conexao(int user) {
  if (conexao_usada[user]==1) {
  conexao_usada[user] = 0;
  close(connection_fd[user]);
  }
  return 1;
}

void *wait_connections(void *parameters) {
  int conn_fd;
  int user_id;
  while(running) {
    conn_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    user_id = adicionar_conexao(conn_fd);
    if (user_id != -1) {
      printf("Novo usuario chegou! ID=%d\n", user_id);
      //TO-DO:Cria nova snake (Fazer com flags, implementar na main)
    } else {
      printf("Maximo de usuarios atingido!\n");
    }
  }
  return NULL;
}



int main() {
  pthread_t esperar_conexoes;
  int msglen;
  int user_iterator;
  char output_buffer[60];
  char input_buffer[50];
  char *input_char;
  std::string send_buffer;

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;
  T = get_now_ms();
  t1 = T;
  
  
  /*** Cria lista de Corpos inicial ***/
  Corpo *c1 = new Corpo(0, 0, (int)SCREEN_WIDTH/2, (int)SCREEN_HEIGHT/2 , COMIDA);
  Corpo *c2 = new Corpo(10, 0, (int)SCREEN_WIDTH/2, (int)SCREEN_HEIGHT/2 , SNAKE_HEAD);
  Corpo *c3 = new Corpo(10, 0, ((int)SCREEN_WIDTH/2)-2, (int)SCREEN_HEIGHT/2 , SNAKE_BODY);
  Corpo *c4 = new Corpo(10, 0, ((int)SCREEN_WIDTH/2)-4, (int)SCREEN_HEIGHT/2 , SNAKE_BODY);
  Corpo *c5 = new Corpo(10, 0, ((int)SCREEN_WIDTH/2)-6, (int)SCREEN_HEIGHT/2 , SNAKE_BODY);


  ListaDeCorpos *l = new ListaDeCorpos();
  l->add_corpo(c1);
  l->add_corpo(c2);
  l->add_corpo(c3);
  l->add_corpo(c4);
  l->add_corpo(c5);


  SnakeController *f = new SnakeController(l);

  std::vector<Corpo *> *corpos = l->get_corpos();
  /* Laço para serialização */
  for(int h=0; h < (*corpos).size(); h++){

    RelevantData DadosCorpo(  (*corpos)[h]->get_velocidade_x(),\
                              (*corpos)[h]->get_velocidade_y(),\
                              (*corpos)[h]->get_posicao_x(),\
                              (*corpos)[h]->get_posicao_y(),\
                              (*corpos)[h]->get_tipo()
                              );

    std::string buffer(50, '#');
    std::cout << "Originais: ";
    DadosCorpo.serialize(buffer);
    std::cout << buffer << "\n";

    send_buffer = send_buffer + buffer;
    std::replace( send_buffer.begin(), send_buffer.end(), '\0', '#'); // replace all '\0' to '#'
    send_buffer+='\0';


  }

  /* Inicializando variaveis */
  client_size = (socklen_t)sizeof(client);
  for (int i=0; i<MAX_CONEXOES; i++) {
    conexao_usada[i] = 0;
  }
  running = 1;

  /* socket, bind, listen */
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");
  myself.sin_family = AF_INET;
  myself.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(myself.sin_addr));
  printf("Tentando abrir porta 3001\n");
  if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
    printf("Problemas ao abrir porta\n");
    return 0;
  }
  printf("Abri porta 3001!\n");
  listen(socket_fd, 2);
  printf("Estou ouvindo na porta 3001!\n");

  /* Dispara thread para ouvir conexoes */
  pthread_create(&esperar_conexoes, NULL, wait_connections, NULL);

  while (running) {
    for (user_iterator=0; user_iterator<MAX_CONEXOES; user_iterator++) {
      if (conexao_usada[user_iterator] == 1) {
        msglen = recv(connection_fd[user_iterator], input_buffer, 2, MSG_DONTWAIT);
        if (msglen > 0) { //Recebeu msg
          printf("Recebi mensagem de %d\n", user_iterator);
          if ( strcmp(input_buffer, "END") == 0) running=0;
          sprintf(output_buffer, "USER %d: %c\n", user_iterator, input_buffer[0]);
          printf("%s\n", output_buffer);
          //dá update no model (Lista de Corpos)
          if(input_buffer[0] == 'q') {
            printf("Usuario %d desconectou!\n", user_iterator);
            remover_conexao(user_iterator);
            //TO-DO remove todos os corpos desse user
          }/*else if(input_buffer[0] == 'w') {
            f->andar_para_cima();
          }else if(input_buffer[0] == 's') {
            f->andar_para_baixo();
          }else if(input_buffer[0] == 'd') {    
           f->andar_para_direita();
          }else if(input_buffer[0] == 'a') {
          f->andar_para_esquerda();      
          } */   
              
          for (int ret=0; ret<MAX_CONEXOES; ret++) {
            if (conexao_usada[ret] == 1) {
              printf("Avisando user %d\n", ret);
              if (send(connection_fd[ret], send_buffer.c_str() , MAX_MSG_STRING, MSG_NOSIGNAL) == -1) { //editei para enviar buffer serializado
               /* Usuario desconectou!?? */
                printf("Usuario %d desconectou!\n", ret);
                remover_conexao(ret);
                //TO-DO remove todos os corpos desse user
              }
            }
          }
        }
      }
    }
    
    
    //TODO:update
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    
    f->update(deltaT);
  }

  printf("Encerrando server...\n");
  for (user_iterator=0; user_iterator<MAX_CONEXOES; user_iterator++)
    remover_conexao(user_iterator);

  pthread_join(esperar_conexoes, NULL);

  return 0;
}
