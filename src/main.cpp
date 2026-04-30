// * main.cpp

/*
Autor: Kauã Cordeiro
Programa: Conexão MQTT
Descrição: Conexão MQTT
Data: 30/04/2026
Versão: 1.2
*/

#include <Arduino.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "DebugManager.h"

void tratarMensagemRecebida(const char *, const String &);

void setup()
{
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
  
}