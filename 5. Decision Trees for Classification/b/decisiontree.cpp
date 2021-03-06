#include "decisiontree.h"

inline double surprise(double prob){
	if(prob <= eps)
		return 0;
	double ret = prob * log2(prob);
	return (-1.0*ret);
}

// Loads the data from CSVs to the data members. Does the median thresholding
void DecisionTree::LoadAllData(){
	vector<raw_data> rdata; string fnm = "train.csv";
	readDataFromFile(fnm, rdata);
	vector<double> medians;
	refineDataForFirstPart(rdata, train, medians, true);
	
	rdata.clear(); fnm = "validation.csv";
	readDataFromFile(fnm, rdata);
	refineDataForFirstPart(rdata, validation, medians, false);

	rdata.clear(); fnm = "test.csv";
	readDataFromFile(fnm, rdata);
	refineDataForFirstPart(rdata, test, medians, false);

	size = train.size();

	if(__MESSAGE__)
		cout <<  " Loading data complete ... \n" << endl;
}

void DecisionTree::FlushVpidToFile(vpid& v, const char* filename){
	filebuf fb;
	fb.open(filename, ios::out);
	ostream file1(&fb);
	for(int i=0; i<v.size(); i++)
		file1 << v[i].first << "," << v[i].second << endl;
	fb.close();
}

void DecisionTree::FlushIntoFile(){

	if(__MESSAGE__)
		cout << "Flushing..." << endl;

	FlushVpidToFile(accTrain, "PruneTainAcc.csv");
	FlushVpidToFile(accVal, "PruneValAcc.csv");
	FlushVpidToFile(accTest, "PruneTestAcc.csv");
	
	filebuf fb;
	fb.open("PruneAllAcc.csv", ios::out);
	ostream file4(&fb);
	for(int i=0; i<accTest.size(); i++)
		file4 << accTest[i].first << "," << accTrain[i].second << "," << accVal[i].second << "," << accTest[i].second << endl;
	fb.close();
	
	if(__MESSAGE__)
		cout << "Flushing data into files finished ... " << endl;
}

//this function tells if the current decision tree correctly predicts the value at data point v
bool DecisionTree::CheckPrediction(vi& v){
	node *curr = root; int val;
	while(!curr->isLeaf){
		val = v[curr->attribute];
		map<int,int>::iterator it = curr->m.find(val);
		if(it == curr->m.end()){ //this means that this value for the attribute has not been encountered before
			//cout << "attribute = " << curr->attribute << " val = " << val << endl;
			//return false;
			//assert(0);
			it = curr->m.begin();
		}
		curr = curr->children[it->second];
	}
	int countZeroes = 0;
	for(int i=0; i<curr->data.size(); i++){
		if(train[curr->data[i]][0] == 0)
			countZeroes++;
	}
	int countOnes = curr->data.size() - countZeroes;
	int prediction = (countOnes > countZeroes) ? 1 : 0;
	return (prediction==v[0]);
}

double DecisionTree::ComputeAccuracy(vvi& v){
	int hits = 0;
	for(int i=0; i<v.size(); i++)
		if(CheckPrediction(v[i]))
			hits++;
	double acc = (hits*100.0)/v.size();
	return acc;
}

void DecisionTree::ComputeAccuracy(){

	if(__MESSAGE__)
		cout << "Tree size = " << tree_size << "..  Computing accuracies of predictions... " << endl;

	double acc = ComputeAccuracy(train);
	accTrain.pb(mp(tree_size,acc));
	if(__MESSAGE__) cout << "Training over .. ";
	
	acc = ComputeAccuracy(validation);
	accVal.pb(mp(tree_size,acc));	
	if(__MESSAGE__) cout << "Validation over .. ";
	
	acc = ComputeAccuracy(test);
	accTest.pb(mp(tree_size, acc));
	if(__MESSAGE__) cout << "Test over .. " << endl;

}

void DecisionTree::GrowDecisionTree(){
	queue<node*> q;
	q.push(root);
	int val; map<int,int>::iterator it;
	while(!q.empty()){
		node* curr = q.front(); q.pop();
		bool isAllSame = true; int ffval = train[curr->data[0]][0];
		for(int i=1; i<curr->data.size(); i++){
			if(train[curr->data[i]][0] != ffval){
				isAllSame = false;
				break;
			}
		}
		if(isAllSame)
			continue;
		int attr = ChooseAttributeToSplit(curr);
		if(attr == -1)
			continue;
		curr->isLeaf = false;
		curr->attribute = attr;
		curr->m.clear();
		for(int i : curr->data){
			val = train[i][attr];
			it = curr->m.find(val);
			if(it != curr->m.end())
				curr->children[it->second]->data.pb(i);
			else{
				//add a new node
				node* temp = new node; temp->isLeaf = true;
				temp->data.pb(i);
				temp->parAttr.insert(curr->parAttr.begin(), curr->parAttr.end());
				temp->parAttr.insert(attr);
				curr->m[val] = curr->children.size();
				curr->children.pb(temp);
				// tree_size++;
				q.push(temp);
			}
		}
		tree_size++;
		// curr->data.clear();
		//Since we are here, size of tree must have increased. So. Call that accuracy check function
		ComputeAccuracy();
		if(__MESSAGE__)
			cout << "accuracy computed ... " << endl;
	}
	//FlushIntoFile(); //not here. Should be done from outside
}

