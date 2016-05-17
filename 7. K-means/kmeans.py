from random import randint
import matplotlib.pyplot as plt

X = []			#Data
Labels = []		#Actual labels
Mu = []			#The centroids for k-means clustering algorithm
C = []			#C[i] denotes the cluster number "i" belongs to
countJ = []		#countJ[i] denotes the number of members in cluster i, based on Mu
allErrors = []
allDistort = []
firstline = ''
pix = []

def readDigitData(filename):
	global firstline;
	lno = 0
	with open(filename) as f:
		for line in f:
			if lno == 0:
				firstline = line.strip();
				lno += 1
				continue
			line = line.strip()
			X.append([])
			X[lno-1] = line.split(' ')
			X[lno-1].pop(0)
			X[lno-1] = [ int(x) for x in X[lno-1] ]
			lno += 1
	print(lno-1)
	parseFirstLine()

def parseFirstLine():
	print('firstline = ', firstline)
	ar = firstline.split(' ')
	for x in ar:
		print(x)
		x = x.strip('\"').lstrip("pixel");
		if(x==''):
			continue
		i = int(x);
		pix.append(i)

def readDigitLabels(filename):
	with open(filename) as f:
		for line in f:
			line = line.strip()
			if(line == '"x"'):
				continue
			curr = line.split(' ')
			Labels.append(int(curr[1]))
	print(len(Labels))

def initMu():
	global Mu
	nums = [ randint(250*i, 250*(i+1) - 1) for i in range(4) ]
	print(nums)
	Mu = [X[i][:] for i in nums]

# compute the distance of X[i] from Mu[j]
def computeDistance(i,j):
	diff = [X[i][k] - Mu[j][k] for k in range(len(Mu[0]))]
	return sum([x*x for x in diff])

# based on current Mu, it allocates the cluster number to each data point
# also returns the current distortion value, as in 1-c
def allocateClusters():
	global C, countJ
	C = len(X)*[0]
	countJ = 4*[0]
	distortion = 0
	for i in range(len(X)):
		c = 0
		val = computeDistance(i,0)
		for j in range(1,4):
			tempVal = computeDistance(i,j)
			if tempVal < val:
				val = tempVal
				c = j
		distortion += val
		C[i] = c
		countJ[c] += 1
	return distortion

# after allocating cluster number to each data point, this function computes erroe 1-d
def computeError():
	countEachVal = [[0 for x in range(10)] for y in range(4)]
	for i in range(len(X)):
		countEachVal[C[i]][Labels[i]] += 1
	error = 0
	for i in range(4):
		error += (sum(countEachVal[i]) - max(countEachVal[i]))
	return error

# k means clustering algorithm iteration. 
def kMeansIteration():
	global Mu
	distortion = allocateClusters()
	error = computeError()
	# Now update Mu
	for i in range(len(Mu)):
		Mu[i] = len(Mu[i])*[0]

	for i in range(len(X)):
		for j in range(len(X[i])):
			Mu[C[i]][j] += X[i][j]

	for i in range(4):
		Mu[i] = [x/countJ[i] for x  in Mu[i]]

	print(distortion, error)
	allDistort.append(distortion)
	allErrors.append(error * 100 / len(X))

def plotPoint(x):
	ncols, nrows = 28,28
	grid = [[0 for y in range(ncols)] for z in range(nrows)]
	index = 0;
	for i in pix:
		grid[i//ncols][i%ncols] = x[index]
		index += 1
	plt.matshow(grid)
	plt.show();

def main():
	readDigitData('digitdata.txt')
	readDigitLabels('digitlabels.txt')
	initMu()
	n = 20
	for i in range(n):
		kMeansIteration()
	xaxis = [x for x in range(1,n+1)]
	plt.plot(xaxis, allDistort)
	plt.ylabel('Distortion')
	plt.xlabel('Iteration no.')
	plt.show()

	plt.plot(xaxis, allErrors)
	plt.ylabel('Error in %')
	plt.xlabel('Iteration no.')
	plt.axis([0,n,0,50])
	plt.show()
	for x in Mu:
		plotPoint(x)
	return

main()