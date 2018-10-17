
#include "serializable.hpp"

#include <string>
#include <cstring>


/*
class RelevantData {
  private:
    int a;
    char b;

  public:
    RelevantData(int a, char b);
    RelevantData(std::string serial_data);
    std::string serialize();
};
*/

RelevantData::RelevantData() {
};

RelevantData::RelevantData( float velocidade_x,
                            float velocidade_y,
                            float posicao_x,
                            float posicao_y,
                            int tipo) {

  this->data.velocidade_x = velocidade_x;
  this->data.velocidade_y = velocidade_Y;
  this->data.posicao_x = posicao_x;
  this->data.posicao_y = posicao_Y;
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
  std::cout << this->data.a << '\n' << this->data.b << '\n';
}
