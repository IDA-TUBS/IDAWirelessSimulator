package src.rtps.apps;

//
// TODO auto-generated type
//
import src.rtps.contract.iRtpsApp;
//
// TODO auto-generated type
//
simple RtpsSubscriberApp like iRtpsApp
{
    parameters:
        @display("i=block/arrival");
        @class(RtpsSubscriberApp);
    gates:
        input AppIn;
        output AppOut;

}
