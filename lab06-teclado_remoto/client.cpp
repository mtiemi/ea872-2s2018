#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>

#include "oo_model.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}


int main() {
  int socket_fd;
  struct sockaddr_in target;
  
  // Inicializando o Teclado
  Teclado *teclado = new Teclado();
  teclado->init();
   initscr();			       /* Start curses mode 		*/
  while(1){
  // Lê o teclado
      char c = teclado->getchar();
      
    if(c == 'w' || c == 's' || c == 'a' || c == 'd' || c == 'q'){  
  
    
        socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        //printf("Socket criado\n");

        target.sin_family = AF_INET;
        target.sin_port = htons(3001);
        inet_aton("127.0.0.1", &(target.sin_addr));
        //printf("Tentando conectar\n");
        if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
          //  printf("Problemas na conexao\n");
            return 0;
        }
       // printf("Conectei ao servidor\n");

        /* Agora, meu socket funciona como um descritor de arquivo usual */
        
        send(socket_fd, &c, 1, 0);
        //printf("Escrevi mensagem de ping!\n");
        sleep(1);

        /* Recebendo resposta */
        char reply[10];
        recv(socket_fd, reply, 10, 0);
        //printf("Resposta:\n%s\n", reply);
        close(socket_fd);
        
    } 
      std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
  endwin();
  teclado->stop();
  return 0;
}

