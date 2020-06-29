simple SimpleHost
{
    parameters:

        @display("i=block/routing");
    gates:
         inout gate[ ];
}
simple SimpleSwitch
{
    parameters:

        @display("i=block/routing");
    gates:
         inout gate[ ];
}
network Network
{
	parameters:
        int k = 4;
        int EXB_SIZE = default(5);
        double SIMULATOR_TIME = default(1); // Simulation time = 1s        
        double OPERATION_CYCLE_TIME_SWITCH = 3334e-9;
        double INTERVAL = 1e-2; 
        double CREDIT_DELAY = default(0.000000001); 
        double MSG_GEN_INTERVAL = default(0.0001);

 	@display("bgb=3000.0, 1200.0");
    
      
types:
		channel Channel extends ned.DatarateChannel
        {
            datarate = 1Gbps;
        }
           submodules:
        
		node0 : SimpleHost {
            parameters:
                @display("p=100.0, 1050");
        }
		node1 : SimpleHost {
            parameters:
                @display("p=287.5, 1050");
        }
		node2 : SimpleHost {
            parameters:
                @display("p=475.0, 1050");
        }
		node3 : SimpleHost {
            parameters:
                @display("p=662.5, 1050");
        }
		node4 : SimpleSwitch {
            parameters:
                @display("p=100.0, 650");
        }
		node5 : SimpleSwitch {
            parameters:
                @display("p=475.0, 650");
        }
		node6 : SimpleSwitch {
            parameters:
                @display("p=100.0, 450");
        }
		node7 : SimpleSwitch {
            parameters:
                @display("p=475.0, 450");
        }
		node8 : SimpleHost {
            parameters:
                @display("p=850.0, 1050");
        }
		node9 : SimpleHost {
            parameters:
                @display("p=1037.5, 1050");
        }
		node10 : SimpleHost {
            parameters:
                @display("p=1225.0, 1050");
        }
		node11 : SimpleHost {
            parameters:
                @display("p=1412.5, 1050");
        }
		node12 : SimpleSwitch {
            parameters:
                @display("p=850.0, 650");
        }
		node13 : SimpleSwitch {
            parameters:
                @display("p=1225.0, 650");
        }
		node14 : SimpleSwitch {
            parameters:
                @display("p=850.0, 450");
        }
		node15 : SimpleSwitch {
            parameters:
                @display("p=1225.0, 450");
        }
		node16 : SimpleHost {
            parameters:
                @display("p=1600.0, 1050");
        }
		node17 : SimpleHost {
            parameters:
                @display("p=1787.5, 1050");
        }
		node18 : SimpleHost {
            parameters:
                @display("p=1975.0, 1050");
        }
		node19 : SimpleHost {
            parameters:
                @display("p=2162.5, 1050");
        }
		node20 : SimpleSwitch {
            parameters:
                @display("p=1600.0, 650");
        }
		node21 : SimpleSwitch {
            parameters:
                @display("p=1975.0, 650");
        }
		node22 : SimpleSwitch {
            parameters:
                @display("p=1600.0, 450");
        }
		node23 : SimpleSwitch {
            parameters:
                @display("p=1975.0, 450");
        }
		node24 : SimpleHost {
            parameters:
                @display("p=2350.0, 1050");
        }
		node25 : SimpleHost {
            parameters:
                @display("p=2537.5, 1050");
        }
		node26 : SimpleHost {
            parameters:
                @display("p=2725.0, 1050");
        }
		node27 : SimpleHost {
            parameters:
                @display("p=2912.5, 1050");
        }
		node28 : SimpleSwitch {
            parameters:
                @display("p=2350.0, 650");
        }
		node29 : SimpleSwitch {
            parameters:
                @display("p=2725.0, 650");
        }
		node30 : SimpleSwitch {
            parameters:
                @display("p=2350.0, 450");
        }
		node31 : SimpleSwitch {
            parameters:
                @display("p=2725.0, 450");
        }
		node32 : SimpleSwitch {
            parameters:
                @display("p=100.0, 50");
        }
		node33 : SimpleSwitch {
            parameters:
                @display("p=750.0, 50");
        }
		node34 : SimpleSwitch {
            parameters:
                @display("p=1500.0, 50");
        }
		node35 : SimpleSwitch {
            parameters:
                @display("p=2250.0, 50");
        }
		 
    connections:
		node4.gate++ <-->  Channel  <--> node0.gate++;
		node4.gate++ <-->  Channel  <--> node1.gate++;
		node5.gate++ <-->  Channel  <--> node2.gate++;
		node5.gate++ <-->  Channel  <--> node3.gate++;
		node4.gate++ <--> Channel  <--> node6.gate++;
		node4.gate++ <--> Channel  <--> node7.gate++;
		node5.gate++ <--> Channel  <--> node6.gate++;
		node5.gate++ <--> Channel  <--> node7.gate++;
		node6.gate++ <--> Channel  <--> node32.gate++;
		node6.gate++ <--> Channel  <--> node33.gate++;
		node7.gate++ <--> Channel  <--> node34.gate++;
		node7.gate++ <--> Channel  <--> node35.gate++;
		node12.gate++ <-->  Channel  <--> node8.gate++;
		node12.gate++ <-->  Channel  <--> node9.gate++;
		node13.gate++ <-->  Channel  <--> node10.gate++;
		node13.gate++ <-->  Channel  <--> node11.gate++;
		node12.gate++ <--> Channel  <--> node14.gate++;
		node12.gate++ <--> Channel  <--> node15.gate++;
		node13.gate++ <--> Channel  <--> node14.gate++;
		node13.gate++ <--> Channel  <--> node15.gate++;
		node14.gate++ <--> Channel  <--> node32.gate++;
		node14.gate++ <--> Channel  <--> node33.gate++;
		node15.gate++ <--> Channel  <--> node34.gate++;
		node15.gate++ <--> Channel  <--> node35.gate++;
		node20.gate++ <-->  Channel  <--> node16.gate++;
		node20.gate++ <-->  Channel  <--> node17.gate++;
		node21.gate++ <-->  Channel  <--> node18.gate++;
		node21.gate++ <-->  Channel  <--> node19.gate++;
		node20.gate++ <--> Channel  <--> node22.gate++;
		node20.gate++ <--> Channel  <--> node23.gate++;
		node21.gate++ <--> Channel  <--> node22.gate++;
		node21.gate++ <--> Channel  <--> node23.gate++;
		node22.gate++ <--> Channel  <--> node32.gate++;
		node22.gate++ <--> Channel  <--> node33.gate++;
		node23.gate++ <--> Channel  <--> node34.gate++;
		node23.gate++ <--> Channel  <--> node35.gate++;
		node28.gate++ <-->  Channel  <--> node24.gate++;
		node28.gate++ <-->  Channel  <--> node25.gate++;
		node29.gate++ <-->  Channel  <--> node26.gate++;
		node29.gate++ <-->  Channel  <--> node27.gate++;
		node28.gate++ <--> Channel  <--> node30.gate++;
		node28.gate++ <--> Channel  <--> node31.gate++;
		node29.gate++ <--> Channel  <--> node30.gate++;
		node29.gate++ <--> Channel  <--> node31.gate++;
		node30.gate++ <--> Channel  <--> node32.gate++;
		node30.gate++ <--> Channel  <--> node33.gate++;
		node31.gate++ <--> Channel  <--> node34.gate++;
		node31.gate++ <--> Channel  <--> node35.gate++;

}