#ifndef SERIALIZABLE_HPP
#define SERIALIZABLE_HPP

#include <fstream>
#include <iostream>
#include <string>

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
    RelevantData::RelevantData( float velocidade_x,float velocidade_y,
                                float posicao_x, float posicao_y, int tipo);
    RelevantData(std::string buffer_in);
    void serialize(std::string &buffer_out);
    void unserialize(std::string buffer_in);
    void dump();
};

#endif
