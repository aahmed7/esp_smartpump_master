#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>
#include <sys/time.h>

extern LiquidCrystal_I2C lcd;
extern uint8_t rcvd[2];
extern uint8_t moisture;

#define LEDPin     26
#define setButton  18
#define nextButton 19

void LCD_init();
void LCD_init_update();
void printLocalTime();
void setMDay();
void setMonth();
void setYear();
void setHour();
void setMin();
void set_working_days();
void set_working_hrs();
void set_min_moisture();
void set_pump_duration();
void set_language();
void Date_setting();
void update_lcd();
void store_new_data_to_uts();
void check_config_work_now();

#endif