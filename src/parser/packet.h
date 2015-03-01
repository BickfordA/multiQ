#ifndef PACKET_H
#define PACKET_H


class Packet
{
public:
	Packet(/*TODO: All the good parameters harvested from parser */);

	double interarrivalTime(const Packet& other) const;
	
	double arrivalTime();
	double packetSize();

	//TODO: add other important stuff
};




#endif //PACKET_H