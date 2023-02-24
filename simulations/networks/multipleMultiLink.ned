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

//package src.networks;
package simulations.networks;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.contract.INetworkNode;
import inet.physicallayer.wireless.common.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;

import src.stack.RtpsHost;

network multipleMultiLink
{
    parameters:
        @display("bgb=650,500;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

		int numberApps;
        int numberReceivingNodes;

    submodules:
        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            parameters:
                @display("p=100,300;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=580,275");
        }      
			
                
        sender[numberApps]: <default("RtpsHost")> like INetworkNode {
            parameters:
                @display("p=50,75,c,,50,50;i=misc/truck");
                numWlanInterfaces = default(1);
        }
        
    	receiversApp0[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 1 {
        	parameters:
           		@display("p=125,75,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp1[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 2 {
        	parameters:
           		@display("p=125,155,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp2[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 3 {
        	parameters:
           		@display("p=125,235,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp3[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 4 {
        	parameters:
           		@display("p=125,315,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp4[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 5 {
        	parameters:
           		@display("p=125,395,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp5[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 6 {
        	parameters:
           		@display("p=125,475,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp6[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 7 {
        	parameters:
           		@display("p=125,555,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp7[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 8 {
        	parameters:
           		@display("p=125,635,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp8[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 9 {
        	parameters:
           		@display("p=125,715,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp9[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 10 {
        	parameters:
           		@display("p=125,785,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
    	
    	receiversApp10[numberReceivingNodes]: <default("RtpsHost")> like INetworkNode if numberApps >= 11 {
        	parameters:
           		@display("p=125,865,r,,50,50;i=misc/truck");
            	numWlanInterfaces = 1;
    	}
        
}
