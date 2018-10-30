
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "serializable.hpp"
#include "oo_model.hpp"

// using namespace std::chrono;
// uint64_t get_now_ms() {
// return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
// }



int main() {
  // // Inicializando o Teclado
  // Teclado *teclado = new Teclado();
  // teclado->init();
  // initscr();			       /* Start curses mode 		*/

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

  /*** Início - Criando dados para enviar para cliente ***/
  printf("\nCriando structs para receber do server!\n");
  RelevantData D1(1.234, 2.345,1.234, 2.345,11);
  RelevantData D2(4.321, 3.456,4.321, 3.456,12);
  //std::string buffer(sizeof(DataContainer), '\0');
  std::cout << "Originais:\n";
  D1.dump();
  D2.dump();
  //String que recebe o buffer do servidor
  char *reply;
  reply = (char *) malloc(50);
  int msg_len;

  char c;

while(1) {

  c = getchar();
  if(c == 'w' || c == 's' || c == 'a' || c == 'd' || c == 'q'){
  }
  else {
    c = '0';
  }
    printf("Escrevi mensagem: %c!\n", c);
    //***** A partir daqui, ele envia dados para o servidor
    /* Agora, meu socket funciona como um descritor de arquivo usual */
    send(socket_fd, &c, 1, 0);
    //send(socket_fd, "PING", 5, 0);

    sleep(1);

    /* Recebendo resposta */
    msg_len = recv(socket_fd, reply, 50, MSG_DONTWAIT);
    if(msg_len > 0) {
    printf("Recebi a mensagem: %d\n", msg_len);
    }
    //edit: recebendo struct do server
    //printf("Unserialized (reply) : %s\n", reply);
    std::cout << "Recuperado:\n";
    //std::cout << "Size reply: " << sizeof(reply) << std::endl;
    D2.unserialize(reply);
    D2.dump();
    //printf("Resposta:\n%s\n", reply);

	c = '0'; //edit: zera comando do teclado
     std::this_thread::sleep_for (std::chrono::milliseconds(100));
}

  close(socket_fd);
  //endwin();
  //teclado->stop();
  return 0;
}
