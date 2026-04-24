/*
Autor: Kauã Cordeiro
Programa: Conexão MQTT
Descrição: Conexão MQTT
Data: 24/04/2026
Versão: 1.0
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"

void setup()
{
  Serial.begin(9600);
  conectarWiFi();
}

void loop()
{
  garantirWiFiConectado();
}