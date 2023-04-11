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

network multipleSingleLink
{
    parameters:
        @display("bgb=835.632,612.864;bgg=100,1,grey95");
        @figure[title](type=label; pos=0,-1; anchor=sw; color=darkblue);

        int numberApps;

    submodules:

        visualizer: <default(firstAvailableOrEmpty("IntegratedCanvasVisualizer"))> like IIntegratedVisualizer if typename != "" {
            parameters:
                @display("p=587.664,364.896;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=580,200");
        }
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            @display("p=580,275");
        }


        vehicle1: <default("RtpsHost")> like INetworkNode if numberApps >=1 {
            parameters:
                @display("p=271.152,358.84802;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera1: <default("RtpsHost")> like INetworkNode if numberApps >=1 {
            parameters:
                @display("p=278.208,305.424;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle2: <default("RtpsHost")> like INetworkNode if numberApps >=2 {
            parameters:
                @display("p=204.62401,305.424;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera2: <default("RtpsHost")> like INetworkNode if numberApps >=2 {
            parameters:
                @display("p=203.61601,358.84802;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle3: <default("RtpsHost")> like INetworkNode if numberApps >=3 {
            parameters:
                @display("p=337.68002,420.336;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera3: <default("RtpsHost")> like INetworkNode if numberApps >=3 {
            parameters:
                @display("p=255.024,420.336;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle4: <default("RtpsHost")> like INetworkNode if numberApps >=4 {
            parameters:
                @display("p=378,305.424;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera4: <default("RtpsHost")> like INetworkNode if numberApps >=4 {
            parameters:
                @display("p=376.992,363.888;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle5: <default("RtpsHost")> like INetworkNode if numberApps >=5 {
            parameters:
                @display("p=406.224,431.424;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera5: <default("RtpsHost")> like INetworkNode if numberApps >=5 {
            parameters:
                @display("p=447.552,369.936;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle6: <default("RtpsHost")> like INetworkNode if numberApps >=6 {
            parameters:
                @display("p=337.68002,283.24802;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera6: <default("RtpsHost")> like INetworkNode if numberApps >=6 {
            parameters:
                @display("p=270.144,235.87201;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle7: <default("RtpsHost")> like INetworkNode if numberApps >=7 {
            parameters:
                @display("p=466.704,313.488;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera7: <default("RtpsHost")> like INetworkNode if numberApps >=7 {
            parameters:
                @display("p=439.488,250.992;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }

        vehicle8: <default("RtpsHost")> like INetworkNode if numberApps >=8 {
            parameters:
                @display("p=385.056,218.73601;i=misc/car3");
                numWlanInterfaces = 1;

        }
        camera8: <default("RtpsHost")> like INetworkNode if numberApps >=8 {
            parameters:
                @display("p=329.616,201.6;i=misc/securitycamera");
                numWlanInterfaces = default(1);
        }



}
