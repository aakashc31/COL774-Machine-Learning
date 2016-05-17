#include "dataloader.h"
#include "decisiontree.h"

using namespace std;

int main(){
	DecisionTree* dt = new DecisionTree();
	dt->GrowDecisionTree();
	dt->FlushIntoFile();

	dt->DoPostPruning();
	return 0;
}