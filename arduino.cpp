IR_SENSOR_1 → D2 (interrupt) 

IR_SENSOR_2 → D3 (interrupt) 

RELAY_WHEEL_1 → D8  // Brake for Wheel 1 

RELAY_WHEEL_2 → D7  // Brake for Wheel 2 

#include <Wire.h> 

#include <LiquidCrystal_I2C.h> 

 

#define IR_SENSOR_1 2 

#define IR_SENSOR_2 3 

#define RELAY_WHEEL_1 8 

#define RELAY_WHEEL_2 7 

 

volatile unsigned int count1 = 0; 

volatile unsigned int count2 = 0; 

unsigned long lastTime = 0; 

 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

 

void isr_sensor1() { 

  count1++; 

} 

 

void isr_sensor2() { 

  count2++; 

} 

 

void setup() { 

  pinMode(IR_SENSOR_1, INPUT); 

  pinMode(IR_SENSOR_2, INPUT); 

  pinMode(RELAY_WHEEL_1, OUTPUT); 

  pinMode(RELAY_WHEEL_2, OUTPUT); 

 

  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_1), isr_sensor1, RISING); 

  attachInterrupt(digitalPinToInterrupt(IR_SENSOR_2), isr_sensor2, RISING); 

 

  lcd.begin(); 

  lcd.backlight(); 

  lcd.print("Traction Ctrl"); 

  delay(1500); 

  lcd.clear(); 

} 

 

void loop() { 

  unsigned long currentTime = millis(); 

   

  if (currentTime - lastTime >= 1000) { 

    // Calculate RPM (adjust slot count if needed) 

    float rpm1 = (count1 * 60.0) / 20.0; 

    float rpm2 = (count2 * 60.0) / 20.0; 

 

    count1 = 0; 

    count2 = 0; 

    lastTime = currentTime; 

 

    lcd.setCursor(0, 0); 

    lcd.print("W1:"); 

    lcd.print((int)rpm1); 

    lcd.print(" W2:"); 

    lcd.print((int)rpm2); 

    lcd.setCursor(0, 1); 

   // Traction logic with dual relays 

    if (abs(rpm1 - rpm2) > 100) { 

      if (rpm1 > rpm2) { 

        digitalWrite(RELAY_WHEEL_1, HIGH);  // Brake W1 

        digitalWrite(RELAY_WHEEL_2, LOW); 

        lcd.print("Braking Wheel 1 "); 

      } else { 

        digitalWrite(RELAY_WHEEL_1, LOW); 

        digitalWrite(RELAY_WHEEL_2, HIGH);  // Brake W2 

        lcd.print("Braking Wheel 2 "); 

      } 

    } else { 

      digitalWrite(RELAY_WHEEL_1, LOW); 

      digitalWrite(RELAY_WHEEL_2, LOW); 

      lcd.print("Balanced         "); 

    } 

  }} 