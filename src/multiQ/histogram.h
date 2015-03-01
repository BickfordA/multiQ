#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "distribution.h"


class Histogram : public Distribution
{
public: 
	Histogram();

	void addValue(double value);

	double interArrivalTime();
};



#endif //HISTOGRAM_H