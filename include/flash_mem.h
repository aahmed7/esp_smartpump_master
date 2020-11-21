#ifndef FLASH_MEM_H
#define FLASH_MEM_H

#include <SPI.h>
#include <SPIFFS.h>
#include <Wire.h>
#include <ArduinoJson.h>

struct Config {
    time_t uts;
    byte wd[7];
    byte wh[24];
    bool wal;
    bool wad;
    byte mml;
    byte pod;
    bool TE;
    bool work_now;
};

extern int i;
extern Config config;
extern const char *filename;

void loadConfiguration(const char *filename, Config &config);
void printFile(const char *filename);
void saveConfiguration(const char *filename, const Config &config);
void init_config();

#endif