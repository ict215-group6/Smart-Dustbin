# Smart-Dustbin
# Project Overview

This project is an Arduino-based Smart Dustbin that automatically opens its lid when an object (like a hand or waste) is detected nearby.
The system was designed, simulated, and prepared for fabrication using Arduino, Proteus, and PCB design tools.

This project was completed as a team academic project.

# How the System Works

1. A sensor detects an object approaching the dustbin.
2. The Arduino processes the sensor input.
3. A servo motor is activated to open the dustbin lid.
4. After a short delay, the lid closes automatically.

# Tools & Technologies Used

* Arduino Uno
* Arduino IDE
* Proteus 8 Professional(for simulation)
* PCB Design Software (for board fabrication)
  
* Electronic components:
  * Ultrasonic / IR sensor
  * Servo motor
  * Resistors
  * Power supply

# Repository Structure

Smart-Dustbin/
│
├── Arduino_Code/
│   └── official_arduinocodegroup6.ino
│
├── Proteus_Design/
│   └── Smart Dustbin.pdsprj
│
├── PCB_Design/
│   └── Gerber_Files/
│       ├── *.GTL
│       ├── *.GBL
│       ├── *.GTO
│       ├── *.GTS
│       ├── *.GBS
│       ├── *.GKO
│       └── *.DRL
│
└── README.md

# How to Run the Simulation

1. Open the Arduino code using Arduino IDE
2. Compile and export the HEX file
3. Open the Proteus project file (`.pdsprj`)
4. Load the HEX file into the Arduino component
5. Run the simulation

# PCB Design

The repository includes Gerber and drill files required for PCB fabrication.
These files can be uploaded directly to a PCB manufacturer for production.

# Applications

* Smart waste management systems
* Touchless public sanitation
* Embedded systems learning
* IoT and automation projects

# Future Improvements

* Add IoT support (WiFi / Bluetooth)
* Implement power optimization
* Add fill-level detection
* Mobile app integration

# Team

*Group 6*
200 Mechatronics Engineering Students
Bells University of Technology
Chima,Joy Chinyere
Kuforiji, Ayomiposi Moyosore
Okoro, Amarachi Agnes
