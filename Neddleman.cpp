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
};

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

    //int Matriz[tamA][tamB];
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


    // // Imprimimos la matriz de puntuaciones
    // cout << "Matriz de puntuaciones:" << endl;
    // for(int i = 0; i < tamA; i++) {
    //     for(int j = 0; j < tamB; j++) {
    //         cout << Matriz[i][j] << " ";
    //     }
    //     cout << endl;
    // }

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

    function<void(string, string, int, int, string, string, int)> backtrack = [&](string seqA, string seqB, int row, int col, string newSeqA, string newSeqB, int score) {
        if (row == 0 && col == 0) {
            reverse(seqA.begin(), seqA.end());
            reverse(seqB.begin(), seqB.end());
            alignments.push_back({seqA, seqB, score});
            return;
        }

        for (const auto& dir : Direcciones[row][col]) {
            string updatedSeqA = newSeqA;
            string updatedSeqB = newSeqB;
            int newRow = row;
            int newCol = col;
            int newScore = score;

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

            backtrack(seqA + (updatedSeqA.back() == '-' ? "-" : string(1, CadenaA[row-1])), 
                      seqB + (updatedSeqB.back() == '-' ? "-" : string(1, CadenaB[col-1])),
                      newRow, newCol, updatedSeqA, updatedSeqB, newScore);
        }
    };

  backtrack("", "", tamA-1, tamB-1, "", "", 0);
 // Ajustamos los índices para empezar desde el último elemento

    // Mostrar todas las alineaciones
    cout << "Alineaciones Posibles:" << endl;
    for (const auto& alignment : alignments) {
        cout << "Secuencia A: " << alignment.sequenceA << endl;
        cout << "Secuencia B: " << alignment.sequenceB << endl;
        cout << "Score: " << alignment.score << endl << endl;
    }
}



int main() {
    //string CadenaA = "AAAC", CadenaB = "AGC";
    string CadenaA = "tcaagcgttagagaagtcat", CadenaB = "attaaaggtttataccttcc";

    //string CadenaA = "attaaaggtttataccttcccaggtaacaa", CadenaB = "atggaagcaatatcactgatgactatacta";

    //string CadenaA = "tcaagcgttagagaagtcattatgtgataa", CadenaB = "atggaagcaatatcactgatgactatacta";
    int tamA = CadenaA.length(), tamB = CadenaB.length();

    Needleman(CadenaA, CadenaB, tamA, tamB);

    return 0;
}
