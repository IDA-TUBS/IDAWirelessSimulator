package src.stack;

import src.rtps.Rtps;
import src.stack.RtpsLayerNodeBase;
import src.rtps.contract.iRtpsApp;

//
// TODO auto-generated type
//
network StandardRtpsHost extends RtpsLayerNodeBase
{
    parameters:
        @display("i=device/pc2;bgb=1253.7,776.475");
        @figure[submodules];
        ipv4.arp.proxyArpInterfaces = default(""); // proxy arp is disabled on hosts by default
        *.routingTableModule = default("^.ipv4.routingTable");
        int nbr_read_apps = default(1);
        int nbr_write_apps = default(1);

    submodules:

        rtps_app_writer[nbr_write_apps]: <> like iRtpsApp {
            @display("p=347.24625,65.9925");
        }

        rtps_app_reader[nbr_read_apps]: <> like iRtpsApp {

            @display("p=883.0425,65.9925");
        }

    connections allowunconnected:

        for i=0..nbr_write_apps-1 {
            rtps_app_writer[i].AppOut --> rtps.appApiIn++;
        }

        for i=0..nbr_read_apps-1 {
            rtps_app_reader[i].AppIn <-- rtps.appApiOut++;
        }
}