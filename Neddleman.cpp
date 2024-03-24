//Falta sacar las demas alineaciones

#include <iostream>
#include <vector>
#include <algorithm> // Para la función reverse
using namespace std;

enum Direction { UP, DIAGONAL, LEFT };

int Max(int arriba, int esquina, int izquierda) {
    int max = arriba;
    if (max < esquina) max = esquina;
    if (max < izquierda) max = izquierda;
    return max;
}

void Needleman(string CadenaA, string CadenaB, int tamA, int tamB) {
    //tamA++; tamB++; // Incrementamos para incluir el espacio vacío
    int Matriz[tamA+1][tamB+1];
  vector<vector<vector<Direction>>> Direcciones(tamA + 1, vector<vector<Direction>>(tamB + 1));


    int menos_hor = -2;
    int menos_vert = -2;

    // Inicializamos la matriz de puntuaciones
    for(int i = 0; i <= tamA; i++) {
        for(int j = 0; j <= tamB; j++) {
            if (i == 0 && j == 0) {
                Matriz[i][j] = 0;
            } else if (i == 0 && j != 0) {
                Matriz[i][j] = menos_hor;
                menos_hor -= 2;
                Direcciones[i][j].push_back(LEFT);
            } else if (i != 0 && j == 0) {
                Matriz[i][j] = menos_vert;
                menos_vert -= 2;
                Direcciones[i][j].push_back(UP);
            }
        }
    }

    // Completamos la matriz
    for(int i = 1; i <= tamA; i++) {
        for(int j = 1; j <= tamB; j++) {
            int match = (CadenaA[i] == CadenaB[j]) ? 1 : -1;
            int diagonalScore = Matriz[i - 1][j - 1] + match;
            int upScore = Matriz[i - 1][j] - 2;
            int leftScore = Matriz[i][j - 1] - 2;

            int maxScore = Max(upScore, diagonalScore, leftScore);

            // Guardamos la dirección de la cual llegamos a la celda
            if (maxScore == upScore) {
                Direcciones[i][j].push_back(UP);
            }
            if (maxScore == diagonalScore) {
                Direcciones[i][j].push_back(DIAGONAL);
            } 
            if (maxScore == leftScore){
                Direcciones[i][j].push_back(LEFT);
            }

            Matriz[i][j] = maxScore;
        }
    }

    // Imprimimos la matriz de puntuaciones
    cout << "Matriz de puntuaciones:" << endl;
    for(int i = 0; i < tamA; i++) {
        for(int j = 0; j < tamB; j++) {
            cout << Matriz[i][j] << " ";
        }
        cout << endl;
    }

    // Imprimimos la matriz de direcciones
    cout << endl << "Matriz de direcciones:" << endl;
    for(int i = 0; i < tamA; i++) {
        for(int j = 0; j < tamB; j++) {
          cout << "Celda (" << i << ", " << j << "): ";
          for (const auto& dir : Direcciones[i][j]) {
              if (dir == UP) cout << "UP ";
              if (dir == DIAGONAL) cout << "DIAGONAL ";
              if (dir == LEFT) cout << "LEFT ";
          }
          cout << endl;
        }
        cout << endl;
    }


    //Encontramos las secuencias
    int fila = tamA-1;
    int columna = tamB-1;
    string Secuencia1, Secuencia2;
    while(fila>0 and columna>0){
      
      if(Direcciones[fila][columna][0]==DIAGONAL){
        Secuencia1.push_back(CadenaA[fila]);
        Secuencia2.push_back(CadenaB[columna]);
        fila--;columna--;
      }
      else if (Direcciones[fila][columna][0]==UP){
        Secuencia1.push_back(CadenaA[fila]);
        Secuencia2.push_back('-');
        fila--;
      }

      else if (Direcciones[fila][columna][0]==LEFT){
        Secuencia1.push_back('-');
        Secuencia2.push_back(CadenaB[columna]);
        columna--;
      }
        
    }
    reverse(Secuencia1.begin(), Secuencia1.end());
    reverse(Secuencia2.begin(), Secuencia2.end());
    cout<<Secuencia1<<"\n"<<Secuencia2<<endl;
}

int main() {
    string CadenaA = "-AAAC", CadenaB = "-AGC";
    int tamA = CadenaA.length(), tamB = CadenaB.length();
  
    Needleman(CadenaA, CadenaB, tamA, tamB);

    return 0;
}
