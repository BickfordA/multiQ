#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>


//
// Bucket 
// - holds the intervals of the histogram
//

struct Bucket
{
	Bucket(int start, int stop, int count)
		:start(start), stop(stop), count(count)
	{
	};

	double start;
	double stop;

	int count;
};

//
// Histogram
//
class Histogram
{
public: 
	Histogram();

	void plotPoints(std::vector<double> values, double width, double bucketWidth = -18.0);
	


	std::vector<int> modes(double significance, int minPoints);

	double kdeProbability(int bucketIdx) const;
	double kdeSigProb(int bucketIdx) const;


	double pos(int i) const { return _left + i * _bucketWidth; }

	double modePos(int i) const { return pos(i); }
	double probability(int bucket) const;
	
protected:

	std::vector<Bucket> _buckets;

	double _totalCount;

	double _bucketWidth;
	double _kernelWidth;

	double _left; //value of left edge of smallest bucket
};


//
// Mode 
// - used for computing the modes in the histogram
//

namespace
{
	struct Mode
	{
		inline Mode(int idx)
			:a(idx), b(idx), i(idx)
		{
		};

		int a, i, b;
	};

	inline bool
		operator<(const Mode &a, const Mode &b)
	{
		if (a.i < 0 || b.i < 0)
			return b.i < a.i;
		else if (a.a != b.a)
			return a.a < b.a;
		else if (a.i != b.i)
			return a.i < b.i;
		else
			return a.b < b.b;
	}
}

#endif //HISTOGRAM_H