#include <wifi_comm.h>
#include <flash_mem.h>
#include <lcd_display.h>

// Global copy of slave
esp_now_peer_info_t slave;
#define WIFI_CHANNEL     1
#define PRINTSCANRESULTS 0
#define DELETEBEFOREPAIR 0

int k;
uint8_t data[5];
uint8_t remoteMac[] = {0x30, 0xAE, 0xA4, 0x21, 0xB4, 0x89};
const esp_now_peer_info_t *peer = &slave;

void InitESPNow()
{
    WiFi.disconnect();
    if (esp_now_init() == ESP_OK) {
        Serial.println("ESPNow Init Success");
    }
    else {
        Serial.println("ESPNow Init Failed");
        // Retry InitESPNow, add a counter and then restart?
        // InitESPNow();
        // or Simply Restart
        ESP.restart();
    }
}

void Wifi_init()
{
    WiFi.mode(WIFI_STA);
    Serial.println("ESPNow/Basic/Master Example");
    Serial.print("STA MAC: ");
    Serial.println(WiFi.macAddress());
    InitESPNow();

    memcpy(&slave.peer_addr, &remoteMac, 6);
    slave.channel = WIFI_CHANNEL;
    slave.encrypt = 0;
    if (esp_now_add_peer(peer) == ESP_OK) {
        Serial.println("Added Peer!");
    }

    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    ScanForSlave();
    if (slave.channel == WIFI_CHANNEL) {
        bool isPaired = manageSlave();
        if (isPaired) {
            sendData();
        }
        else {
            Serial.println("Slave pair failed!");
        }
    }
    else {
    }
}

void ScanForSlave()
{
    int8_t scanResults = WiFi.scanNetworks();
    // reset on each scan
    bool slaveFound = 0;
    memset(&slave, 0, sizeof(slave));

    Serial.println("");
    if (scanResults == 0) {
        Serial.println("No WiFi devices in AP Mode found");
    }
    else {
        Serial.print("Found ");
        Serial.print(scanResults);
        Serial.println(" devices ");
        for (int i = 0; i < scanResults; ++i) {
            // Print SSID and RSSI for each device found
            String SSID = WiFi.SSID(i);
            int32_t RSSI = WiFi.RSSI(i);
            String BSSIDstr = WiFi.BSSIDstr(i);

            if (PRINTSCANRESULTS) {
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(SSID);
                Serial.print(" (");
                Serial.print(RSSI);
                Serial.print(")");
                Serial.println("");
            }
            delay(10);
            // Check if the current device starts with `Slave`
            if (SSID.indexOf("Slave") == 0) {
                // SSID of interest
                Serial.println("Found a Slave.");
                Serial.print(i + 1);
                Serial.print(": ");
                Serial.print(SSID);
                Serial.print(" [");
                Serial.print(BSSIDstr);
                Serial.print("]");
                Serial.print(" (");
                Serial.print(RSSI);
                Serial.print(")");
                Serial.println("");
                // Get BSSID => Mac Address of the Slave
                int mac[6];
                if (6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x%c",
                                &mac[0], &mac[1], &mac[2], &mac[3], &mac[4],
                                &mac[5])) {
                    for (int ii = 0; ii < 6; ++ii) {
                        slave.peer_addr[ii] = (uint8_t)mac[ii];
                    }
                }

                slave.channel = WIFI_CHANNEL;  // pick a channel
                slave.encrypt = 0;             // no encryption

                slaveFound = 1;
                // we are planning to have only one slave in this example;
                // Hence, break after we find one, to be a bit efficient
                break;
            }
        }
    }

    if (slaveFound) {
        Serial.println("Slave Found, processing..");
    }
    else {
        Serial.println("Slave Not Found, trying again.");
    }

    // clean up ram
    WiFi.scanDelete();
}

bool manageSlave()
{
    if (slave.channel == WIFI_CHANNEL) {
        if (DELETEBEFOREPAIR) {
            deletePeer();
        }

        Serial.print("Slave Status: ");
        const esp_now_peer_info_t *peer = &slave;
        const uint8_t *peer_addr = slave.peer_addr;
        // check if the peer exists
        bool exists = esp_now_is_peer_exist(peer_addr);
        if (exists) {
            // Slave already paired.
            Serial.println("Already Paired");
            return true;
        }
        else {
            // Slave not paired, attempt pair
            esp_err_t addStatus = esp_now_add_peer(peer);
            if (addStatus == ESP_OK) {
                // Pair success
                Serial.println("Pair success");
                return true;
            }
            else if (addStatus == ESP_ERR_ESPNOW_NOT_INIT) {
                // How did we get so far!!
                Serial.println("ESPNOW Not Init");
                return false;
            }
            else if (addStatus == ESP_ERR_ESPNOW_ARG) {
                Serial.println("Invalid Argument");
                return false;
            }
            else if (addStatus == ESP_ERR_ESPNOW_FULL) {
                Serial.println("Peer list full");
                return false;
            }
            else if (addStatus == ESP_ERR_ESPNOW_NO_MEM) {
                Serial.println("Out of memory");
                return false;
            }
            else if (addStatus == ESP_ERR_ESPNOW_EXIST) {
                Serial.println("Peer Exists");
                return true;
            }
            else {
                Serial.println("Not sure what happened");
                return false;
            }
        }
    }
    else {
        // No slave found to process
        Serial.println("No Slave found to process");
        return false;
    }
}

void deletePeer()
{
    const esp_now_peer_info_t *peer = &slave;
    const uint8_t *peer_addr = slave.peer_addr;
    esp_err_t delStatus = esp_now_del_peer(peer_addr);
    Serial.print("Slave Delete Status: ");
    if (delStatus == ESP_OK) {
        // Delete success
        Serial.println("Success");
    }
    else if (delStatus == ESP_ERR_ESPNOW_NOT_INIT) {
        // How did we get so far!!
        Serial.println("ESPNOW Not Init");
    }
    else if (delStatus == ESP_ERR_ESPNOW_ARG) {
        Serial.println("Invalid Argument");
    }
    else if (delStatus == ESP_ERR_ESPNOW_NOT_FOUND) {
        Serial.println("Peer not found.");
    }
    else {
        Serial.println("Not sure what happened");
    }
}

void sendData()
{
    // data++;
    data[0] = config.wal;
    data[1] = config.wad;
    data[2] = config.mml;
    data[3] = config.pod;
    data[4] = config.work_now;

    const uint8_t *peer_addr = slave.peer_addr;
    for (i = 0; i < 5; i++) {
        Serial.print("Sending: ");
        Serial.println(data[i]);
        esp_err_t result = esp_now_send(peer_addr, &data[i], sizeof(data[i]));
        delay(100);
    }
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
             mac_addr[5]);
    Serial.print("Last Packet Sent to: ");
    Serial.println(macStr);
    Serial.print("Last Packet Send Status: ");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                  : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
    Serial.printf("\r\nReceived\t%d Bytes\t%d", data_len, *data);
    rcvd[k] = *data;
    k++;
    if (k > 1) k = 0;
    moisture = rcvd[0];
    config.TE = rcvd[1];
}