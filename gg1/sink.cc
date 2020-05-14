#include <omnetpp.h>

using namespace omnetpp;

class Sink : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msgin);
};


Define_Module(Sink);


void Sink::handleMessage(cMessage *msgin)
{
	delete msgin;
}
