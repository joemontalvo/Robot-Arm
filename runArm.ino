#include <Servo.h>
#include <Adafruit_PWMServoDriver.h>

int prev_values[] = {0,0,0,0,0};
static const uint8_t analog_pins[] = {A0,A1,A2,A3,A4};

Adafruit_PWMServoDriver driver = Adafruit_PWMServoDriver();

#define SERVOMIN 150
#define SERVOMAX 600

#define OPEN_BUTTON_PIN 2
#define CLOSE_BUTTON_PIN 3

#define CLAW_ANGLE_MIN 0
#define CLAW_ANGLE_MAX 100

int claw_angle = 0;

float percent = 0.5;

void setup() {
  Serial.begin(9600);
  driver.begin();
  driver.setPWMFreq(60);
  pinMode(OPEN_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CLOSE_BUTTON_PIN, INPUT_PULLUP);
  yield();
}

void loop() {

  for(int i = 0; i < 4; i++){
    
    int input = analogRead(analog_pins[i]);
    input = smooth(input, prev_values[i], percent);
    prev_values[i] = input;
    
    int angle = map(input, 0,1023,165,0);

    int pwm = map(angle,0, 180, SERVOMIN, SERVOMAX);
    driver.setPWM(i,0,pwm);
    
  }
  
  if(!digitalRead(OPEN_BUTTON_PIN)){
    if(claw_angle > CLAW_ANGLE_MIN){
      claw_angle--;
    }
  }
  else if(!digitalRead(CLOSE_BUTTON_PIN)){
    if(claw_angle < CLAW_ANGLE_MAX){
      claw_angle++;
    }
  }

  int pwm = map(claw_angle,0, 180, SERVOMIN, SERVOMAX);
  
  driver.setPWM(4,0,pwm);

  delay(20);
  
}

float smooth(int input, int prev, float percent){
  return ((float)input * percent) + ((float)prev * (1-percent));
}
