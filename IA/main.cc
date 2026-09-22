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
  int min{0};
  int val_min{-1};
  for(int i = 0; i < v.size(); i++) {
    if(v[i].value_ < val_min) {
      min = i;
      val_min = v[i].value_;
    }
  }

  return min;
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

  // Busqueda del camino


  int pos_x_dest;
  int pos_y_dest;
  Node nodo_inicial;
  int n_row{0};
  for(const auto& row : map) {
    int n_col{0};
    for(int value : row) {
      if(value == 10) {
        pos_x_dest = n_col;
        pos_y_dest = n_row;
      } else if(value == 0) {
        nodo_inicial = Node(n_row, n_col, value);
      }
      n_col++;
    }
    n_row++;
  }

  std::vector<Node> A; // Lista Abierta
  std::vector<Node> C; // Lista Cerrada
  std::vector<Node> C; // Lista Cerrada

  A.push_back(nodo_inicial); // Inicializanos la lista solo con el nodo inicial


  while (true) { 

    Node aux = A[BuscarNodoMenorCoste(A)]; // Empezamos con el primer nodo



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