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
import src.rtps.entities.Writer;
import src.rtps.entities.Reader;
import src.helper.RtpsMessageDispatcher;


//
// TODO auto-generated type
//
module Rtps
{
    parameters:

        //Nbr of writers and readers
        int nbr_writers;
        int nbr_readers;

        @class(Rtps);
        @display("i=block/layer;bgb=450,385");
    gates:
        input socketIn;
        output socketOut;

        input appApiIn[]; //From Writer
        output appApiOut[]; //To Reader

    submodules:
        inet_adapter: InetAdapter {
            @display("p=222,313");
        }

        reader[sizeof(appApiOut)]: Reader {
            @display("p=343,72");
        }

        writer[sizeof(appApiIn)]: Writer {
            @display("p=128,72");
        }

        dispatcher: RtpsMessageDispatcher {
            @display("p=222,150");
        }
    connections:

        inet_adapter.udp_socket_in <-- socketIn;
        inet_adapter.udp_socket_out --> socketOut;

        inet_adapter.dispatcher_out --> dispatcher.adapter_in;

        // Reader ports
        for i=0..sizeof(appApiOut)-1 {
            dispatcher.reader_out++ --> reader[i].dispatcher_in;
            dispatcher.reader_in++ <-- reader[i].dispatcher_out;

            reader[i].app_out --> appApiOut++;
        }

        // Writer ports
        for i=0..sizeof(appApiIn)-1 {
            dispatcher.writer_in++ <-- writer[i].dispatcher_out;
            dispatcher.writer_out++ --> writer[i].dispatcher_in;

            writer[i].app_in <-- appApiIn++;
        }
}
