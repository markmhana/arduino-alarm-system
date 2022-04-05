#include <SimpleDHT.h> 


#include <LiquidCrystal.h>// includes the LiquidCrystal Library and other Librarys
#include <Keypad.h>
SimpleDHT11  dht11;
/////////////////////////////////////////////////////////////////////////
#define buzzer 8   //diifen pins we need for the program 
#define trigPin 9
#define echoPin 10
int pinDHT11 = A0; 
int led=31; 
///////////////////////////////////////////////////////////////////////////
//diifen valibers we need for the program 

long duration;
int distance, initialDistance, currentDistance, i;
int screenOffMsg =0;
String password="1234";
String tempPassword;
boolean activated = false; // State of the alarm
boolean isActivated;
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean enteredPassword; // State of the entered password to stop the alarm
boolean passChangeMode = false;
boolean passChanged = false;
/////////////////////////////////////////////////////////////////////

//diffen the keys for the keybead
const byte ROWS = 4; //four rows 
const byte COLS = 4; //four columns
char keypressed;
//define the cymbols on the buttons of the keypads
char keyMap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 15, 16, 17}; //Row pinouts of the keypad
byte colPins[COLS] = {18, 19, 20, 21}; //Column pinouts of the keypad
//////////////////////////////////////////////////////////////////////////////////////////

//crutting two opjacts for the keybaid and the sreen
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
LiquidCrystal lcd(11, 2, 4, 5, 6, 7); // Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7) 
///////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() { 

  lcd.begin(16,2); //inchlizg the lcd 
  //difin the pin moods 
  pinMode(buzzer, OUTPUT); // Set buzzer as an output
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(led,OUTPUT);// Sets the LED as an Output
Serial.begin(9600);//serial monter


}

void loop() {
///////////////////////////////////////////////////////////////////
  // read with raw sample data.
  byte temperature = 0; // set the temp to zero  
  byte humidity = 0;     //set the humidity to zero  
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
 //   Serial.print("Read DHT11 failed");
    return;
  }

 // Serial.print("Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
 //   Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
  //    Serial.print(' ');
    }
  }
  //Serial.println("");

 // Serial.print("Sample OK: ");*/
  Serial.print((int)temperature); //Serial.print(" *C, ");
  Serial.print("/                          /");
  Serial.println((int  )humidity); //Serial.println(" %");
  //Serial.println("C____RH ");
 

  // DHT11 sampling rate is 1HZ.
  delay(2000);
  
/////////////////////////////////////////////////////////

