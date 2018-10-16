//EA872 - LAB4
// Mariane Tiemi Iguti (RA147279) e Gabriela Akemi Shima (RA135819)
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream> //edit: include from playback.cpp
#include <sstream> //edit: include from playback.cpp
#include <string> //edit: include from playback.cpp
#include <random> //edit: include from playback.cpp
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include "oo_model.hpp"

int flag_cresceu = FALSE;

using namespace std::chrono;

Corpo::Corpo( float velocidade_x,
              float velocidade_y,
              float posicao_x,
              float posicao_y,
              int tipo
            ) {

  this->velocidade_x = velocidade_x;
  this->velocidade_y = velocidade_y;
  this->posicao_x = posicao_x;
  this->posicao_y = posicao_y;
  this->tipo = tipo;

}

void Corpo::update( float nova_velocidade_x,
                    float nova_velocidade_y,
                    float nova_posicao_x ,
                    float nova_posicao_y
                  ) {
  this->velocidade_x = nova_velocidade_x;
  this->velocidade_y = nova_velocidade_y;
  this->posicao_x = nova_posicao_x;
  this->posicao_y = nova_posicao_y;

}


float Corpo::get_velocidade_x() {
  return this->velocidade_x;
}

float Corpo::get_velocidade_y() {
  return this->velocidade_y;
}

float Corpo::get_posicao_x() {
  return this->posicao_x;
}

float Corpo::get_posicao_y() {
  return this->posicao_y;
}

int Corpo::get_tipo() {
  return this->tipo;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo( (*corpos)[k]->get_velocidade_x(),\
                          (*corpos)[k]->get_velocidade_y(),\
                          (*corpos)[k]->get_posicao_x(),\
                          (*corpos)[k]->get_posicao_y(),\
                          (*corpos)[k]->get_tipo()
                        );
    this->add_corpo(c);
  }

}


void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}

SnakeController::SnakeController(ListaDeCorpos *ldc) {
  this->lista = ldc;
}

void SnakeController::add_corpo(Corpo *c){
  (this->lista)->add_corpo(c);
}
void SnakeController::update(float deltaT) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  float new_vel_x, new_vel_y, new_pos_x, new_pos_y;
  //float old_pos_x_head, old_pos_y_head;
  float old_pos_x, old_pos_y, old_vel_x, old_vel_y;

  for (int i = 0; i < (*c).size(); i++) {

    //trata atualizacao da posicao da cabeça
    if((*c)[i]->get_tipo() == SNAKE_HEAD) {

      old_pos_x = (*c)[i]->get_posicao_x();
      old_pos_y = (*c)[i]->get_posicao_y();
      old_vel_x = (*c)[i]->get_velocidade_x();
      old_vel_y = (*c)[i]->get_velocidade_y();

      new_vel_x = (*c)[i]->get_velocidade_x();
      new_vel_y = (*c)[i]->get_velocidade_y();
      new_pos_x = (*c)[i]->get_posicao_x() + (float)deltaT * new_vel_x/5000;
      new_pos_y = (*c)[i]->get_posicao_y() + (float)deltaT * new_vel_y/5000;

      //Verifica se comeu a COMIDA

      int i_comida = (int)((*c)[0]->get_posicao_x());
      //std::cout << i_comida <<std::endl;
      int j_comida = (int)((*c)[0]->get_posicao_y());
      //std::cout << j_comida <<std::endl;
      int i_head = (int)(new_pos_x);
      //std::cout << i_head <<std::endl;
      int j_head = (int)(new_pos_y);
      //std::cout << j_head<<std::endl;

       if( i_comida == i_head && j_comida == j_head){
          //Escreve a comida em outra posicao
          srand(time(NULL)); //inicializa random seed
          float new_pos_x1 = rand() % SCREEN_WIDTH ;
          float new_pos_y1 = rand() % SCREEN_HEIGHT;
          (*c)[0]->update(0,0,new_pos_x1,new_pos_y1);
          //Adiciona novo corpo à lista de corpos(cobra)
          flag_cresceu = TRUE;
          // Corpo *cp = new Corpo ( (*c)[(*c).size() - 1]->get_velocidade_x(),\
          //                         (*c)[(*c).size() - 1]->get_velocidade_y(),\
          //                         (*c)[(*c).size() - 1]->get_posicao_x(),\
          //                         (*c)[(*c).size() - 1]->get_posicao_y(),
          //                         SNAKE_BODY
          //                       );
          // this->lista->add_corpo(cp);
      }

      // //Verifica se morreu/comeu
      // std::vector<Corpo *> *cp = this->lista->get_corpos();
      // for (int k = 0; k < (*cp).size(); k++) {
      //
      //
      //   if((*c)[k]->get_tipo() == COMIDA){
      //     int i_comida,j_comida, i_head, j_head;
      //     i_comida = (int)((*c)[0]->get_posicao_x());
      //     j_comida = (int)((*c)[0]->get_posicao_y());
      //     i_head = (int)(new_pos_x);
      //     j_head = (int)(new_pos_y);
      //     if( i_comida == i_head && j_comida == j_head){
      //         flag_comeu = TRUE;
      //         //Escreve a comida em outra posicao
      //         srand(time(NULL)); //inicializa random seed
      //         float new_pos_x = rand() % SCREEN_WIDTH ;
      //         float new_pos_y = rand() % SCREEN_HEIGHT;
      //         (*c)[i]->update(0, 0, new_pos_x, new_pos_y);
      //         flag_comeu = FALSE;
      //
      //         //Adiciona novo corpo à lista de corpos(cobra)
      //     }
      //   }
      // }

      //Essa parte trata as bordas da tela
      if(new_pos_x >= SCREEN_WIDTH && new_vel_x > 0)
        new_pos_x = 0;
      else if (new_pos_x <= 0 && new_vel_x < 0)
        new_pos_x = SCREEN_WIDTH;
      else if (new_pos_y >= SCREEN_HEIGHT && new_vel_y > 0)
        new_pos_y = 0;
      else if (new_pos_y <= 0  && new_vel_y < 0)
        new_pos_y = SCREEN_HEIGHT;

      (*c)[i]->update(new_vel_x, new_vel_y, new_pos_x, new_pos_y);
    }

    //trata atualizacao da posicao do body

    else if((*c)[i]->get_tipo() == SNAKE_BODY) {
      if( (*c)[i]->get_velocidade_x() > 0 ) {
        new_pos_x = old_pos_x - 0.3;
        new_pos_y = old_pos_y;
        new_vel_x = old_vel_x;
        new_vel_y = old_vel_y;
      }
      else if( (*c)[i]->get_velocidade_x() < 0 ) {
        new_pos_x = old_pos_x + 0.3;
        new_pos_y = old_pos_y;
        new_vel_x = old_vel_x;
        new_vel_y = old_vel_y;
      }
      else if( (*c)[i]->get_velocidade_y() > 0 ) {
        new_pos_x = old_pos_x;
        new_pos_y = old_pos_y - 0.3;
        new_vel_x = old_vel_x;
        new_vel_y = old_vel_y;
      }
      else if( (*c)[i]->get_velocidade_y() < 0 ) {
        new_pos_x = old_pos_x;
        new_pos_y = old_pos_y + 0.3;
        new_vel_x = old_vel_x;
        new_vel_y = old_vel_y;
      }

      // new_pos_x = old_pos_x;
      // new_pos_y = old_pos_y;
      // new_vel_x = old_vel_x;
      // new_vel_y = old_vel_y;

      old_pos_x = (*c)[i]->get_posicao_x();
      old_pos_y = (*c)[i]->get_posicao_y();
      old_vel_x = (*c)[i]->get_velocidade_x();
      old_vel_y = (*c)[i]->get_velocidade_y();
      (*c)[i]->update(new_vel_x, new_vel_y, new_pos_x, new_pos_y);
    }

  }
}

