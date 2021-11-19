// Arduino pin assignment
#include <Servo.h>
#define PIN_SERVO 10

#define PIN_IR A0
#define PIN_LED 9
#define _DIST_ALPHA 0.35 // ema필터에서 적용할 알파값

Servo myservo;

int a, b; // unit: mm
float dist_ema, alpha;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  digitalWrite(PIN_LED, 1);
  
// initialize serial port
  Serial.begin(57600);

  a = 95;
  b = 407;
  alpha = _DIST_ALPHA;
  dist_ema = 0;
}

float ir_distance(void){ // return value unit: mm
  float val;
  float volt = float(analogRead(PIN_IR));
  val = ((6762.0/(volt-9.0))-4.0) * 10.0;
  return val;
}

void loop() {
  myservo.attach(PIN_SERVO); 
  float raw_dist = ir_distance();
  float dist_cali = 100 + 300.0 / (b - a) * (raw_dist - a);
  if (dist_ema == 0){
    dist_ema = dist_cali;
  }
  else{
    dist_ema = alpha * dist_cali + (1-alpha) * dist_ema;
  }
  Serial.print("min:0,max:410,dist_cali:");
  Serial.print(dist_cali);
  Serial.print(",dist_ema:");
  Serial.println(dist_ema + 100);
  if(raw_dist > 156 && raw_dist <224) digitalWrite(PIN_LED, 0);
  else digitalWrite(PIN_LED, 255);
  if (raw_dist > 255){
    myservo.writeMicroseconds(1115);
  }
  else{
    myservo.writeMicroseconds(1640);
  }
  delay(20);
}
