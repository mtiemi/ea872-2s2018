
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "serializable.hpp"
#include "oo_model.hpp"

int socket_fd;

void *receber_respostas(void *parametros) {
  /* Recebendo resposta */
  char reply[MAX_MSG_STRING];
  int msg_len;
  int msg_num;
  msg_num = 0;

  /* Inicializando  as variáveis de recebimento de dados */
  std::cout <<"\n========================================================\n";
  printf("\nCriando structs para receber do server!\n");
  RelevantData D1(1.234, 2.345,1.234, 2.345,11);
  RelevantData D2(4.321, 3.456,4.321, 3.456,12);
  //std::string buffer(sizeof(DataContainer), ' ');
  std::cout << "Originais:\n";
  D1.dump();
  D2.dump();
  std::cout <<"\n========================================================\n";

  while(1) {

    msg_len = recv(socket_fd, reply, MAX_MSG_STRING, MSG_DONTWAIT);

    if (msg_len > 0) {
      printf("[%d][%d] RECEBI:\n%s\n", msg_num, msg_len, reply);
      std::cout << "Recuperado:\n";
      std::cout << "Size reply: " << sizeof(reply) << std::endl;
      char *ptr = strtok (reply,"#");
      while(ptr != NULL) {
        printf ("OLAR STRING: %s\n",ptr);
        D2.unserialize(ptr);
        D2.dump();
        ptr = strtok (NULL,"#\n\0");
        //D2.dump();
      }
      //D2.unserialize(reply);
      //D2.dump();
      msg_num++;
    }
  }

}

int main() {
  struct sockaddr_in target;
  pthread_t receiver;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");

  //String que recebe o buffer do servidor
  char *reply;
  reply = (char *) malloc(50);
  int msg_len;

  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(target.sin_addr));
  printf("Tentando conectar\n");
  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    printf("Problemas na conexao\n");
    return 0;
  }
  printf("Conectei ao servidor\n");

  pthread_create(&receiver, NULL, receber_respostas, NULL);

  while(1) {
  /* Agora, meu socket funciona como um descritor de arquivo usual */
  send(socket_fd, "PING", 5, 0);
  printf("Escrevi mensagem de ping!\n");
  sleep(3);
  }
  return 0;
}