void SnakeController::andar_para_cima() {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    if((*c)[i]->get_velocidade_y() > 0);
    else {
      if((*c)[i]->get_tipo() == SNAKE_HEAD ) {
        float new_vel_x = 0;
        float new_vel_y = - VELOCIDADE;
        float new_pos_x = (*c)[i]->get_posicao_x();
        float new_pos_y = (*c)[i]->get_posicao_y();
        (*c)[i]->update(new_vel_x, new_vel_y, new_pos_x, new_pos_y);
      }
    }
  }
}

void SnakeController::andar_para_baixo() {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    if((*c)[i]->get_velocidade_y() < 0);
    else {
      if((*c)[i]->get_tipo() == SNAKE_HEAD ) {
        float new_vel_x = 0;
        float new_vel_y = VELOCIDADE;
        float new_pos_x = (*c)[i]->get_posicao_x();
        float new_pos_y = (*c)[i]->get_posicao_y();
        (*c)[i]->update(new_vel_x, new_vel_y, new_pos_x, new_pos_y);
      }
    }
  }
}

void SnakeController::andar_para_direita() {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    if((*c)[i]->get_velocidade_x() < 0);
    else {
      if((*c)[i]->get_tipo() == SNAKE_HEAD ) {
        float new_vel_x = VELOCIDADE;
        float new_vel_y = 0;
        float new_pos_x = (*c)[i]->get_posicao_x();
        float new_pos_y = (*c)[i]->get_posicao_y();
        (*c)[i]->update(new_vel_x, new_vel_y, new_pos_x, new_pos_y);
      }
    }
  }
}

