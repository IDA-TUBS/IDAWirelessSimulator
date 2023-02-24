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

package src.rtps.adapters;

//
// TODO auto-generated type
//
simple InetAdapter
{
    int timeToLive = default(-1); // if not -1, set the TTL (IPv4) or Hop Limit (IPv6) field of sent packets to this value
    
    
    string interfaceTableModule;   							// The path to the InterfaceTable module
    int localPort = default(-1);  							// local port (-1: use ephemeral port)
    int numberDestinations = default(-1);
    string destAddresses = default(""); 					// list of IP addresses, separated by spaces ("": don't send)
    string localAddress = default("");
    string packetName = default("UdpBasicAppData");
    int destPort;
    
    double startTime @unit(s) = default(this.sendInterval); // application start time (start of the first packet)
    double stopTime @unit(s) = default(-1s);  				// time of finishing sending, -1s means forever
    
    bool dontFragment = default(false); 					// if true, asks IP to not fragment the message during routing
    int dscp = default(-1); 								// if not -1, set the DSCP field (on IPv4/IPv6) of sent packets to this value
    int tos = default(-1); 									// if not -1, set the Type Of Service (IPv4) / Traffic Class (IPv6) field of sent packets to this value
    string multicastInterface = default("");  				// if not empty, set the multicast output interface option on the socket (interface name expected)
        
	bool receiveBroadcast = default(false); 				// if true, makes the socket receive broadcast packets
    bool joinLocalMulticastGroups = default(false); 		// if true, makes the socket receive packets from all multicast groups set on local interfaces
	
	@lifecycleSupport;
    double stopOperationExtraTime @unit(s) = default(-1s);    // extra time after lifecycle stop operation finished
    double stopOperationTimeout @unit(s) = default(2s);    // timeout value for lifecycle stop operation
    
    @signal[packetSent](type=inet::Packet);
    @signal[packetReceived](type=inet::Packet);

    @display("i=block/segm");
    
    gates:
        //UDP inputs and outputs
        input udpSocketIn;
        output udpSocketOut;
        //Writer inputs
        input dispatcherIn;
        //Reader Outputs
        output dispatcherOut;
}
