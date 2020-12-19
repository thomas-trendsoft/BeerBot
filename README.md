# BeerBot

Erster Versuch eines simplen mobilen und teilautonomen Roboter. Basis-Aufgabe wird
"Hol ein Bier" oder anderen Gegenstandes von der Lager-Position bis zum anfordernden Benutzer.

Basis Aufgaben für die erste Version:

- Navigation / Mapping (Basis Orientierung)
- Object Detection (Bier erkennung)
- Steuerung der Aktuatoren zum Heben (Bier aufnehmen)

## Basis Aufbauplan

### Software

- TinySLAM als Navigation und Mapping Algorithmus
- YOLOvX as Object Detection Network mit eigenem Training für die Bierdose
- KiCAD für Schaltpläne
- FreeCAD für Bauteile und 3D Druck-Teile

### Hardware

Für die elektronische Steuerung und Wahrnehmung sind bisher folgende Sensoren und Bauteile vorgesehen:

- Raspberry Pi 3 (oder 4) als Basisboard
- HC-SR04 Distanzsensor für eine Feature und Hinderniserkennung
- 28BYJ-48 Schrittmotoren als Host für die Distanzmessung in unterschiedlichen Richtungen
- ULN2003 Schrittmotor Steuerung zum Ansprechen der Schrittmotoren
- MPU 6050 Gyroscope für die Berechnung von Winkeländerung und Beschleunigung
- 2 DC Motoren für den 2 Rad Antrieb  
- L298N Motortreiber für den Antrieb

Eine genaue Einkaufsliste soll später erstellt werden zum Nachbauen.

#### Hardware

##### PIN Belegung

Die PIN Nummer entspricht der GPIO Nummer bei der Raspberry Belegung.

- DC Motor 1
    - Pin0 = enable
    - Pin1 = in1
    - Pin7 = in2
- DC Motor 2
    - Pin9 = enable
    - Pin10 = in1
    - Pin11 = in2
- Servo Motor 1
    - Pin12 = PWM
- Servo Motor 2
    - Pin13 = PWM
- Schritt Motor
    - Pin22 = StepPin0
    - Pin23 = StepPin1
    - Pin24 = StepPin2
    - Pin25 = StepPin3
- HC-SR04 Distanzsensor
    - Pin17 = Echo
    - Pin18 = Trigger
- Taster1
    - Pin14 = IN
- Taster2
    - Pin15 = IN
- ODO Meter 1
    - Pin5 = IN
- ODO Meter 2
    - Pin6 = IN
- Gyroscope / MPU 6050
    - Pin2 = SDA (have to check I2C on Raspberry if not exact wrong)
    - Pin3 = SCL


##### Datenblätter/Papers

- TinySLAM: https://ieeexplore.ieee.org/document/5707402
- MPU6050 Basic Datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf
- MPU6050 Register Datasheet: https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
