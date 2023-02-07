package src.rtps.apps;

//
// TODO auto-generated type
//
import src.rtps.contract.iRtpsApp;


simple RtpsPublisherApp like iRtpsApp
{
    parameters:
        // data properties        
        int sampleSize @unit(B) = default(20*1000B);
        
        // timing properties
        double samplePeriod @unit(ms) = default(100ms);
        double offset @unit(s) = default(0us);
        double jitter @unit(s) = default(0us);
        
        // Other
        @display("i=block/departure");
        @class(RtpsPublisherApp);
    gates:
        input AppIn;
        output AppOut;

}
