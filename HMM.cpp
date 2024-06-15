#include <iostream>
#include "string.h"
#include <vector>
#include <map>

using namespace std;

string Alfabeto = {"VGAHEYNDKSTFIPK"};


int cuantosNoEstan(const map<char, int>& Columna) {
    int cuantos = 0;

    for(char c : Alfabeto) {
        if (Columna.find(c) == Columna.end()) {
            cuantos++;
        }
    }

    return cuantos;
}

void HMM(vector<string> sec){
  int tamaño = sec[0].length();
  for(int i =0; i< tamaño-1; i++){
    int j = i+1;
    cout<<"\nNueva seccion -------------"<<endl;
    int match = 0, deletion = 0;
    map<char, int> Columna1;
    for(int cadena = 0; cadena < sec.size(); cadena++){
      cout << sec[cadena][i]<< "\t" << sec[cadena][j] << endl;
      if(Columna1.find(sec[cadena][i]) == Columna1.end()){
        Columna1.insert(make_pair(sec[cadena][i], 1));
      }
      else{
        Columna1[sec[cadena][i]]++;
      }
      
      if(sec[cadena][j] == '-')
        deletion++;
      else
        match++;

      
    }
    if(Columna1['-'] > sec.size()/2){
      cout<<"Hay más gaps que letras, se salta"<<endl;
      continue;
    }

    
    cout<<endl;
    for (const auto& pair : Columna1) {
      if(pair.first == '-') continue;
        cout<<"Probabilidad de: "<<endl;
        // std::cout << pair.first << ": " << static_cast<float>(pair.second + 1)/ (sec.size() + 20 )<< std::endl;

      cout << pair.first << ": " << (pair.second + 1)<<"/"<< (sec.size() + 20 )<< std::endl;
    }
    cout<<"Resto: "<<cuantosNoEstan(Columna1) + 6<<"/"<< (sec.size() + 20 )<<endl;
    cout<<"\nMatch:" << match<< "\t insertion: 1/27" << "\t deletion: "<< deletion<<endl;

    if(deletion > sec.size()/2)
      cout<<"Hay más gaps que letras, se salta"<<endl;
    
  }
}


int main() {
  vector<string> Secuencias = {"VGA--HAGEY", "V----NVDEV","VEA--DVAGH", "VKG------D", "VYS--TYETS", "FNA--NIPKH", "IAGADNGAGY"};

  HMM(Secuencias);
  
}
