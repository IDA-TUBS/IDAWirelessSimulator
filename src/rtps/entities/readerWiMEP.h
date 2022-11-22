/*
 *
 */

#ifndef RTPS_ENTITIES_READERWIMEP_H_
#define RTPS_ENTITIES_READERWIMEP_H_

#include <omnetpp.h>

#include "reader.h""

using namespace omnetpp;
using namespace inet;

class ReaderWiMEP : public Reader
{
  public:
    /*
     * empty default constructor
     */
    ReaderWiMEP() {}

    /*
     * default destructor
     */
    ~ReaderWiMEP()
    {

    };

  private:
    /// same attributes as Reader

    /// the readers priority
    unsigned int priority;

  protected:
    /*
     * Overwritten method, initializes modules after its instantiation
     */
    virtual void initialize() override;

    /*
     * Overwritten method, called at the end of the simulation
     */

    virtual void finish() override;
};


#endif /* RTPS_ENTITIES_READERWIMEP_H_ */