if (alarmActivated == true){
    int sensor=analogRead(A5);//this function will read date from the sincor to arduoi
  Serial.println(sensor);
  if (sensor>120){
    digitalWrite(led,HIGH);
    digitalWrite(buzzer, HIGH);
    delay(100);
       digitalWrite(buzzer, LOW);
       Serial.println("water dedicted Zone 2 ");
}
    
  }
  else{
  digitalWrite(led,LOW);
}
  /// ////////////////////////////////////////////////////////////
  
  if (activateAlarm) {//1      5   we check if the alrem actived or no 
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm will be");
    lcd.setCursor(0,1);
    lcd.print("activated in");
   
    int countdown = 9; // 9secnds seconds count down before activating the alarm
    while (countdown != 0) { //6   count dowm 
      lcd.setCursor(13,1);
      lcd.print(countdown);
      countdown--;
      tone(buzzer, 700, 100);
     delay(1000);

     }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarm Activated!"); //7  messge will show 
    initialDistance = getDistance();//8 maggring the initial distance to the object opzet of it 
    activateAlarm = false;
    alarmActivated = true;
  }

  if (alarmActivated == true){ //9 the ultrasonc sincer will keep cheeking the distnce
    
      currentDistance = getDistance() + 10;
      if ( currentDistance < initialDistance) {
        
        tone(buzzer, 1000); //10 Send 1KHz sound signal 
        lcd.clear();
        enterPassword();//11  the ente password castom fanction will be called  
      }
    }

  if (!alarmActivated) { //2    if the alrem is not activaed in the lcd we will have to choose A OR b
    if (screenOffMsg == 0 ){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - Activate");
      lcd.setCursor(0,1);
      lcd.print("B - Change Pass");
      screenOffMsg = 1;
    }
    keypressed = myKeypad.getKey();//   3   uesing this function we read wich bottom is pressd
     if (keypressed =='A'){        // 4    If A is pressed, activate the alarm
      tone(buzzer, 1000, 200); //200ms sound
      activateAlarm = true; //the actived alrem virble will be come true   
    //////////////////////////////////////////////////////////////////////////////////////////  //        
    }
    else if (keypressed =='B') {
      lcd.clear();
      int i=1;
      tone(buzzer, 2000, 100);
      tempPassword = "";
      lcd.setCursor(0,0);
      lcd.print("Current Password");
      lcd.setCursor(0,1);
      lcd.print(">");
      passChangeMode = true;
      passChanged = true;   
      while(passChanged) {      
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
         tempPassword += keypressed;
         lcd.setCursor(i,1);
         lcd.print("*");
         i++;
         tone(buzzer, 2000, 100);
        }
      }
      if (i > 5 || keypressed == '#') {
        tempPassword = "";
        i=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Current Password");
        lcd.setCursor(0,1);
        lcd.print(">"); 
      }
      if ( keypressed == '*') {
        i=1;
        tone(buzzer, 2000, 100);
        if (password == tempPassword) {
          tempPassword="";
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("Set New Password");
          lcd.setCursor(0,1);
          lcd.print(">");
          while(passChangeMode) {
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||  
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
                tone(buzzer, 2000, 100);
              }
            }
            if (i > 5 || keypressed == '#') {
              tempPassword = "";
              i=1;
              tone(buzzer, 2000, 100);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Set New Password");
              lcd.setCursor(0,1);
              lcd.print(">");
            }
            if ( keypressed == '*') {
              i=1;
              tone(buzzer, 2000, 100);
              password = tempPassword;
              passChangeMode = false;
              passChanged = false;
              screenOffMsg = 0;


              
            }            
          }
        }
      }
    }
   }
 }
}

void enterPassword() { //12  that will priant a messge says the alrem is actvied and we need 
  int k=5;
  tempPassword = "";
  activated = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" *** ALARM *** ");
  lcd.setCursor(0,1);
  lcd.print("Pass>");//enter a passsword to stop the alrem
      while(activated) {//cheaking if theres any keys are pressed 
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
          tempPassword += keypressed; //temerey password viber when u presss anykey 
          lcd.setCursor(k,1);
          lcd.print("*");
          k++;
        }
      }
     //13 if press more than 4 bottoms the lcde will clear or if u inter hesg key   
      if (k > 9 || keypressed == '#') {
        tempPassword = "";
        k=5;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" *** ALARM *** ");
        lcd.setCursor(0,1);
        lcd.print("Pass>");
      }
      //14 if we enter the star key it will cheek if the password is rigt and if the true
      if ( keypressed == '*') {
        if ( tempPassword == password ) { // alrem will stop and the buzzer will stop
          activated = false;
          alarmActivated = false;
          noTone(buzzer);
          screenOffMsg = 0; // and we will get back to the home screen
        }
        else if (tempPassword != password) { //15  if you enter a wrong password 
          lcd.setCursor(0,1);
          lcd.print("Wrong! Try Again");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" *** ALARM *** ");
          lcd.setCursor(0,1);
          lcd.print("Pass>");
        }
      }    
    }
}
// Custom function for the Ultrasonic sensor
long getDistance(){
  //int i=10;
  
  //while( i<=10 ) {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration*0.034/2;
  //sumDistance += distance;
  //}
  //int averageDistance= sumDistance/10;
  return distance;

}


  
