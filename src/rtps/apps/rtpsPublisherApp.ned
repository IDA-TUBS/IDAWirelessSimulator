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

package src.rtps.apps;

//
// TODO auto-generated type
//
import src.rtps.contract.iRtpsApp;
//
// TODO auto-generated type
//
simple RtpsPublisherApp like iRtpsApp
{
    parameters:
        // Simulation Time Parameters
        double startTime @unit(s) = default(0s);
        // Application Parameters
        double sampling_period @unit(ms) = default(100ms);
        int sample_size @unit(B) = default(20*1000B);
        
        double offset @unit(s) = default(0us);
        // Other
        @display("i=block/departure");
        @class(RtpsPublisherApp);
    gates:
        input AppIn;
        output AppOut;

}
