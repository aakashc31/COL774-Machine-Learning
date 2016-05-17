Decision Trees for Classification


In this problem, we will work with one of the Kaggle competition datasets to predict Titanic survivors.
https://www.kaggle.com/c/titanic
You have to implement the decision tree algorithm for predicting
which passengers survived the tragedy; some groups of people were more likely to survive than others, such
as women, children, and the upper-class passengers etc. The dataset provided to you is split into 3 disjoint
subsets: training, validation and test data. The dataset has been preprocessed to remove any missing
values. It also specifies which attributes to treat as discrete and which ones as numerical. Look at the file
feature info.txt to get a basic understanding of the dataset. In addition to the class notes/slides, Chapter 3 of Mitchell's book
is a good reference for this problem. You may also want to read the original paper on the ID3 decision tree
learning algorithm by Ross Quinlan (available on the course website).

(a) Construct a decision tree using the given data to predict whether a passenger survives the
incident. The dataset contains a mix of categorical as well as numerical attributes. Preprocess (before
growing the tree) each numerical attribute into a Boolean attribute by a) computing the median
value of the attribute in the training data (make sure not to ignore the duplicates) b) replacing each
numerical value by a 1/0 value based on whether the value is greater than the median threshold or not.
Note that this process should be repeated for each attribute independently. For non-Boolean (discrete
valued) attributes, you should use a multi-way split as discussed in class. Use information gain as the
criterion for choosing the attribute to split on. In case of a tie, choose the attribute which appears
first in the ordering as given in the training data. Plot the train, validation and test set accuracies
against the number of nodes in the tree as you grow the tree. On X-axis you should plot the number
of nodes in the tree and Y-axis should represent the accuracy. Comment on your observations.


(b) One of the ways to reduce overfitting in decision trees is to grow the tree fully and then use
post-pruning based on a validation set. In post-pruning, we greedily prune the nodes of the tree (and
sub-tree below them) by iteratively picking a node to prune so that resultant tree gives maximum increase
in accuracy on the validation set. In other words, among all the nodes in the tree, we prune the
node such that pruning it(and sub-tree below it) results in maximum increase in accuracy over the validation
set. This is repeated until any further pruning leads to decrease in accuracy over the validation
set. Post prune the tree obtained in step (a) above using the validation set. Again plot the training,
validation and test set accuracies against the number of nodes in the tree as you successively prune
the tree. Comment on your findings. 


(c) In Part(a) we used the median value of a numerical attribute to convert it in a 1/0 valued
attribute as a pre-processing step. In a more sophisticated setting, no such pre-processing is done in
the beginning. At any given internal node of the tree, a numerical attribute is considered for a two
way split by calculating the median attribute value from the data instances coming to that node, and
then computing the information gain if the data was split based on whether the numerical value of
the attribute is greater than the median or not. As earlier, the node is split on the attribute which
maximizes the information gain. Note that in this setting, the original value of a numerical attribute
remains intact, and a numerical attribute can be considered for splitting in the tree mutliple times.
Implement this new way of handing numerical attributes. Report the numerical attributes which are
split multiple times in a branch (along with the maximum number of times they are split in any given
branch and the corresponding thresholds). Replot the curves in part (a). Comment on which results
(Part (a) or Part (c)) are better and why. You don't have to implement pruning for this part.
