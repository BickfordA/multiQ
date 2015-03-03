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
	
	
	
	std::vector<int> cleanUpTinyModes(vector<int> modes, const Histogram& hist, bool& maxScale, bool& maxScaleAdjusted);
	double adjustMaxScale( const vector<int > modes, double tallestModeMinScale);
	
	
	std::vector<Capacity> checkForCapacity(std::vector<Capacity>);


	std::string _errMsg();

	FlowType _flowType;
};



#endif //FLOW_H