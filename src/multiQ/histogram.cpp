#include "histogram.h"

#include <algorithm>
#include <assert.h>

using namespace std;

double kdeKernel(double x)
{
	double f = 1 - (x * x);
	return 0.9375 * f * f;
}

Histogram::Histogram()
{
}

void Histogram::plotPoints(vector<double> values, double kernelWidth, double bucketWidth)
{

	_bucketWidth = bucketWidth;
	_kernelWidth = kernelWidth;
	assert(!values.empty());
	//sort the values so that we know where the smallest value is
	sort(values.begin(), values.end());

	if (_bucketWidth < 0)
		_bucketWidth = _kernelWidth / -_bucketWidth;


	_left = values[0] - kernelWidth - 1.5 * _bucketWidth;
	if (_left > values[0]){
		//check for underflow
		_left = 0;
	}

	vector<double>::iterator valIt = values.end();
	int bucketCount = (int)((valIt[- 1] + _kernelWidth + 1.5* _bucketWidth - _left) / _bucketWidth) + 3; //why three? start stop and ??? middle???

	_totalCount = values.size();

	//first bin is empty ( multiplied by -1.5 for finding _left)


	_buckets.push_back(Bucket(pos(0)-_kernelWidth, pos(0) + _kernelWidth, 0));

	//thes are used to mark the begin and end
	//of the section of values which
	//belong to a given bin
	int startBin = 0;
	int stopBin = 0;

	double inverseWidth = 1 / _kernelWidth;

	for (int i = 1; i < bucketCount; i++)
	{
		//find the bucket bound in the input vector
		double buckPos = modePos(i);
		
		double currentStart = buckPos - _kernelWidth;
		double currentStop = buckPos + _kernelWidth;

		while (startBin < values.size() ? values[startBin] < currentStart : false){
			startBin++;
		}

		while (stopBin< values.size() ? values[stopBin] < currentStop : false){
			stopBin ++;
		}

		//calculate the probability for the current bucket given its points
		double prob = 0;
		for (int idx = startBin; idx < stopBin; idx++){
			prob += kdeKernel((values[idx] - buckPos) *inverseWidth);
		}
		_buckets.push_back(Bucket(currentStart, currentStop, prob));

		currentStart = currentStart;
	}
}

list<string> Histogram::bucketStringList()
{
	list<string> outStats;

	for(Bucket b: _buckets)
	{
		outStats.push_back(to_string(b.start) + "," + to_string(b.stop) + "," + to_string(b.count) + "\n");
	}

	return outStats;
}

double Histogram::kdeProbability(int bucketIdx) const
{
	return(_buckets[bucketIdx].count / (_totalCount * _kernelWidth));
}

double Histogram::kdeSigProb(int bucketIdx) const
{
	return(sqrt(_buckets[bucketIdx].count) / _totalCount * _kernelWidth);
}
/*
* A mode is the highest point in any region statistically more likely than one before it
* and after it.
*
* This is calculated in three passes
* 1: Find all local maxima
* 2: Mark the significant points
* 3: Combine duplicate points
*/
vector<int> Histogram::modes(double significance, int minPoints)
{
	// Find local maxima 
	vector<Mode> modes;

	vector<Mode> potentialMode;

	for (int buckIdx = 0; buckIdx < _buckets.size(); buckIdx ++){
		//find the left hand neighbor value
		double leftDelta = (buckIdx == 0) ? -1 : _buckets[buckIdx - 1].count - _buckets[buckIdx].count;
		
		//find the right hand neighbor value
		double rightDelta = (buckIdx + 1 == _buckets.size()) ? -1 : _buckets[buckIdx + 1].count - _buckets[buckIdx].count;
		
		if (leftDelta <= 0) //freq to the left was less or equal to current freq
			potentialMode.push_back(Mode(buckIdx));
		
		if (rightDelta > 0) // the next mode increases
			potentialMode.clear();

		if (rightDelta < 0){ //the next mode decreases 
			for (Mode m : potentialMode)
				modes.push_back(m);
		}
	}



	//bracket each mode by expanding the bounds (a,b)
	for (vector<Mode>::iterator m = modes.begin(); m != modes.end(); m++) {
		double prob = kdeProbability(m->i);

		double probThresh = max(prob - significance * kdeSigProb(m->i), 0.0);

		bool killMode = false;
		for (m->a = (m->i - 1); m->a >= 0 && kdeProbability(m->a) > probThresh; m->a--){
			if (kdeProbability(m->a) > 0){ //earlier max with ono dip
				killMode = true;
				break;
			}
		}

		for (m->b = m->i + 1; m->b < _buckets.size() && kdeProbability(m->b) > probThresh && killMode; m->b++){
			if (kdeProbability(m->b) > prob){ // later max with no dip
				killMode = true;
			}
		}

		if (killMode){
			m->i = -1;
		}
	}

	//combine overlapping modes to "mode of modes"

	// coalesce overlapping modes into "mode of modes" (i.e. tallest)
	{
		std::sort(modes.begin(), modes.end());
		vector<Mode>::iterator m = modes.begin();
		// We sorted dead modes to the end of the list, so exit when we
		// encounter one.  (We kill some modes ourselves, but we skip them
		// right away.)
		for (vector<Mode>::iterator next_m = m + 1; next_m < modes.end() && next_m->i >= 0; next_m++) {
			if (next_m->i < m->b) { // next mode inside
				if (kdeProbability(next_m->i) > kdeProbability(m->i))
					m->i = next_m->i;
				if (next_m->b > m->b)
					m->b = next_m->b;
				next_m->i = -1;
			}
			else
				m = next_m;
		}
	}

	vector<int> modeBucketIdx;
	// sort modes in decreasing order of importance
	for (vector<Mode>::iterator m = modes.begin(); m < modes.end(); m++)
		if (m->i >= 0)
			modeBucketIdx.push_back(m->i);

	return modeBucketIdx;
}

double Histogram::probability(int bucket) const
{
	return (double)_buckets[bucket].count / _totalCount;
}