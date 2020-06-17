package inet.examples.inet.udpburst;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;
import ned.DatarateChannel;


network UDPBurst
{
    parameters:
        int numhosts;
        int numhosts2;
    types:
        channel C extends DatarateChannel
        {
            datarate = 1Mbps;
            delay = 0.1us;
        }
        channel C2 extends DatarateChannel
        {
            datarate = 2Mbps;
            delay = 0.1us;
        }
    submodules:
        host[numhosts + numhosts2]: StandardHost {
            parameters:
                @display("i=device/pc3");
        }
        sink: StandardHost {
            parameters:
                @display("p=250,300;i=device/pc2");
        }
        router2: Router {
            parameters:
                @display("p=100,200");
        }
        router: Router {
            parameters:
                @display("p=250,200");
        }
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
        }
    connections:
        for i=0..(numhosts)-1 {
            host[i].pppg++ <--> C <--> router.pppg++;
        }
        for i=(numhosts)..numhosts+numhosts2-1 {
            host[i].pppg++ <--> C <--> router2.pppg++;
        }
        router2.pppg++ <--> C2 <--> router.pppg++;
        sink.pppg++ <--> C <--> router.pppg++;
}

