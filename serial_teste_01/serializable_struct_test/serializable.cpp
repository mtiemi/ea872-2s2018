
#include "serializable.hpp"

#include <string>
#include <cstring>


/*
struct DataContainer {
  float velocidade_x;
  float velocidade_y;
  float posicao_x;
  float posicao_y;
  int tipo;
};

class RelevantData {
  private:
    DataContainer data;

  public:
    RelevantData();
    RelevantData::RelevantData( float velocidade_x,float velocidade_y, float posicao_x, float posicao_y, int tipo);
    RelevantData(std::string buffer_in);
    void serialize(std::string &buffer_out);
    void unserialize(std::string buffer_in);
    void dump();
};

*/

RelevantData::RelevantData() {
};

RelevantData::RelevantData ( float velocidade_x, float velocidade_y,float posicao_x, float posicao_y, int tipo) {

  this->data.velocidade_x = velocidade_x;
  this->data.velocidade_y = velocidade_y;
  this->data.posicao_x = posicao_x;
  this->data.posicao_y = posicao_y;
  this->data.tipo = tipo;

}

RelevantData::RelevantData(std::string buffer_in) {
  this->unserialize(buffer_in);
}

void RelevantData::serialize(std::string &buffer_out) {
  std::memcpy((void*)buffer_out.c_str(), &(this->data), sizeof(DataContainer));
}

void RelevantData::unserialize(std::string buffer_in) {
  std::memcpy(&(this->data), (void*)buffer_in.c_str(), sizeof(DataContainer));
}

void RelevantData::dump() {
  //std::cout << this->data.a << '\n' << this->data.b << '\n';
  std::cout << this->data.velocidade_x << '\n' << this->data.velocidade_y << '\n'<<  this->data.posicao_x << '\n' << this->data.posicao_y << '\n'<< this->data.tipo << '\n';
}
