#include <iostream>

class Node {
public:
  size_t pos_x_;
  size_t pos_y_;
  int value_pos_; // Valor de lo que cuesta llegar a esta casilla desde el nodo inicial
  int value_f_; // value_pos_ + lo que cuasta llegar al final (h)


  Node();
  Node(size_t x, size_t y, int v, int f);


};