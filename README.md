# SMARTGUARD: Ultimate Bike Safety System

## Overview
SMARTGUARD is an innovative bike safety system designed to enhance rider safety through collision detection, autonomous braking, and emergency alert systems. The project integrates multiple sensors and communication modules to provide a comprehensive safety solution for bikers.

## Features
- **Object Detection:** Uses ultrasonic sensors to detect nearby obstacles and potential collisions.
- **Active Brake Assist:** Automatically applies brakes using a servo motor if a collision is imminent.
- **Accident Detection:** Monitors bike orientation and movement using an MPU6050 accelerometer and gyroscope to detect accidents.
- **GPS Tracking:** Captures the bike's location in real-time using a GPS module.
- **SOS Alerts:** Sends an emergency alert with the bike's location via GSM in case of an accident.

## Components Used
- **Microcontroller:** Arduino (e.g., Arduino Uno)
- **Sensors:**
  - Ultrasonic Sensor (HC-SR04)
  - MPU6050 (Accelerometer and Gyroscope)
- **Communication Modules:**
  - GPS Module (e.g., NEO-6M)
  - GSM Module (e.g., SIM800L)
- **Actuator:** Servo Motor for braking

## Circuit Diagram
Refer to the `circuit.jpg` for the detailed circuit diagram and connections.

## Installation and Setup
1. **Clone the Repository:**
    ```bash
    git clone https://github.com/HemanthKusanur/SMARTGUARD.git
    ```
2. **Install Required Libraries:**
    - Install necessary Arduino libraries for MPU6050, GPS, and GSM modules.
    - You can find these libraries in the Arduino Library Manager or include them manually in the `libraries/` folder.

3. **Upload the Code:**
    - Open `main_code/SMARTGUARD.ino` in the Arduino IDE.
    - Select the appropriate board and port.
    - Upload the code to your Arduino board.

4. **Hardware Setup:**
    - Connect the components as per the circuit diagram provided in the `circuit.jpg` file.
    - Power the Arduino and the connected modules.

## Usage
1. **Power on the System:**
    - The system will initialize and begin monitoring for potential hazards.

2. **Obstacle Detection:**
    - If an object is detected within a certain range, the system will activate the servo motor to apply the brakes.

3. **Accident Detection and SOS Alert:**
    - If an accident is detected, the system will send an SOS message with the GPS location via the GSM module.

## Future Enhancements
- **Enhanced Collision Detection:** Integrate more advanced sensors for better accuracy.
- **Voice Alerts:** Implement voice alerts or notifications to warn the rider of potential hazards.
- **Mobile App Integration:** Develop a mobile app to monitor the bike's status in real-time.

## Acknowledgments
- Thanks to the open-source community for providing valuable resources and libraries that made this project possible.
