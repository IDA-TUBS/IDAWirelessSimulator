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

package rtps.entities;

simple Reader
{
    parameters:

        // History size defines for how many last samples the latency is evaluated         
        int appID = default(-1);
        
        @display("i=block/rx");
    gates:
        
        output app_out;
        input dispatcher_in;
        output dispatcher_out;

}
