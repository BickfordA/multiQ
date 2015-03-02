#include "flow.h"

#include "packet.h"

#include <algorithm>
#include <vector>

using namespace std;

//This should be set to a value that cut off 
// spaces between packets that are unreasonably large
#define INTERARRIVAL_CUTTOFF 35000

#define MIN_SCALE 10
#define MAX_SCALE 10000
#define SCALE_STEP 1.1
#define SCALE_STEP_NOMODES 1.5
#define SIGNIFICANCE 2
#define MIN_POINTS 10 
#define GAP_SIGNIFICANCE 1 
#define GAP_MIN_POINTS 2 
#define MODES_SIMILAR 0.05

Flow::Flow(vector<Packet> packetFlow, FlowType type)
{
	//find the inter arival times
	vector<double> interarrivalTimes;



	//sort the interarival times
	sort(interarrivalTimes.begin(), interarrivalTimes.end());


	

}



vector<Flow::Capacity> Flow::createCapacities(vector<double> interarrivalTimes)
{
	
	//remove the values which are too big
	while (interarrivalTimes.back() > INTERARRIVAL_CUTTOFF){
		interarrivalTimes.pop_back();
	}

	if (interarrivalTimes.empty()){
		return vector<Capacity>();
	}


	double maxScale = MAX_SCALE;
	bool maxScaleAdjusted = false;
	int lastNModes = INT_MAX;
	double lastNTT = 0;

	double scale = MIN_SCALE;
	while (scale < maxScale){
		//compute the kernel PDF
		Histogram hist;
		hist.plotPoints(interarrivalTimes, scale);

		//find the modes
		vector<int> modes = hist.modes(SIGNIFICANCE, MIN_POINTS);

		//if no modes were found increase the size and try again
		if (modes.empty()){
			scale *= SCALE_STEP_NOMODES;
			continue;
		}


		//clean up the tiny modes


		//check for capacity


		//



	}

	
}

Flow::Capacity::Capacity(FlowType type, double scale, double ntt)
	:scale(scale), ntt(ntt)
{
	//TODO: there is some interesting logic here in the original method
}