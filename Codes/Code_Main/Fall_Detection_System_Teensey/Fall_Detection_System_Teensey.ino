  //  available pins 
  
  // >>S No. part name/peripheral         purpose fo the sensor                                        pin name            pin number       interrupt??
  
  // 1     hall effect sensor 1         to ensure that the band is worn securely                     secureFit_pin        20              no
  // 2     hall effect sensor 2         to flip it to fall detection modefrom drowning mode          config_pin           21              yes
  // 3     capacative touch 1           to ensure that the band is on user                           skinDetect_pin       23 & 22(touch)  no
  // 4     capacative touch 2           to ensure that users hand comes in and out of water          waterDetect_pin      1 & 0 (touch)   no                                  no
  // 5     thermistor sensor            to detect water temperature and ensure external conditions   thermistor_pin       14 (A0)         no
  // 6     knock sensor easy            to detect motion/jerks on a easy threshold scale             knock_easy_pin       16              yes
  // 7     knock sensor medium          to detect motion/jerks in a medium threshold scale           knock_medium_pin     17              yes
  // 8     knock sensor hard            to detect motion/jerks in a hard threshold scale             knock_hard_pin       15              yes
  // 9     accelerometer                to poll the acceleration values of all the axies             SCL0 and SDA0        19 & 18         no
  // 10    timer                        to act as a clock and check events                           interrupt timer      xx              yes
  // 11    vibration motor              to act as a haptic feedback device                           vibration_pin        6               yes/no
  // 12    led builtin                  for development and testing                                  ledPin               13              yes/no
  // 13    RGBWled                      for visible output feedback                                  Red/Green/Blue/White 2&3&4&5         yes/no
  // 14    push button                  for vlountary emergency declaration                          distressACK_pin      2               yes

  // 15    3.7 volt battery             for powering up the teensey                                     
  // 16    li po charger                for charging teensey                
  // 17    magnets                      for feedback and secure fit
  


  // steps to make sure that drowning detector is working!!

  // 0  just as soon it turns on it checks sensors health
  // 1  it needs to make sure that the user has securely put on his band before enterning in water
  // 2  once that happens give a OK-signal that yes every thing is hunky dory
  // 3  now user is ecxpected to enter the water. Band should give a entering signal that yes now the user has entered the water... 
  //    ... this is checked by capacative sense pin as well as the temperature sensor
  //    ... guardians are advised to be near the person if he/she (the user of band) doesn't know how to swim
  // 4  make sure that every thing is allright by reading the acceleometer,knock sensors,push button values

  // how to make sure that every thing is allright !!

  //  what does the timer function do?
  //  it will generate 3 types of routines
      //  1 will be called every 10 ms
      //  2 will be called every 1 second
      //  3 will be called every 6 seconds   

  // lets begin
  
  // 1- check hall effect sensor for secure fit.... set parameter secure fit on!!
  
  // 2- check capacative sensor for secure fit...set secure fit on... this ensures that accelerometer values are read properly
  //    and an added parameter for conforming check that the user has worn the band and not just connected it without wearing it!!!
  
  // 3- if condition 1 and 2 are met set parameter proper device mount successful

  // 4- read accelerometer patterns and make sure of nothing but just for the sake of doing something

  // 5- if the user enters the water... set user in water parameter... this should be unset just as soon the water is not in contact with it

  // now lets see how our system works 
          // we get the values from accelerometer and store it in a variable.... say x, y and z
          // we also check the values of 2 things in in ISR mode
              //the device is mounted securely i.e. the magnet is in place'
              // the knock sensors and their behaviour
              // if something is wrong... then tell the user's guardian and the user immidiately.
          // we create 4 different types of event sets.
              // 1- keeps the log of 10 ms activities
              // 2- keeps the log of 1 sec activities
              // 3- keeps the log of 10 sec activities
              
//red means danger/call guardian/SOS
//blue means something is SOS call on purpose
//green means strap loose.. tighten it MrUser
//white means channging mode of operation
//######################################//
//accelerometer not used on purpose as it was not generating values that were consistent with fall detection
//compared across wide range of motions including free falls, boxing, and lounges (not that all people do that)
//######################################//
//#include <Wire.h>
#include <CapacitiveSensor.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_ADXL343.h>

#define thermistor_pin A0 // pin14
#define SERIESRESISTOR 10000    
#define SkinNumber 500
IntervalTimer myTimer;

//CapacitiveSensor  waterDetect = CapacitiveSensor(1,0);        
//CapacitiveSensor  skinDetect = CapacitiveSensor(23,22);        
const long waterDetect_Threshold=20000;
const long skinDetect_Threshold1=20000;
const long skinDetect_Threshold2=3000;

