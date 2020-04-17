# Wearable Falling Detection Device

## About the project
The wearable falling detection device is a low cost, a prototype device that is developed for the aged/senior people. The person will wear the device on their hand and based on the type of movement, this device detects whether the user is performing their routine tasks or they have experienced sudden jerk/fall. 

The main intent being that the device is supposed to communicate with the concerned party should the person wearing it takes a fall, the device also featured with manual distress alert functionality. Only the difference was that now the user can also send an SOS in case he/she experiences danger or distresses not visible to the device itself (for example experiencing lightheadedness).

## Development Platform
The development platform used for this prototype is teensy 3.2 featuring an ARM Cortex M-4 core NXP MK20DX256 Microcontroller. Not only it has got ample processing power for the project, but also its ultra-low-power features will ensure that the battery life is very long and even with low battery, the system can be relied upon to some extent (although not recommended though).

We can also use a Teensy 4.0 (600 MHz) over Teensy 3.2 (72 MHz) but talking from a wearable device perspective, the Cortex M-4 is more conservative when it comes to consuming power. Also, modeled more towards signal processing models, Teensy 4.0 although more powerful might not the best choice for the project.

### Teensy 3.2 Schematic
![](https://github.com/rhitvik/Wearable_Falling_Detection_Device/blob/master/Images_and_Screenshots/schematic_Teensey3.2.gif)


## How to detect the fall?
For detecting a fall we will need to devise an event-driven machine that reads multiple sensors to conclude whether or not a person wearing it has fallen or not along with added optimizations of course.

Let's assume the following scenarios where the person is wearing the Fall Detection Device:
* **The person wearing it is walking/sitting in a bus**
Here the bus may be standing still, or it may be moving, the sensors should not mistake the movement of the bus or the jerks experienced by the bus should not be mistaken with an actual fall. So, we can set the thresholds of the system to not get triggered in such an environment. 
* **The person wearing it takes a fall**
Now here the system should read the event and conclude that the person has fallen. Now experiencing sudden jerks doesn't necessarily conclude that the person wearing it has fallen, but a person not moving at all once he/she has experienced a fall is indeed a very strong indicator that the person has fallen and now he/she is rendered unconscious because of the fall. So we can make the system count the time after the fall that no motion has occurred and if this time exceeds a particular amount of time say 5 seconds, the device should generate an SOS. 
* **The person wearing it takes a jerk to the only hand he/she have her device on but he/she is okay/not Ok**
So, as mentioned above, if the user is ok then he/she will move their respective hands and it should be assumed that the person is fine. But it is likely possible that the person might not be fine. So we can ask the system to get acknowledgment from the user is they are fine or not with a press of a button after an led indicator turns on. Now, if the user pushes the button within a certain time-bound, the SOS need not be sent but in case the button is not pressed in time then the SOS should be sent. This opposite system behavior can make sure that the user is only capable to cancel the SOS once they are aware of their surrounding. And in case the user is not ok, he/she doesn't have to do anything because the device will automatically send the SOS in a certain time frame after experiencing a shock. 
* **The person wearing it is exercising/clapping his/her hands**
The jerks might be sufficient enough to trigger the system to detect the fall but the characteristics of movement occurring repetitively can be discarded as a potentially dangerous scenario. Plus, the likely hood of exercising/clapping hard enough to trigger the system is very less likely for the targeted age group.
* **The person wearing it has gone to sleep after taking off the device and placing it on their respective nightstand**
Now the system should doze off after some time it doesn't detect any movement. This measure will save power and will ensure longer battery life.
* **The person is wearing it but the band comes off loose!!**
If the band comes off loose then any motion detected by it might not be considered correct as the bad itself will move around because of its stored momentum. So, in such an event, the person wearing it should be acknowledged about the loose band with another indicator LED
* **The person is wearing it but haven't moved at all for some time**
Can this be treated as the same condition in which the person has taken off the band and it can shut down to save power? No, because the person wearing the device might also be in some other distressed condition like a Heart Attack. So we will need it to treat it like the condition where the person wearing it takes a jerk to the only hand he/she have her device on but he/she is okay/not Ok as described above.
* **The person wearing it is running his/her errands and suddenly starts to feel light-headed and no one is around**
In this case, the system is not triggered by any sudden movement but we can be sure that the person is distressed. So here the person can either sit down and press the distress if he/she needs help or in case they fall, the SOS will be generated automatically. 
* **A thief comes to the person and demands their money**
An SOS call can be placed manually by the person wearing it. Although not designed to serve the self-defense person, the manual distress call may be helpful to some extent if not at all.


## Firmware Code
[Please Click Here](https://github.com/rhitvik/Wearable_Falling_Detection_Device/blob/master/Codes/Code_Main/Fall_Detection_System_Teensey/Fall_Detection_System_Teensey.ino)

## Components Used
### Sensors 
So based on the possibility of the events the following sensors were used:
* **Vibration Sensors**
These vibration switches are non-directional vibration induced trigger switches. Inside is a spring coiled around a long metal pin (Normally Open configuration). When the switch is moved, the spring touches the center pole to make contact. So, when there's motion, the two pins will act like a closed switch. When everything is still, the switch is open. 

3 Different Vibration Sensitivity switches were employed:
1. Easy Vibration Sensor
* Can be triggered with the slightest of the motion. So, not very useful to detect strong acceleration but can be used to turn on/off the device when taking off the user's hands and placing it on the shelf. This feature can help a lot to save power.
Get it from [Here](https://www.adafruit.com/product/1766)
2. Medium Vibration Sensor
These are not so easy to trigger sensors but in case a shock is experienced, the coils will trigger several times, the frequency of which can be taken into consideration along with the Hard Vibration sensor to conclude an occurred event.
Get it from [Here](https://www.adafruit.com/product/2384)
3. Hard Vibration Sensor
Similar to medium vibration sensors but with a stiffer spring, these sensors are going to make a certain number of triggers but are lower in count compared to the medium switch for the same amount of vibration.
Get it from [Here](https://www.adafruit.com/product/1767)
* **ADXL343 Accelerometer**
This is a triple-axis accelerometer with digital I2C and SPI interface breakout. It has a wide sensitivity range and high resolution, operating with a 10 or 13-bit internal ADC. Built-in motion detection features make tap, double-tap, activity, inactivity, and free-fall detection trivial. There are two interrupt pins, and you can map any of the interrupts independently to either of them.
The sensor has three axes of measurements, X Y Z, and pins that can be used either as I2C or SPI digital interfacing. You can set the sensitivity level to either +-2g, +-4g, +-8g or +-16g. The lower range gives more resolution for slow movements, the higher range is good for high-speed tracking. 
Purchase the ADXL343 Accelerometer from [Here](https://www.adafruit.com/product/4097)
* **US5881LUA (Hall-Effect Sensor)**
Hall effect sensors detect whether a magnet is near. Useful for non-contact/waterproof type switches, position sensors, rotary/shaft encoders.
Runs at 3.5V up to 24V. To use connect power to pin 1 (to the left), ground to pin 2 (middle) and then a 10K pull up resistor from pin 3 to power. Then listen on pin 3, when the south pole of a magnet is near the front of the sensor, pin 3 will go down to 0V. Otherwise, it will stay at whatever the pullup resistor is connected to. Nothing occurs if a magnet's north pole is nearby (unipolar).
This is used to ensure that the device put on by the user has not come loose as the magnets will immediately trigger the microcontroller about such an event and a warning Led can be turned on. 

Purchase the US5881LUA (Hall-Effect Sensor) fom this [Link](https://www.adafruit.com/product/158)

### Development Platform | Teeny 3.2

The Teensy 3.2 is a complete USB-based microcontroller development system, in a very small footprint, capable of implementing many types of projects. All programming is done via the USB port. No special programmer is needed, only a standard Mini-B USB cable and a PC or Macintosh with a USB port.

![](https://github.com/rhitvik/Wearable_Falling_Detection_Device/blob/master/Images_and_Screenshots/teensy32.jpg)
- 32-bit ARM Cortex-M4 72MHz CPU (M4 = DSP extensions)
- 256K Flash Memory, 64K RAM, 2K EEPROM
- 21 High-Resolution Analog Inputs (13 bits usable, 16-bit hardware)
- 34 Digital I/O Pins (5V tolerance on Digital Inputs)
- 12 PWM outputs
- 7 Timers for intervals/delays, separate from PWM
- USB with dedicated DMA memory transfers
- 3 UARTs (serial ports)
- SPI, I²C, I²S, CAN Bus, IR modulator
- I²S (for high-quality audio interface)
- Real-Time Clock (with user-added 32.768 crystal and battery)
- 16 DMA channels (separate from USB)
- Touch Sensor Inputs

### Power Supply and Battery
* **Battery**
Lithium-Ion Polymer Battery - 3.7v 500mAh
The batteries come pre-attached with a genuine 2-pin JST-PH connector as shown and include the necessary protection circuitry. Because they have a genuine JST connector, not a knock-off, the cable won't snag or get stuck in a matching JST jack, they click in and out smoothly. 
The included protection circuitry keeps the battery voltage from going too high (over-charging) or low (over-use) which means that the battery will cut-out when completely dead at 3.0V. It will also protect against output shorts. However, even with this protection, it is very important that you only use a LiIon/LiPoly constant-voltage/constant-current charger to recharge them and at a rate of 500mA or less.

Purchase the Battery from [Here](https://www.adafruit.com/product/1578)
* **Charge Controller**
Adafruit Micro Lipo - USB LiIon/LiPoly charger - v1 
To be with Adafruit LiPoly/LiIon batteries only! Other batteries may have different voltage, chemistry, polarity or pinout.
5V input via PCB-style USB connector For charging single Lithium Ion/Lithium Polymer 3.7/4.2v batteries (not for older 3.6/4.1v cells)
100mA charge current, adjustable to 500mA by soldering a jumper closed
Free 2-pin JST cable included!
The MicroLipo charger can get hot during charging. Grab it by the sides and unplug then let cool before removing the battery - take care not to touch the components during charging!

Purchase the Charger from [Here](https://www.adafruit.com/product/1904)

### Miscellaneous 
* **Solderless BreadBoard**
For initial prototyping and debugging
* **Perforated Board**
To integrate everything we can solder everything on to the solder board so that the components in the final prototype can be rugged and reliable.
* **Header Pins**
To be soldered on to the teensy board as well as the accelerometer boards for fixing it onto the perforated board.
* **Resistors**
Used for Biasing, Current limiting and, pulling up certain pins in the microcontroller.
* **Capacitors**
For signal conditioning and improved stability. It also can be used for mechanical debouncing. 
* **LEDs (4 Different Colors)**
4 Different LEDs can tell 4 scenarios:
1. A falling event has occurred
2. Potential accident detected. Press button on the device within a given amount of time or an SOS call will be placed.
3. Manual distress detected as the user has invoked the SOS event himself/herself.
4. The band has come off loose and the user might want to put on the device properly. 
* **Single Core Wire**
For general circuitry and signal communication

* **Magnets**
Size of a mini magnet - 8 x 3 mm, each of our Small Round Magnets has flux index of Magnetic Flux: 42kG2mm2 
Used for triggering the Hall Effect Sensors in case the band comes off loose in the device worn by the user. Also, these magnets make sure that the band securely grpis the user's hand, one of the required conditions for correctly reading the sensors.
The image below describs the placement of magnets around the Hall-Effect Sensor. These magnets ensure the secure for of the band. There is another magnet that sits directly above the sensor to ensure that the band is properly secured.  
![](https://github.com/rhitvik/Wearable_Falling_Detection_Device/blob/master/Images_and_Screenshots/PSX_20200416_014423.jpg)
*Placement of Magnets on the Band*

Purchase the Magnets from this [Link](https://www.amazon.com/gp/product/B075TJNQQH/ref=ppx_yo_dt_b_asin_title_o06_s00?ie=UTF8&psc=1)

## Engineering the System
Based on the potential events described, the system was designed by considering all the possible factors to detect the fall and even exercise some abstruse contingencies to invoke alerts even in the distressful events that are not related to falling but can still help.
Many power saving features were also used and some that are described were not implemented as the prototype was developed to cover the base case contingencies.

## Pictures

## Future Possibilities


 
