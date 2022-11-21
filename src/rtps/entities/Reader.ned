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

import src.rtps.contract.iReader;

simple Reader like iReader
{
    parameters:      
        int appID = default(-1);
        int historySize = default(1);
        
        double deadline @unit(s) = default(100ms);
        double readerResponseDelay @unit(s) = default(0ms);
        
        string destAddresses = default(""); 		// list of IP addresses, separated by spaces ("": don't send)
        // TODO static delay
        
        @display("i=block/rx");
    gates:        
        output appOut;
        input dispatcherIn;
        output dispatcherOut;

}
