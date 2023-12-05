Railway Worker Safety Alert System
Overview
Railway Worker Safety Alert System is a cutting-edge project designed to enhance the safety of workers operating on railway lines. The system employs state-of-the-art technology, leveraging the ESP32 4G LTE board for seamless communication and Firebase for cloud-based control. The primary goal is to provide timely warnings to railway workers about approaching trains, ensuring they have ample time to clear the tracks and avoid potential hazards.

Features
1. ESP32 4G LTE Board:
Utilizes the ESP32 microcontroller for robust performance and efficient communication.
Integrated 4G LTE connectivity ensures reliable data exchange in real-time.
2. Firebase Integration:
Cloud integration with Firebase enables remote control and seamless communication between the station engineer and the worker's device.
Real-time updates and secure data transmission provide a responsive and reliable alert system.
3. Visual Signaling:
LED indicators are strategically placed to signal the approaching train's direction.
Workers can quickly identify the specific lane from which the train is coming, improving situational awareness.
4. Audible and Vibrational Alerts:
Loud noise generation ensures that workers are alerted audibly, even in noisy environments.
Vibration mechanisms provide tactile feedback, enhancing alertness and ensuring workers are informed, especially in situations where audible alerts might be insufficient.
5. Remote Control:
Station engineers can remotely send signals through Firebase to trigger alerts on the worker's device.
This feature allows for immediate responsiveness to changing conditions and emergencies.
Components
ESP32 4G LTE Board: The core microcontroller for processing and communication.
LEDs: Visual indicators for signaling the train's direction.
Audio Module: Generates loud noise alerts for audible warnings.
Vibration Motor: Provides tactile feedback to workers.
Firebase: Cloud-based platform for remote control and data exchange.
Getting Started
Hardware Setup:

Connect the ESP32 board, LEDs, audio module, and vibration motor following the provided schematics.
Firebase Configuration:

Set up a Firebase project and configure the necessary credentials in the Arduino IDE.
Code Installation:

Upload the Arduino code provided in this repository to the ESP32 board.
Usage:

Station engineers can send signals through Firebase to trigger alerts on the worker's device.
Workers will receive visual, audible, and tactile warnings, ensuring they are promptly informed about approaching trains.
