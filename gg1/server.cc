#include <omnetpp.h>
#include <histogram.h>
#include <fstream>

using namespace omnetpp;

class Server : public cSimpleModule
{		
  private:
    cQueue queue1;	 //the queue; first job in the queue is being serviced
    cQueue queue2;	 //the queue; first job in the queue is being serviced
	cMessage *departure1; //message-reminder of the end of service (job departure)
	cMessage *departure2; //message-reminder of the end of service (job departure)
	simtime_t departure_time;  //time of the next departure
	QueueHist* hist;
	int maxSize;
	double avgtime;
	int avgcounter;
	bool priority;
	simtime_t get_departure_time(cMessage *msgin);
	void handleMessage_fifo(cMessage *msgin);
	void handleMessage_buffer(cMessage *msgin);

  protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msgin);

};

Define_Module(Server);

omnetpp::simtime_t Server::get_departure_time(cMessage *msgin)
{
	if (msgin->getKind() == 0)
		{
			return simTime()+ par("service_timeA");
		}
		else
		{
			return simTime()+ par("service_timeB");
		}
}

void Server::finish()
{	
	hist->createPv();
	std::ofstream ofs ("server.txt", std::ofstream::out);

  	ofs << "\nsredni czas w kolejce: " << avgtime/avgcounter << std::endl;

	hist->printPv(ofs);
  	ofs.close();
}

void Server::initialize()
{	
	departure1 = new cMessage("Departure1");
	departure2 = new cMessage("Departure2");
	hist = new QueueHist;
	maxSize = par("queue_len");
	priority = (int)par("priority")==1;
	avgtime = 0;
	avgcounter = 0;
}

void Server::handleMessage_fifo(cMessage *msgin)
{	
	auto t1 = msgin->getClassName();
	auto t2 = msgin->getSenderModule();
	if (msgin==departure1 || msgin==departure2) //job departure
	{
		if (msgin==departure1)
		{
			cMessage *msg = (cMessage *)queue1.pop(); //remove job from the head of the queue
			avgtime += (simTime() - msg->getTimestamp()).dbl();
			avgcounter++;
			send(msg,"outA");
			if (!queue1.empty()) //schedule next departure event
			{
				departure_time=get_departure_time(msg);
				scheduleAt(departure_time,departure1);
			}
			hist->get();
		}
		else
		{
			cMessage *msg = (cMessage *)queue2.pop(); //remove job from the head of the queue
			avgtime += (simTime() - msg->getTimestamp()).dbl();
			avgcounter++;
			send(msg,"outA");
			if (!queue2.empty()) //schedule next departure event
			{
				departure_time=get_departure_time(msg);
				scheduleAt(departure_time,departure2);
			}
			hist->get();
		}
	}
	else //job arrival
	{	
		if (std::string(msgin->getArrivalGate()->getName()) == "inA")
		{
			if (queue1.empty())
			{
				scheduleAt(get_departure_time(msgin) ,departure1);
			}
			if (queue1.length() >= maxSize)
			{
				delete msgin;
				return;
			}
			msgin->setTimestamp(simTime());
			hist->put();
			queue1.insert(msgin); //put job at the end of the queue
		}
		else
		{
			if (queue2.empty())
			{
				scheduleAt(get_departure_time(msgin) ,departure2);
			}
			if (queue2.length() >= maxSize)
			{
				delete msgin;
				return;
			}
			msgin->setTimestamp(simTime());
			hist->put();
			queue2.insert(msgin); //put job at the end of the queue
		}
	}
}

void Server::handleMessage_buffer(cMessage *msgin)
{	
	if (msgin==departure1 || msgin==departure2) //job departure
	{
		//####TODO#####
		if (msgin==departure1)
		{
			cMessage *msg = (cMessage *)queue1.pop(); //remove job from the head of the queue
			avgtime += (simTime() - msg->getTimestamp()).dbl();
			avgcounter++;
			if (!queue1.empty()) //schedule next departure event
			{
				departure_time=get_departure_time(msg);
				scheduleAt(departure_time,departure1);
			}
			hist->get();
		}
		else
		{
			cMessage *msg = (cMessage *)queue2.pop(); //remove job from the head of the queue
			avgtime += (simTime() - msg->getTimestamp()).dbl();
			avgcounter++;
			if (!queue1.empty())
			{
				departure_time=get_departure_time(msg);
				scheduleAt(departure_time,departure1);
			}
			else if (!queue2.empty())
			{
				departure_time=get_departure_time(msg);
				scheduleAt(departure_time,departure2);
			}
			hist->get();
		}
	}
	else //job arrival
	{	
		if (msgin->getKind() == 0) // A type
		{
			if (queue1.empty() && queue2.empty())
			{
				scheduleAt(get_departure_time(msgin) ,departure1);
			}
			if (queue1.length() + queue2.length() >= maxSize)
			{
				delete msgin;
				return;
			}
			msgin->setTimestamp(simTime());
			hist->put();
			queue1.insert(msgin); //put job at the end of the queue
		}
		else //B type
		{
			if (queue1.empty() && queue2.empty())
			{
				scheduleAt(get_departure_time(msgin) ,departure2);
			}
			if (queue1.length() + queue2.length() >= maxSize)
			{
				delete msgin;
				return;
			}
			msgin->setTimestamp(simTime());
			hist->put();
			queue2.insert(msgin); //put job at the end of the queue
		}
	}
}

void Server::handleMessage(cMessage *msgin)
{		
    if (priority)
	{
		handleMessage_buffer(msgin);
	}
	else
	{
		handleMessage_fifo(msgin);
	}
}








