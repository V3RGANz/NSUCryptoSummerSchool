#include <iostream>
#include <vector>
#include <stdlib.h>
#include <alloca.h>
#include <algorithm>



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

char display[] = {'1','2','3','4','5','6'};

int main() {

    displayStringVect(comb(4,2));
    std::vector<pairs> row1{makePairs(17, "12"),makePairs(13, "13"),makePairs(27, "14"),makePairs(-11, "23"),makePairs(-12, "24"),makePairs(18, "34"),};
    vector<pairs> row2{makePairs(0, "1"),makePairs(2, "2"),makePairs(7, "3"),makePairs(-1, "4"),};
    //multipli2Vects(row1, row2);
    //displayPairVector(multipli2Vects(row1, row2));
    //    cout<<parity("123456789", "263495718")<<endl;
    std::vector<string> combi {"13", "23", "12"};
    // cout<<checkExactConcordance("21", combi)<<endl;
    //cout<<checkIndexConcordance("21", combi)<<endl;
//    vector<vector<int>> S{{46 , 67  ,  0 , -59 , 43 , 83 , 59  ,  8 , -92 , 19 , 50 , -76},
//                          { 85 , -86 , 24 , -28 , -64  ,  9 , -46 , -26 , -43 , -21 , -81 , 93},
//                          {-11 , -80 , -3 , -61 , 71 , -44 , -88 , 15 , 87  ,  6 , 74 , 34},
//                          {-70 , 32 , 86 , 40 , 54 , 21 , 76 , 57 , -31 , 81 , -38 , -13},
//                          {-99 , -32 , 82 , 23 , -5 , 42 , 98 , -42 , -52  ,  3 , -51 , 29},
//                          {-35 , 66 , 65 , -55 , -95 , -93 , 53 , 41 , -14 , -39 , 58 , 100},
//                          {-85 , 78 , 17 , -41 , -22 , 96 , 64 , 70 , -97 , -25 , -75 , -91},
//                          {-68 , -72 , 75 , 45 , 27 , -63 , -67 , 13 , -60 , 25 , -79 , 38},
//                          { 90  ,  5 , 20 , 77 , -89 , 95 , -58 , -45 , 35 , -30 , 51 , 33},
//                          {-62 , -33 , 39 , -15 , 94 , 62 , 11 , 69 , -49 , -40 , -84 , 48},
//                          {-53 , 99 , -57 , -54 , 49 , -94 , 26 , -47 , 16  ,  4 , 22 , 79},
//                          {-27 , -29 , -1 , -36 , -17 , -65 , 56 , -16 , -24 , 73 , -83 , 14}};




    vector<vector<int>> S{{5, 1, 4, 2},
                          {-2 , 3, 1, 6},
                          {0, 2, 7, 5},
                          {5, 3, 7, 8}};
//    vector<vector<int>> S{{5, 1, 4,},
//                          {-2 , 3, 1},
//                          {0, 2, 7}};
    //grassmanCalcul(S,3);
    display2Dim(grassmanCalcul(S,2));
}



