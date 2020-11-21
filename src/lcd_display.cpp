#include <lcd_display.h>
#include <flash_mem.h>

/* Setting for date and month. */
String days[7] = {"Sunday          ", "Monday          ", "Tuesday         ",
                  "Wednesday       ", "Thursday        ", "Friday          ",
                  "Saturday        "};
String months[12] = {
    "January         ", "February        ", "March           ",
    "April           ", "May             ", "June            ",
    "July            ", "August          ", "September       ",
    "October         ", "November        ", "December        "};


LiquidCrystal_I2C lcd(0x27, 20, 4);

struct tm setdate;
struct timeval tv;
int language;
uint8_t moisture = 0;
uint8_t rcvd[2] = {0, 0};
time_t randtime;

void LCD_init()
{
    lcd.init();
    lcd.init();
    lcd.backlight();
}

void LCD_init_update()
{
    getLocalTime(&setdate);
    set_language();
    lcd.clear();
    update_lcd();
}

void printLocalTime()
{
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setMDay()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set day of month");
    int i = setdate.tm_mday;
    while (1) {
        lcd.setCursor(1, 1);
        if (i < 10) lcd.print('0');
        lcd.print(i);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            setdate.tm_mday = i;
            return;
        }
        i++;
        if (i > 30 && ((setdate.tm_mon == 3) || (setdate.tm_mon == 5) ||
                       (setdate.tm_mon == 8) || (setdate.tm_mon == 10)))
            i = 1;
        else if (i > 28 && (setdate.tm_mon == 1))
            i = 1;
        else if (i > 31)
            i = 1;
        delay(200);
    }
    delay(50);
}

void setMonth()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set the month       ");
    i = setdate.tm_mon;
    while (1) {
        lcd.setCursor(1, 1);
        lcd.print(months[i]);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);

        if (!digitalRead(setButton)) {
            setdate.tm_mon = i;
            return;
        }
        i++;
        if (i > 11) i = 0;
        delay(200);
    }
    delay(50);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Month Set");
}

void setYear()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set Year");
    int i = 2019;
    while (1) {
        lcd.setCursor(1, 1);
        lcd.print(i);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            setdate.tm_year = i - 1900;
            return;
        }
        i++;
        if (i > 2030) i = 2019;
        delay(200);
    }
}

void setHour()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set Hour");
    int i = setdate.tm_hour;
    while (1) {
        lcd.setCursor(1, 1);
        if (i < 10) lcd.print('0');
        lcd.print(i);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            setdate.tm_hour = i;
            return;
        }
        i++;
        if (i == 24) i = 0;
        delay(200);
    }
}

void setMin()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set Minutes");
    int i = setdate.tm_min;
    while (1) {
        lcd.setCursor(1, 1);
        if (i < 10) lcd.print('0');
        lcd.print(i);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            setdate.tm_min = i;
            setdate.tm_sec = 0;
            return;
        }
        i++;
        if (i == 60) i = 0;
        delay(200);
    }
}

void set_working_days()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Work all Week?");
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);
    if (!digitalRead(setButton)) {
        config.wal = true;
        return;
    }
    config.wal = false;

    lcd.setCursor(1, 0);
    lcd.print("Work on Sundays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);
    if (!digitalRead(setButton)) {
        config.wd[0] = 0;
    }
    else {
        config.wd[0] = 10;
    }
    lcd.setCursor(1, 0);
    lcd.print("Work on Mondays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wd[0] = 1;
    }
    else {
        config.wd[0] = 10;
    }
    lcd.setCursor(1, 0);
    lcd.print("Work on Tuesdays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wd[0] = 2;
    }
    else {
        config.wd[0] = 10;
    }
    lcd.setCursor(1, 0);
    lcd.print("Work on Wednesdays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wd[0] = 3;
    }
    else {
        config.wd[0] = 10;
    }
    lcd.setCursor(1, 0);
    lcd.print("Work on Thursdays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wd[0] = 4;
    }
    else {
        config.wd[0] = 10;
    }
    lcd.setCursor(1, 0);
    lcd.print("Work on Fridays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wd[0] = 5;
    }
    else {
        config.wd[0] = 10;
    }
    lcd.setCursor(1, 0);
    lcd.print("Work on Saturdays?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wd[0] = 6;
    }
    else {
        config.wd[0] = 10;
    }
}

