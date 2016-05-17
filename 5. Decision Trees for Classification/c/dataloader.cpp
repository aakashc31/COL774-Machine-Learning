#include "dataloader.h"
using namespace std;

// read data from a file fnm and fill in v
void readDataFromFile(string fnm, vector<raw_data>& v){
	string line; ifstream myfile(fnm.c_str());
	if(myfile.is_open()){
		bool isFirstLine = true;
		while(!myfile.eof()){
			stringstream ss; getline(myfile, line);
			if(isFirstLine || line.length() == 0){
				isFirstLine = false;
				continue;
			}
			//replace all commas with whitespaces
			for(int i=0; i<line.length(); i++)
				line[i] = (line[i] == ',' ? ' ' : line[i]);
			ss.str(line);
			raw_data *d = new raw_data;
			ss >> d->survived >> d->pclass >> d->sex >> d->age >> d->sibsp >> d->parch >> d->ticket >> d->fare >> d->embarked >> d->cabin_a >> d->cabin_b;
			v.pb(*d);
		}
	}
	if(__MESSAGE__)
		cout << "Reading complete from file " << fnm << " ... " << endl;
}

//print for 
ostream &operator<<(ostream &output, const raw_data& d){
	string delim = ",";
	output << d.survived << delim << d.pclass << delim << d.sex << delim << d.age << delim << d.sibsp << delim << d.parch << delim << d.ticket << delim << d.fare << delim << d.embarked << delim << d.cabin_a << delim << d.cabin_b;
	return output;
}

int testingReadFunctions(){
	vector<raw_data> v; string fnm = "train.csv";
	readDataFromFile(fnm, v);
	cout << v.size() << endl;
	for(int i=0; i<10; i++)
		cout << v[i] << endl;
	return 0;
}

double computeMedian(vi& v){
	if(v.empty())
		return 0;
	sort(v.begin(), v.end());
	int size = v.size();
	if(size&1)
		return v[size/2];
	else{
		double ret = ((double)(v[size/2])) + v[(size/2)-1];
		return ret/2.0;
	}
}

double computeMedian(vector<double>& v){
	sort(v.begin(), v.end());
	int size = v.size();
	if(size&1)
		return v[size/2];
	else
		return (v[size/2] + v[(size/2)-1])/2.0;
}

// convert raw_data to vector of ints
void refineDataForFirstPart(vector<raw_data>& v, vvi& ar, vector<double>& medians, bool isTrainData){
	int size = v.size();
	ar.resize(size, vi(NUM_ATTR, 0));
	
	//Fill the discrete variables first
	for(int i=0; i<size; i++){
		ar[i][_SURVIVED] = v[i].survived;
		ar[i][_PCLASS] = v[i].pclass;
		ar[i][_SEX] = (v[i].sex.compare("male") == 0) ? MALE : FEMALE;
		ar[i][_EMBARKED] = (int)(v[i].embarked[0]);
		ar[i][_CABIN_A] = (int)(v[i].cabin_a[0]);
	}
	int numInd[] = {_SIBSP, _PARCH, _AGE, _TICKET, _FARE, _CABIN_B};

	if(isTrainData){
		medians.resize(NUM_ATTR,0);
		vi tempInt[2];
		vector<double> tempDouble[4];
		for(int i=0; i<size; i++){
			tempInt[0].pb(v[i].sibsp);
			tempInt[1].pb(v[i].parch);
			tempDouble[0].pb(v[i].age);
			tempDouble[1].pb(v[i].ticket);
			tempDouble[2].pb(v[i].fare);
			tempDouble[3].pb(v[i].cabin_b);
		}
		medians[_SIBSP] = computeMedian(tempInt[0]);
		medians[_PARCH] = computeMedian(tempInt[1]);
		medians[_AGE] = computeMedian(tempDouble[0]);
		medians[_TICKET] = computeMedian(tempDouble[1]);
		medians[_FARE] = computeMedian(tempDouble[2]);
		medians[_CABIN_B] = computeMedian(tempDouble[3]);

		// cout << "medians ";
		// for(int j : numInd){
		// 	cout << medians[j] << " ";
		// }
		// cout << endl;
	}

	for(int i=0; i<size; i++){
		ar[i][_SIBSP] = (v[i].sibsp > medians[_SIBSP]);
		ar[i][_PARCH] = (v[i].parch > medians[_PARCH]);
		ar[i][_AGE] = (v[i].age > medians[_AGE]);
		ar[i][_TICKET] = (v[i].ticket > medians[_TICKET]);
		ar[i][_FARE] = (v[i].fare > medians[_FARE]);
		ar[i][_CABIN_B] = (v[i].cabin_b > medians[_CABIN_B]);
	}

}

void refineDataForThirdPart(vector<raw_data>& v, vvi& ar, vector<double>& medians, bool isTrainData){
	int size = v.size();
	ar.resize(size, vi(NUM_ATTR, 0));
	
	//Fill the discrete variables first
	for(int i=0; i<size; i++){
		ar[i][_SURVIVED] = v[i].survived;
		ar[i][_PCLASS] = v[i].pclass;
		ar[i][_SEX] = (v[i].sex.compare("male") == 0) ? MALE : FEMALE;
		ar[i][_EMBARKED] = (int)(v[i].embarked[0]);
		ar[i][_CABIN_A] = (int)(v[i].cabin_a[0]);
		ar[i][_SIBSP] = v[i].sibsp;
		ar[i][_PARCH] = v[i].parch;
		ar[i][_AGE] = (int)(v[i].age * 100);
		ar[i][_TICKET] = (int)(v[i].ticket * 100);
		ar[i][_FARE] = (int)(v[i].fare * 10000);
		ar[i][_CABIN_B] = (int)(v[i].cabin_b * 100);
	}
}