vector<string> comb (int n, int k){
    std::vector<Loop *> arrs;  // represent the different variables in the for loops
    int r = 0;
    int end = n;
    vector<string> result;
    string temp;

    cout<<"comb of "<<k<<" elements out of "<<n<<endl;
    /* Initialisation*/
    arrs.resize(n);
    for(int r=0; r<k; r++){
        arrs[r] = new Loop;
        arrs[r]->end = n-k+r;
        //cout<<r<<"  "<<arrs[r]->end<<endl;
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
                //cout<<row1[i].first*row2[j].first<<"   "<< row1[i].second << row2[j].second<<endl;
          //  }
        }
    }
    for ( vector<pairs>::iterator it = result.begin() ; it != result.end();){
        // cout<<it->second<<endl;
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
        // or std::cout << std::get<0>(p) << ", " << std::get<1>(p) << std::endl;
    }
    /*for(vector<pairs>::iterator i = vector.begin(); i!=vector.end(); ++i){
        cout<<vector[i].first<<"  "<<vector[i].second<<endl;
    }*/
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



    for(int h=0; h<combinaisons.size(); h++){

        cout<<"###########################################################"<<endl;

        vector<pairs> result;
        for (int a = 0; a<S[0].size(); a++){
            result.push_back(makePairs(S[stringToInt(combinaisons[h], 0)][a],std::to_string(a) ));
        }



        for(int i = 1; i<combinaisons[h].size(); i++){
            cout<<"             ##########################"<<endl;

            row1 = result;
            result.clear();
            row2.clear();


            for (int a = 0; a<S[0].size(); a++){
                row2.push_back(makePairs(S[stringToInt(combinaisons[h], i)][a],std::to_string(a) ));
            }


            cout<<"\n   row1:"<<endl;
            displayPairVector(row1);
            cout<<"\n   row2:"<<endl;
            displayPairVector(row2);

            result = multipli2Vects(row1, row2);
            //displayPairVector(result);


            vector<string> combTemp = comb(maxIndex(result)+1, result[0].second.size());
            //displayStringVect(combTemp);
            // int j = 0;
            //            for ( vector<pairs>::iterator it = result.begin() ; it != result.end(); it++){
            //             cout<<it->first<<"   "<<it->second<<endl;


            int size = result.size();
            for(int j = 0; j<size; ){
                if(checkExactConcordance(result[j].second  , combTemp) == -1){
                    int combinaisonNumber=checkIndexConcordance(result[j].second, combTemp);
                    if(combinaisonNumber!= -1){
                                                cout<<"if combNumber != -1"<<endl;
                        int sign = parity(result[j].second, combTemp[combinaisonNumber] );
                                                cout<<"before sym:"<<endl;
                                                displayPairVector(result);
                        result = useSym(result,j,  sign*result[j].first, combTemp[combinaisonNumber] );
                                                cout<<"after sym:"<<endl;
                                                displayPairVector(result);
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



            cout<<"end of loop"<<endl;
            displayPairVector(result);

//            row1 = result;
//            row2.clear();

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
    cout<<"end exact concordance"<<endl;
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
    cout<<"end index concordance"<<endl;
    return -1;
}

int parity(string combinaison, string vector){
    //cout<<combinaison<<"  "<<vector<<endl;
    int power=0;
    std::vector<int> alreadyRead;
    int i = 0;

    /*  cout<<combinaison[0]<<" -> alreadyRead"<<endl;
    alreadyRead.push_back(stringToInt(combinaison, 0));*/
    while(alreadyRead.size() < vector.size()){//cout<<"lll \n";
        int j = i;
        int compteur = 0;
        do{

            //cout<<combinaison[i]<<"  =?  "<<vector[j]<<"sizes:"<<alreadyRead.size() <<"  "<< vector.size()<<endl;
            compteur++;
            int k=0;
            //  cout<<combinaison[j]<<" -> alreadyRead"<<endl;
            alreadyRead.push_back(stringToInt(combinaison, j));
            while(combinaison[k] != vector[j]){
                k++;
            }
            j = k;


            //cout<<std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, i)) != alreadyRead.end()<<endl;
        }while(combinaison[i] != vector[j]);
        if (std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, j)) == alreadyRead.end() ){
            compteur++;
            //cout<<combinaison[j]<<" -> alreadyRead"<<endl;
            alreadyRead.push_back(stringToInt(combinaison, j));
        }
        i++;
        //cout<<endl<<stringToInt(vector, i)<<" dans AlreadyRead ?"<<endl;
        for(int l=0; l<alreadyRead.size(); l++){
            //cout<<alreadyRead[l]<<endl;
        }
        //alreadyRead.push_back(2);
        while( ( std::find(alreadyRead.begin(), alreadyRead.end(), stringToInt(combinaison, i)) != alreadyRead.end() ) && ( i<combinaison.size()-1 ) ){
            //cout<<vector[i]<<"  ";
            i++;
        }
        //cout<<"pas de "<<stringToInt(vector, i)<<" ds alreadyRead "<<endl;

        //cout<<"            compteur ="<<compteur<<endl;
        power += compteur-1;
    }
    return pow(-1, power);
}

vector<pairs> useSym(std::vector<pairs> vector, int pos, int value, string comb){
    int i = 0;
    cout<<"remplacing "<<vector[pos].first<<" with index "<<vector[pos].second<< " by adding "<<value<<" to index "<<comb<<endl;
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
