uint16_t pulse;
uint8_t bpm;
uint8_t rollingBpm[8];
uint8_t oldestBpm = 0;
bool high = false;
uint16_t Time = 0;

void setup() {
  Serial.begin(115200);
  pinMode(3, INPUT);
  Time = millis();
}
void loop() {
  pulse = analogRead(3); // Get the voltage
  if (pulse > 2900 && !high) { // If voltage is high enough, a pulse is read
    //Get time since last pulse and bpm
    Time = millis() - Time; 
    bpm = 60000 / Time; 

    //Assign bpm and recalculate rolling average
    rollingBpm[oldestBpm] = bpm;  pulse = 0;
    for (bpm = 0; bpm < 8; bpm++) pulse += rollingBpm[bpm];
    bpm = pulse >> 3;
    
    //Variables
    Time = millis(); high = true; oldestBpm++;
    if (oldestBpm > 8) oldestBpm = 0;

    // Printing
    Serial.println(bpm);
  }
  else if (pulse < 2000 && high) high = false;
  delay(1);
}