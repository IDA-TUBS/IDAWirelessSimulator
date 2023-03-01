# IDAWirelessSimulator

IDA Wireless Simulator is an open source library for the simulation of wireless networks, such as used for V2X communication. Specifically, the IDA Wireless Simulator focuses on the evaluation of sample-based protocols (so far based on DDS) used for the exchange of large data objects in lossy wireless channels.

The library is designed to run within the OMNeT++ simulation engine. The IDA Wireless Simulator is developed by the Institute for Computer and Network Engineering (IDA) at TU Braunschweig.

## Requirements

The IDA Wireless Simulator is build upon [OMNeT++](https://omnetpp.org/) version 6. Additionally, the [INET framework](https://inet.omnetpp.org/) in version 4.4 is needed.

The library can be used on any operating system running OMNeT++ 6.x, including Linux and Windows. The instructions for installation and usage below are for Linux-based systems. For Windows, the procedure might differ.

For the purpose of automated experiment evaluation, Python 3 and the following packages are required:

- [numpy](https://numpy.org/install/)
- [matplotlib](https://matplotlib.org/stable/users/getting_started/index.html#installation-quick-start)
- [seaborn](https://seaborn.pydata.org/installing.html)
- [pandas](https://pandas.pydata.org/docs/getting_started/index.html#getting-started)

## Installation

Clone this repository.

```bash
git clone https://github.com/IDA-TUBS/IDAWirelessSimulator
```

Execute the setup script

```bash
./setup/setup.sh
```

and enter the path to your inet package installation. 

A small patch to the INET package is necessary. It can be applied by

```bash
./patchINET.sh
```

Then, the IDA Wireless Simulator can be added to your workspace within the OMNeT++ GUI, compiling and executing all relevant parts using the Eclipse IDE. Alternatively, everything can be build and executed from the command line. For further information on how to execute OMNeT++ code check the latest [OMNeT++ user guide](https://doc.omnetpp.org/omnetpp/UserGuide.pdf). All provided simulations can be executed using the *run.sh* scripts.

## Features

The IDA Wireless Simulator supports:

- Simulation of 802.11 wireless communication (cf. INET framework)

  - including MAC layer retransmissions for unicast communication
  - simulation of other applications
  - abstraction of channel interference using an average arbitration time
  - configurable bit error rates
  - Gilbert-Elliot model to simulate burst errors
- A simplified but standard-compliant RTPS middleware

  - BEC functionality (reliability QoS - stateful writer implementation)
  - configurable heartbeat period
- The W2RP protocol [1, 2] to ensure reliable communication in unicast scenarios

  - writer configuration: periodic shaping
- The WiMEP [3] protocol enabling dedicated error protection for multicast applications

  - multicast fragment transmissions and retransmissions
  - retransmission prioritization of readers
  - multicast timeouts
- Exemplary networks and simulation configurations

## Usage

Exemplary networks and simulation configurations can be found in `IDAWirelessSimulator/simulation`. These examples include simple unicast and multicast applications using either default RTPS, W2RP or WiMEP. In  `IDAWirelessSimulator/simulation/publications`, configurations used for the reference papers mentioned beneath can be found.

Execute either from the OMNeT++ GUI or command line. For the former option, just select the desired *.ini*-file and hit *run*:

<img src="misc/run.png" width="200">

A GUI will open, prompting the user to select a configuration.

To run existing parameter studies (*ini*-files) from the command line execute in the corresponding directory:

```bash
./run.sh
```

This will also automatically process and visualize the results.

To create new simulation configurations just follow the examples or consult the OMNeT++ documentation.

 Any protocol (RTPS, W2RP and WiMEP) can be modified or used as a basis for extended protocols to address new/different issues.

## References

[1] J. Peeck, M. Möstl, T. Ishigooka and R. Ernst, "A Middleware Protocol for Time-Critical Wireless Communication of Large Data Samples," *2021 IEEE Real-Time Systems Symposium (RTSS)*, Dortmund, DE, 2021, pp. 1-13, doi: 10.1109/RTSS52674.2021.00013.

[2] A. Bendrick, J. Peeck and R. Ernst, "On the effectiveness of W2RP in physical environments", 2023, *technical memorandum*, doi: 10.24355/dbbs.084-202301231301-0

[3] A. Bendrick, J. Peeck and R. Ernst, "An Error Protection Protocol for the Multicast Transmission of Data Samples in V2X environments", 2023, *currently under review*

## Contact

Alex Bendrick (bendrick@ida.ing.tu-bs.de)

Jonas Peeck (peeck@ida.ing.tu-bs.de)
