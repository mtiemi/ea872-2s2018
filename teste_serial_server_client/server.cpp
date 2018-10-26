
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
  RelevantData D1(11, 's');
  RelevantData D2(10, 'b');

  std::string buffer(sizeof(DataContainer), ' ');

  std::cout << "Originais:\n";
  D1.dump();
  D2.dump();

  D1.serialize(buffer);
  //edit: Transforma em array de char para poder enviar dados
  printf("Serializado (buffer) :");
  std::cout << buffer << "\n";
  char send_buffer[buffer.size() + 1];
  strcpy(send_buffer, buffer.c_str());

  // std::string vetor1 = "Olar!";
  // char vetor2[vetor1.size() + 1];
  // strcpy(vetor2, vetor1.c_str());
  // std::cout << "vector1 = " << vetor1 << '\n';
  // std::cout << "vector2 = " << vetor2 << '\n';

  // char *send_buffer;
  // send_buffer = (char *) malloc( sizeof(buffer));
  // for(int l = 0;l <= buffer.length(); l++ ){
  //   printf("Caractere %d: %c\n", l, buffer[l]);
  //   send_buffer[l] = buffer.c_str()[l];
  // }

  printf("Copiado para array de char (send_buffer) : %s", buffer.c_str());
  std::cout << buffer << "\n";
  D2.unserialize(buffer);
  // std::cout << "Size buffer: " << sizeof(buffer) << std::endl;
  // std::cout << "Size buffer.c_str(): " << sizeof(buffer.c_str()) << std::endl;
  // std::cout << "Size D2: " << sizeof(D2) << std::endl;
  // std::cout << "Size DataContainer: " << sizeof(DataContainer) << std::endl;
  // std::cout << "Size RelevantData: " << sizeof(char) << std::endl;
  // std::cout << "Size char: " << sizeof(char) << std::endl;
  // std::cout << "Size int: " << sizeof(int) << std::endl

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
    if (send(connection_fd, buffer.c_str(), 100, 0) < 0) { //edit:
      printf("Erro ao enviar mensagem de retorno\n");
    } else {
      printf("Sucesso para enviar mensagem de retorno\n");
    }
  }

  close(socket_fd);
  return 0;
}
