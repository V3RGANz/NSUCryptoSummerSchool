#include <iostream>
#include <vector>
#include <stdlib.h>
#include <alloca.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <time.h>
#include <math.h>

#include "Matrix.hpp"


class Loop {
public:
    int start;
    int end;
    int current;
};

class pairs{
public:
    int first;
    std::string second;
};

#include <iostream>
#define MAXROWS 2           /// k
#define MAXVALUES 6         ///n

using namespace std;
vector<string> comb(int n, int k);
void displayStringVect(vector<string> vect);
void display2Dim(vector<vector<int>> vect);
void calcul();
vector<pairs > multipli2Vects(vector<pairs > row1, vector<pairs > row2);
void displayPairVector(std::vector<pairs> vector);
bool checkRedonduncy(string str);
int maxIndex(vector<pairs> vector);
int stringToInt(string str, int pos);
vector<vector<int>> grassmanCalcul(vector<vector<int>> S, int k);
int checkExactConcordance(string vector, std::vector<string> combinaisons);
int checkIndexConcordance(string vector, std::vector<string> combinaisons);
int parity(string combinaison, string vector);
vector<pairs> useSym(std::vector<pairs> vector, int pos, int value, string comb);
pairs makePairs(int val, string vector);
vector<vector<int> >  matrixModulo(vector<vector<int> > matrix, int modulo);
vector<vector<int> > newMatrix(vector<vector<int>> in);
Matrix vectToMat(vector<vector<int>> in);
vector <vector<int>> matToVect(Matrix in);

char display[] = {'1','2','3','4','5','6'};

int main() {
    int n = 4;
    int m = 9;
    int k = 3;
    int comSize = (int) comb(n, k).size();

    ///Definition of matrix

    Matrix A ("A", n, n);
    Matrix B ("B", n, n);
    Matrix W ( "W", comSize, comSize );

    Matrix f1 ("f1", n, n);
    Matrix f2 ("f2", n, n);
    Matrix h1 ("h1", n, n);
    Matrix h2 ("h2", n, n);

    Matrix f1p ("f1p", comSize, comSize );
    Matrix f2p ("f2p", comSize, comSize );
    Matrix h1p ("h1p", comSize, comSize );
    Matrix h2p ("h2p", comSize, comSize );

    Matrix R1 ("R1", comSize, comSize );
    Matrix R2 ("R2", comSize, comSize );

    ///Generate A, W non invertible
    A.random(m, 0, 0);
    B.random(m, 0, 0);
    W.random(m, 1, 0);

    cout<<A<<endl;
    ///Generation of the polynomials
    f1 = A.randomPolynomial(100);
    f1.modulo(m);
    cout<<f1<<endl;
    f2 = A.randomPolynomial(100);
    f2.modulo(m);
    h1 = B.randomPolynomial(100);
    h1.modulo(m);
    h2 = B.randomPolynomial(100);
    h2.modulo(m);

    ///Calcul of the grassman change to k-base
    f1p = vectToMat(grassmanCalcul(matToVect(f1), k) );
    f2p = vectToMat(grassmanCalcul(matToVect(f2), k) );
    h1p = vectToMat(grassmanCalcul(matToVect(h1), k) );
    h2p = vectToMat(grassmanCalcul(matToVect(h2), k) );


    ///Calcul of the two versions of the key
    R1 = f1p*f2p;
    R1.modulo(m);
    R1 = R1 *W;
    R1.modulo(m);
    R1 = R1 *h2p;
    R1.modulo(m);
    R1 = R1 *h1p;
    R1.modulo(m);


    R2 = f2p*f1p;
    R2.modulo(m);
    R2 = R2 * W;
    R2.modulo(m);
    R2 = R2 * h1p;
    R2.modulo(m);
    R2 = R2 * h2p;
    R2.modulo(m);

    cout<<R1<<endl;
    cout<<R2<<endl;
}



