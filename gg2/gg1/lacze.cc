#include <omnetpp.h>
#include <histogram.h>
#include <fstream>
#include <random>

using namespace omnetpp;

class Lacze : public cSimpleModule
{		
  private:
	cMessage* send_event;
	cMessage* last_message;
  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msgin);
};

Define_Module(Lacze);

void Lacze::finish()
{	
}

void Lacze::initialize()
{	
	send_event = new cMessage("Send!");
}


void Lacze::handleMessage(cMessage *msgin)
{	
	if (msgin == send_event)
	{
		if (last_message->getArrivalGate()->getName() == std::string("in1"))
		{
			send(last_message, "out1");
		}
		else
		{
			send(last_message, "out2");
		}
	}
	else
	{
		auto size = (unsigned long)msgin->getContextPointer();
		last_message = msgin;
		double slip_time = (double)(size*8)/128000;
		scheduleAt(simTime() + slip_time, send_event);
	}
}