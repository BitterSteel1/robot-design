#include <Servo.h>
#include <Stepper.h>
+
// Defines the number of steps per rotation
const int stepsPerRevolution = 2038;

// Creates an instance of stepper class
// Pins entered in sequence IN1-IN3-IN2-IN4 for proper step sequence
Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

//Servo servo_ud;
Servo servo_oc;

void setup() {
  //servo_ud.attach(9);
  servo_oc.attach(12);
}

void clockwise(Servo &servo, int angle) {
 servo.write(angle);
}

void counterCW(Servo &servo, int angle) {
 servo.write(angle);
}

void loop() {

  // clockwise(servo_ud , 180);
  // delay(1000);
  // counterCW(servo_ud , 0);

  clockwise(servo_oc , 0);
  delay(1000);
  counterCW(servo_oc , 180);
  delay(1000);

  // Rotate CW slowly at 5 RPM
	myStepper.setSpeed(10);
	myStepper.step(stepsPerRevolution);
	delay(1000);
	
	// Rotate CCW quickly at 10 RPM
	myStepper.setSpeed(10);
	myStepper.step(-stepsPerRevolution);
	delay(1000);

}
