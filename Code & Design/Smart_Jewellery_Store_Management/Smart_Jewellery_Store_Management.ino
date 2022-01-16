#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

LiquidCrystal lcd1(A4, A5, A3, A2, A1, A0);
LiquidCrystal lcd2(A4, A6, A3, A2, A1, A0);
Servo myservo;

const byte rows=4; //four rows
const byte cols=3; //three columns
char key[rows][cols]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows]={21,20,19,18}; //connect to the row pinouts of the keypad
byte colPins[cols]={17,16,15}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(key),rowPins,colPins,rows,cols);
char* password = "8014";
int currentposition = 0;
int wr_count = 0;
int buzzer = 14; //buzzer to arduino pin 14
int buzzer1 = 5; //buzzer to arduino pin 5
int buzz_led = 2; //connect Led to arduino pin 2
int buzz_led1 = 8; //connect Led to arduino pin 8
int LED1 = 23; // connect Led to arduino pin 23
int LED2 = 24; // connect Led to arduino pin 24
int LED3 = 25; // connect Led to arduino pin 25
int LED4 = 26; // connect Led to arduino pin 26
int LED5 = 27; // connect Led to arduino pin 27
int pos = 0; // position of servo motor
int IRSensor = 6; // connect ir sensor to arduino pin 6
int LED = 7; // connect Led to arduino pin 7
int value = 0;
float millivolts, cel;
int lm35 = A7; // connect lm35 sensor to arduino pin A7
int fan = 3; //motor to arduino pin 3
int fan1 = 4; //motor to arduino pin 4

void setup() {
  // put your setup code here, to run once:
  lcd1.begin(16,2);
  lcd2.begin(16,2);
  pinMode(buzzer, OUTPUT); // Set buzzer pin 14 as an output
  pinMode(buzzer1, OUTPUT); // Set buzzer pin 5 as an output
  pinMode(buzz_led, OUTPUT);
  pinMode(buzz_led1, OUTPUT);
  pinMode(IRSensor, INPUT); // sensor pin INPUT
  pinMode(LED, OUTPUT); // Led pin OUTPUT
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  myservo.attach(9); //Servo motor connection
  pinMode(fan, OUTPUT);
  pinMode(fan1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor_status = digitalRead (IRSensor);
  value = analogRead(lm35);
  millivolts = (value / 1024.0) * 5 * 1000;
  cel = millivolts / 10;

  if(currentposition == 0)
  {
    displayscreen(); 
  }
  int l ;
  char code = keypad.getKey();
  if(code != NO_KEY)
  {
    lcd1.clear();
    lcd1.setCursor(0,0);
    lcd1.print("PASSWORD:");
    lcd1.setCursor(7,1);
    lcd1.print(" ");
    lcd1.setCursor(7,1);
    
    for(l=0;l<=currentposition;++l)
    {    
      lcd1.print("*"); //keypress();
    }
   
    if (code == password[currentposition])
    {
      ++currentposition;
      if(currentposition == 4)
      {  
        digitalWrite(buzz_led, LOW);
        if (sensor_status == 1) 
        {
          digitalWrite(LED, HIGH); // LED ON
          unlockdoor();
          currentposition=0;
        }
        else
        {
          lcd1.clear();
          lcd1.setCursor(1,0);
          lcd1.print("Error Occured!");
          delay(1000);
          digitalWrite(LED, LOW); // LED OFF
          lcd1.clear();
          currentposition=0;
          displayscreen();
        }
      }
    }
    else
    {
      wr_count++;
      incorrect();
      currentposition=0;
    }
  }
}

//------------Function 1 - DISPLAY FUNCTION --------------------//
void displayscreen()
{  
  lcd1.setCursor(0,0);
  lcd1.println("*ENTER THE CODE*");
  lcd1.setCursor(0,1); 
  lcd1.println("TO Open DOOR!!"); 
}

//-------------- Function 2 - OPEN THE DOOR ----------------//
void unlockdoor()
{
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.println(" ");
  lcd1.setCursor(1,0);
  lcd1.print("Access Granted");
  delay(300); 
  lcd1.setCursor(4,1);
  lcd1.println("WELCOME!!");
  delay(300); 
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.println(" ");
  lcd1.setCursor(2,0);
  lcd1.print("Door Opening");
  lcd1.setCursor(1,1);
  lcd1.println("PLEASE COME IN!");

  displayTemperature();
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);
  digitalWrite(LED5, HIGH);
  digitalWrite(fan, HIGH);

  for(pos = 0; pos <= 120; pos +=5) // open the door
  { 
    myservo.write(pos); 
    delay(205);     
  }

  counterbeep();

  for(pos = 120; pos>=0; pos-=5) // close the door
  {
    myservo.write(pos); 
    delay(205); 
  } 

  currentposition=0;
 
  lcd1.clear();
  displayscreen();
}

