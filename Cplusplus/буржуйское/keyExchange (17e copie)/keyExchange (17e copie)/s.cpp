
vector<vector<int>> grassmanCalcul(vector<vector<int>> S, int k){
    vector<vector<int>> SI;
    int n = S.size();
    vector<pairs> row1;

    vector<pairs>  row2;
    vector<string> combinaisons = comb(n, k);



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
                                                displayPairVector(result);
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

        vector<int> vectorTemp;
        vectorTemp.clear();
        for(int k = 0; k<result.size(); k++){
            vectorTemp.push_back(result[k].first);
        }
        SI.push_back(vectorTemp);
    }
    return SI;
}
