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

package src.rtps.entities;

import src.rtps.contract.iWriter;

simple Writer like iWriter
{
    parameters:
        // Other configuration parameters
        int fragmentSize = default(1000);
        double deadline @unit(s) = default(100ms);
        int historySize = default(1);
        
        int numberReaders = default(0);        
        
        // Timing Model
        double shaping @unit(s) = default(10us);
        double hbPeriod @unit(s) = default(2000us);
        double nackSuppressionDuration @unit(s) = default(0us);
        
        // enabling or disabling NACK Suppression
        bool enableNackSuppression = default(false);
        // enabling or disabling separate HBs
        bool enableSeparateHBs = default(true);
        bool measureEfficiency = default(false);
        int appID = default(-1);
        string destAddresses = default(""); 		// list of IP addresses, separated by spaces ("": don't send)
        
        // Analysis configuration
        bool pushBackFragmentData = default(true);
        
        // Display Parameters
        @display("i=block/tx");
        
    gates:
        input appIn;
        input dispatcherIn;
        output dispatcherOut;
}
