// EA872 - LAB07
// Mariane Tiemi Iguti (RA147279) e Gabriela Akemi Shima (RA135819)

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

  //edit: inicializa o Player
    uint64_t t0_player, t1_player;

    Audio::Sample *asample;
    asample = new Audio::Sample();
    asample->load("assets/blip.dat");

    Audio::Player *player;
    player = new Audio::Player();
    player->init();


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

    Tela *tela = new Tela(l, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT);
    tela->init();

    Teclado *teclado = new Teclado();
    teclado->init();

    uint64_t t0;
    uint64_t t1;
    uint64_t deltaT;
    uint64_t T;

    int i = 0;

    T = get_now_ms();
    t1 = T;

  while(1){
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    // Atualiza modelo
    f->update(deltaT);
    //Atualiza tela
    tela->update();
  // edit: Lê o teclado
    char c = teclado->getchar();

    if(c == 'w' || c == 's' || c == 'a' || c == 'd' || c == 'q'){  //edit: envia apenas esses caracteres como entrada

      if (c=='w') {
        f->andar_para_cima();
        player->play(asample);
        asample->set_position(0);
      }

      if (c=='s') {
        f->andar_para_baixo();
        player->play(asample);
        asample->set_position(0);
      }

      if (c=='d') {
        f->andar_para_direita();
        player->play(asample);
        asample->set_position(0);
      }

      if (c=='a') {
        f->andar_para_esquerda();
        player->play(asample);
        asample->set_position(0);
      }

      if (c=='q') {
        break;
      }

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
       //printf("Conectei ao servidor\n");

        /* Agora, meu socket funciona como um descritor de arquivo usual */

        send(socket_fd, &c, 1, 0); //edit: Envia char
        //printf("Escrevi mensagem de ping!\n");
        //sleep(1);

        /* Recebendo resposta */
        char reply[10];
        recv(socket_fd, reply, 10, 0);
        //printf("Resposta:\n%s\n", reply);
        close(socket_fd);

    }
      std::this_thread::sleep_for (std::chrono::milliseconds(100));
      i++;
    }
  endwin();
  tela->stop();
  teclado->stop();
  return 0;
}
