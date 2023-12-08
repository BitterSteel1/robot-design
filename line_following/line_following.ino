// Define pin numbers
uint8_t const IR_pins[] = {A8, A9, A10, A11, A12, A13, A14, A15};
uint16_t sensorValues[8];
uint16_t const threshold = 250;
uint8_t IR_digital_array[8];

// ENTC - Motor driver pins
uint8_t const INA_1 = 2;
uint8_t const INB_1 = 23;
uint8_t const INA_2 = 3;
uint8_t const INB_2 = 22;

// PID & errors
float const line_kp = 0.0067; //5-54-60-65-70
float const line_kd = 0.053; //0.018// 28//35// 
float const line_ki = 0.0005;
uint16_t const baseSpeed = 60;

int previousLineError = 0;
int line_error;  //temporary use for checking the ir array           
int control_signal_1;  //for temp use of tunning and debuging- can be made local in pid func   
int control_signal_2;     
int IR_trigger_count = 0; 
int total_line_error = 0; 
int line_integral = 0;

uint8_t checkpoint = 0;

void setup() {
    // Initialize Builtin led
    pinMode(LED_BUILTIN, OUTPUT);

    // Initialize Serial Monitor
    Serial.begin(9600);

    // Initialize motor driver
    initializeMotorDriver();

    // Initialize IR array 
    initializeIRArray();

    // Initialize indicator
    startIndicator();
}

void loop() {
    pid_line_following();
    delay(30);
    find_junction();
}

// //////////////////////////////////////////////////////
// function to get debug output of IR array
void PIDLineDebug() {
    for (uint8_t i = 0; i < 8; i++) {
        Serial.print(sensorValues[i]);
        Serial.print('\t');
    }
    Serial.println('\t');
    for (uint8_t i = 0; i < 8; i++) {
        Serial.print(IR_digital_array[i]);
        Serial.print('\t');
    }
    Serial.print(control_signal_1);
    Serial.print('\t');
    Serial.print(control_signal_2);
    Serial.print('\t');
    Serial.print(line_error);
    Serial.print('\t');
    Serial.print(IR_trigger_count);
    Serial.print('\t');
    Serial.println(total_line_error);
}

// function to move the robot along the line according to PID
void pid_line_following() {

    int currentLineError = get_line_following_error();
    
    line_error = currentLineError;
    line_integral += currentLineError;

    int line_delta = currentLineError - previousLineError;
    float line_CT = currentLineError * line_kp + line_delta * line_kd + line_integral * line_ki;
    previousLineError = currentLineError;

    control_signal_2 = baseSpeed + round(constrain(line_CT, -255, 255));
    control_signal_1 = baseSpeed - round(constrain(line_CT, -255, 255));

    rotate_wheels(control_signal_1, control_signal_2);

    delay(4);

    PIDLineDebug(); // comment out in normal operation
}



// function to initialize IR array
void initializeIRArray() {
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(IR_pins[i], INPUT);
    }
}

// function to initialize motor driver
void initializeMotorDriver() {
    pinMode(INA_1, OUTPUT);
    pinMode(INB_1, OUTPUT);
    pinMode(INA_2, OUTPUT);
    pinMode(INB_2, OUTPUT);
}

// function to start led indicator
void startIndicator() {
    for (uint8_t i = 0; i < 10; i++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(200);
        digitalWrite(LED_BUILTIN, LOW);
        delay(200);
    }
}

// function to rotate wheels
void rotate_wheels(int speedA, int speedB) {
    if (speedA > 0) {
        digitalWrite(INB_1, LOW);
        analogWrite(INA_1, speedA);
    } else {
        digitalWrite(INB_1, HIGH);
        analogWrite(INA_1, -speedA);
    }
    if (speedB > 0) {
        digitalWrite(INB_2, LOW);
        analogWrite(INA_2, speedB);
    } else {
        digitalWrite(INB_2, HIGH);
        analogWrite(INA_2, -speedB);
    }
}

// function to blink the LED builtin
void blink_builtin_led() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

// function to get the digital out of the IR array
uint8_t get_ir_digital_out(uint8_t pin) {
    int IR_analog_value = analogRead(pin);
    if (IR_analog_value < threshold) {
        return 1;
    } else {
        return 0;
    }
}

// function to get the line error
int get_line_following_error() {
    total_line_error = 0;
    IR_trigger_count = 0;
    int error_v;
    for (uint8_t i = 0; i < 8; i++) {
        IR_digital_array[i] = get_ir_digital_out(IR_pins[i]);
        if (IR_digital_array[i] == 1) {
            IR_trigger_count += 1;
            total_line_error = total_line_error + 1000 * i;
            sensorValues[i] = analogRead(IR_pins[i]);
        }
        sensorValues[i] = analogRead(IR_pins[i]);
    }
    if (IR_trigger_count != 0) {
        error_v = total_line_error / IR_trigger_count - 3500;
    }
    return error_v;
}



// function to kick start the motor
void shock() {
    rotate_wheels(100, 100);
    delay(40);
    rotate_wheels(0, 0);
}


// function to find junctions, dead ends
void find_junction() {
    // TODO: Change value here
    uint8_t no_iterations = 8;


    uint16_t turn_speed = 65;
    uint16_t turn_time = 130;
    

    if (IR_trigger_count >= 5) {
        bool left_turn = IR_digital_array[0] == 1;
        bool right_turn = IR_digital_array[7] == 1;
        
        for (uint8_t i = 0; i < no_iterations; i++) {
            pid_line_following();
            delay(34);
        }

        if (left_turn && right_turn) {
            if (IR_trigger_count == 0) {
                rotate_wheels(0, turn_speed);
                delay(turn_time);
            } else {
                checkpoint++;
                while (true) {
                    rotate_wheels(0, 0);
                }
            }
        } else if (left_turn) {
            if (IR_trigger_count == 0) {
                rotate_wheels(0, turn_speed);
                delay(turn_time);
            } else {
                return;
            }
        } else if (right_turn) {
            if (IR_trigger_count == 0) {
                rotate_wheels(turn_speed, 0);
                delay(turn_time);
            } else {
                return;
            }
        }
    } else if (IR_trigger_count == 0) {
        rotate_wheels(turn_speed, -turn_speed);
        delay(turn_time);
    }
}



