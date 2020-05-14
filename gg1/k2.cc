#include <omnetpp.h>
#include <histogram.h>
#include <fstream>
#include <random>

using namespace omnetpp;
class K2 : public cSimpleModule
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
	cStdDev statek;
  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msgin);
};

Define_Module(K2);

void K2::finish()
{	
}

void K2::initialize()
{	
	send_event = new cMessage("Send!");
	current_type = TYPES::NONE;
}


void K2::handleMessage(cMessage *msgin)
{		
	static cStdDev statek;
	if (msgin == send_event)
	{
		if (current_type == TYPES::NONE)
		{
			return;
		}
		std::string s = "Job K2 " + std::to_string((int)current_type);
		cMessage *job = new cMessage(s.c_str());
		job->setContextPointer((void*)20);
		job->setKind(current_type);
		send(job, "out");
	}
	else //job arrival
	{	
		if (msgin->getKind() == TYPES::OPEN)
		{
			current_type = TYPES::OPEN_ACK;
		}
		else if (msgin->getKind()  == TYPES::DATA)
		{
			current_type = TYPES::DATA_ACK;
		}
		else if (msgin->getKind()  == TYPES::REL);
		{
			current_type = TYPES::REL_ACK;
		}
		scheduleAt(simTime(), send_event);
		statek.collect(msgin->getCreationTime());
		delete msgin;
	}
}