vector<string> comb (int n, int k){
    std::vector<Loop *> arrs;  // represent the different variables in the for loops
    int r = 0;
    int end = n;
    vector<string> result;
    string temp;

    ///  cout<<"comb of "<<k<<" elements out of "<<n<<endl;
    /* Initialisation*/
    arrs.resize(n);
    for(int r=0; r<k; r++){
        arrs[r] = new Loop;
        arrs[r]->end = n-k+r;
    }
    arrs[0]->start = 0-1 ;
    arrs[0]->current = arrs[r]->start;


    while(true){
        if (r >= k-1){
            for(int i = arrs[r]->start+1; i<arrs[r]->end+1; i++){
                arrs[r]->current = i;
                temp = "";
                //afficherCombinaisons();
                for(int j=0; j<k; j++){
                    temp.append( std::to_string(arrs[j]->current) );
                }

                result.push_back(temp);

            }
            r--;
        }
        else{
            if (arrs[r]->current >= arrs[r]->end){
                if(r == 0){
                    return result;
                }
                arrs[r]->current = arrs[r]->start;
                r--;
            }
            else{
                arrs[r]->current++;
                r++;
                arrs[r]->start = arrs[r-1]->current;
                arrs[r]->current = arrs[r]->start;
            }
        }
    }

}



void displayStringVect(vector<string> vect){
    for (int i = 0; i<vect.size(); i++){
        cout<<endl;
        cout<<vect[i];
    }
    cout<<endl;
}


void display2Dim(vector<vector<int>> vect){
    for (int i = 0; i<vect.size(); i++){
        cout<<endl;
        for(int j=0; j<vect[i].size(); j++){
            cout<<vect[i][j]<<"   ";
        }
    }
    cout<<endl;
}

void calcul(){
    /*  int n =3;
    int k =2;
    vector<vector<int>> basis = comb(n, k);
    int m = basis.size();
    vector<vector<int>> S {{5,1,4},
                          {-2, 3, 1},
                          {0, 2, 7}  };

    for(int i=0; i<m; i++){
        row1 =S[basis[i][0]][0];
        for(int row = 1; row < m; row++){
            row2 = S[basis[0][row]][0];
            row1 = multipli2Vects(row1, row2);

        }

    }
*/

}

vector<pairs> multipli2Vects(vector<pairs> row1, vector<pairs> row2){
    vector<pairs> result;
    for(int i=0; i<row1.size(); i++){
        for(int j=0; j<row2.size(); j++){
            int val = row1[i].first*row2[j].first;
            //if (val!=0){
            result.push_back(makePairs(val, row1[i].second + row2[j].second));
        }
    }
    for ( vector<pairs>::iterator it = result.begin() ; it != result.end();){
        if(checkRedonduncy(it->second)){
            result.erase(it);
        }
        else{
            ++it;
        }
    }

    return result;
}

void displayPairVector(std::vector<pairs> vector){

    for (const auto& p : vector)
    {
        std::cout << p.first << ", " << p.second << std::endl;
    }
}


bool checkRedonduncy(string str){       /*return true if redondecy found*/
    bool array[10] = {false};


    for (int i=0; i<str.size(); i++){
        if (array[str[i] - 48]){
            return true;
        }
        array[str[i] - 48] = true;
    }

    return false;
}

int maxIndex(vector<pairs> vector){
    int max = -1;
    for(int i = 0; i<vector.size(); i++){
        for(int j = 0; j<vector[i].second.size(); j++){
            int index = stringToInt(vector[i].second, j);
            max = std::max(max, index);

        }
    }
    return max;
}
int stringToInt(string str, int pos){
    char c = str[pos];
    int result = c-'0';
    return result;
}

vector<vector<int>> grassmanCalcul(vector<vector<int>> S, int k){
    vector<vector<int>> SI;
    int n = S.size();
    vector<pairs> row1;

    vector<pairs>  row2;
    vector<string> combinaisons = comb(n, k);
    displayStringVect(combinaisons);


    for(int h=0; h<combinaisons.size(); h++){
        vector<pairs> result;
        for (int a = 0; a<S[0].size(); a++){
            result.push_back(makePairs(S[stringToInt(combinaisons[h], 0)][a],std::to_string(a) ));
        }



        for(int i = 1; i<combinaisons[h].size(); i++){
            row1 = result;
            result.clear();
            row2.clear();


            for (int a = 0; a<S[0].size(); a++){
                row2.push_back(makePairs(S[stringToInt(combinaisons[h], i)][a],std::to_string(a) ));
            }



            result = multipli2Vects(row1, row2);
            vector<string> combTemp = comb(maxIndex(result)+1, result[0].second.size());


            int size = result.size();
            for(int j = 0; j<size; ){
                if(checkExactConcordance(result[j].second  , combTemp) == -1){
                    int combinaisonNumber=checkIndexConcordance(result[j].second, combTemp);
                    if(combinaisonNumber!= -1){
                        int sign = parity(result[j].second, combTemp[combinaisonNumber] );
                        result = useSym(result,j,  sign*result[j].first, combTemp[combinaisonNumber] );
                        size--;
                    }
                    else{
                        j++;
                    }

                }
                else{
                    j++;
                }
            }
        }

        vector<int> vectorTemp;                                               ///To be removed
        vectorTemp.clear();
        for(int k = 0; k<result.size(); k++){
            vectorTemp.push_back(result[k].first);
        }
        SI.push_back(vectorTemp);
    }
    return SI;
}

