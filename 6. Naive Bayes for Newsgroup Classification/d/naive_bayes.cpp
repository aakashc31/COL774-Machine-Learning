#include <bits/stdc++.h>
using namespace std;

#define pb	push_back
#define mp	make_pair

#define __MESSAGE__ false
#define __CONFUSION_MATRIX__ false

typedef long long ll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> pii;

map<string,int> ngToInt;
map<string, vi> M;

vector<string> allData;
vi countNg;

void readDataFromFile(string fnm){
	if(__MESSAGE__) cout << "Reading from file " << fnm << " ... " << endl;
	string line; ifstream myfile(fnm.c_str());
	if(myfile.is_open()){
		while(!myfile.eof()){
			getline(myfile, line);
			if(line.size())
				allData.pb(line);
		}
	}
	if(__MESSAGE__){
		cout << "Reading complete.. " << endl;	
	}
}

void splitDataSet(vvi& v){
	if(__MESSAGE__) cout << "splitDataSet() .. " << endl;
	int size = allData.size();
	int splitSize = size/5;
	cout << "size = " << size << " splitSize = " << splitSize << endl;
 	default_random_engine generator;
  	uniform_int_distribution<int> distribution(0,4);	
  	int num;
  	for(int i=0; i<size; i++){
  		num = distribution(generator);
  		if(v[num].size() == splitSize){
  			i--;
  			continue;
  		}
  		v[num].pb(i);
  	}
  	if(__MESSAGE__) cout << "splitting done .. " << endl;
}

void createMaps(vvi& v, vi& locs){
	if(__MESSAGE__) cout << "createMaps()" << endl;
	countNg.resize(8,0);
	string newsgroup, word; map<string,int>::iterator it; int idx;
	for(int i : locs){
		for(int j : v[i]){
			string& line = allData[j];
			stringstream ss; ss.str(line);
			ss >> newsgroup;
			it = ngToInt.find(newsgroup);
			idx = ngToInt.size();
			if(it == ngToInt.end())
				ngToInt[newsgroup] = idx;
			else
				idx = it->second;
			while(ss >> word){
				map<string, vi>::iterator it1 = M.find(word);
				if(it1 == M.end()){
					vi vt(8,0);
					vt[idx] = 1;
					M[word] = vt;
				}
				else{
					vi& vt = it1->second;
					vt[idx]++;
				}
				countNg[idx]++;
			}
		}
	}
	if(__MESSAGE__)	cout << "createMaps() end .. " << endl;
}

void createMaps(vi& v){
	vvi vx(1, v);
	vi locs(1,0);
	createMaps(vx, locs);
}

double probWordGivenLabel(string& word, int idx){
	map<string,vi>::iterator it = M.find(word);
	int numerator = 0, denominator = 0;
	if(it == M.end())
		numerator = 1;
	else
		numerator = 1 + it->second[idx];
	denominator = countNg[idx] + M.size();
	double ret = log10(numerator) - log10(denominator);
	return ret;
}

int getPrediction(vector<string>& v){
	vector<pair<double,int> > probs;
	for(int label = 0; label < 8; label++){
		double curr = 0;
		for(string &st : v){
			curr += probWordGivenLabel(st, label);
		}
		probs.pb(mp(curr, label));
	}
	sort(probs.begin(), probs.end());
	return probs.back().second;
}

double computeAccuracy(vvi& v, int idx){
	if(__MESSAGE__) cout << "computeAccuracy() " << endl;
	vvi ConfusionMatrix(8, vi(8,0));
	int hits = 0;
	string newsgroup, word; vector<string> words;
	for(int i : v[idx]){
		stringstream ss; ss.str(allData[i]);
		ss >> newsgroup;
		words.clear();
		while(ss >> word)
			words.pb(word);
		int answer = ngToInt.find(newsgroup)->second;
		int prediction = getPrediction(words);
		if(answer == prediction)
			hits++;
		ConfusionMatrix[answer][prediction]++;
		if(__MESSAGE__) cout << "*";
	}
	cout << endl;
	if(__CONFUSION_MATRIX__){
		cout << "ConfusionMatrix : " << endl;
		for(auto& temp : ConfusionMatrix){
			for(int& c : temp)
				cout << c << " ";
			cout << endl;
		}
	}
	return (hits * 100.0) / (v[idx].size());
}

double computeAccuracy(vi &v){
	vvi vx; vx.pb(v);
	return computeAccuracy(vx, 0);
}

void run(){
	readDataFromFile("20ng-rec_talk.txt");
	vvi dataSet(5,vi(0));
	splitDataSet(dataSet);
	double sum = 0;
	for(int idx = 0; idx <= 4; idx++){
		ngToInt.clear(); M.clear();
		vi locs(4);
		for(int i=0; i<5; i++)
			if(i != idx)
				locs.pb(i);
		createMaps(dataSet, locs);
		double ret = computeAccuracy(dataSet, idx);
		cout << "accuracy = " << ret << endl;
		sum += ret;
	}
	cout << "Average accuracy = " << sum/5 << endl;
}

void getXRandomFromGivenSet(vvi& v, vi& locs, vi& ret, int x){
	ret.clear();
	for(int i:locs){
		for(int j:v[i])
			ret.pb(j);
	}
	unsigned seed = 100;//std::chrono::system_clock::now().time_since_epoch().count();
	shuffle (ret.begin(), ret.end(), std::default_random_engine(seed));
	ret.resize(x);
}

void runOnVaryingSizeTrainData(){
	time_t init; time(&init);
	readDataFromFile("20ng-rec_talk.txt");
	vvi dataSet(5,vi(0));
	splitDataSet(dataSet);
	vector<double> avgTest(6,0), avgTrain(6,0);
	for(int idx = 0; idx <= 4; idx++){
		ngToInt.clear(); M.clear();
		vi locs(4);
		for(int i=0; i<5; i++) if(i != idx) locs.pb(i);
		int cnt = 0;
		for(int x = 1000; ; x+=1000){
			if(x==6000)
				x = 5784;
			vi nDataSet;
			getXRandomFromGivenSet(dataSet, locs, nDataSet, x);
			createMaps(nDataSet);
			double testAcc = computeAccuracy(dataSet, idx);
			double trainAcc = computeAccuracy(nDataSet);
			cout << "idx = " << idx << " x = " << x << " testAcc = " << testAcc << " trainAcc = " << trainAcc << endl;
			avgTest[cnt]+=testAcc/5.0;
			avgTrain[cnt] += trainAcc/5.0;
			if(x == 5784)
				break;
			cnt++;
		}
	}
	cout << "Average test accuracy : ";
	for(double& a : avgTest)
		cout << a << " ";
	cout << endl;
	cout << "Average train accuracy: ";
	for(double& b : avgTrain)
		cout << b << " ";
	cout << endl;

	time_t end; time(&end);
	cout << "Total time = " << difftime(end, init) << endl;
}

int main(){
	// run();
	runOnVaryingSizeTrainData();
	return 0;
}

			