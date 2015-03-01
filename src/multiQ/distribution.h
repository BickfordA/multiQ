#ifndef DISTRIBUTION_H
#define DISTRIBUTION_H


class Distribution
{

public:
	Distribution();

	virtual void addValue(double value) = 0 ;


	virtual double interArrivalTime() = 0 ;
};


#endif //DISTRIBUTION_H