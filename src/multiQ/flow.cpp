#include "flow.h"

#include "packet.h"

#include <algorithm>
#include <vector>

#include <assert.h>

using namespace std;

//This should be set to a value that cut off 
// spaces between packets that are unreasonably large
#define INTERARRIVAL_CUTOFF 35000

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




Flow::Flow(vector<double> arrivalTimes, FlowType type)
{
	//sort the interarival times
	sort(arrivalTimes.begin(), arrivalTimes.end());

	vector<Flow::Capacity> capacities = createCapacities(arrivalTimes);

	filterCapacity(capacities);

	_capacities = capacities;
}



vector<Flow::Capacity> Flow::createCapacities(vector<double> interarrivalTimes)
{
	vector<Flow::Capacity> capacities;

	//remove the values which are too big
	while (interarrivalTimes.back() > INTERARRIVAL_CUTOFF){
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
		cleanUpTinyModes(modes, interarrivalTimes, hist, maxScale, maxScaleAdjusted);

		//check for capacity
		checkForCapacity(modes, lastNModes, lastNTT, scale, hist, capacities);
	}

	return capacities;
}

void Flow::cleanUpTinyModes( vector<int>& modes, const vector<double>& aTimes , const Histogram& hist,double& maxScale, bool& maxScaleAdjusted)
{

	int max_prob_mode = *std::max_element(modes.begin(), modes.end(), ModeProbabilityCompare(hist));

	// adjust max_scale
	if (!maxScaleAdjusted) {
		// If we're looking at acks, then we want to ignore the
		// leftmost mode when adjusting max_scale.
		int max_prob_mode2 = max_prob_mode;
		if (_flowType == AK_FLOW && max_prob_mode == modes[0] && modes.size() > 1)
			max_prob_mode2 = *std::max_element(modes.begin() + 1, modes.end(), ModeProbabilityCompare(hist));

		maxScale = adjustMaxScale(aTimes, hist.modePos(max_prob_mode2));
		maxScaleAdjusted = true;
	}

	double threshold = 0.01 * hist.probability(max_prob_mode);


	vector<int>::iterator outEnd = modes.begin();
	for (vector<int>::iterator x = modes.begin(); x < modes.end(); x++)
		if (hist.probability(*x) >= threshold)
			*outEnd++ = *x;
	modes.erase(outEnd, modes.end());
}


double Flow::adjustMaxScale(const std::vector<double> modes, double tallestModeMinScale)
{
	double next_scale = tallestModeMinScale / 2.;

	Histogram hh;
	hh.plotPoints(modes, next_scale);

	vector<int> next_modes = hh.modes(SIGNIFICANCE, MIN_POINTS);

	// Ignore the leftmost mode if we're looking at acks.
	vector<int>::iterator next_modes_start = next_modes.begin();
	if (_flowType == AK_FLOW && next_modes_start < next_modes.end())
		next_modes_start++;

	vector<int>::iterator tallest_ptr = std::max_element(next_modes_start, next_modes.end(), ModeProbabilityCompare(hh));
	double tallest_mode_next_scale = (tallest_ptr < next_modes.end() ? hh.modePos(*tallest_ptr) : -1);

	return std::min(std::max(tallestModeMinScale, tallest_mode_next_scale),(double) MAX_SCALE);
}


void Flow::checkForCapacity(vector<int> modes, 
							int& lastNModes, 
							double& lastNtt, 
							double& scale, 
							const Histogram& hist,  
							vector<Flow::Capacity>& capacities)
{
	// check for capacity
	if ((int)modes.size() > lastNModes) {
		// skip if number of modes is increasing (odd behavior)
		scale *= SCALE_STEP;

	}
	else if (modes.size() == 1) {
		// output this final mode, if it's significantly different from
		// last capacity mode
		double ntt = hist.modePos(modes[0]);
		if ((ntt - lastNtt) / (ntt + lastNtt) > MODES_SIMILAR)
			capacities.push_back(Capacity(_flowType, scale, ntt));
		return;

	}
	else if (modes.size() == 2 && scale < MAX_SCALE / 4) {
		// try and resolve two modes into one if at a smallish scale
		scale *= SCALE_STEP;

	}
	else if (modes.size() == 2) {
		// end if two modes at a large scale
		// output a heuristic combination of the modes
		double x1 = hist.modePos(modes[0]), h1 = hist.probability(modes[0]);
		double x2 = hist.modePos(modes[1]), h2 = hist.probability(modes[1]);

		// first mode if it is pretty large probability
		if (h1 > 0.25*h2
			&& (x1 - lastNtt) / (x1 + lastNtt) > MODES_SIMILAR) {
			capacities.push_back(Flow::Capacity(_flowType, scale, x1));
			lastNtt = x1;
		}

		// second mode if it is very large probability; or it is
		// relatively large probability, and at a distance, but not
		// extremely far away
		double relative_dist = (x2 - x1) / x1;
		if ((h2 > 2 * h1	// very large probability
			|| (h2 > 0.7*h1 // large probability
			&& !(0.985 < relative_dist && relative_dist < 1.015)
			// at a distance
			&& (x2 - x1) < 3 * x1)) // not extremely far away
			&& (x2 - lastNtt) / (x2 + lastNtt) > MODES_SIMILAR) {
			capacities.push_back(Capacity(_flowType , scale, x2));
		}

		return;

	}
	else {
		double ntt = modes2NTT(hist, modes);
		if (ntt >= 0 && (ntt - lastNtt) / (ntt + lastNtt) > MODES_SIMILAR) {
			capacities.push_back(Capacity(_flowType, scale, ntt));
			lastNtt = ntt;
			scale = std::max(ntt, scale) * SCALE_STEP;
		}
		else
			scale *= SCALE_STEP;
	}
}

