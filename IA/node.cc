#include "node.h"

Node::Node(){};

Node::Node(size_t x, size_t y, int value) : pos_x_(x), pos_y_(y), value_(value) {};

int Node::H(int x_f, int y_f) {
  return abs(pos_x_ - x_f) + abs(pos_y_ - y_f);
}
