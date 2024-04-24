#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <climits>
#include <algorithm>

using namespace std;

string findConsensusSequence(const vector<string>& sequences) {
    int maxLength = 0;
    // Encontrar la longitud máxima de las secuencias
    for (const string& sequence : sequences) {
        maxLength = max(maxLength, (int)sequence.length());
    }

    string consensus;
    // Iterar a través de todas las posiciones
    for (int i = 0; i < maxLength; ++i) {
        unordered_map<char, int> nucleotideCount;
        // Contar la frecuencia de cada nucleótido en todas las secuencias
        for (const string& sequence : sequences) {
            if (i < sequence.length()) {
                nucleotideCount[sequence[i]]++;
            }
        }
        // Encontrar el nucleótido más frecuente en la posición actual
        char mostFrequent = ' ';
        int maxCount = 0;
        for (const auto& pair : nucleotideCount) {
            if (pair.second > maxCount) {
                maxCount = pair.second;
                mostFrequent = pair.first;
            }
        }
        consensus += mostFrequent;
    }
    return consensus;
}

// Función para generar el complemento reverso de una secuencia de ADN
string reverseComplement(const string& sequence) {
    string complement;
    for (char nucleotide : sequence) {
        switch (nucleotide) {
            case 'A':
                complement += 'T';
                break;
            case 'T':
                complement += 'A';
                break;
            case 'C':
                complement += 'G';
                break;
            case 'G':
                complement += 'C';
                break;
        }
    }
    reverse(complement.begin(), complement.end());
    return complement;
}


vector<string> caminosHamilton(vector<string> sequences, size_t minOverlap, int l) {
    // Paso 4: Combinar las secuencias para alcanzar la longitud deseada
    vector<vector<string>> secuenciasCombinadas;
    for(int indice = 0; indice < sequences.size(); indice++)
    {
      string combinedSequence = sequences[indice]; // Inicialmente, tomamos la primera secuencia como secuencia combinada
      vector<string> contributingSequences; // Para almacenar las secuencias que contribuyen a la secuencia combinada
      contributingSequences.push_back(sequences[indice]);
      for (size_t i = 0; i < sequences.size() && combinedSequence.length() <= l; ++i)     {

          if(i == indice){
            continue;
          }
        
          size_t overlap = 0;
          size_t len1 = combinedSequence.length();
          size_t len2 = sequences[i].length();
  
  
          
          // Encontrar la longitud máxima de superposición entre la secuencia combinada y la secuencia actual
          for (size_t j = minOverlap; j <= min(len1, len2); ++j) {
              if (combinedSequence.substr(len1 - j) == sequences[i].substr(0, j)) {
                  overlap = j;
              }
          }
          // Si la superposición no alcanza la longitud mínima requerida, omitimos esta secuencia
          if (overlap < minOverlap) {
              continue;
          }
          // Agregar la parte no superpuesta de la secuencia actual a la secuencia combinada
        
          combinedSequence += sequences[i].substr(0, overlap);
          //contributingSequences.push_back(sequences[i].substr(overlap)); // Almacenar la parte contribuyente de la secuencia actual
          contributingSequences.push_back(sequences[i]);
      }

      //----------IMPRIMIR LA SECUENCIA----------------
      cout<<"\nCadena "<<indice<<": "<<endl;
      for (const string& sequence : contributingSequences) {
          cout << sequence << "\t->\t";
      }
      //-----------------GUARDAR lA COMBINACION FINAL-------------
  
      contributingSequences.push_back(combinedSequence);
      secuenciasCombinadas.push_back(contributingSequences);
    }

    //devolver la mejor combinacion
    int indiceMejor = 0;
    for(int i = 1; i < secuenciasCombinadas.size(); i++){
      if(secuenciasCombinadas[indiceMejor][secuenciasCombinadas[0].size()-1].length() < secuenciasCombinadas[i][secuenciasCombinadas[i].size()-1].length())
        indiceMejor = i;
    }
    
    
    return secuenciasCombinadas[indiceMejor];
}



int main() {
    vector<string> sequences = {
        "ATCCGTTGAAGCCGCGGGC",
        "TTAACTCGAGG",
        "TTAAGTACTGCCCG",
        "ATCTGTGTCGGG",
        "CGACTCCCGACACA",
        "CACAGATCCGTTGAAGCCGCGGG",
        "CTCGAGTTAAGTA",
        "CGCGGGCAGTACTT"
    };

    // Paso 1: Encontrar la secuencia de consenso
    string consensusSequence = findConsensusSequence(sequences);
    cout << "Secuencia de Consenso: " << consensusSequence << endl;
  cout<<"------------------------------------------------------------"<<endl;
    // Paso 2: Generar las secuencias complemento-reversas
    vector<string> allSequences = sequences;
    
    for (const string& sequence : sequences) {
        string reverseComp = reverseComplement(sequence);
        allSequences.push_back(reverseComp);
    }

    // Paso 3: Encontrar la secuencia que se aproxime más al tamaño deseado
    int desiredLength = 55;
    

  // Paso 4: Combinar las secuencias para alcanzar la longitud deseada
  size_t minOverlap = 1; // Longitud mínima de superposición requerida
  vector<string> Resultados = caminosHamilton(allSequences, minOverlap, desiredLength);


  cout<<"\n------------------------------------------------------------"<<endl;
    cout << "Secuencia proxima a longitud " << desiredLength << ": " << Resultados[Resultados.size()-1] << endl;
    cout<<"Tamaño: "<<Resultados[Resultados.size()-1].length()<<endl;
  
    cout << "Camino Hamiltoneano:" << endl;
    for (const string& sequence : Resultados) {
        cout << sequence << "\t->\t";
    }
    cout<<endl;

   
    return 0;
}
