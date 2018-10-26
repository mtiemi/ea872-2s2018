
#include "serializable.hpp"
#include <stdio.h>
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

RelevantData::RelevantData(int a, char b) {
  this->data.a = a;
  this->data.b = b;
}

RelevantData::RelevantData(std::string buffer_in) {
  this->unserialize(buffer_in);
}

void RelevantData::serialize(std::string &buffer_out) {
  //std::memcpy((void*)buffer_out.c_str(), &(this->data), sizeof(DataContainer));
  int c = this->data.a;
  char d = this->data.b;
  sprintf((char*) buffer_out.c_str() , "%d %c\n", c, d);
}

void RelevantData::unserialize(std::string buffer_in) {
  //std::memcpy(&(this->data), (void*)buffer_in.c_str(), sizeof(DataContainer));
  int c;
  char d;
  sscanf(buffer_in.c_str(), "%d %c", &c, &d);
  this->data.a = c;
  this->data.b = d;
}

void RelevantData::dump() {
  std::cout << this->data.a << '\n' << this->data.b << '\n';
}
