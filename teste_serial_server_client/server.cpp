
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "serializable.hpp"
#include <stdlib.h>

int main() {
  int socket_fd, connection_fd;
  struct sockaddr_in myself, client;
  socklen_t client_size = (socklen_t)sizeof(client);
  char input_buffer[50];

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

  /*** Início - Criando dados para enviar para cliente ***/
  printf("\nCriando structs para enviar para o cliente!\n");
  RelevantData D1(9.87, 8.76,9.87, 8.76, 21);
  RelevantData D2(7.65, 6.54,7.65, 6.54, 22);

  std::string buffer(50, ' ');

  std::cout << "Originais:\n";
  D1.dump();
  D2.dump();

  D1.serialize(buffer); //edit: Transforma em array de char para poder enviar dados

  printf("Copiado para array de char (send_buffer) : %s\n", buffer.c_str());
  std::cout << buffer << "\n====================\n";
  printf("Copiado para array de char (send_buffer) : %s\n", buffer.c_str());
  D2.unserialize(buffer);
  D2.dump();
  /*** Fim - Criando dados para enviar para cliente ***/


  while (1) {
    printf("Vou travar ate receber alguma coisa\n");
    connection_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    printf("Recebi uma mensagem:\n");
    recv(connection_fd, input_buffer, 5, 0);
    printf("%s\n", input_buffer);

    /* Identificando cliente */
    char ip_client[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &(client.sin_addr), ip_client, INET_ADDRSTRLEN );
    printf("IP que enviou: %s\n", ip_client);

    /* Respondendo */
    printf("Enviando mensagem de retorno\n");
    if (send(connection_fd, buffer.c_str(), 50, 0) < 0) { //edit:
      printf("Erro ao enviar mensagem de retorno\n");
    } else {
      printf("Sucesso para enviar mensagem de retorno\n");
    }
  }

  close(socket_fd);
  return 0;
}
