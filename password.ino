#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Key.h>
#include <Keypad.h>

#define ROWS 4
#define COLS 4

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
  };
  
//Specifying pins of the rows and columns
byte rowPins[ROWS] = {10,9,8,7};
byte colPins[COLS] = {6,5,4,3};
int password_len = 0;

//The set password
char password[] = "1234";

//Password to be keyed in by user
char input_password[5];

//Counts the characters entered
byte i = 0;
char key;

int flag = 0;
int lcd_col = 5;
int dt = 500;
int delay_t = 1000;

int servoPin = 12;
int pos = 170;
 
Keypad keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

LiquidCrystal_I2C lcd(0x27,16,2);

Servo myServo;

SoftwareSerial mySerial(30,32);//RX, TX connecting to TX and RX respectivel on GSM

void setup() {
  Serial.begin(9600);

  mySerial.begin(9600);

  myServo.attach(servoPin);

  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {

  lcd.setCursor(1,0);
  lcd.print("ENTER PASSWORD:");

  int j = 0;
  
  //Holds the char input
  key = keypad.getKey();
  
  enter_password();

//Check if the password length has been reached
  if (flag == 0){

    //***********REVISIT THIS!!!!************
    if (key == 'D')
    {
      if (i > 0)
      {
        Serial.println();
        password_len--;
        for (i = 0; i < password_len; i++)
        {
          Serial.print(input_password[i]);
          lcd.setCursor(lcd_col,1);
          lcd.print(" ");
          lcd_col--;
        }
    
        }
      }
    
   if (key == '#')
   {
     //strcmp compares the strings and returns 0 if they are equal
    if (!strncmp(input_password, password, 4))
    {
      delay(dt); 
      Serial.println("\nPassword correct!");
      flag = 1;
      password_correct();
      open_door();
      }
      else
      {
        delay(dt);
        flag = 2;
        j += 1;
        if (j <= 3)
        {
          Serial.println("\nPassword INCORRECT");
          password_incorrect();  
          lcd.clear();
          lcd.setCursor(5, 1);
          enter_password();
        }
        }
    }
  } 
}

void enter_password(){
  if (key)
  {
    if (key != '#' && key != 'D')
    {
      input_password[i] = key;
      Serial.print(input_password[i]);
      lcd.setCursor(lcd_col,1);
      lcd.print("*");
      lcd_col++;
      i++; 
      password_len++;
    }
   }
  
  }
  
void password_correct(){
  lcd.clear();
  lcd.setCursor(1,0);  
  lcd.print("Password");
  lcd.setCursor(3,1);
  lcd.print("CORRECT");
  delay(delay_t);
  lcd.clear();
  }
void password_incorrect(){
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Wrong Password!");
  lcd.setCursor(3,1);
  lcd.print("Try again");
  delay(delay_t);
  lcd.clear();
  } 
void open_door(){
  myServo.write(pos);
  }  
   
  
