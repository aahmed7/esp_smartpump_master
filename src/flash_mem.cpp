#include <flash_mem.h>

int i;
String datastream;
const char *filename = "/config.txt";
Config config;

void loadConfiguration(const char *filename, Config &config)
{
    File file = SPIFFS.open(filename);
    StaticJsonDocument<664> doc;

    DeserializationError error = deserializeJson(doc, file);
    if (error)
        Serial.println(F("Failed to read file, using default configuration"));

    JsonArray wd = doc["wd"];
    JsonArray wh = doc["wh"];

    config.uts = doc["uts"] | 1569235298;
    for (i = 0; i < 7; i++) config.wd[i] = wd[i];
    for (i = 0; i < 24; i++) config.wh[i] = wh[i];
    config.wal = doc["wal"] | true;
    config.wad = doc["wad"] | true;
    config.mml = doc["mml"] | 20;
    config.pod = doc["pod"] | 10;
    config.TE = doc["TE"] | true;

    datastream = "JSON =" + doc.as<String>();

    file.close();
}

void printFile(const char *filename)
{
    // Open file for reading
    File file = SPIFFS.open(filename);
    if (!file) {
        Serial.println(F("Failed to read file"));
        return;
    }

    // Extract each characters by one by one
    while (file.available()) {
        Serial.print((char)file.read());
    }
    Serial.println();

    // Close the file
    file.close();
}

void saveConfiguration(const char *filename, const Config &config)
{
    SPIFFS.remove(filename);

    File file = SPIFFS.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }
    StaticJsonDocument<664> doc;

    doc["uts"] = config.uts;
    doc["wal"] = config.wal;
    doc["wad"] = config.wad;
    doc["mml"] = config.mml;
    doc["pod"] = config.pod;
    doc["TE"] = config.TE;
    JsonArray wd = doc.createNestedArray("wd");
    wd.add(config.wd[0]);
    wd.add(config.wd[1]);
    wd.add(config.wd[2]);
    wd.add(config.wd[3]);
    wd.add(config.wd[4]);
    wd.add(config.wd[5]);
    wd.add(config.wd[6]);
    JsonArray wh = doc.createNestedArray("wh");
    wh.add(config.wh[0]);
    wh.add(config.wh[1]);
    wh.add(config.wh[2]);
    wh.add(config.wh[3]);
    wh.add(config.wh[4]);
    wh.add(config.wh[5]);
    wh.add(config.wh[6]);
    wh.add(config.wh[7]);
    wh.add(config.wh[8]);
    wh.add(config.wh[9]);
    wh.add(config.wh[10]);
    wh.add(config.wh[11]);
    wh.add(config.wh[12]);
    wh.add(config.wh[13]);
    wh.add(config.wh[14]);
    wh.add(config.wh[15]);
    wh.add(config.wh[16]);
    wh.add(config.wh[17]);
    wh.add(config.wh[18]);
    wh.add(config.wh[19]);
    wh.add(config.wh[20]);
    wh.add(config.wh[21]);
    wh.add(config.wh[22]);
    wh.add(config.wh[23]);

    if (serializeJson(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }
}

void init_config()
{
    while (!SPIFFS.begin()) {
        Serial.println(F("Failed to initialize SPIFFS library"));
        delay(1000);
    }

    Serial.println(F("Loading configuration..."));
    loadConfiguration(filename, config);
    for (i = 0; i < 24; i++) config.wh[i] = i;
    for (i = 0; i < 7; i++) config.wd[i] = i;
    Serial.println(F("Saving configuration..."));
    saveConfiguration(filename, config);
    Serial.println(F("Print config file..."));
    printFile(filename);
}