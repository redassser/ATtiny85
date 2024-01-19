#define PIN 0
#define CLK 1
#define DIO 2

uint16_t pulse;
uint8_t bpm ;
uint8_t rollingBpm[8];
uint8_t oldestBpm = 0;
bool high = false;
bool zero = false;
uint16_t Time = 0;
byte num[4] = {8,8,8,8};
short mult[4] = {1000,100,10,1};

void start(void) 
{ 
    digitalWrite(CLK,HIGH);//send start signal to TM1637 
    digitalWrite(DIO,HIGH); 
    delayMicroseconds(5); 
  
    digitalWrite(DIO,LOW); 
    digitalWrite(CLK,LOW); 
    delayMicroseconds(5); 
} 
  
void stop(void) 
{ 
    digitalWrite(CLK,LOW); 
    digitalWrite(DIO,LOW); 
    delayMicroseconds(5); 
 
    digitalWrite(CLK,HIGH); 
    digitalWrite(DIO,HIGH); 
    delayMicroseconds(5); 
} 

bool writeValue(uint8_t value) 
{ 
    for(uint8_t i = 0; i < 8; i++) 
    { 
        digitalWrite(CLK, LOW); 
        delayMicroseconds(5); 
        digitalWrite(DIO, (value & (1 << i)) >> i); 
        delayMicroseconds(5); 
        digitalWrite(CLK, HIGH); 
        delayMicroseconds(5); 
    } 
  

    digitalWrite(CLK,LOW); 
    delayMicroseconds(5); 
    digitalWrite(CLK,HIGH); 
    delayMicroseconds(5); 
 
    return 1; 
}

void setup() {
  //Pins
  pinMode(PIN, INPUT);
  pinMode(CLK,OUTPUT);
  pinMode(DIO,OUTPUT);

  //Pulse setup
  Time = millis();
  //Display setup
  start();
  writeValue(0x8f);
  stop();
}
void loop() {
  pulse = analogRead(PIN); // Get the voltage

  if (pulse > 2700 && !high) { // If voltage is high enough, a pulse is read
    //Get time since last pulse and bpm
    Time = millis() - Time; 
    bpm = 60000 / Time; 

    //Assign bpm and recalculate rolling average
    rollingBpm[oldestBpm] = bpm;  pulse = 0;
    for (bpm = 0; bpm < 8; bpm++) pulse += rollingBpm[bpm] == 0 ? rollingBpm[0] : rollingBpm[bpm];
    bpm = pulse >> 3; zero = false;
    for (pulse = 0; pulse < 4; pulse++) {
      if(bpm / mult[pulse] % 10) zero = true;
      switch(bpm / mult[pulse] % 10) {
        case 1:
          num[pulse] = 0x06;
          break;
        case 2:
          num[pulse] = 0x5B;
          break;
        case 3:
          num[pulse] = 0x4F;
          break;
        case 4:
          num[pulse] = 0x66;
          break;
        case 5:
          num[pulse] = 0x6D;
          break;
        case 6:
          num[pulse] = 0x7D;
          break;
        case 7:
          num[pulse] = 0x07;
          break;
        case 8:
          num[pulse] = 0x7F;
          break;
        case 9:
          num[pulse] = 0x67;
          break;
        default:
          if (zero) num[pulse] = 0x3F;
          else num[pulse] = 0x00;
          break;
      }
    }
    
    //Variables
    Time = millis(); high = true; oldestBpm++;
    if (oldestBpm > 8) oldestBpm = 0;

    // Printing
  }
  else if (pulse < 2000 && high) high = false;


  start();
  writeValue(0x40);
  stop();
  start();
  writeValue(0xc0);
  writeValue(0);
  writeValue(num[1]);
  writeValue(num[2]);
  writeValue(num[3]);
  stop();

  delay(1);
}