int DecisionTree::ChooseAttributeToSplit(node* curr){
	if(__MESSAGE__)
		cout << "Choosing an attribute to split ... " << endl;
	vector<pair<double,int> > v;
	double mutualInfo = 0;
	for(int i=1; i<NUM_ATTR; i++){
		if(curr->parAttr.count(i))
			continue;
		mutualInfo = ComputeEntropy(i, curr->data) - ComputeEntropyGivenSurvival(i, curr->data);
		v.pb(mp(mutualInfo, NUM_ATTR - i));
	}
	if(v.empty())
		return -1;
	sort(v.begin(), v.end());
	int ret = (NUM_ATTR - v.back().second);
	if(__MESSAGE__)
		cout << "Chosen attribute number for splitting : " << ret << "..." <<endl;
	return ret;
}

double DecisionTree::ComputeEntropy(int attr, vi& data){
	map<int,int> m; map<int,int>::iterator it;
	int s = data.size();
	for(int i : data){
		it = m.find(train[i][attr]);
		if(it != m.end())
			it->second++;
		else
			m[train[i][attr]] = 1;
	}
	double entropy = 0;
	for(it = m.begin(); it != m.end(); it++){
		double prob = ((double)(it->second))/s;
		entropy += surprise(prob);
	}
	return entropy;
}

double DecisionTree::ComputeEntropyGivenSurvival(int attr, vi& data){
	vi ones, zeroes;
	for(int i : data){
		if(train[i][0] == 0)
			zeroes.pb(i);
		else
			ones.pb(i);
	}
	double entropy = (ones.size() * ComputeEntropy(attr, ones) + zeroes.size() * ComputeEntropy(attr, zeroes)) / data.size();
	return entropy;
}

pair<node*,double> DecisionTree::GetNodeForPruning(node* curr, double& currValAcc){
	if(curr->isLeaf || curr->children.size()==0){
		curr->subtree_size = 0;
		return mp(curr, 0);
	}
	pair<node*,double> bestChild = GetNodeForPruning(curr->children[0], currValAcc);
	curr->subtree_size = 1 + curr->children[0]->subtree_size;
	for(int i=1; i<curr->children.size(); i++){
		pair<node*,double> temp = GetNodeForPruning(curr->children[i], currValAcc);
		if(temp.second > bestChild.second)
			bestChild = temp;
		curr->subtree_size += curr->children[i]->subtree_size;
	}

	//Now we compute validation set accuracy after removing the current node
	curr->isLeaf = true;
	double accChange = ComputeAccuracy(validation) - currValAcc;
	curr->isLeaf = false;
	if(accChange > bestChild.second){
		return mp(curr, accChange);
	}
	return bestChild;
}

void DecisionTree::DoPostPruning(){

	if(__MESSAGE__)
		cout << "DoPostPruning()" << endl; 
	double currValAcc = ComputeAccuracy(validation); node* prune;
	
	vpid tr, te, va; 
	tr.pb(mp(tree_size,ComputeAccuracy(train)));
	te.pb(mp(tree_size,ComputeAccuracy(test)));
	va.pb(mp(tree_size,ComputeAccuracy(validation)));

	while(1){
		if(__MESSAGE__)
			cout << "tree_size = " << tree_size << endl;
		pair<node*,double> pnd = GetNodeForPruning(root, currValAcc);
		if(pnd.second < 0 || pnd.first->isLeaf)
			break;
		currValAcc += pnd.second;
		prune = pnd.first;
		prune->isLeaf = true;
		tree_size -= prune->subtree_size;
		tr.pb(mp(tree_size,ComputeAccuracy(train)));
		te.pb(mp(tree_size,ComputeAccuracy(test)));
		va.pb(mp(tree_size,currValAcc));
	}
	accTrain = tr; accVal = va; accTest = te;
	FlushIntoFile();
	PrintRequiredOutput();
	if(__MESSAGE__)
		cout << "Pruing completed successfully..." << endl;
}

pii ComputeTreeSize(node* curr){
	if(curr->isLeaf)
		return mp(0,1);
	pii ret = mp(1,0);
	for(auto& v : curr->children){
		pii temp = ComputeTreeSize(v);
		ret.first += temp.first;
		ret.second += temp.second;
	}
	return ret;
}

void DecisionTree::PrintRequiredOutput(){
	cout << "Training set accuracy = " << accTrain.back().second << " \% " << endl;
	cout << "Validation set accuracy = " << accVal.back().second << " \% " << endl;
	cout << "Test set accuracy = " << accTest.back().second << " \% " << endl;
	pii ss = ComputeTreeSize(root);
	cout << "Nodes = " << ss.first << " Leaves = " << ss.second << endl;
}