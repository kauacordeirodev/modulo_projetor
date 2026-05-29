// * main.cpp

#include <Arduino.h>
#include <ArduinoJson.h>
#include <AdaFruit_NeoPixel.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "DebugManager.h"
#include <Bounce2.h>
#include <EpsonIR.h>

constexpr uint16_t PINO_LED_EMISSOR = 5;
EpsonIR controleProjetor(PINO_LED_EMISSOR);

void tratarMensagemRecebida(const char *, const String &);
void tratarJsonComando(const String &);
void enviarComandoProjetor(uint8_t);

constexpr char TOPICO_COMANDO[] = {"senai134/esp32/comando"};

constexpr uint32_t COMANDOS_PROJETOR[] = {
    EPSON_CMD_POWER,
    EPSON_CMD_FREEZE,
    EPSON_CMD_MUTE,
    EPSON_CMD_ESC,

    EPSON_CMD_ENTER,
    EPSON_CMD_UP,
    EPSON_CMD_DOWN,
    EPSON_CMD_RIGHT,
    EPSON_CMD_LEFT,
    EPSON_CMD_HOME,
    EPSON_CMD_MENU,

    EPSON_CMD_VOL_UP,
    EPSON_CMD_VOL_DOWN,

    EPSON_CMD_ZOOM_IN,
    EPSON_CMD_ZOOM_OUT,

    EPSON_CMD_HDMI,
    EPSON_CMD_COMPUTER,
    EPSON_CMD_USB,
    EPSON_CMD_LAN,
    EPSON_CMD_SOURCE_SEARCH,

    EPSON_CMD_COLOR_MODE,
    EPSON_CMD_ASPECT,
    EPSON_CMD_SPLIT,

    EPSON_CMD_0,
    EPSON_CMD_1,
    EPSON_CMD_2,
    EPSON_CMD_3,
    EPSON_CMD_4,
    EPSON_CMD_5,
    EPSON_CMD_6,
    EPSON_CMD_7,
    EPSON_CMD_8,
    EPSON_CMD_9,

    EPSON_CMD_ID,
    EPSON_CMD_USER,
    EPSON_CMD_DEFAULT
};

constexpr uint8_t QUANTIDADE_COMANDOS = sizeof(COMANDOS_PROJETOR) / sizeof(COMANDOS_PROJETOR[0]);

enum indiceProjetor
{
  power,
  freeze,
  mute,
  esc,

  enter,
  up,
  down,
  right,
  left,
  home,
  menu,

  volumeUp,
  volumeDown,

  zoomIn,
  zoomDown,

  hdmi,
  computer,
  usb,
  lan,
  sourceSearch,

  colorMode,
  aspect,
  split,

  num0,
  num1,
  num2,
  num3,
  num4,
  num5,
  num6,
  num7,
  num8,
  num9,

  id,
  user,
  defaultValue
};

indiceProjetor obterIndiceComando(String);

void setup()
{
  Serial.begin(9600);
  Serial.println();

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
    tratarJsonComando(mensagem);
    return;
  }

  debugErro("Tópico não tratado: " + String(topico));
}

void tratarJsonComando(const String &mensagem)
{
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);
  static uint8_t indiceComando = 0;

  if (erro)
  {
    debugErro("Erro na estrutura JSON.");
    debugErro(erro.c_str());
    return;
  }

  if (doc["projetor"]["comando"].is<uint8_t>()) indiceComando = doc["projetor"]["comando"].as<uint8_t>();

  enviarComandoProjetor(indiceComando);
}

void enviarComandoProjetor(uint8_t indiceComando)
{
  uint32_t comando = COMANDOS_PROJETOR[indiceComando];

  for(size_t i = 0; i < QUANTIDADE_COMANDOS; i++)
  {
    if(comando == COMANDOS_PROJETOR[i])
    {
      controleProjetor.send((uint8_t)comando);
    }
  }
}