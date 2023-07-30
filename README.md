# Vehicle Theft Detection System
An anti theft system for vehicles, especially cars, developed using Arduino. This system is a prototype smart lock for vehicles
which can track the location of the vehicle in real time and can jam the ignition on command.

## Components
The major components used are:
| S.No. | Component                                |
| :---- | :--------------------------------------- |
| 1     | Arduino Mega 2560                        |
| 2     | GPS Module                               |
| 3     | SMCOM SIM900A GSM Module                 |
| 4     | 16x2 LCD Display                         |
| 5     | Membrane Keypad                          |
| 6     | Relay Module                             |

## Circuit
The circuit diagram was created using [circuito.io](https://www.circuito.io/).

![Circuit](https://github.com/Nesasio/Anti-Theft-System/assets/110229836/23decbbe-5285-4745-9024-b6b396e17bb2)
*Basic Circuit*

## Hardware Simulation
Circuit designing, component testing and simulation of individual components and the prototype circuit was done using
[Proteus Design Suite](https://www.labcenter.com/).

![Proteus](https://github.com/Nesasio/Anti-Theft-System/assets/110229836/690fa4a9-01ee-44b6-80f1-b5f24f0f13f8)
*Proteus Simulation of GSM* 

## Functionality
This system adds an additional layer of security to the vehicle by password locking the ignition system of the 
vehicle. Whenever the ignition is to be turned on, this system will ask for a passowrd, and will grant access if
the correct password is entered. However, when incorrect password is entered, this system will not allow the
ignition to be turned on and will notify the owner the current location of the vehicle via SMS, hence, saving
from theft.