void set_working_hrs()
{
    lcd.setCursor(1, 0);
    lcd.print("Work all Day?");
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);

    if (!digitalRead(setButton)) {
        config.wad = true;
        return;
    }
    config.wad = false;
    lcd.print("Keep Default?");
    delay(200);
    while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
    delay(10);
    if (!digitalRead(setButton)) {
    }

    for (i = 0; i < 24; i++) {
        lcd.setCursor(1, 0);
        lcd.print("Work at ");
        if (i < 10) lcd.print('0');
        lcd.print(i);
        lcd.print("?");
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            config.wh[i] = i;
        }
        else {
            config.wh[i] = 50;
        }
        delay(200);
    }
}

void set_min_moisture()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set min moisture level");
    int i = config.mml;
    while (1) {
        lcd.setCursor(1, 1);
        lcd.print(i);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            config.mml = i;
            return;
        }
        i++;
        if (i == 101) i = 0;
        delay(200);
    }
    delay(200);
}

void set_pump_duration()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Set Pump on Duration(min)");
    int i = config.pod;
    while (1) {
        lcd.setCursor(1, 1);
        lcd.print(i);
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            config.pod = i;
            return;
        }
        i++;
        if (i == 31) i = 0;
        delay(200);
    }
    delay(200);
}

void set_language()
{
    lcd.clear();
    lcd.setCursor(1, 0);
    while (1) {
        lcd.setCursor(1, 0);
        lcd.print("Set Language");
        lcd.setCursor(1, 1);
        lcd.print("English");
        while (digitalRead(setButton) && digitalRead(nextButton)) delay(10);
        delay(10);

        if (!digitalRead(setButton)) {
            return;
        }
        delay(200);
    }
}

void Date_setting()
{
    int j = 0;
    // setWDay();
    setMonth();
    delay(200);
    setMDay();
    delay(200);
    setYear();
    delay(200);
    setHour();
    delay(200);
    setMin();
    delay(200);
    set_working_days();
    delay(200);
    set_working_hrs();
    delay(200);
    set_min_moisture();
    delay(200);
    set_pump_duration();
    delay(200);
    for (i = 0; i < 7; i++)
        if (config.wd[i] != 10) j++;
    if (j == 7) config.wal = true;
    j = 0;
    delay(200);
    for (i = 0; i < 24; i++)
        if (config.wd[i] != 50) j++;
    if (j == 24) config.wal = true;

    tv.tv_sec = config.uts;
    settimeofday(&tv, NULL);

    store_new_data_to_uts();

    delay(200);
}

void update_lcd()
{
    getLocalTime(&setdate);
    lcd.setCursor(0, 0);
    lcd.print(days[setdate.tm_wday].substring(0, 2));
    lcd.setCursor(4, 0);
    if (setdate.tm_mday < 10) lcd.print("0");
    lcd.print(setdate.tm_mday);
    lcd.setCursor(6, 0);
    lcd.print('/');
    lcd.setCursor(7, 0);
    if (setdate.tm_mon < 9) lcd.print("0");
    lcd.print(setdate.tm_mon + 1);
    lcd.setCursor(9, 0);
    lcd.print('/');
    lcd.setCursor(10, 0);
    lcd.print(setdate.tm_year + 1900);
    lcd.setCursor(0, 1);
    if (setdate.tm_hour < 10) lcd.print("0");
    lcd.print(setdate.tm_hour);
    lcd.setCursor(2, 1);
    lcd.print(':');
    lcd.setCursor(3, 1);
    if (setdate.tm_min < 10) lcd.print("0");
    lcd.print(setdate.tm_min);
    lcd.setCursor(8, 1);
    if (moisture < 10) lcd.print("0");
    lcd.print(moisture);
    lcd.setCursor(10, 1);
    lcd.print('%');
}

void store_new_data_to_uts()
{
    config.uts = mktime(&setdate);
    saveConfiguration(filename, config);
    printFile(filename);
}

void check_config_work_now()
{
    if (config.wad && config.wal) {
        config.work_now = true;
        return;
    }
    else if (!config.wad && config.wal) {
        for (i = 0; i < 24; i++)
            if (config.wh[i] == setdate.tm_hour) {
                config.work_now = true;
                return;
            }
            else
                config.work_now = false;
    }
    else if (config.wad && !config.wal) {
        for (i = 0; i < 7; i++)
            if (config.wd[i] == setdate.tm_wday) {
                config.work_now = true;
                return;
            }
            else
                config.work_now = false;
    }
    else if (!config.wad && !config.wal)
        for (i = 0; i < 7; i++)
            if (config.wd[i] == setdate.tm_wday) {
                for (i = 0; i < 24; i++)
                    if (config.wh[i] == setdate.tm_hour) {
                        config.work_now = true;
                        return;
                    }
                    else
                        config.work_now = false;
            }
            else
                config.work_now = false;

    printf("%d", config.work_now);
}