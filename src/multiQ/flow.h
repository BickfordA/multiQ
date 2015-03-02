#ifndef FLOW_H
#define FLOW_H

#include "packet.h"
#include "histogram.h"

#include <string>



class Flow
{
	//forward dec..
	struct Capacity;
public:

	enum FlowType { DATA_FLOW, AK_FLOW };

	
	Flow(std::vector<Packet> packetFlow, FlowType type);
	

	std::vector<Capacity> createCapacities(std::vector<double> interArrivalTimes);


	bool hasError();
	std::string errMsg();


	struct Capacity
	{
		double scale;
		double ntt;

		double bandwidth;

		double commonBandwidth;
		const char* commonBandwithName;

		double bandwidth52;
		const char* commonBandwidth52Name;

		Capacity(FlowType type, double scale, double ntt);
	};

private:

	std::string _errMsg();
};



#endif //FLOW_H