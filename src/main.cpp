#include <Arduino.h>
#include <flash_mem.h>
#include <lcd_display.h>
#include <wifi_comm.h>

unsigned long start;
double fin = 10000.0;  // 10 sec

void setup()
{
    LCD_init();

    Serial.begin(115200);
    while (!Serial) continue;

    init_config();
    Wifi_init();
    
    pinMode(setButton, INPUT_PULLUP);
    pinMode(nextButton, INPUT_PULLUP);
    // attachInterrupt(digitalPinToInterrupt(setButton), setPressed, RISING);
    // attachInterrupt(digitalPinToInterrupt(nextButton), nextPressed, RISING);

    LCD_init_update();
    delay(1000);
    // Date_setting();
}

/*
  MON 23/09/19
  04:38   100%
*/

void loop()
{
    if (!digitalRead(setButton)) {
        delay(200);
        Date_setting();
        lcd.clear();
        update_lcd();
        // Serial.println(setdate.tm_year);

        delay(1000);
    }

    if (millis() - start > fin)  // store the settings after every 10s
    {
        store_new_data_to_uts();
        start = millis();
        if (digitalRead(setButton) && digitalRead(setButton)) {
            update_lcd();
        }
        if (config.TE == true) {
            digitalWrite(LEDPin, HIGH);
            lcd.setCursor(12, 1);
            lcd.print('!');
        }
        else {
            digitalWrite(LEDPin, LOW);
            lcd.setCursor(12, 1);
            lcd.print(' ');
        }
        check_config_work_now();
        sendData();
    }
}
