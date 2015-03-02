#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "distribution.h"

#include <vector>


//
// Histogram
//
class Histogram
{
public: 
	Histogram();

	void plotPoints(std::vector<double> values, double width, double bucketWidth = -18.0);
	

	std::vector<int> modes(double significance, int minPoints);

	double kdeProbability(int bucketIdx);
	double kdeSigProb(int bucketIdx);


	double pos(int i) const { return _left + i * _bucketWidth; }
protected:

	std::vector<Bucket> _buckets;

	double _totalCount;
	double _kdeWidth;

	double _bucketWidth;
	double _kernelWidth;

	double _left; //value of left edge of smallest bucket
};

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