//--------------------Function 3 - Wrong code --------------//
void incorrect()
{
  if(wr_count<=10)
  {
    if(wr_count<=2)
    {
      lcd1.clear();
      lcd1.setCursor(1,0);
      lcd1.print("Wrong password!");
      delay(1000);
      lcd1.clear();
      displayscreen();
    }
    else
    {
      lcd1.clear();
      lcd1.setCursor(1,0);
      lcd1.print("Intruder Alert!");
      delay(1000);
      digitalWrite(buzz_led, HIGH);
      tone(buzzer, NOTE_C4, 500);
      delay(500);
      tone(buzzer, NOTE_D4, 500);
      delay(500);
      tone(buzzer, NOTE_E4, 500);
      delay(500);
      tone(buzzer, NOTE_F4, 500);
      delay(500);
      tone(buzzer, NOTE_G4, 500);
      delay(500);
      lcd1.clear();
      displayscreen();
    }
  }
}

//-------------- Function 4 - Count down ------------------//
void counterbeep()
{
  lcd1.clear();
  lcd1.setCursor(2,0);
  lcd1.println("GET IN WITHIN:::");

  for(int i = 3; i > 0; i--)
  {
    lcd1.setCursor(8,1);
    lcd1.print(i);
    delay(1000);
  }

  lcd1.clear();
  lcd1.setCursor(2,0);
  lcd1.print("RE-LOCKING");
  delay(500);
  lcd1.setCursor(12,0);
  lcd1.print(".");
  delay(500);
  lcd1.setCursor(13,0);
  lcd1.print(".");
  delay(500);
  lcd1.setCursor(14,0);
  lcd1.print(".");
  delay(400);
  lcd1.clear();
  lcd1.setCursor(4,0);
  lcd1.print("LOCKED!");
  delay(400);
}

//-------------- Function 5 - Display Temperature ------------------//
void displayTemperature()
{
  lcd2.clear();
  lcd2.setCursor(0,0);
  lcd2.print("Temp Inside Case");
  lcd2.setCursor(0,1);
  lcd2.print("Temperature:");
  lcd2.setCursor(12, 1);
  lcd2.print(cel);
  lcd2.setCursor(14, 1);
  lcd2.print("*C"); 

  if(cel < 8)
  {
    digitalWrite(fan1, LOW);
    digitalWrite(buzz_led1, LOW);
  }
  else if(cel >= 8 && cel <= 58)
  {
    digitalWrite(fan1, HIGH);
    digitalWrite(buzz_led1, LOW);
  }
  else
  {
    digitalWrite(fan1, HIGH);
    digitalWrite(buzz_led1, HIGH);
    tone(buzzer1, NOTE_C4, 500);
    delay(500);
    tone(buzzer1, NOTE_D4, 500);
    delay(500);
    tone(buzzer1, NOTE_E4, 500);
    delay(500);
    tone(buzzer1, NOTE_F4, 500);
    delay(500);
    tone(buzzer1, NOTE_G4, 500);
    delay(500);
  }
}
