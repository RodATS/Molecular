#include <iostream>
#include <vector>
#include <algorithm> // Para la función reverse
#include <functional> // Para std::function
using namespace std;

enum Direction { UP, DIAGONAL, LEFT };

struct Alignment {
    string sequenceA;
    string sequenceB;
    int score;
    int peso;
};

// Función para comparar dos estructuras Alignment por peso
bool compareByPeso(const Alignment &a, const Alignment &b) {
    return a.peso < b.peso;
}

int Max(int arriba, int esquina, int izquierda) {
    int max = arriba;
    if (max < esquina) max = esquina;
    if (max < izquierda) max = izquierda;
    return max;
}

void Needleman(string CadenaA, string CadenaB, int tamA, int tamB) {
    // Incrementamos el tamaño de las cadenas para incluir el espacio vacío
    tamA++;
    tamB++;

    for(int fila=0; fila < tamA; fila++){
      for(int columna=0; columna<tamB; columna++){
        if(CadenaA[fila]==CadenaB[columna]){
          cout << "*\t";
        }
        else{
          cout<<" \t";
        }
      }
      cout<<endl;
    }

    cout<<endl;

  
      vector<vector<int>> Matriz(tamA, vector<int>(tamB));
  vector<vector<vector<Direction>>> Direcciones(tamA, vector<vector<Direction>>(tamB));

    
    int menos_hor = -2;
    int menos_vert = -2;

    // Inicializamos la matriz de puntuaciones
    for(int i = 0; i < tamA; i++) {
        for(int j = 0; j < tamB; j++) {
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
    for(int i = 1; i < tamA; i++) {
        for(int j = 1; j < tamB; j++) {
            int match = (CadenaA[i-1] == CadenaB[j-1]) ? 1 : -1; // Ajustamos los índices
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
            cout << Matriz[i][j] << "\t";
        }
        cout << endl;
    }

    // // Imprimimos la matriz de direcciones
    // cout << endl << "Matriz de direcciones:" << endl;
    // for(int i = 0; i < tamA; i++) {
    //     for(int j = 0; j < tamB; j++) {
    //       cout << "Celda (" << i << ", " << j << "): ";
    //       for (const auto& dir : Direcciones[i][j]) {
    //           if (dir == UP) cout << "UP ";
    //           if (dir == DIAGONAL) cout << "DIAGONAL ";
    //           if (dir == LEFT) cout << "LEFT ";
    //       }
    //       cout << endl;
    //     }
    //     cout << endl;
    // }


    // Función para encontrar todas las alineaciones posibles
    vector<Alignment> alignments;

    function<void(string, string, int, int, string, string, int, int)> backtrack = [&](string seqA, string seqB, int row, int col, string newSeqA, string newSeqB, int score, int peso) {
        if (row == 0 && col == 0) {
            reverse(seqA.begin(), seqA.end());
            reverse(seqB.begin(), seqB.end());
            alignments.push_back({seqA, seqB, score, peso});
            return;
        }

        for (const auto& dir : Direcciones[row][col]) {
            string updatedSeqA = newSeqA;
            string updatedSeqB = newSeqB;
            int newRow = row;
            int newCol = col;
            int newScore = score;
            int newPeso = peso;

            if (dir == DIAGONAL) {
                updatedSeqA += CadenaA[row-1]; // Ajustamos los índices
                updatedSeqB += CadenaB[col-1]; // Ajustamos los índices
                newRow--;
                newCol--;
                newScore += (CadenaA[row-1] == CadenaB[col-1]) ? 1 : -1; // Ajustamos los índices
                
            } else if (dir == UP) {
                updatedSeqA += CadenaA[row-1]; // Ajustamos los índices
                updatedSeqB += '-';
                newRow--;
                newScore -= 2;
            } else if (dir == LEFT) {
                updatedSeqA += '-';
                updatedSeqB += CadenaB[col-1]; // Ajustamos los índices
                newCol--;
                newScore -= 2;
            }

            newPeso += Matriz[row][col];

            backtrack(seqA + (updatedSeqA.back() == '-' ? "-" : string(1, CadenaA[row-1])), 
                      seqB + (updatedSeqB.back() == '-' ? "-" : string(1, CadenaB[col-1])),
                      newRow, newCol, updatedSeqA, updatedSeqB, newScore, newPeso);
        }
    };



   // Ajustamos los índices para empezar desde el último elemento
  backtrack("", "", tamA-1, tamB-1, "", "", 0, 0);


    
    // Mostrar todas las alineaciones

    // Ordenar el vector por el campo peso de menor a mayor
    sort(alignments.begin(), alignments.end(), compareByPeso);

  
    cout << "\nAlineaciones Posibles:" << endl;
    for (const auto& alignment : alignments) {
        cout << "Secuencia A: " << alignment.sequenceA << endl;
        cout << "Secuencia B: " << alignment.sequenceB << endl;
        cout << "Score: " << alignment.score << endl;
        cout<< "PESO: " << alignment.peso << endl << endl;
    }

    cout<< "\n---------\nAlineacion optima:\n";
    
    cout << "Secuencia A: " << alignments.begin()->sequenceA << endl;
    cout << "Secuencia B: " << alignments.begin()->sequenceB << endl;
    cout << "Score: " << alignments.begin()->score << endl;
    cout<< "PESO: " << alignments.begin()->peso << endl << endl;

}


int main() {
    string CadenaA = "AAAC", CadenaB = "AGC";
    //string CadenaA = "tcaagcgttagagaagtcat", CadenaB = "attaaaggtttataccttcc";

    //string CadenaA = "attaaaggtttataccttcccaggtaacaa", CadenaB = "atggaagcaatatcactgatgactatacta";

    //string CadenaA = "tcaagcgttagagaagtcattatgtgataa", CadenaB = "atggaagcaatatcactgatgactatacta";
    int tamA = CadenaA.length(), tamB = CadenaB.length();

    Needleman(CadenaA, CadenaB, tamA, tamB);

    return 0;
}
