#ifndef WIFI_COMM_H
#define WIFI_COMM_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

void InitESPNow();
void Wifi_init();
void ScanForSlave();
bool manageSlave();
void deletePeer();
void sendData();
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

#endif
