// * main.cpp

/*
Autor: Kauã Cordeiro
Programa: Conexão MQTT
Descrição: Conexão MQTT
Data: 28/04/2026
Versão: 1.1
*/

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "WiFiManager.h"
#include "MQTTManager.h"

void setup()
{
  Serial.begin(9600);
  conectarWiFi();
}

void loop()
{
  garantirWiFiConectado();
}