void SnakeController::andar_para_esquerda() {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    if((*c)[i]->get_velocidade_x() > 0);
    else {
      if((*c)[i]->get_tipo() == SNAKE_HEAD) {
        float new_vel_x = - VELOCIDADE;
        float new_vel_y = 0;
        float new_pos_x = (*c)[i]->get_posicao_x();
        float new_pos_y = (*c)[i]->get_posicao_y();
        (*c)[i]->update(new_vel_x, new_vel_y, new_pos_x, new_pos_y);
      }
    }
  }
}

Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  int i,j;

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  // Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++)
  {
    i = (int) ((*corpos_old)[k]->get_posicao_x()) * (this->maxI / this->maxX);
    j = (int) ((*corpos_old)[k]->get_posicao_y()) * (this->maxI / this->maxX);
    if(move(j, i) != ERR) echochar(' ');  /* Prints character, advances a position */
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++)
  {
    i = (int) ((*corpos)[k]->get_posicao_x()) * (this->maxI / this->maxX);
    j = (int) ((*corpos)[k]->get_posicao_y()) * (this->maxI / this->maxX);
    //if(move(j, i) != ERR) echochar('@');

    if((*corpos)[k]->get_tipo() == COMIDA) {
      if(move(j, i) != ERR) echochar('*');  /* Prints character, advances a position */
    }
    else {
      if(move(j, i) != ERR) echochar('@');  /* Prints character, advances a position */
    }


    // Atualiza corpos antigos
    (*corpos_old)[k]->update(   (*corpos)[k]->get_velocidade_x(),\
                                (*corpos)[k]->get_velocidade_y(),\
                                (*corpos)[k]->get_posicao_x(),\
                                (*corpos)[k]->get_posicao_y()
                             );
  }

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}


/*
class Teclado {
  private:
    char ultima_captura;
    int rodando;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
}

*/

void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
  // Inicializa ncurses
  raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);
}

void Teclado::stop() {
  this->rodando = 0;
  (this->kb_thread).join();
}

char Teclado::getchar() {
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}

using namespace Audio;

Sample::Sample() {

}

Sample::~Sample() {

}

bool Sample::finished() {
  if ( (this->position) >= (this->data.size())) return true;
  else return false;
}

void Sample::load(const char *filename) {
  std::string buffer;
  float fdata;
  std::ifstream input_file;
  unsigned int count = 0;

  input_file.open(filename, std::ios_base::in);
  if (!input_file) {
    std::cerr << "Arquivo " << filename << " nao encontrado" << std::endl;
    return;
  }

  while (std::getline(input_file, buffer) ) {
    std::stringstream(buffer) >> fdata;
    (this->data).push_back(fdata);
    count ++;
  }
  this->position = 0;

  //std::cerr << "Total: " << count << " samples" << std::endl;

}

unsigned int Sample::get_position() {
  return this->position;
}

void Sample::set_position(unsigned int pos) {
  this->position = pos;
}

std::vector<float> Sample::get_data() {
  return this->data;
}



Player::Player() {
  this->playing = false;
  this->audio_sample = NULL;
}

void Player::pause() {
  this->playing = false;
}

Player::~Player() {

}

Sample *Player::get_data() {
  return this->audio_sample;
}

//funcao de callback:
int mix_and_play (const void *inputBuffer, void *outputBuffer,
                  unsigned long framesPerBuffer,
                  const PaStreamCallbackTimeInfo* timeInfo,
                  PaStreamCallbackFlags statusFlags,
                  void *userData )

{
  Player *player = (Player*) userData;
  float *buffer = (float *) outputBuffer;
  Sample *s;
  s = player->get_data();
  if (s != NULL) {
    std::vector<float> data = s->get_data();
    unsigned int pos = s->get_position();

    // Fill the buffer with samples!
    for (int i=0; (i<framesPerBuffer); i++) {
      if (pos < data.size())
        buffer[i] = data[pos];
      else
        buffer[i] = 0;
      //i++;
      pos+=2;
    }
    s->set_position(pos);
  }
  return 0;
}

void Player::play(Sample *audiosample) {
  this->audio_sample = audiosample;
}

void Player::init() {
  PaError err;

  err = Pa_Initialize();
  if( err != paNoError ) {
    std::cerr << "Error on Pa_Initialize()" << std::endl;
    return;
  }

  outputParameters.device = Pa_GetDefaultOutputDevice(); /* Default output device. */
  if (outputParameters.device == paNoDevice) {
    std::cerr << "Error: No default output device on Pa_GetDefaultOutputDevice()" << std::endl;
    return;
  }

  outputParameters.channelCount = 1;                     /* Mono output. */
  outputParameters.sampleFormat = paFloat32;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
  err = Pa_OpenStream( &stream,
                         NULL,      /* No input. */
                         &outputParameters,
                         44100,
                         64,       /* Frames per buffer. */
                         paClipOff, /* We won't output out of range samples so don't bother clipping them. */
                         mix_and_play,
                         this );

  if( err != paNoError ) {
    std::cerr << "Error on Pa_OpenStream()" << std::endl;
    return;
  }

  err = Pa_StartStream( stream );
  if( err != paNoError ) {
    std::cerr << "Error on Pa_StartStream()" << std::endl;
    return;
  }


}

void Player::stop() {
  PaError err;
  err = Pa_StopStream( stream );
  if( err != paNoError ) {
    std::cerr << "Error on Pa_StopStream()" << std::endl;
    return;
  }

  err = Pa_CloseStream( stream );
  if( err != paNoError ) {
    std::cerr << "Error on Pa_StopStream()" << std::endl;
    return;
  }

  Pa_Terminate();

}
