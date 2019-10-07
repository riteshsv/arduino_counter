#include <LiquidCrystal.h>
#include <EEPROM.h>

/* LCD circuit setup
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 */

/*
 * LCD setup
 * 
  */
  const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
  LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
/*
 * Start Address of EEPROM
 */
#define EEP_START_ADDR 0

/*Pin setup*/
#define TRIG 8
#define ECHO 7

//globals
unsigned long duration;
int distance;
unsigned int count;
uint8_t entry_start=0;
uint8_t entry_stop=0;

void saveCount(unsigned int count){
  
  for(int i=0;i<sizeof(int);i++){
    EEPROM.update(EEP_START_ADDR,count);                              //updates data if it does not match.
  }
}

void writeToLcd(){
    //write to the lcd
  lcd.setCursor(0,0);
  lcd.print("Count: ");
  lcd.print(count);
}

void runUltraSonicCounter(){
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG,HIGH);                            /*sets the trig pin for output*/
  delayMicroseconds(10);                              /*10 ms delay is required to setup the trig pin and send 8 cycle sonic burst*/
  
  digitalWrite(TRIG,LOW);                             /*set it to low to stop the wave*/

  duration = pulseIn(ECHO,HIGH);                      /*read the output from echo pin the duration is in micro seconds and is double the one way distance*/
  duration /= 2;                                      /*one way travel duration*/

  distance = duration*0.034;                          /*distance travelled in cm = time * speed of sound in microsecond
                                                        speed of sound = 340 m / sec which is = 0.034 cm / microsecond */

  //count a person
  if(distance >0 && distance <= 50){
    entry_start = 1;    
    delay(1000);   //switch on
  }

  if(distance > 50 && (entry_start == 1)){
    entry_start=0;                                      /*switch off*/
    count++;                                            /*count on person*/
    if((count % 25) == 0){                            
      saveCount(count);                                 /*update count to store*/
    }
  }
  Serial.print("Count: \t");
  Serial.print(count);
  Serial.print("\tDistance: \t");
  Serial.print(distance);
  Serial.print(" cm\n");

  writeToLcd();
}

void setup() {
  //read initial count state
  //EEPROM.put(EEP_START_ADDR,count);
  EEPROM.get(EEP_START_ADDR,count);
  
  // set up the Ultrasonic sensor 
  pinMode(TRIG,OUTPUT);   
  pinMode(ECHO,INPUT);

    lcd.begin(16, 2);                               //set up lcd cols and rows (16 cols and 2 rows)
    //lcd.print("Hi-Tech Counter");                   // Print a message to the LCD.
    
    Serial.begin(9600);           /*start the serial communication at 9600 baud rate*/
//    if(count == 65535){
//      count = 0;
//    }
//    Serial.print(count);
//    count = 50;
//    if((count % 25) == 0){                            
//      saveCount(count);                                 /*update count to store*/
//    }
//   
//    EEPROM.get(EEP_START_ADDR,count);
//    Serial.print(count);
}

void loop() {
  runUltraSonicCounter();
  
  

 
  
}
