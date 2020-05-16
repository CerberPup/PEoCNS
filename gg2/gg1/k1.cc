#include <omnetpp.h>
#include <histogram.h>
#include <fstream>
#include <random>

using namespace omnetpp;

class K1 : public cSimpleModule
{		
  private:
  	cMessage *send_event;
	enum TYPES{
		NONE=0,
		OPEN,
	  	OPEN_ACK,
		DATA,
		DATA_ACK,
		REL,
		REL_ACK
	  };
	TYPES current_type;
	int licznik_pakietow;
	int pakiety_do_wyslania;
	cStdDev statek;

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msgin);

};

Define_Module(K1);

void K1::finish()
{	
	std::cout << "Total jobs processed: " << statek.getCount() << endl;
    std::cout << "Avg queueing time:    " << statek.getMean() << endl;
    std::cout << "Max queueing time:    " << statek.getMax() << endl;
    std::cout << "Standard deviation:   " << statek.getStddev() << endl;
}

void K1::initialize()
{	
	send_event = new cMessage("Send!");
    scheduleAt(simTime(), send_event);
	current_type = TYPES::NONE;
	pakiety_do_wyslania = (int)par("data_volume").doubleValue();
	std::cout << "Pakiety do wyslania: " << pakiety_do_wyslania << std::endl;
	statek.setName("Z k1");
}

void K1::handleMessage(cMessage *msgin)
{		
	if (msgin == send_event)
	{
		if (current_type == TYPES::NONE)
		{
			current_type = TYPES::OPEN;
		}
		std::string s = "Job K1 " + std::to_string((int)current_type);
		cMessage* job = new cMessage(s.c_str());
		job->setContextPointer(current_type ==TYPES::DATA ?(void*)1500:(void*)20);
		job->setKind(current_type);
		send(job, "out");
	}
	else //job arrival
	{	
		if (msgin->getKind() == TYPES::OPEN_ACK)
		{
			current_type = TYPES::DATA;
		}
		else if (msgin->getKind()  == TYPES::DATA_ACK)
		{
			licznik_pakietow++;
			if (licznik_pakietow >= pakiety_do_wyslania)
			{
				current_type = TYPES::REL;
			}
		}
		else if (msgin->getKind()  == TYPES::REL_ACK)
		{
			current_type = TYPES::NONE;
		}
		if (current_type != TYPES::NONE)
		{
			scheduleAt(simTime(), send_event);
		}
		statek.collect(msgin->getCreationTime());
		delete msgin;
	}
}