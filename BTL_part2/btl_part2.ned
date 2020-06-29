//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//
simple SimpleSource
{
    parameters:    
        @display("i=block/source");                
    gates:       
        output out;
        input in;
}

simple SimpleSwitch
{
    parameters:    
        @display("i=block/switch");                
    gates:       
        output out[];
        input in[];
}

simple SimpleDestination
{	
    gates:
        input in;
        output out;        
}

network SimpleNetwork
{
    parameters:
        int EXB_SIZE = default(2);
        double SIMULATOR_TIME = default(1); // Simulation time = 1s
        double MSG_GEN_INTERVAL = default(0.005);
        int numberOfSource = 3;
        int numberOfDestination = 1;
    submodules:
        
        source[3]: SimpleSource;
        switch : SimpleSwitch;      
        destination: SimpleDestination;
        
    connections:
        source[0].out --> {  delay = 15ms; } --> switch.in++;
        source[1].out --> {  delay = 15ms; } --> switch.in++; 
        source[2].out --> {  delay = 15ms; } --> switch.in++;
            
        switch.out++ -->  {  delay = 1ms; } --> source[0].in;  
        switch.out++ -->  {  delay = 1ms; } --> source[1].in;  
        switch.out++ -->  {  delay = 1ms; } --> source[2].in;
        
        switch.out++ -->  {  delay = 15ms; } --> destination.in;
        destination.out -->  {  delay = 1ms; } --> switch.in++;
}