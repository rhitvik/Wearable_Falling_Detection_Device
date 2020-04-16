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

## Sensors Employed

## Pictures

## Future Possibilities


 
