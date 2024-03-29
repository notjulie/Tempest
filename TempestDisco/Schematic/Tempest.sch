EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Tempest-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Tempest Control Panel & Sound Board"
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L STM32F4Discovery U1
U 1 1 5ACEC6CF
P 4350 1850
F 0 "U1" H 4400 1800 60  0000 C TNN
F 1 "STM32F4Discovery" H 4400 1800 60  0000 C BNN
F 2 "" H 4400 1800 60  0001 C CNN
F 3 "" H 4400 1800 60  0001 C CNN
	1    4350 1850
	1    0    0    -1  
$EndComp
$Comp
L Audio-Jack-3 J?
U 1 1 5AD396A6
P 4650 1250
F 0 "J?" H 4600 1425 50  0001 C CNN
F 1 "Audio" H 4750 1180 50  0000 C CNN
F 2 "" H 4900 1350 50  0001 C CNN
F 3 "" H 4900 1350 50  0001 C CNN
	1    4650 1250
	0    1    1    0   
$EndComp
$Comp
L Screw_Terminal_01x12 J1
U 1 1 5AD39888
P 2250 1900
F 0 "J1" H 2250 2500 50  0000 C CNN
F 1 "Screw_Terminal_01x12" H 2250 1200 50  0000 C CNN
F 2 "" H 2250 1900 50  0001 C CNN
F 3 "" H 2250 1900 50  0001 C CNN
	1    2250 1900
	-1   0    0    -1  
$EndComp
$Comp
L GNDREF #PWR?
U 1 1 5AD39DB1
P 2750 1600
F 0 "#PWR?" H 2750 1350 50  0001 C CNN
F 1 "GNDREF" H 2750 1450 50  0001 C CNN
F 2 "" H 2750 1600 50  0001 C CNN
F 3 "" H 2750 1600 50  0001 C CNN
	1    2750 1600
	1    0    0    -1  
$EndComp
$Comp
L R R1
U 1 1 5AD3A0BD
P 2850 2100
F 0 "R1" V 2930 2100 50  0000 C CNN
F 1 "470" V 2850 2100 50  0000 C CNN
F 2 "" V 2780 2100 50  0001 C CNN
F 3 "" H 2850 2100 50  0001 C CNN
	1    2850 2100
	0    1    1    0   
$EndComp
Wire Wire Line
	3500 1400 2450 1400
Wire Wire Line
	3500 1550 3500 1500
Wire Wire Line
	3500 1500 2450 1500
Wire Wire Line
	2450 1600 2750 1600
Wire Wire Line
	2450 1800 2900 1800
Wire Wire Line
	2900 1800 2900 1700
Wire Wire Line
	2900 1700 3500 1700
Wire Wire Line
	2450 1900 3000 1900
Wire Wire Line
	3000 1900 3000 1850
Wire Wire Line
	3000 1850 3500 1850
Wire Wire Line
	2450 2000 3500 2000
Wire Wire Line
	2450 2200 3000 2200
Wire Wire Line
	3000 2200 3000 2300
Wire Wire Line
	3000 2300 3500 2300
Wire Wire Line
	2450 2100 2700 2100
Wire Wire Line
	3000 2100 3250 2100
Wire Wire Line
	3250 2100 3250 2150
Wire Wire Line
	3250 2150 3500 2150
$Comp
L R R2
U 1 1 5AD3A235
P 3150 2450
F 0 "R2" V 3230 2450 50  0000 C CNN
F 1 "470" V 3150 2450 50  0000 C CNN
F 2 "" V 3080 2450 50  0001 C CNN
F 3 "" H 3150 2450 50  0001 C CNN
	1    3150 2450
	0    1    1    0   
$EndComp
Wire Wire Line
	3300 2450 3500 2450
Wire Wire Line
	2450 2300 2750 2300
Wire Wire Line
	2750 2300 2750 2450
Wire Wire Line
	2750 2450 3000 2450
Wire Wire Line
	2450 2400 2700 2400
Wire Wire Line
	2700 2400 2700 2800
Wire Wire Line
	2700 2800 4200 2800
Wire Wire Line
	4200 2800 4200 2750
