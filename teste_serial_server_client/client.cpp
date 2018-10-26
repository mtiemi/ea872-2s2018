
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "serializable.hpp"

int main() {
  int socket_fd;
  struct sockaddr_in target;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");

  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(target.sin_addr));
  printf("Tentando conectar\n");
  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    printf("Problemas na conexao\n");
    return 0;
  }
  printf("Conectei ao servidor\n");

  /* Agora, meu socket funciona como um descritor de arquivo usual */
  send(socket_fd, "PING", 5, 0);
  printf("Escrevi mensagem de ping!\n");
  sleep(1);

  /*** Início - Criando dados para enviar para cliente ***/
  printf("\nCriando structs para receber do server!\n");
  RelevantData D1(5000, 'z');
  RelevantData D2(1000, 'x');

  //std::string buffer(sizeof(DataContainer), '\0');

  std::cout << "Originais:\n";
  D1.dump();
  D2.dump();

  /*** Fim - Criando dados para enviar para cliente ***/

  /* Recebendo resposta */
  char *reply;
  reply = (char *) malloc(sizeof(DataContainer));
  int msg_len;
  msg_len = recv(socket_fd, reply, 100, MSG_DONTWAIT); //edit: mudei de '10' para 'sizeof(DataContainer)'
  if(msg_len > 0) {
    printf("Recebi a mensagem: %d\n", msg_len);
  }
  //edit: recebendo struct do server
  printf("Unserialized (reply) : %s\n", reply);
  std::cout << "Recuperado:\n";
  //std::cout << "Size reply: " << sizeof(reply) << std::endl;
  D2.unserialize(reply);
  D2.dump();
  //printf("Resposta:\n%s\n", reply);
  close(socket_fd);
  return 0;
}
