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

package src.rtps.entities.protocolW2RP;

import src.rtps.contract.iWriter;

simple WriterW2RP like iWriter
{
    parameters:
        
        // Other configuration parameters
        int fragmentSize = default(1000);
        double deadline @unit(s) = default(100ms);
        int historySize = default(1);
        
        int numberReaders = default(1);        
        
        // Timing Model
        double shaping @unit(s) = default(1000us);
        double nackSuppressionDuration @unit(s) = default(0us);
        double timeout @unit(s) = default(0us); //0us == disabled
        
        // enabling or disabling NACK Suppression
        bool enableNackSuppression = default(false);
        
        int appID = default(-1);
        string destAddresses = default(""); 		// list of IP addresses, separated by spaces ("": don't send)
        
        // multicast extension
        bool prioritized = default(true);
        string prioMode = default("FIXED"); // options: "FIXED"/"ADAPTIVE_LOW_PDR"/"ADAPTIVE_HIGH_PDR" 
        
        // Analysis configuration
        bool pushBackFragmentData = default(true);
        
        // Display Parameters
        @display("i=block/tx");
        
    gates:
        input appIn;
        input dispatcherIn;
        output dispatcherOut;
}
