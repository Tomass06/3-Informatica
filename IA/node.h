#include <iostream>

class Node {
public:
  int pos_x_;
  int pos_y_;
  int value_pos_; // Valor de lo que cuesta llegar a esta casilla desde el nodo inicial
  int value_f_; // value_pos_ + lo que cuasta llegar al final (h)
  int parent_x_;
  int parent_y_;


  Node();
  Node(int x, int y, int v, int f, int parent_x = -1, int parent_y = -1);


};