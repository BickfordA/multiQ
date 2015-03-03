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



struct ModeProbabilityCompare
{
	const Histogram hist;
	ModeProbabilityCompare(const Histogram & histogram) : hist(histogram) { ; }

	inline bool operator()(int buckIdxA, int buckIdxB)
	{
		return hist.kdeProbability(buckIdxA) < hist.kdeProbability(buckIdxB);
	}
};




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
		modes = cleanUpTinyModes(modes, hist, maxScale, maxScaleAdjusted);

		//check for capacity
		vector<Flow::Capacity>  capcacities = checkForCapacity(modes);

	}

	
}

vector<int> Flow::cleanUpTinyModes( vector<int> modes, const Histogram& hist,bool& maxScale, bool& maxScaleAdjusted)
{

	int max_prob_mode = *std::max_element(modes.begin(), modes.end(), ModeProbabilityCompare(hist));

	// adjust max_scale
	if (!maxScaleAdjusted) {
		// If we're looking at acks, then we want to ignore the
		// leftmost mode when adjusting max_scale.
		int max_prob_mode2 = max_prob_mode;
		if (_flowType == AK_FLOW && max_prob_mode == modes[0] && modes.size() > 1)
			max_prob_mode2 = *std::max_element(modes.begin() + 1, modes.end(), ModeProbabilityCompare(h));

		maxScale = adjustMaxScale( modes, hist.modePos(max_prob_mode2));
		maxScaleAdjusted = true;
	}

	double threshold = 0.01 * hist.prob(max_prob_mode);
	int *out = modes.begin();
	for (int *x = modes.begin(); x < modes.end(); x++)
		if (h.prob(*x) >= threshold)
			*out++ = *x;
	modes.erase(out, modes.end());
}

double Flow::adjustMaxScale( const vector<int > modes, double tallestModeMinScale)
{


}



vector<Flow::Capacity> checkForCapacity(vector<int> modes)
{

}


Flow::Capacity::Capacity(FlowType type, double scale, double ntt)
	:scale(scale), ntt(ntt)
{
	//TODO: there is some interesting logic here in the original method
}