bool access_vibration_sensors_10ms = 0;  
bool access_vibration_sensors_100ms = 0;
bool access_vibration_sensors_1sec = 0;
bool access_vibration_sensors_10sec = 0;

/////////////////////////////////////////
 unsigned int knock_easy_count_1sec = 0;
 unsigned int knock_easy_count_1sec_Preset = 0;

 unsigned int knock_easy_count_10sec = 0;
 unsigned int knock_easy_count_10sec_Preset = 0;
//////////////////////////////////////////
 
 unsigned int knock_medium_count_1sec = 0;
 unsigned int knock_medium_count_10sec = 0;
 
 unsigned int knock_hard_count_1sec = 0;
 unsigned int knock_hard_count_10sec = 0;

bool vibration_pin_State = 0;
bool ledState = 0;
bool RedState = 0;
bool BlueState = 0;
bool GreenState = 0;
bool WhiteState = 0;
bool secureFit_pin_State = 0;
bool config_pin_State = 0;
bool isUserSwimming = 0;
volatile bool copyData = 0;
volatile bool distressParam = 0;

int vibration_pin = 7; 

int led = 13; //the builtin LED in teensey 3.2

int Red = 2;

int Blue = 3;

int Green = 4;

int White = 5;

int DistressPin = 6;

int knock_easy_pin = 16;
int knock_medium_pin = 17;
int knock_hard_pin = 15;

int secureFit_pin = 20; 

int config_pin = 21;

bool deviceWornProperly = 0;

volatile unsigned int knock_easy_count = 0;
volatile unsigned int knock_medium_count = 0;
volatile unsigned int knock_hard_count = 0;
volatile unsigned int HES_count = 0;

void ISR_knock_easy (void){
  knock_easy_count++;
}
void ISR_knock_medium (void){
  knock_medium_count++;
}
void ISR_knock_hard (void){
  knock_hard_count++;
}

void ISR_DISTRESS (void){
  // something
  distressParam = 1;
  detachInterrupt(digitalPinToInterrupt(DistressPin)); //put just before exiting void setup
}

void ISR_CONFIG (void){
  //
}

volatile unsigned int Mr10msCount = 0;  //counts the number if times 10 milliseconds have passed
volatile unsigned int Mr100msCount = 0; //counts the number of times 100 milliseconds have passed
volatile unsigned int Mr1secCount = 0;  // counts the number of times 1 second has passed
volatile unsigned int Mr10secCount = 0; // counts the number of times 10 seconds has passed

void myTimerFunction(){
  //access every 10 ms
  //increment every 1 second for events
  //count every 10 seconds for event
  //ledPin_State = !ledPin_State;
  //digitalWrite(ledPin, ledPin_State);
  Mr10msCount++;  //starts with 1 counts up..
  
  if (Mr10msCount >= 10){ //if reaches 10 ...means 100 ms have elapsed
    Mr10msCount = 0;      //reset Mr10msCount
    Mr100msCount++;
    ///see here if anything potential bad events have oucured at high frequency

    if(Mr100msCount >= 10){//this means that 1 second has elapsed
      Mr100msCount = 0;    //reset Mr100msCount
      Mr1secCount++;
      copyData = 1;
      knock_easy_count_1sec = knock_easy_count; 
      knock_easy_count = 0; //reset the value of knock easy count
      knock_medium_count_1sec = knock_medium_count;
      knock_medium_count = 0; // reset the value of knock medium count
      knock_hard_count_1sec = knock_hard_count;
      knock_hard_count = 0; // reset the value of knock hard count
      
      // do the routine supposed to happen every second
        //digitalWrite(led, !(digitalRead(led)));

        //  Serial.println("####");
      
      if (Mr1secCount >= 10 ){// this means that 10 seconds hae passed ... call it if you think anyhitning suspicious
        Mr1secCount = 0;      //reset Mr1secCount
        Mr10secCount++;

        if (Mr10secCount >= 6){ //this means that a minute has passed
          Mr10secCount = 0;// reset Mr10secCount second count... no need in dragging stuff!!
        }
      }
    }
  }
}
void Check_for_distress (void){
  if(distressParam == 1){
    distressParam = 0;
    int outCount2 = 0;
      while (23){
        outCount2 ++;
        digitalWrite(Blue, !(digitalRead(Blue)));
        delay(50);
        if(outCount2 >= 100){
          break;
        }
      }
    attachInterrupt(digitalPinToInterrupt(DistressPin), ISR_DISTRESS, FALLING); //put just before exiting void setup
  }
}

