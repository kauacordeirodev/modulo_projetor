// * main.cpp

/*
Autor: Kauã Cordeiro
Programa: Conexão MQTT
Descrição: Conexão MQTT
Data: 30/04/2026
Versão: 1.2
*/

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AdaFruit_NeoPixel.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "DebugManager.h"

void tratarMensagemRecebida(const char *, const String &);
void configurarLedRGB();
void alterarCorLedRGB(int, int, int);
void tratarJsonComando(const String &);
void alterarEstadoLampada(const String &);

// constantes
const uint8_t PINO_LED_RGB = 48;
const uint8_t QUANTIDADE_LEDS = 1;
const uint8_t PINO_RELE_LAMPADA = 38;

const char TOPICO_COMANDO[] = "senai134/kauac/esp32/comando";

// instâncias
Adafruit_NeoPixel ledRGB(QUANTIDADE_LEDS, PINO_LED_RGB, NEO_GRB + NEO_KHZ800);

void setup()
{
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();

  pinMode(PINO_RELE_LAMPADA, OUTPUT);
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
    tratarJsonComando(mensagem);
    return;
  }

  debugErro("Tópico não tratado: " + String(topico));
}

void tratarJsonComando(const String &mensagem)
{
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro na estrutura JSON.");
    debugErro(erro.c_str());
    return;
  }

  if (!doc["lampada"].is<bool>())
  {
    debugInfo("Não encontrado o comando para a lâmpada.");
  }
  else
  {
    alterarEstadoLampada(mensagem);
  }

  if (!doc["led"].is<JsonObject>())
  {
    debugInfo("Não encontrado o comando para o LED RGB.");
  }

  else
  {
    if (!doc["led"]["r"].is<int>() || !doc["led"]["g"].is<int>() || !doc["led"]["b"].is<int>())
    {
      debugErro("Json inválido. Use led.r, led.g, led.b.");
    }

    else
    {
      int vermelho = doc["led"]["r"].as<int>();
      int verde = doc["led"]["g"].as<int>();
      int azul = doc["led"]["b"].as<int>();

      alterarCorLedRGB(vermelho, verde, azul);
    }
  }
}

void configurarLedRGB()
{
  ledRGB.begin();
  ledRGB.setBrightness(80);
  ledRGB.clear();
  ledRGB.show();

  debugInfo("LED RGB configurado no pino " + String(PINO_LED_RGB));
}

void alterarCorLedRGB(int vermelho, int verde, int azul)
{
  vermelho = constrain(vermelho, 0, 255);
  verde = constrain(verde, 0, 255);
  azul = constrain(azul, 0, 255);

  ledRGB.setPixelColor(0, ledRGB.Color(vermelho, verde, azul));
  ledRGB.show();

  debugInfo("Cor aplicada no LED RGB");
  debugInfo("R: " + String(vermelho));
  debugInfo("G: " + String(verde));
  debugInfo("B: " + String(azul));
}

void alterarEstadoLampada(const String &mensagem)
{
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);

  if (erro)
  {
    debugErro("Erro na estrutura JSON.");
    return;
  }

  bool estadoReleLampada = doc["lampada"].as<bool>();
  digitalWrite(PINO_RELE_LAMPADA, estadoReleLampada);
}