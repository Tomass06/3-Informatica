#include "node.h"

Node::Node(){};

Node::Node(int x, int y, int value_pos, int value_f, int parent_x, int parent_y)
    : pos_x_(x), pos_y_(y), value_pos_(value_pos), value_f_(value_f), parent_x_(parent_x), parent_y_(parent_y) {}