int checkExactConcordance(string vector, std::vector<string> combinaisons){
    for(int i = 0; i<combinaisons.size(); i++){
        if(!vector.compare(combinaisons[i])){
            return i;
        }
    }
    return -1;
}


int checkIndexConcordance(string vector, std::vector<string> combinaisons){
    string sortedVector = vector;
    std::sort(sortedVector.begin(), sortedVector.end());
    for(int i=0; i<combinaisons.size(); i++){
        string sortedComb = combinaisons[i];
        std::sort(sortedComb.begin(), sortedComb.end());
        if(!sortedVector.compare(sortedComb)){
            return i;
        }
    }
    return -1;
}

int parity(string combinaison, string vector){
    int power=0;
    std::vector<int> alreadyRead;
    int i = 0;

    while(alreadyRead.size() < vector.size()){
        int j = i;
        int compteur = 0;
        do{

            compteur++;
            int k=0;
            alreadyRead.push_back(stringToInt(combinaison, j));
            while(combinaison[k] != vector[j]){
                k++;
            }
            j = k;


        }while(combinaison[i] != vector[j]);
        if (std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, j)) == alreadyRead.end() ){
            compteur++;
            alreadyRead.push_back(stringToInt(combinaison, j));
        }
        i++;
        for(int l=0; l<alreadyRead.size(); l++){
        }
        while( ( std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, i)) != alreadyRead.end() ) && ( i<combinaison.size()-1 ) ){
            i++;
        }

        power += compteur-1;
    }
    return pow(-1, power);
}

vector<pairs> useSym(std::vector<pairs> vector, int pos, int value, string comb){
    int i = 0;
    while(vector[i].second.compare(comb)!=0){
        i++;
    }
    vector[i].first += value;
    std::vector<pairs>::iterator it = vector.begin()+pos;
    vector.erase(it);
    return vector;
}


pairs makePairs(int val, string vector){
    pairs newPair;
    newPair.first = val;
    newPair.second = vector;
    return newPair;
}


vector<vector<int> > matrixModulo(vector<vector<int> > matrix, int modulo){
    for(int i=0; i<matrix.size(); i++){
        for(int j=0; j<matrix[0].size(); j++){
            if(matrix[i][j]>=0){
                matrix[i][j] = matrix[i][j]%modulo;
            }
            else{
                matrix[i][j] =(modulo +  (matrix[i][j]%modulo)) %modulo;
            }
        }
    }
    return matrix;
}


vector<vector<int> > newMatrix(vector<vector<int>> in){
    vector<vector<int> > out(in[0].size(), vector<int>(in.size()));

    for(int i=0; i<in.size(); i++){
        for(int j=0; j<in[0].size(); j++){
            cout<<i<<"   "<<j<<endl<< in[i][j]<<endl;
            out[j][i] = in[i][j];
        }
    }
    return out;
}

Matrix vectToMat(vector<vector<int>> in){
    Matrix out("t22", 4, 4);
    for(int i=0; i<in.size(); i++){
        for(int j=0; j<in[0].size(); j++){
            out.setElement(j,i,in[j][i]);
        }
    }
    return out;
}


vector <vector<int>> matToVect(Matrix in){
    vector<vector<int> > out(in.getN(), vector<int>(in.getM()));
    for(int i=0; i<in.getN(); i++){
        for(int j=0; j<in.getM(); j++){
            out[i][j] = in.getElement(i,j);
        }
    }
    return out;
}
