#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Función de puntuación para los emparejamientos de bases
int score(char a, char b) {
     if((a=='C' and b =='G') || (a=='G' and b =='C')){
        return -5;
      }

      if((a=='A' and b =='U') || (a=='U' and b =='A')){
        return -4;
      }

      if((a=='G' and b =='U') || (a=='U' and b =='G')){
        return -1;
      }
    return 0;
}

int score2(char a, char b){
  if((a=='C' and b =='G') || (a=='G' and b =='C')){
    return -1;
  }

  if((a=='A' and b =='U') || (a=='U' and b =='A')){
    return -1;
  }



  return 0;
}


// Función para encontrar el mínimo K
pair<int, int> min_K(const vector<vector<int>>& E, int i, int j) {
    int min_val = 9999;
    int min_k = -1;
    for (int k = i; k < j; ++k) {
        int val = E[i][k] + E[k + 1][j];
        if (val < min_val) {
            min_val = val;
            min_k = k;
        }
    }
    return {min_val, min_k};
}

// Función para imprimir la matriz
void printMatrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (auto val : row) {
            cout << val << "\t";
        }
        cout << "\n";
    }
}

// Función para imprimir la matriz P
void printMatrixP(const vector<vector<pair<int, int>>>& P) {
    for (const auto& row : P) {
        for (const auto& val : row) {
            cout << "(" << val.first << "," << val.second << ")\t";
        }
        cout << "\n";
    }
}

// Función para realizar el traceback y encontrar los emparejamientos
void traceback(const vector<vector<pair<int, int>>>& P, int i, int j, string& structure) {
    if (i >= j) return;

    if (P[i][j].second == -1) {
        if (P[i][j].first == P[i + 1][j].first) {
            traceback(P, i + 1, j, structure);
        } else if (P[i][j].first == P[i][j - 1].first) {
            traceback(P, i, j - 1, structure);
        } else if (P[i][j].first == P[i + 1][j - 1].first + score(structure[i], structure[j])) {
            structure[i] = '(';
            structure[j] = ')';
            traceback(P, i + 1, j - 1, structure);
        }
    } else {
        int k = P[i][j].second;
        traceback(P, i, k, structure);
        traceback(P, k + 1, j, structure);
    }
}


void imprimir_struct(string& structure, const vector<vector<pair<int, int>>>& P){
    int j = structure.size()-1;
    int i = 0;
    // cout<< i << "\t"<< j<<endl;
    cout<<structure[i] << "\t-\t" << structure[j]<<endl;
    j = P[i][j].second;
    i = P[i][j].first;
    while(i != 0){
        // cout<< i << "\t"<< j<<endl;
        cout<<structure[i] << "\t-\t" << structure[j]<<endl;
        j = P[i][j].second;
        i = P[i][j].first;
    }
}


int main() {
    string cadena = "ACUCGAUUCCGAG";
    int n = cadena.length();

    // Matriz de energía
    vector<vector<int>> E(n, vector<int>(n, 0));
    // Matriz de posiciones (valor, k)
    vector<vector<pair<int, int>>> P(n, vector<pair<int, int>>(n, {0, -1}));

    // Inicialización de la matriz E
    for (int d = 1; d < n; ++d) {
        for (int i = 0; i + d < n; ++i) {
            int j = i + d;
            auto minK = min_K(E, i, j);
            int min_val = min({E[i + 1][j], E[i][j - 1], E[i + 1][j - 1] + score(cadena[i], cadena[j]), minK.first});

            if (min_val == E[i + 1][j]) {
                P[i][j] = {i+1, j};
            } else if (min_val == E[i][j - 1]) {
                P[i][j] = {i, j-1};
            } else if (min_val == E[i + 1][j - 1] + score(cadena[i], cadena[j])) {
                P[i][j] = {i+1, j-1};
            } else {
                P[i][j] = {i, minK.second};
            }
            E[i][j] = min_val;
        }
    }

    cout << "Matriz de Scores (E):\n";
    printMatrix(E);

    cout << "\nMatriz de Caminos (P):\n";
    printMatrixP(P);

    cout << "\nEstrutura:\n";
    imprimir_struct(cadena, P);

    return 0;
}
