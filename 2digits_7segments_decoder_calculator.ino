 #include <Keypad.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

//define the cymbols on the buttons of the keypads

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
 
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

/////////////////////////////CD4511 the decoder of the 7 segments\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

//set the BCD 1 pins of the first CD4511 chip

int a1 = 10;
int a2 = 13;
int a3 = 12;
int a4 = 11;

//set the BCD 2 pins of the second CD4511 chip
int b1 = A0;
int b2 = A3;
int b3 = A2;
int b4 = A1;


//initialize the variables that will be used in the void loop
int n1, n2, rslt, turn, digitCount;
int d1, d2;
char op, key;
boolean digit1, digit2;

void setup(){
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  Serial.begin(9600);
  turn=1;
  digit1=0;
  digit2=0;
}

void loop(){
//detect the input
  key = customKeypad.getKey();
  if (key){
    Serial.print("the entered button is: ");
    Serial.println(key);
    buttonControl(); 
  }

  if (turn==3) {
    getRslt();
  }

  disp1();
  disp2();
}
  

void buttonControl () {
  if (rtrnInt(key)<=9){

    if (turn == 1){
      if (digit1){
        digitCount++;
        n1 = n1 * 10;
      }
      else {
        digitCount=1;
        digit1 = 1;
      }
      if (digitCount<3){
        n1 = n1 + rtrnInt(key);
        Serial.println("the entered number edited the first input");
        Serial.print("n1 = " );
        Serial.println(n1);
      }
      else {
        Serial.println("Error!! the first input is more than 2 digits.");
      }
      
      d1=n1%10; // This will divide n by 10 and the remainder will be stored in d1, this will hold the placeholder value for the first 7 segment display
      d2=n1/10; // This will divide n by 10 and the value before decimal will be stored in d2, this will hold the placeholder value for the second 7 segment display
         
    }
    
    else if (turn == 2){
      if (digit2){
        digitCount++;
        n2 = n2 * 10;
      }
      else {
        d1=0;
        d2=0;
        digit2 = 1;
        digitCount=1;
      }
      if (digitCount<3){
        Serial.println("the entered number edited the second input");
        n2 = n2 + rtrnInt(key);
        Serial.print("n2 = " );  
        Serial.println(n2);
        d1=n2%10; 
        d2=n2/10;
      }     
      else{
        Serial.println("Error!! the second input is more than 2 digits.");
      }
    }
  }
  else{
    if (key=='#'){
      if (turn!=3){
        turn ++;
        Serial.print("the button is = and the turn is increased to ");
        Serial.println(turn);
      }

    }
    else if (key == '*'){
      turn = 1;      
      digit1 = 0;
      digit2 = 0;
      n1 = 0;
      n2 = 0;
      d1=0;
      d2=0;
      Serial.println("it is reset time!! * button is pressed!!");
    }
    else{
      op = key;
      Serial.print("the operation is " );
      Serial.println(op);
      if (turn <3){
        turn++;
      }
    }
  }


}

int rtrnInt(char customKey){
  int n;
  switch (customKey){
   case '1':
    n = 1; 
   break;
   case '2':
    n = 2; 
    break;
   case '3':
    n = 3; 
    break;
   case '4':
    n = 4; 
    break;
   case '5':
    n = 5; 
    break;
   case '6':
    n = 6; 
    break;
   case '7':
    n = 7; 
    break;
   case '8':
    n = 8; 
    break;
   case '9':
    n = 9; 
    break;
   case '0':
    n = 0; 
    break;
  }
  return n;
}

void getRslt(){
  switch (op) {
    case 'A':
      rslt = n1 + n2 ;
      break;
    case 'B':
      rslt = n1 - n2 ;
      break;
    case 'C':
      rslt = n1 / n2 ;
      break;
    case 'D':
      rslt = n1 * n2 ;
      break;
  }
  if (rslt<100){
    d1=rslt%10; 
    d2=rslt/10; 
  } 
  else {
    Serial.println("Error the result is more than 2 digits!!");
    d1=0;
    d2=0;
  }
  Serial.print("the result is " );
  Serial.println(rslt);
}