$Comp
L GNDREF #PWR?
U 1 1 5AD3A39C
P 4450 2750
F 0 "#PWR?" H 4450 2500 50  0001 C CNN
F 1 "GNDREF" H 4450 2600 50  0001 C CNN
F 2 "" H 4450 2750 50  0001 C CNN
F 3 "" H 4450 2750 50  0001 C CNN
	1    4450 2750
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5AD3A3D8
P 2450 2950
F 0 "R3" V 2530 2950 50  0000 C CNN
F 1 "2K" V 2450 2950 50  0000 C CNN
F 2 "" V 2380 2950 50  0001 C CNN
F 3 "" H 2450 2950 50  0001 C CNN
	1    2450 2950
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5AD3A40D
P 2450 3450
F 0 "R4" V 2530 3450 50  0000 C CNN
F 1 "2K" V 2450 3450 50  0000 C CNN
F 2 "" V 2380 3450 50  0001 C CNN
F 3 "" H 2450 3450 50  0001 C CNN
	1    2450 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2500 2450 2800
Wire Wire Line
	2450 3100 2450 3300
Wire Wire Line
	2450 3300 5300 3300
Wire Wire Line
	5300 3300 5300 1700
$Comp
L R R5
U 1 1 5AD3A55D
P 3050 3700
F 0 "R5" V 3130 3700 50  0000 C CNN
F 1 "2K" V 3050 3700 50  0000 C CNN
F 2 "" V 2980 3700 50  0001 C CNN
F 3 "" H 3050 3700 50  0001 C CNN
	1    3050 3700
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5AD3A5D5
P 3050 4200
F 0 "R6" V 3130 4200 50  0000 C CNN
F 1 "2K" V 3050 4200 50  0000 C CNN
F 2 "" V 2980 4200 50  0001 C CNN
F 3 "" H 3050 4200 50  0001 C CNN
	1    3050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 1700 2600 1700
Wire Wire Line
	2600 1700 2600 3100
Wire Wire Line
	2600 3100 3050 3100
Wire Wire Line
	3050 3100 3050 3550
Wire Wire Line
	3050 3850 3050 4050
Wire Wire Line
	3050 4050 5500 4050
Wire Wire Line
	5500 4050 5500 1550
Wire Wire Line
	5500 1550 5300 1550
$Comp
L GNDREF #PWR?
U 1 1 5AD3A6C6
P 2450 3600
F 0 "#PWR?" H 2450 3350 50  0001 C CNN
F 1 "GNDREF" H 2450 3450 50  0000 C CNN
F 2 "" H 2450 3600 50  0001 C CNN
F 3 "" H 2450 3600 50  0001 C CNN
	1    2450 3600
	1    0    0    -1  
$EndComp
$Comp
L GNDREF #PWR?
U 1 1 5AD3A6FA
P 3050 4350
F 0 "#PWR?" H 3050 4100 50  0001 C CNN
F 1 "GNDREF" H 3050 4200 50  0000 C CNN
F 2 "" H 3050 4350 50  0001 C CNN
F 3 "" H 3050 4350 50  0001 C CNN
	1    3050 4350
	1    0    0    -1  
$EndComp
Text Label 2900 1400 0    60   ~ 0
RX
Text Label 2950 1500 0    60   ~ 0
TX
Text Label 3100 1700 0    60   ~ 0
ZAP
Text Label 3100 1850 0    60   ~ 0
FIRE
Text Label 3200 2000 0    60   ~ 0
P2
Text Label 3200 2300 0    60   ~ 0
P1
Text Label 2450 2700 0    60   ~ 0
ENC2
Text Label 2500 1700 0    60   ~ 0
ENC1
Text Label 2500 2300 0    60   ~ 0
P2LED
Text Label 2500 2100 0    60   ~ 0
P1LED
$Comp
L USB_OTG J?
U 1 1 5AD3AC34
P 4850 2200
F 0 "J?" H 4650 2650 50  0001 L CNN
F 1 "USB (Power)" H 4650 2550 50  0000 L CNN
F 2 "" H 5000 2150 50  0001 C CNN
F 3 "" H 5000 2150 50  0001 C CNN
	1    4850 2200
	0    1    1    0   
$EndComp
$EndSCHEMATC
