// * main.cpp

#include <Arduino.h>
#include <ArduinoJson.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "DebugManager.h"
#include <Bounce2.h>
#include <EpsonIR.h>
#include "EpsonManager.h"

constexpr uint16_t PINO_LED_EMISSOR = 5;
EpsonIR controleProjetor(PINO_LED_EMISSOR);

void tratarMensagemRecebida(const char *, const String &);

void setup()
{
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();
  controleProjetor.begin();
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
}

void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
  debugInfo("================================");
  debugInfo(" MENSAGEM RECEBIDA NA APLICAÇÃO ");
  debugInfo("================================");

  if (topico == nullptr)
  {
    debugErro("Tópico MQTT inválido.");
    return;
  }

  debugInfo("Tópico: " + String(topico));
  debugInfo("Mensagem: " + mensagem);

  if (strcmp(topico, TOPICO_COMANDO) == 0)
  {
    tratarJsonProjetor(mensagem, controleProjetor);
    return;
  }

  debugErro("Tópico não tratado: " + String(topico));
}