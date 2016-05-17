#include <bits/stdc++.h>
using namespace std;

#ifndef __DATALOADER__
#define __DATALOADER__

#define pb	push_back
#define mp	make_pair

#define FEMALE 	0
#define MALE	1

#define _SURVIVED 	0
#define _PCLASS 	1
#define _SEX 		2
#define _AGE 		3
#define _SIBSP 		4
#define _PARCH 		5
#define _TICKET 	6
#define _FARE 		7
#define _EMBARKED 	8
#define _CABIN_A 	9
#define _CABIN_B 	10

#define __MESSAGE__ false

typedef long long int ll;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int,int> pii;
typedef pair<int,double> pid;
typedef vector<pid> vpid;

#define NUM_ATTR 11

const double eps = 0.00001;

struct raw_data{
	int pclass, sibsp, parch, survived;
	string sex, embarked, cabin_a;
	double fare, age, ticket, cabin_b;
};
ostream &operator<<(ostream &output, const raw_data& d);

void readDataFromFile(string fnm, vector<raw_data>& v);
void refineDataForFirstPart(vector<raw_data>& v, vvi& ar, vector<double>& medians, bool isTrainData);
double computeMedian(vector<double>& v);
double computeMedian(vi& v);
void refineDataForThirdPart(vector<raw_data>& v, vvi& ar, vector<double>& medians, bool isTrainData);

#endif 