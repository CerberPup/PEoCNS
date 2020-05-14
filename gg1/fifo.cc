#include <omnetpp.h>
#include <histogram.h>
#include <fstream>
#include <random>

using namespace omnetpp;

class fifo : public cSimpleModule
{		
  private:
    cQueue queue;	 //the queue; first job in the queue is being serviced
	cMessage *departure; //message-reminder of the end of service (job departure)
	simtime_t departure_time;  //time of the next departure
	QueueHist* hist;
	int maxSize;
	double avgtime;
	int avgcounter;

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msgin);

};

Define_Module(fifo);

void fifo::finish()
{	
	hist->createPv();
	std::ofstream ofs ("fifo0.txt", std::ofstream::out);

  	ofs << "\nsredni czas w kolejce: " << avgtime/avgcounter << std::endl;

	hist->printPv(ofs);
  	ofs.close();
}

void fifo::initialize()
{	
	departure = new cMessage("Departure");
	hist = new QueueHist;
	maxSize = par("queue_len");
	avgtime = 0;
	avgcounter = 0;
	srand(time(NULL));
}


void fifo::handleMessage(cMessage *msgin)
{		
    if (msgin==departure) //job departure
	{
		cMessage *msg = (cMessage *)queue.pop(); //remove job from the head of the queue
		avgtime += (simTime() - msg->getTimestamp()).dbl();
		avgcounter++;

		string send_to = ((double)rand() / RAND_MAX) <=0.4 ? "outA" : "outB";
		send(msg,send_to.c_str());
		std::cout<<"sending to: " + send_to<< std::endl;
		if (!queue.empty()) //schedule next departure event
		{
			if (msgin->getKind() == 0)
			{
				departure_time=simTime()+ par("service_timeA");
			}
			else
			{
				departure_time=simTime()+ par("service_timeB");
			}
	        scheduleAt(departure_time,departure);
		}
		hist->get();
	}
	else //job arrival
	{	
		if (queue.empty())
		{
			if (msgin->getKind() == 0)
			{
				departure_time=simTime()+ par("service_timeA");
			}
			else
			{
				departure_time=simTime()+ par("service_timeB");
			}
            scheduleAt(departure_time,departure);
		}
		if (queue.length() >= maxSize)
		{
			delete msgin;
			return;
		}
		msgin->setTimestamp(simTime());
		hist->put();
		queue.insert(msgin); //put job at the end of the queue
	}
}








