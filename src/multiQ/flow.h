#ifndef FLOW_H
#define FLOW_H

#include "packet.h"
#include "histogram.h"

#include <string>
#include <vector>

#define NUMBER_OF_BANDWIDTH_SPECS 10

// common bandwidths
struct BandwidthSpec
{
	const char *name;
	double bandwidth, range_lo, range_hi;
};


static const BandwidthSpec bandwidthSpecs[NUMBER_OF_BANDWIDTH_SPECS]{
		{ "modem", 0.056, 0.000, 0.064     },
		{ "cable", 0.512, .75*0.512, 1.25*0.512 },
		{ "T1", 1.544, .75*1.544, 1.25*1.544 },
		{ "10bT", 10, 7, 11.7 },
		{ "T3", 29 * 1.544, 26 * 1.544, 32 * 1.544 },
		{ "100bT", 100, 71, 120 },
		{ "OC3", 155.52, 130, 165 },
		{ "OC12", 4 * 155.52, 3 * 155.52, 5 * 155.52 },
		{ "OC48", 16 * 155.52, 12 * 155.52, 20 * 155.52 },
		{ "OC192", 48 * 155.52, 36 * 155.52, 54 * 155.52 }
};


static const BandwidthSpec *closestCommonBandwidthSpec(double bandwidth)
{
	for (const BandwidthSpec *bw = &bandwidthSpecs[0]; bw < bandwidthSpecs + NUMBER_OF_BANDWIDTH_SPECS; bw++)
		if (bw->range_lo < bandwidth && bandwidth < bw->range_hi)
			return bw;
	return 0;
};

class Flow
{
public:

	enum FlowType { DATA_FLOW, AK_FLOW };

	struct Capacity
	{
		double scale;
		double ntt;

		double bandwidth;

		double commonBandwidth;
		const char* commonBandwithName;

		double commonbandwidth52;
		const char* commonBandwidth52Name;

		Capacity(FlowType type, double scale, double ntt);
	};

	Flow(std::vector<double> arrivalTimes, FlowType type);
	std::vector<Capacity> createCapacities(std::vector<double> interArrivalTimes);


	std::vector<Capacity> getCapacities() { return _capacities; }

	bool hasError();
	std::string errMsg();

private:
	
	
	
	void cleanUpTinyModes(std::vector<int>& modes, 
						const std::vector<double>& aTimes,
						const Histogram& hist, 
						double& maxScale, 
						bool& maxScaleAdjusted);

	double adjustMaxScale( const std::vector<double > modes, double tallestModeMinScale);
	
	
	void checkForCapacity(std::vector<int> modes, 
							int& lastNModes, 
							double& lastNtt, 
							double& scale, 
							const Histogram& hist,  
							std::vector<Flow::Capacity>& capacities);

	double modes2NTT(const Histogram hist, const std::vector<int>& modes) const;

	void filterCapacity(std::vector<Flow::Capacity>& capacities);

	std::string _errMsg();

	FlowType _flowType;

	std::vector<Capacity> _capacities;
};



#endif //FLOW_H