void Check_if_the_device_is_worn_proper (void){
    secureFit_pin_State = digitalRead(secureFit_pin); 
    if(secureFit_pin_State == 0){
      digitalWrite(Green, LOW);
    }
    if(secureFit_pin_State == 1){
      digitalWrite(Green, HIGH);
    }
    ///Serial.print("secureFit_pin_State = ");   Serial.println(secureFit_pin_State);
  
//    long skinDetect_Value =  skinDetect.capacitiveSensor(30);
//    if (skinDetect_Value <= SkinNumber ){
//      digitalWrite(Blue, HIGH);
//    }
//    if(skinDetect_Value > SkinNumber ){
//      digitalWrite(Blue, LOW);
//    }
   // Serial.println(skinDetect_Value);          // print sensor output 1   
}

void setup(void)
{
  pinMode (config_pin, INPUT);
  pinMode (config_pin, INPUT_PULLUP);
  
  pinMode (knock_easy_pin,INPUT);
  pinMode (knock_easy_pin,INPUT_PULLUP);
  
  pinMode (knock_medium_pin,INPUT);
  pinMode (knock_medium_pin,INPUT_PULLUP);  
  
  pinMode (knock_hard_pin,INPUT);
  pinMode (knock_hard_pin,INPUT_PULLUP);
  
  pinMode (DistressPin,INPUT);
  pinMode (DistressPin,INPUT_PULLUP);

  pinMode (secureFit_pin,INPUT); //needs to be monitored continuously
  pinMode (secureFit_pin,INPUT_PULLUP);         
  
  pinMode (led, OUTPUT);
  digitalWrite(led, ledState);
  
  pinMode (Red, OUTPUT);
  digitalWrite(Red, RedState);

  pinMode (Blue, OUTPUT);
  digitalWrite(Blue, BlueState);
  
  pinMode (Green, OUTPUT);
  digitalWrite(Green, GreenState);
  
  pinMode (White, OUTPUT);
  digitalWrite(White, WhiteState);
  
  //Serial.begin(9600);
  //while (!Serial);//##############################################################

  //Serial.println("");
  
  attachInterrupt(digitalPinToInterrupt(knock_easy_pin), ISR_knock_easy, FALLING); //put just before exiting void setup
  attachInterrupt(digitalPinToInterrupt(knock_medium_pin), ISR_knock_medium, FALLING); //put just before exiting void setup
  attachInterrupt(digitalPinToInterrupt(knock_hard_pin), ISR_knock_hard, FALLING); //put just before exiting void setup
  attachInterrupt(digitalPinToInterrupt(DistressPin), ISR_DISTRESS, FALLING); //put just before exiting void setup

  //attachInterrupt(digitalPinToInterrupt(config_pin), ISR_CONFIG, FALLING);
  myTimer.begin(myTimerFunction, 10000);  // myTimerFunction to be called every 10 milli second

}

unsigned int knock_easy_count_1sec_read = 0;
unsigned int knock_medium_count_1sec_read = 0;
unsigned int knock_hard_count_1sec_read = 0;
unsigned int accelerometer_event_count = 0;
bool allow_acc_count_print = 0;

void loop(void)
{
  // Check_if_the_device_is_worn_proper();

//    while (66){
//      digitalWrite(Green, !(digitalRead(Green)));
//      digitalWrite(Blue, !(digitalRead(Blue)));
//      digitalWrite(Red, !(digitalRead(Red)));
//      digitalWrite(White, !(digitalRead(White)));
//      digitalWrite(led, !(digitalRead(led)));
//      delay(50);
//    } 
  accelerometer_event_count++;  
  if(copyData){
    copyData = 0;
    noInterrupts();
    ////////////////////////////////////
    //copy all interrupts related data//
    ////////////////////////////////////   
    
    knock_easy_count_1sec_read = knock_easy_count_1sec;
    knock_medium_count_1sec_read = knock_medium_count_1sec;
    knock_hard_count_1sec_read = knock_hard_count_1sec;
 // Serial.print("****"); 
   // Serial.print(knock_medium_count_1sec_read);   Serial.print("****"); 

      //  Serial.println(knock_hard_count_1sec_read); 

    allow_acc_count_print = 1;

    if(knock_hard_count_1sec_read >= 20 || knock_medium_count_1sec_read >= 80){
      int outCount = 0;
      while (13){
        outCount ++;
        digitalWrite(Red, !(digitalRead(Red)));
        delay(50);
        if(outCount >= 100){
          break;
        }
      } 
    }
    Check_if_the_device_is_worn_proper();
    Check_for_distress();
    interrupts();
  }
//    while (66){
//        digitalWrite(Green, !(digitalRead(Green)));
//        digitalWrite(Blue, !(digitalRead(Blue)));
//        digitalWrite(Red, !(digitalRead(Red)));
//        digitalWrite(led, !(digitalRead(led)));
//      delay(50);
//    }    
    


  delay(100);
}
