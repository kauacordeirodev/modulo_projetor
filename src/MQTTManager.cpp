// * MQTTManager.cpp

#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <Arduino.h>

#include "secrets.h"
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "DebugManager.h"

WiFiClient wifiClient;
WiFiClientSecure wifiClientSecure;

PubSubClient mqttClient;

CallbackMensagemMQTT callbackDaAplicacao = nullptr;

void registrarCallbackMensagem(CallbackMensagemMQTT callback)
{
    callbackDaAplicacao = callback;

    if (callbackDaAplicacao != nullptr)
    {
        debugInfo("Callback da aplicação registrado com sucesso.");
    }
    else
    {
        debugErro("Callback da aplicação não foi registrado.");
    }
}

const char *ObterTopicoPublicacao(int indiceTopico)
{
    if (indiceTopico < 0 || indiceTopico >= TOTAL_TOPICOS_PUBLICAR)
    {
        debugErro("Índice invalido para tópico de publicação " + String(indiceTopico));
        return "";
    }
    return TOPICOS_PUBLICAR[indiceTopico];
}

const char *obterTopicoRecebimento(int indiceTopico)
{
    if (indiceTopico < 0 || indiceTopico >= TOTAL_TOPICOS_RECEBER)
    {
        debugErro("Índice invalido para tópico de publicação " + String(indiceTopico));
        return "";
    }
    return TOPICOS_RECEBER[indiceTopico];
}

void callbackInternoMQTT(char *topico, byte *payload, uint64_t tamanho)
{
    String mensagem = "";

    for (uint8_t i = 0; i < tamanho; i++)
    {
        mensagem += (char)payload[i];
    }

    debugInfoSemLinha("========================\n\r");
    debugInfoSemLinha(" MENSAGEM MQTT RECEBIDA \n\r");
    debugInfoSemLinha("========================\n\r");
    debugInfoSemLinha("TÓPICO: " + String(topico) + "\n\r");
    debugInfo("MENSAGEM: " + mensagem);

    if (callbackDaAplicacao != nullptr)
        callbackDaAplicacao(topico, mensagem);
    //\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\ 
    else debugErro("Mensagem recebida, mas nenhum callback da aplicação foi registrado.");
}

void configurarMQTT()
{
    debugInfoSemLinha("======================\n\r");
    debugInfoSemLinha(" CONFIGURANDO MQTT... \n\r");
    debugInfoSemLinha("======================\n\r");

    if (USAR_AWS_IOT)
    {
        // TODO: Implementar código para usar broker IOT Core da AWS
    }
    else if (MQTT_TLS)
    {
        // TODO: Implementar código para conectar ao broker com certificado TLS
    }
    else // Conectar ao broker público sem certificado
    {
        debugInfo(" MODO SELECIONADO: MQTT SEM TLS.");

        mqttClient.setClient(wifiClient);
        mqttClient.setServer(MQTT_BROKER, MQTT_PORTA);

        debugInfo(" Broker MQTT: " + String(MQTT_BROKER));
        debugInfo(" Porta MQTT: "  + String(MQTT_PORTA));
    }

    mqttClient.setCallback(callbackInternoMQTT);
    debugInfo("Callback interno no MQTT configurado.");
}