// * main.cpp

#include <Arduino.h>
#include <ArduinoJson.h>
#include <EpsonIR.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "DebugManager.h"
#include "EpsonManager.h"

// Configuração do LED IR
constexpr uint16_t PINO_LED_EMISSOR = 5;
EpsonIR controleProjetor(PINO_LED_EMISSOR);

// Prototipação das funções
void tratarMensagemRecebida(const char *, const String &);
void enviarHandshakeProjetor();

void setup()
{
  // Configuração de WiFI, MQTT e monitor serial
  configurarDebug();
  conectarWiFi();
  configurarMQTT();
  registrarCallbackMensagem(tratarMensagemRecebida);
  conectarMQTT();

  // Inicialização do objeto do projetor
  controleProjetor.begin();
}

void loop()
{
  garantirWiFiConectado();
  garantirMQTTConectado();
  loopMQTT();
  enviarHandshakeProjetor();
}

// Função que trata a mensagem recebida no tópico de comando do broker
void tratarMensagemRecebida(const char *topico, const String &mensagem)
{
  debugInfo("================================");
  debugInfo(" MENSAGEM RECEBIDA NA APLICAÇÃO ");
  debugInfo("================================");

  // O programa sai da função caso o tópico esteja vazio
  if (topico == nullptr)
  {
    debugErro("Tópico MQTT inválido.");
    return;
  }

  debugInfo("Tópico: " + String(topico));
  debugInfo("Mensagem: " + mensagem);

  // Se o tópico recebido for igual ao tópico de comando, o programa envia o comando para a função tratarJsonProjetor()
  if (strcmp(topico, TOPICO_COMANDO) == 0)
  {
    tratarJsonProjetor(mensagem, controleProjetor);
    return;
  }

  debugErro("Tópico não tratado: " + String(topico));
}

// Envia a confirmação do envio do comando
void enviarHandshakeProjetor()
{
  time_t timestamp = time(nullptr);
  if (!obterStatusHandshake()) return;

  if (obterStatusHandshake())
  {
    JsonDocument doc;
    String mensagem;

    doc["statusComando"]["comando"] = indiceComandoEnviado;
    doc["statusComando"]["timestamp"] = timestamp;

    serializeJson(doc, mensagem);
    publicarMensagemNoTopico(0, mensagem.c_str());
  }

  statusHandshakeProjetor = false;
}