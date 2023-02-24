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

package src.rtps;

import src.rtps.adapters.InetAdapter;
import src.rtps.contract.iWriter;
import src.rtps.contract.iReader;
import src.rtps.entities.Writer;
import src.rtps.entities.Reader;
import src.rtps.helper.RtpsMessageDispatcher;


module Rtps
{
    parameters:
        //Nbr of writers and readers
        int numberWriters;
        int numberReaders;

        int nbrReadersPerApp;
        int nbrApps;

        @class(Rtps);
        @display("i=block/layer;bgb=449,299");
        
    gates:
        input socketIn;
        output socketOut;

        input appApiIn[]; //From Writer
        output appApiOut[]; //To Reader

    submodules:
        inetAdapter: InetAdapter 
        {
            @display("p=222,243");
        }

        reader[sizeof(appApiOut)]: <default("Reader")> like iReader 
        {
            @display("p=343,72");
        }

        writer[sizeof(appApiIn)]: <default("Writer")> like iWriter 
        {
            @display("p=128,72");
        }

        dispatcher: RtpsMessageDispatcher 
        {
            @display("p=222,150");
        }
        
    connections:
        inetAdapter.udpSocketIn <-- socketIn;
        inetAdapter.udpSocketOut --> socketOut;
        inetAdapter.dispatcherOut --> dispatcher.adapterIn;        
        dispatcher.adapterOut --> inetAdapter.dispatcherIn;

        // Reader ports
        for i=0..sizeof(appApiOut)-1 
        {
            dispatcher.readerOut++ --> reader[i].dispatcherIn;
            dispatcher.readerIn++ <-- reader[i].dispatcherOut;

            reader[i].appOut --> appApiOut++;            
        }

        // Writer ports
        for i=0..sizeof(appApiIn)-1 
        {
            dispatcher.writerIn++ <-- writer[i].dispatcherOut;
            dispatcher.writerOut++ --> writer[i].dispatcherIn;

            writer[i].appIn <-- appApiIn++;
        }
}
