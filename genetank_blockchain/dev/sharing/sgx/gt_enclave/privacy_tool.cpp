#include <unordered_set>
#include <string>
#include <iostream>
#include <climits>
#include <cstring>
​
using namespace std;
​
int hashString(string text){
	const int u1 = 2;
	const int u2 = 5;
​
	int hash = u1;
	for(auto it = text.begin(); it!= text.end(); ++it){
		hash = hash*u2 + *it;
	}
    //cout<<hash<<"---";
	return hash;
}
​
void toShinglesHash(string text, size_t window_len, unordered_set<int> &result){
​
	for(size_t i=0;i<=text.length()-window_len;i++){
		string temp = text.substr(i,window_len);
		result.insert(hashString(temp));
	}
	//cout<<endl;
}
​
​
double getJaccard(unordered_set<int> s1, unordered_set<int> s2, size_t num_hash){
	unordered_set<int> common;
​
	common.insert(s1.begin(), s1.end());
	common.insert(s2.begin(), s2.end());
​
	size_t seed = common.size();
	int arr[common.size()][2];
​
	int i=0;
	for(auto it = common.begin(); it != common.end(); ++it){
		arr[i][0]=(s1.find(*it)!=s1.end())?1:0;
		arr[i][1]=(s2.find(*it)!=s2.end())?1:0;
		//cout<<arr[i][0]<<":"<<arr[i][1]<<endl;
		i++;
	}
​
	size_t rows = common.size();

	int sigTab[num_hash][2];
	int hashFunTab[rows][num_hash];
​
	for(size_t i=0;i<num_hash;i++){
		sigTab[i][0]=INT_MAX;
		sigTab[i][1]=INT_MAX;
	}
​
	for(size_t i = 0; i< rows; i++){
		for(size_t j = 0; j< num_hash; j++){
			hashFunTab[i][j] = ((2*j+1)*i + 1)%seed;
		}
	}
​
	for(size_t i = 0; i< 2; i++){
		for(size_t j = 0; j< rows; j++){
            if(arr[j][i]!=0){
            	for(size_t k =0; k<num_hash; k++){
            		if(sigTab[k][i]>hashFunTab[j][k]){
            			sigTab[k][i]=hashFunTab[j][k];
            		}
            	}
            }
		}
	}
​
//	for(size_t i=0;i<num_hash;i++){
//		cout<<sigTab[i][0]<<":"<<sigTab[i][1]<<endl;
//	}
​
	int jac = 0;
	for(size_t i =0 ; i<num_hash; ++i){
		if(sigTab[i][0]==sigTab[i][1]){
			jac+=1;
		}
	}
​
	return (double)(jac)/num_hash;
}

void test_encryption(){

}
​
double test_minhash(){
	string test = "dein muss sein kempf zeit";
	string test2 = "mit und muss I dein";
​
	unordered_set<int> ret;
	unordered_set<int> ret1;
​
	toShinglesHash(test, 1, ret);
	toShinglesHash(test2, 1, ret1);
​
	double ans = getJaccard(ret, ret1, 100);
​
    return ans;
}

