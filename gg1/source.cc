#include <omnetpp.h>
#include <string>
using namespace omnetpp;

class Source : public cSimpleModule
{
	cMessage *send_event; //message-reminder: send next job

  protected:
	virtual void initialize();
	virtual void handleMessage(cMessage *msgin);
  private:
	enum TYPES {A=0,B=1};
	TYPES type;
};

Define_Module(Source);

void Source::initialize()
{  
	type = (int)par("type") == 0 ? TYPES::A : TYPES::B;
	for(int i=0;i<(int)par("initial_queue");i++) //this loop builds the initial queue
	{
		cMessage *job = new cMessage(" Job");
		job->setKind(type);
		send(job, "out" );
	}

	send_event = new cMessage("Send!");
	send_event->setKind(type);
    scheduleAt(par("interarrival_time"), send_event);
}

void Source::handleMessage(cMessage *msgin) //send next job
{	
    cMessage *job = new cMessage(" Job");
	job->setKind(type);
	send(job, "out" );
	scheduleAt(simTime()+par("interarrival_time"), send_event); //schedule next send event
}
