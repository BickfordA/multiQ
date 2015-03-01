#ifndef FLOW_H
#define FLOW_H

#include "packet.h"


#include <string>

class Flow
{

public:
	Flow();

	void addPacket(Packet nextPacket);


	double flowCapacity();



	bool hasError();
	std::string errMsg();
private:

	Distribution _arrivalDistribution;

	std::string _errMsg();

};


#endif //FLOW_H