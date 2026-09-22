#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "node.h"

typedef std::vector<std::vector<int>> matrix;

void FillMap(matrix& map, std::ifstream& file) {
  
  std::string line;
  int line_counter{0};

  while(std::getline(file, line)) {
    
    std::vector<int> row;
    std::string value;
    
    for(char c : line) {
      if(c == ' ') {
        row.push_back(std::stoul(value)); // Inserto valor en el vector
        value.clear();

      } else {
        value += c;
      }
    }
    if(!value.empty()) {
      row.push_back(std::stoul(value)); // Para el ultimo valor de la fila
    }
    map.push_back(row); // Inserto la fila

    line_counter++;
  }
}

// Devuelve el indice del nodo con menor coste de la lista
int BuscarNodoMenorCoste(std::vector<Node>& v) { 
  int min{-1};
  int val_min{999};
  for(size_t i = 0; i < v.size(); i++) {
    if(v[i].value_f_ < val_min) {
      min = i;
      val_min = v[i].value_f_;
    }
  }

  return min;
}

bool ExisteNodoEnVector(size_t x, size_t y, std::vector<Node>& v) {
  for(Node n : v) {
    if(n.pos_x_ == x && n.pos_y_ == y) {
      return true;
    }
  }
  return false;
}

int H(int x_i, int y_i, int x_f, int y_f) {
  return abs(x_i - x_f) + abs(y_i - y_f);
}

int main(int argc, char* argv[]){
  if(argc == 2){
    std::string arg1 = argv[1];
    
  } else {
    std::cout << "Modo de empleo: ./busqueda fichero.txt\n";
    return 0;
  }

  std::string nombre_fin = argv[1];
  std::ifstream filein(argv[1]);

  if (!filein) { 
    std::cout << "Error al abrir archivo de entrada\n";
    return 0;
  }

  matrix map;
  std::cout << "Rellenando mapa...\n";
  FillMap(map, filein);
  int map_size_x = map.size();
  int map_size_y = map[0].size();

  // Inicio algoritmo busqueda del camino

  // Inicializo el nodo inicial y busco el final
  size_t pos_x_dest;
  size_t pos_y_dest;
  size_t pos_x_ini;
  size_t pos_y_ini;
  Node nodo_inicial;
  int n_row{0};
  for(const auto& row : map) {
    int n_col{0};
    for(int value : row) {
      if(value == 10) {
        pos_x_dest = n_col;
        pos_y_dest = n_row;
      } else if(value == 0) {
        pos_x_ini = n_row;
        pos_y_ini = n_col;
      }
      n_col++;
    }
    n_row++;
  }
  nodo_inicial = Node(pos_x_ini, pos_y_ini, 0, H(pos_x_ini,pos_y_ini, pos_x_dest, pos_y_dest));

  std::vector<Node> A; // Lista Abierta
  std::vector<Node> C; // Lista Cerrada

  A.push_back(nodo_inicial); // Inicializanos la lista solo con el nodo inicial

  std::cout << "Iniciando iteraciones...\n";
  // Comienzo de las iteraciones
  while (true) { 

    int indice_menor_coste = BuscarNodoMenorCoste(A);// Buscamos el nodo con menor coste de la abierta
    if(indice_menor_coste < 0) {
      std::cout << "No quedan nodos por explorar, finalizando iteraciones...\n";
      break;
    }
    Node aux = A[indice_menor_coste]; 
    A.erase(A.begin() + indice_menor_coste); // quitar de la abrieta y meter en la cerrada
    C.push_back(aux);

    if(aux.pos_x_ == pos_x_dest && aux.pos_y_ == pos_y_dest) { // Si esta en el estado final acaba
      std::cout << "Llegó al estado final\nLista de los nodos cerrados:\n";
      int contador{0};
      for(Node n : C) {
        std::cout << contador << "(" << n.pos_x_ + 1 << "," << n.pos_y_ + 1 << ")\n";
        contador++;
      }
      // Rellenar
      break;
    }

    int aux_x = aux.pos_x_;
    int aux_y = aux.pos_y_;
    // Explorar e insertar movimientos posibles
    // Si no es un obstaculo, esta dentro del mapa y no esta en los nodos cerrados es valido
    if(aux_x + 1 < map_size_x && map[aux_x + 1][aux_y] != -1 && !ExisteNodoEnVector(aux_x + 1, aux_y, C)) {
      int v = aux.value_pos_ + map[aux_x + 1][aux_y];
      int f = v + H(aux_x + 1,aux_y, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x + 1, aux_y, v, f);
      A.push_back(temp);
    }
    if(aux_x - 1 >= 0 && map[aux_x - 1][aux_y] != -1 && !ExisteNodoEnVector(aux_x - 1, aux_y, C)) {
      int v = aux.value_pos_ + map[aux_x - 1][aux_y];
      int f = v + H(aux_x - 1,aux_y, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x - 1, aux_y, v, f);
      A.push_back(temp);
    }
    if(aux_y + 1 < map_size_y && map[aux_x][aux_y + 1] != -1 && !ExisteNodoEnVector(aux_x, aux_y + 1, C)) {
      int v = aux.value_pos_ + map[aux_x][aux_y + 1];
      int f = v + H(aux_x,aux_y + 1, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x, aux_y + 1, v, f);
      A.push_back(temp);
    }
    if(aux_y - 1 >= 0 && map[aux_x][aux_y - 1] != -1 && !ExisteNodoEnVector(aux_x, aux_y - 1, C)) {
      int v = aux.value_pos_ + map[aux_x][aux_y - 1];
      int f = v + H(aux_x,aux_y - 1, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x, aux_y - 1, v, f);
      A.push_back(temp);
    }


  }


  // Test
  // for(const auto& row : map) {
  //   for(int value : row) {
  //     std::cout << value << " ";
  //   }
  //   std::cout << "\n";
  // }

  return 0;
}