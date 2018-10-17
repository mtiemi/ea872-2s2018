// EA872 - LAB07
// Mariane Tiemi Iguti (RA147279) e Gabriela Akemi Shima (RA135819)

#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "oo_model.hpp"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  int socket_fd, connection_fd;
  struct sockaddr_in myself, client;
  socklen_t client_size = (socklen_t)sizeof(client);
  char input_buffer[50];

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  //printf("Socket criado\n");

  myself.sin_family = AF_INET;
  myself.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(myself.sin_addr));

  //printf("Tentando abrir porta 3001\n");
  if (bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
    //printf("Problemas ao abrir porta\n");
    return 0;
  }
  //printf("Abri porta 3001!\n");

  listen(socket_fd, 2);
  //printf("Estou ouvindo na porta 3001!\n");

  while (1) {
    //printf("Vou travar ate receber alguma coisa\n");
    connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    //printf("Recebi uma mensagem:\n");
    recv(connection_fd, input_buffer, 2, 0); //edit: recebe apena 1 caractere na string
    printf("%s\n", input_buffer);

    /* Identificando cliente */
    char ip_client[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &(client.sin_addr), ip_client, INET_ADDRSTRLEN );
    //printf("IP que enviou: %s\n", ip_client);

    //edit: Adicionado leitura de comandos
    if (input_buffer[0]=='w') { // Comando para andar pra cima
        //printf("Para cima\n");
    }

    if (input_buffer[0]=='s') { // Comando para andar para baixo
        //printf("Para baixo\n");
    }

    if (input_buffer[0]=='d') { // Comando para andar para direita
        //printf("Para direita\n");
    }

    if (input_buffer[0]=='a') { // Comando para andar para esquerda
        //printf("Para esquerda\n");
    }

    if (input_buffer[0]=='q') { // Comando para terminar o jogo
        //printf("Saida do jogo\n");
    }

    /* Respondendo */
    //printf("Enviando mensagem de retorno\n");
    if (send(connection_fd, "PONG", 5, 0) < 0) {
      //printf("Erro ao enviar mensagem de retorno\n");
    } else {
      //printf("Sucesso para enviar mensagem de retorno\n");
    }
  }

  close(socket_fd);
  return 0;
}
