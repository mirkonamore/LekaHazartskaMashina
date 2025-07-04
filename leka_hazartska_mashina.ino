// 0 1 2 3 4

#define gamble_button 8
#define amount_button 9

#include <LiquidCrystal.h>

LiquidCrystal lcd(0,1,10,11,12,13);

int isGamblePressed;
int isAmountPressed;

bool gambleHoldCheck = true;

bool amountHoldCheck = false;
int amountState = LOW;
unsigned long amountPressTime;
int amountPressDiff;
bool amountDebug = false;

int balance = 2000;

int gamble = 0;

void setup()
{
  //Serial.begin(9600);
  
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  
  pinMode(gamble_button,INPUT);
  pinMode(amount_button,INPUT);
  
  randomSeed(analogRead(2));
  
  lcd.begin(16,3);
  lcd.clear();
  
  
  lcdPrint();
}

void loop()
{
  amount_buttonPress();
  gamble_buttonPress();
  
  
}

void roll()
{
  int rolls[5] = {
    random(2), random(2), random(2), random(2), random(2)
  };
  for (int i = 0; i < 5; i++)
  {
    delay(50);
    //Serial.print(rolls[i]);
    digitalWrite(i+3,rolls[i]);
  }
  //Serial.print(" ");
  //Serial.println(gamble);
  
  balanceUpdate(rolls);
}

void reset()
{
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
}

void gamble_buttonPress()
{
  isGamblePressed = digitalRead(gamble_button);
  if (isGamblePressed == HIGH && gambleHoldCheck)
  {
    if (gamble != 0)
    {
      reset();
    delay(100);
    roll();
    
    gambleHoldCheck = false;
    }
  }
  if (isGamblePressed == LOW)
  {
    gambleHoldCheck = true;
  }
}

void amount_buttonPress()
{
  amountState = digitalRead(amount_button);
  
  if (amountState == HIGH && !amountHoldCheck)
  {
	amountPressTime = millis();
    amountHoldCheck = true;
  }
  
  if (amountState == HIGH && amountHoldCheck)
  {
	 amountPressDiff = millis()-amountPressTime;
	 if (amountPressDiff >= 1000)
     {
		gamble = 0;
		lcdPrint();
       
		amountHoldCheck = false;
		amountDebug = true;
     }
  }
  
  if (amountState == LOW && amountHoldCheck)
  {
    if (amountPressDiff < 1000)
    {
		if (amountDebug)
        {
			amountDebug = false; 
        }
		else
        { 
			gamble += 50;
			lcdPrint();
        }
    }
	amountHoldCheck = false; 
  }
}

void balanceUpdate(int rolls[5])
{
  int gambleAmount = gamble;
  balance -= gambleAmount;
  
  bool onethreefive = rolls[0] == rolls[2] && rolls[2] == rolls[4] && rolls[4] == 1;
  bool twofour = rolls[1] == rolls[3] && rolls[3] == 1;
  bool all = twofour && onethreefive;
  
  if (all)
  {
    gambleAmount *= 11;
  }
  else if (onethreefive)
  {
    gambleAmount *= 2;
  }
  else if (twofour)
  {
	 gambleAmount *= 1.5;
  }
  else gambleAmount = 0;
  
  balance += gambleAmount;
  
  lcdPrint();
}

void lcdPrint()
{
  lcd.clear();
  lcd.setCursor(0,0);
	lcd.print("balance:");
	lcd.print(balance);
  
  lcd.setCursor(0,1);
	lcd.print("bet:");
	lcd.print(gamble); 
}