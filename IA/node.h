#include <iostream>

class Node {
public:
  int value_;
  size_t pos_x_;
  size_t pos_y_;



  Node();
  Node(size_t x, size_t y, int value);

  int H(int x_f, int y_f);

};