
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "serializable.hpp"
#include "oo_model.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int socket_fd;

void *receber_respostas(void *parametros) {
  /* Recebendo resposta */
  char reply[MAX_MSG_STRING];
  int msg_len;
  int msg_num;
  msg_num = 0;

  /* Inicializando  as variáveis de recebimento de dados */
  //std::cout <<"\n========================================================\n";
  //printf("\nCriando structs para receber do server!\n");
  //RelevantData D1(1.234, 2.345,1.234, 2.345,11);
  //RelevantData D2(4.321, 3.456,4.321, 3.456,12);
  //std::cout << "Originais:\n";
  //D1.dump();
  //D2.dump();
  //std::cout <<"\n========================================================\n";

  ListaDeCorpos *l = new ListaDeCorpos();
  RelevantData DadosCorpo(0,0,0,0,0);
  while(1) {

    msg_len = recv(socket_fd, reply, MAX_MSG_STRING, MSG_DONTWAIT);

    if (msg_len > 0) {
      //printf("[%d][%d] RECEBI:\n%s\n", msg_num, msg_len, reply);
      char *ptr = strtok (reply,"#");

      std::vector<Corpo *> *c_ptr = l->get_corpos();
      int h = 0;

      while(ptr != NULL) {
        //printf ("OLAR STRING: %s\n",ptr);
        DadosCorpo.unserialize(ptr);
        //DadosCorpo.dump();
        ptr = strtok (NULL,"#\n\0");

        if( h < (*c_ptr).size() ) { //caso os corpos já existam ele apenas atualiza os dados

          (*c_ptr)[h]->update(  DadosCorpo.get_velocidade_x(),\
                                DadosCorpo.get_velocidade_y(),\
                                DadosCorpo.get_posicao_x(),\
                                DadosCorpo.get_posicao_y() );

          h++;
        }
        else { //acabaram os corpos existentes, adicionar novos corpos
          //printf("Vou adicionar corpo novo!\n");
          Corpo *c1 = new Corpo(DadosCorpo.get_velocidade_x(),\
                                DadosCorpo.get_velocidade_y(),\
                                DadosCorpo.get_posicao_x(),\
                                DadosCorpo.get_posicao_y(),\
                                DadosCorpo.get_tipo() );
          l->add_corpo(c1);
          h++;
        }


      }
      //Imprime lista de Corpos
      // printf("========== Lista de Corpos ==========\n");
      // //std::vector<Corpo *> *c_ptr = l->get_corpos();
      // for( h = 0; h < (*c_ptr).size(); h++)
      //   printf("%f %f %f %f %d\n", (*c_ptr)[h]->get_velocidade_x(), (*c_ptr)[h]->get_velocidade_y(), (*c_ptr)[h]->get_posicao_x(), (*c_ptr)[h]->get_posicao_y(), (*c_ptr)[h]->get_tipo());

      msg_num++;
    }
  }

}

int main() {
  struct sockaddr_in target;
  pthread_t receiver;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  //printf("Socket criado\n");

  //String que recebe o buffer do servidor
  char *reply;
  reply = (char *) malloc(50);
  int msg_len;

  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  inet_aton("127.0.0.1", &(target.sin_addr));
  //printf("Tentando conectar\n");
  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    //printf("Problemas na conexao\n");
    return 0;
  }
  //printf("Conectei ao servidor\n");

  pthread_create(&receiver, NULL, receber_respostas, NULL);


  //inicia as coisas do SNAKE_BODY
  initscr();
  Teclado *teclado = new Teclado();
  teclado->init();

  while(1) {
  /* Agora, meu socket funciona como um descritor de arquivo usual */
   char c = 'k';
  c = teclado->getchar(); //envia caracter o tempo todo para o server
  if(c == 'w' || c == 's' || c == 'a' || c == 'd'){

    send(socket_fd, &c, 1, 0);
    // player->play(asample);
    // asample->set_position(0);
  }
  else if(c == 'q') {
    send(socket_fd, &c, 1, 0);
    // player->play(asample);
    // asample->set_position(0);
    break;
  }
  else { send(socket_fd, "k", 1, 0);}

  //send(socket_fd, &c, 1, 0);

  //printf("Escrevi mensagem de %c!\n", c);
  sleep(1); //mudar para ms
  //std::this_thread::sleep_for (std::chrono::milliseconds(100));
}
  //tela->stop();
  teclado->stop();
  return 0;
}