double Flow::modes2NTT(const Histogram hist, const vector<int>& modes) const
{
	vector<double> gaps;

	double last_x = 0;		// insert artificial mode at 0
	double min_gap = 1e15;

	// Ignore the leftmost mode if we're looking at acks
	vector<int>::const_iterator modes_begin = modes.begin();
	if (_flowType == AK_FLOW)
		last_x = hist.modePos(*modes_begin), modes_begin++;

	for (vector<int>::const_iterator m = modes_begin; m < modes.end(); m++) {
		double x = hist.modePos(*m);
		if (x >= 0) {
			double gap = x - last_x;
			int n = (int)(hist.probability(*m) * 1000);
			gaps.resize(gaps.size() + n, gap);
			min_gap = std::min(gap, min_gap);
			last_x = x;
		}
	}

	if (gaps.size() == 0)
		return -1;

	std::sort(gaps.begin(), gaps.end());
	assert(gaps.back() < INTERARRIVAL_CUTOFF);

	Histogram gap_h;
	gap_h.plotPoints(gaps, 0.4*min_gap);

	vector<int> gap_modes = gap_h.modes(GAP_SIGNIFICANCE, GAP_MIN_POINTS);

	// if the first mode in the list is tallest, return it
	if (gap_modes.size() >= 1
		&& std::max_element(gap_modes.begin(), gap_modes.end(), ModeProbabilityCompare(gap_h)) == gap_modes.begin())
		return gap_h.modePos(gap_modes[0]);
	else
		return -1;

}

void Flow::filterCapacity(vector<Flow::Capacity>& capacities)
{
	std::reverse(capacities.begin(), capacities.end());
	bool flag = false;
	double lastBandwidth = 0;
	vector<Flow::Capacity>::iterator out = capacities.begin();
	for (vector<Flow::Capacity>::iterator n = capacities.begin(); n < capacities.end(); n++) {
		if (30 < n->ntt && n->ntt < 46 && flag)
			continue;
		if ((100 < n->ntt && n->ntt < 170)
			|| (950 < n->ntt && n->ntt < 1350))
			flag = true;
		if (n->ntt > n->scale && n->commonBandwidth != lastBandwidth) {
			*out++ = *n;
			lastBandwidth = n->commonBandwidth;
		}
	}
	capacities.erase(out, capacities.end());
}


Flow::Capacity::Capacity(FlowType type, double scale, double ntt)
	:scale(scale), ntt(ntt)
{
	if (type == DATA_FLOW)
		bandwidth = 1500 * 8 / ntt, commonbandwidth52 = 52 * 8 / ntt;
	else
		bandwidth = 1552 * 8 / ntt, commonbandwidth52 = 52 * 8 / ntt;

	if (const BandwidthSpec *bw = closestCommonBandwidthSpec(bandwidth)) {
		commonBandwidth = bw->bandwidth;
		commonBandwithName= bw->name;
	}
	else {
		commonBandwidth = bandwidth;
		commonBandwithName= "?";
	}
	if (const BandwidthSpec *bw = closestCommonBandwidthSpec(commonbandwidth52)) {
		commonbandwidth52 = bw->bandwidth;
		commonBandwidth52Name= bw->name;
	}
	else {
		commonbandwidth52 = commonbandwidth52;
		commonBandwidth52Name = "?";
	}
}