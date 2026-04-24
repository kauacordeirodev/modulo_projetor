// WiFIManager.cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "WiFiManager.h"
#include "secrets.h"

bool WiFiEstaConectado()
{
    return WiFi.status() == WL_CONNECTED;
}

void conectarWiFi()
{
    Serial.println("=========================");
    Serial.println("Iniciando conexão WiFi...");
    Serial.println("=========================");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_SENHA);

    Serial.print("Conectando...");

    uint8_t tentativas = 0;
    const uint8_t maxTentativas = 30;

    while (WiFi.status() != WL_CONNECTED && tentativas < maxTentativas)
    {
        delay(500);
        tentativas++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi conectado com sucesso!");
        Serial.print("Endereço IP: ");
        Serial.println(WiFi.localIP());
    }
    else
    {
        Serial.println("Falha ao conectar ao WiFi!");
        Serial.println("Verifique o SSID, senha e sinal de rede.");
    }
}

void garantirWiFiConectado()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFI desconectado! Tentando reconectar...");
        conectarWiFi();
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Não foi possível reconectar ao WiFi.");
    }
}