void disp1(){ 
//this method entails making 0-9 combinations using the BCD pins by sending different outputs to turn each segment on or off. 
  if(d1 == 0)//0000
  {
    digitalWrite(a1, LOW);//0
    digitalWrite(a2, LOW);//0
    digitalWrite(a3, LOW);//0
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 1)//0001
  {
    digitalWrite(a1, HIGH);//1
    digitalWrite(a2, LOW);//0
    digitalWrite(a3, LOW);//0
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 2)//0010
  {
    digitalWrite(a1, LOW);//0
    digitalWrite(a2, HIGH);//1
    digitalWrite(a3, LOW);//0
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 3)//0011
  {
    digitalWrite(a1, HIGH);//1
    digitalWrite(a2, HIGH);//1
    digitalWrite(a3, LOW);//0
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 4)//0100
  {
    digitalWrite(a1, LOW);//0
    digitalWrite(a2, LOW);//0
    digitalWrite(a3, HIGH);//1
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 5)//0101
  {
    digitalWrite(a1, HIGH);//1
    digitalWrite(a2, LOW);//0
    digitalWrite(a3, HIGH);//1
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 6)//0110
  {
    digitalWrite(a1, LOW);//0
    digitalWrite(a2, HIGH);//1
    digitalWrite(a3, HIGH);//1
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 7) //0111
  {
    digitalWrite(a1, HIGH);//1
    digitalWrite(a2, HIGH);//1
    digitalWrite(a3, HIGH);//1
    digitalWrite(a4, LOW);//0
  }
  if(d1 == 8) //1000
  {
    digitalWrite(a1, LOW);//0
    digitalWrite(a2, LOW);//0
    digitalWrite(a3, LOW);//0
    digitalWrite(a4, HIGH);//1
  }
  if(d1 == 9)//1001
  {
    digitalWrite(a1, HIGH);//1
    digitalWrite(a2, LOW);//0
    digitalWrite(a3, LOW);//0
    digitalWrite(a4, HIGH);//1
  }
}

void disp2() 
//same as above
{  
  if(d2 == 0)//0000
  {
    digitalWrite(b1, LOW);//0
    digitalWrite(b2, LOW);//0
    digitalWrite(b3, LOW);//0
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 1)//0001
  {
    digitalWrite(b1, HIGH);//1
    digitalWrite(b2, LOW);//0
    digitalWrite(b3, LOW);//0
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 2)//0010
  {
    digitalWrite(b1, LOW);//0
    digitalWrite(b2, HIGH);//1
    digitalWrite(b3, LOW);//0
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 3)//0011
  {
    digitalWrite(b1, HIGH);//1
    digitalWrite(b2, HIGH);//1
    digitalWrite(b3, LOW);//0
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 4)//0100
  {
    digitalWrite(b1, LOW);//0
    digitalWrite(b2, LOW);//0
    digitalWrite(b3, HIGH);//1
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 5) //0101
  {
    digitalWrite(b1, HIGH);//1
    digitalWrite(b2, LOW);//0
    digitalWrite(b3, HIGH);//1
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 6) //0110
  {
    digitalWrite(b1, LOW);//0
    digitalWrite(b2, HIGH);//1
    digitalWrite(b3, HIGH);//1
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 7) //0111
  {
    digitalWrite(b1, HIGH);//1
    digitalWrite(b2, HIGH);//1
    digitalWrite(b3, HIGH);//1
    digitalWrite(b4, LOW);//0
  }
  if(d2 == 8) //1000
  {
    digitalWrite(b1, LOW);//0
    digitalWrite(b2, LOW);//0
    digitalWrite(b3, LOW);//0
    digitalWrite(b4, HIGH);//1
  }
  if(d2 == 9)//1001
  {
    digitalWrite(b1, HIGH);//1
    digitalWrite(b2, LOW);//0
    digitalWrite(b3, LOW);//0
    digitalWrite(b4, HIGH);//1
  }
}
