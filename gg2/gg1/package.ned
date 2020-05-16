simple Source
{
    parameters:
        volatile double interarrival_time; 
        volatile int initial_queue;        
		volatile int type;
    gates:
        output out;
}


//////////////////////////////////////////////////
simple fifo
{
    parameters:      
        volatile double service_timeA;   
        volatile double service_timeB;     
	    volatile int queue_len;
    gates:
        input inA;
        input inB;
        output outA;
        output outB;
}

simple K1
{
    parameters:
        volatile double data_volume; 
    gates:
        input in;
        output out;
}

simple K2
{
    gates:
        input in;
        output out;
}

simple Lacze
{
    gates:
        input in1;
        input in2;
        output out1;
        output out2;
}

simple Server
{
    parameters:
        volatile double service_timeA;   
        volatile double service_timeB;            
	    volatile int queue_len;
	    volatile int priority;
    gates:
        input inA;
        input inB;
        output outA;
        output outB;
}

//////////////////////////////////////////////////
simple Sink
{
    gates:
        input in1;
        input in2;
}


//////////////////////////////////////////////////
module GG1
{
	parameters:
	
    submodules:
        k1: K1 {
            parameters:
                @display("i=block/app2");
        }
        k2: K2 {
            parameters:
                @display("i=block/app2");
        }
        lacze: Lacze {
            parameters:
                @display("i=block/queue");
        }
    connections:
        k1.out --> {  } --> lacze.in1;
        k2.out --> {  } --> lacze.in2;
        lacze.out1 --> {  } --> k2.in;
        lacze.out2 --> {  } --> k1.in;
}

//////////////////////////////////////////////////
network gg1 extends GG1
{
    parameters:
}
