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
        int EXB_SIZE = default(2);        
    gates:       
        output out;
}

simple SimpleDestination
{	
    parameters:
        @display("i=block/switch");
    gates:
        input in;
        
}

network Network1
{
    parameters:
        double SIMULATOR_TIME = default(1); // Simulation time = 1s
        double MSG_GEN_INTERVAL = default(0.005);
    submodules:
        
        source: SimpleSource;
                
        destination: SimpleDestination;
        
    connections:
        source.out --> {  delay = 15ms; } --> destination.in;        
}