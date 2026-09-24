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

bool ExisteNodoEnVector(int x, int y, std::vector<Node>& v) {
  for(Node n : v) {
    if(n.pos_x_ == x && n.pos_y_ == y) {
      return true;
    }
  }
  return false;
}

int BuscarNodo(int x, int y, const std::vector<Node>& v) {
  for(size_t i = 0; i < v.size(); i++) {
      if(v[i].pos_x_ == x && v[i].pos_y_ == y) {
        return i;
      }
  }
  return -1;
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

  std::cout << "Selecciona la opción\n(1)Guardar en un fichero formato visual\n"
            << "(2)Mostrar y guardar en un fichero iteracion a iteracion\n";
  int opcion;
  std::cin >> opcion;
  if(opcion != 1 && opcion != 2) {
    std::cout << "Opción no válida\n";
    return 1;
  }
  std::cout << "Introduce nombre del fichero: ";
  std::string fileout;
  std::cin >> fileout;
  std::ofstream file(fileout);
  if (!file.is_open()) {
      std::cout << "No se pudo abrir el archivo." << std::endl;
      return 1;
  }


  matrix map;
  std::cout << "Rellenando mapa...\n";
  FillMap(map, filein);
  filein.close();
  int map_size_x = map[0].size();
  int map_size_y = map.size();

  // Inicio algoritmo busqueda del camino

  // Inicializo el nodo inicial y busco el final
  int pos_x_dest;
  int pos_y_dest;
  int pos_x_ini;
  int pos_y_ini;
  Node nodo_inicial;
  int n_row{0};
  for(const auto& row : map) {
    int n_col{0};
    for(int value : row) {
      if(value == 10) {
        pos_x_dest = n_col;
        pos_y_dest = n_row;
        map[n_row][n_col] = 2; // Cambio el valor de la casilla final para que no afecte a la busqueda
      } else if(value == 0) {
        pos_x_ini = n_col;
        pos_y_ini = n_row;
      }
      n_col++;
    }
    n_row++;
  }
  nodo_inicial = Node(pos_x_ini, pos_y_ini, 0, H(pos_x_ini,pos_y_ini, pos_x_dest, pos_y_dest));

  std::vector<Node> A; // Lista Abierta
  std::vector<Node> C; // Lista Cerrada
  std::vector<Node> camino; // Camino final optimo usando los padres de el nodo final

  A.push_back(nodo_inicial); // Inicializanos la lista solo con el nodo inicial

  std::cout << "Iniciando iteraciones...\n";
  int contador_iter{0};
  if(opcion == 2) {
    std::cout << "Iteración 0\n---------\nAbiertos = (" << nodo_inicial.pos_x_ << "," 
              << nodo_inicial.pos_y_ << ")\nCerrados =\n----------------\n";
    file << "Iteración 0\n---------\nAbiertos = (" << nodo_inicial.pos_x_ << "," 
              << nodo_inicial.pos_y_ << ")\nCerrados =\n----------------\n";
  }
  // Comienzo de las iteraciones
  while (true) { 
    contador_iter++;
    int indice_menor_coste = BuscarNodoMenorCoste(A);// Buscamos el nodo con menor coste de la abierta
    if(indice_menor_coste < 0) {
      std::cout << "No quedan nodos por explorar, finalizando iteraciones...\n";
      break;
    }
    Node aux = A[indice_menor_coste]; 
    A.erase(A.begin() + indice_menor_coste); // quitar de la abrieta y meter en la cerrada
    C.push_back(aux);

    if(aux.pos_x_ == pos_x_dest && aux.pos_y_ == pos_y_dest) { // Si esta en el estado final acaba
      std::cout << "Llegó al estado final\n";
      // Reconstruir camino optimo A*
      Node actual = aux;
      while(true) {
        camino.push_back(actual);
        // Hemos llegado al inicial
        if(actual.pos_x_ == pos_x_ini && actual.pos_y_ == pos_y_ini) {
          break;
        }

        int indice = BuscarNodo(actual.parent_x_, actual.parent_y_, C);

        if(indice == -1) {
            std::cout << "Error reconstruyendo el camino\n";
            break;
        }
        actual = C[indice];
      }
      break;
    }

    int aux_x = aux.pos_x_;
    int aux_y = aux.pos_y_;
    // Explorar e insertar movimientos posibles
    // Si no es un obstaculo, esta dentro del mapa y no esta en los nodos cerrados es valido
    if(aux_x + 1 < map_size_x && map[aux_y][aux_x + 1] != -1 
      && !ExisteNodoEnVector(aux_x + 1, aux_y, C)
      && !ExisteNodoEnVector(aux_x + 1, aux_y, A)) {
      int v = aux.value_pos_ + map[aux_y][aux_x + 1];
      int f = v + 2 * H(aux_x + 1,aux_y, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x + 1, aux_y, v, f, aux_x, aux_y);
      A.push_back(temp);
    } 
    if(aux_x - 1 >= 0 && map[aux_y][aux_x - 1] != -1 
      && !ExisteNodoEnVector(aux_x - 1, aux_y, C)
      && !ExisteNodoEnVector(aux_x - 1, aux_y, A)) {
      int v = aux.value_pos_ + map[aux_y][aux_x - 1];
      int f = v + 2 * H(aux_x - 1,aux_y, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x - 1, aux_y, v, f, aux_x, aux_y);
      A.push_back(temp);
    } 
    if(aux_y + 1 < map_size_y && map[aux_y + 1][aux_x] != -1 
      && !ExisteNodoEnVector(aux_x, aux_y + 1, C)
      && !ExisteNodoEnVector(aux_x, aux_y + 1, A)) {
      int v = aux.value_pos_ + map[aux_y + 1][aux_x];
      int f = v + 2 * H(aux_x,aux_y + 1, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x, aux_y + 1, v, f, aux_x, aux_y);
      A.push_back(temp);
    } 
    if(aux_y - 1 >= 0 && map[aux_y - 1][aux_x] != -1 
      && !ExisteNodoEnVector(aux_x, aux_y - 1, C)
      && !ExisteNodoEnVector(aux_x, aux_y - 1, A)) {
      int v = aux.value_pos_ + map[aux_y - 1][aux_x];
      int f = v + 2 * H(aux_x,aux_y - 1, pos_x_dest, pos_y_dest);
      Node temp = Node(aux_x, aux_y - 1, v, f, aux_x, aux_y);
      A.push_back(temp);
    } 
    if(opcion == 2) {
      file << "Iteración "<< contador_iter << "\n---------\nAbiertos = ";
      std::cout << "Iteración "<< contador_iter << "\n---------\nAbiertos = ";
      for(Node n : A) {
        file << "(" << n.pos_x_ << "," << n.pos_y_ << ")  ";
        std::cout << "(" << n.pos_x_ << "," << n.pos_y_ << ")  ";
      }
      std::cout << "\nCerrados = ";
      file << "\nCerrados = ";
      for(Node n : C) {
        file << "(" << n.pos_x_ << "," << n.pos_y_ << ")  ";
        std::cout << "(" << n.pos_x_ << "," << n.pos_y_ << ")  ";
      }
      std::cout << "\n----------------\n";
      file << "\n----------------\n";
    }
  }

  if(opcion == 1 && !camino.empty()) {
    for(size_t i{0}; i < map.size(); i++) {
      for(size_t j{0}; j < map[i].size(); j++) {
        if(ExisteNodoEnVector(j, i, camino)) {
          file << "* ";
        } else {
          file << map[i][j] << " ";
        }
      }
      file << "\n";
    }
    std::cout << "Resultado guardado en: " << fileout << std::endl;
  } else if (opcion == 2 && !camino.empty()) { 
    std::cout << "Camino: ";
    file << "Camino: ";
    int cam_sz = camino.size();
    for(int i{cam_sz-1}; i >= 0; i--) {
      std::cout << "(" << camino[i].pos_x_ << "," << camino[i].pos_y_ << ") ";
      file << "(" << camino[i].pos_x_ << "," << camino[i].pos_y_ << ") ";
      if(i != 0) {
        std::cout << " -> ";
        file << " -> ";
      }
    }
    std::cout << "\nCoste: " << camino[0].value_pos_ << "\n";
  }

  file.close();
  return 0;
}