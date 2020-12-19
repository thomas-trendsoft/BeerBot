EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Distanzscanner"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x05_Row_Letter_First CON2
U 1 1 5FDD3936
P 2750 3150
F 0 "CON2" H 2800 3567 50  0000 C CNN
F 1 "HC-SR04" H 2800 3476 50  0000 C CNN
F 2 "Connector_JAE:JAE_LY20-10P-DT1_2x05_P2.00mm_Vertical" H 2750 3150 50  0001 C CNN
F 3 "~" H 2750 3150 50  0001 C CNN
	1    2750 3150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_02x02_Row_Letter_First CON1
U 1 1 5FDD5865
P 4050 3150
F 0 "CON1" H 4100 3350 50  0000 C CNN
F 1 "PI" H 4100 3250 50  0000 C CNN
F 2 "Connector_JAE:JAE_LY20-4P-DT1_2x02_P2.00mm_Vertical" H 4050 3150 50  0001 C CNN
F 3 "~" H 4050 3150 50  0001 C CNN
	1    4050 3150
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FDDED4F
P 3250 3350
F 0 "R1" H 3180 3304 50  0000 R CNN
F 1 "R330" H 3180 3395 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3180 3350 50  0001 C CNN
F 3 "~" H 3250 3350 50  0001 C CNN
	1    3250 3350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 5FDDF1EC
P 3600 3400
F 0 "R2" H 3530 3354 50  0000 R CNN
F 1 "R10K" H 3530 3445 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal" V 3530 3400 50  0001 C CNN
F 3 "~" H 3600 3400 50  0001 C CNN
	1    3600 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	3050 3150 3250 3200
Wire Wire Line
	3250 3500 3500 3500
Wire Wire Line
	3500 3500 3500 3400
Wire Wire Line
	3500 3400 4350 3400
Wire Wire Line
	4350 3400 4350 3250
Wire Wire Line
	3050 3250 3100 3250
Wire Wire Line
	3100 3250 3100 3350
Wire Wire Line
	3100 3350 3400 3350
Wire Wire Line
	3400 3350 3400 3250
Wire Wire Line
	3400 3250 3600 3250
Wire Wire Line
	3850 3250 3600 3250
Connection ~ 3600 3250
Wire Wire Line
	3850 3050 3850 3150
Wire Wire Line
	3050 3050 3850 3050
Wire Wire Line
	4350 2950 4350 3150
Wire Wire Line
	3500 3500 3500 3550
Wire Wire Line
	3500 3550 3600 3550
Connection ~ 3500 3500
Wire Wire Line
	3050 2950 4350 2950
NoConn ~ 2550 2950
NoConn ~ 2550 3050
NoConn ~ 2550 3150
NoConn ~ 2550 3250
NoConn ~ 2550 3350
NoConn ~ 3050 3350
$EndSCHEMATC
