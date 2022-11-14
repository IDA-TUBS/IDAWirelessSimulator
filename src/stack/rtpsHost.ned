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

package src.stack;

import inet.node.base.TransportLayerNodeBase;
import inet.common.MessageDispatcher;
import src.rtps.Rtps;
import src.rtps.contract.iRtpsApp;
import src.rtps.apps.RtpsSubscriberApp;
import src.rtps.apps.RtpsPublisherApp;

//
// TODO auto-generated type
//
network RtpsHost extends TransportLayerNodeBase
{
    parameters:
        
        // int numApps = default(0);
        int numberReaderApps = default(0);
        int numberWriterApps = default(0);
        
   //     rtpsAppWriter[0..numberWriterApps] = [0..numberWriterApps];
        
        @figure[applicationLayer](type=rectangle; pos=250,6; size=1000,130; lineColor=#808080; cornerRadius=5; fillColor=#ffff00; fillOpacity=0.1);
        @figure[applicationLayer.title](type=text; pos=1245,11; anchor=ne; text="application layer");
        @figure[submodules];
        @display("i=device/pc2;bgb=1253.7,776.475");
        ipv4.arp.proxyArpInterfaces = default(""); // proxy arp is disabled on hosts by default

        
        //rtpsAppWriter[0..sizeof(rtpsAppWriter)] = 1;
        
        
    submodules:

        rtpsAppWriter[numberWriterApps]:  <default("RtpsPublisherApp")> like iRtpsApp {
            @display("p=347.24625,65.9925");
            
        }

        rtpsAppReader[numberReaderApps]:  <default("RtpsSubscriberApp")> like iRtpsApp {

            @display("p=883.0425,65.9925");
        }
        
        rtps: Rtps {
            @display("p=585.89996,66.15");
            numberWriters = parent.numberWriterApps;
            numberReaders = parent.numberReaderApps;
        }


        at: MessageDispatcher {
            parameters:
                @display("p=750,146;b=1000,5,,,,1");
        }




        
    connections allowunconnected:

        // Connection of Apps
        for i=0..numberWriterApps-1 {
            rtpsAppWriter[i].AppOut --> rtps.appApiIn++;
        }

        for i=0..numberReaderApps-1 {
            rtpsAppReader[i].AppIn <-- rtps.appApiOut++;
        }

        // Integration of RTPS
        rtps.socketOut --> at.in++;
        rtps.socketIn <-- at.out++;

        at.out++ --> udp.appIn if hasUdp;
        at.in++ <-- udp.appOut if hasUdp;

        at.out++ --> tcp.appIn if hasTcp;
        at.in++ <-- tcp.appOut if hasTcp;

        at.out++ --> sctp.appIn if hasSctp;
        at.in++ <-- sctp.appOut if hasSctp;

        at.out++ --> tn.in++;
        at.in++ <-- tn.out++;


}
