#include <Arduino.h>
#include <EpsonIR.h>
#include <ArduinoJson.h>
#include "EpsonManager.h"
#include "DebugManager.h"

// Função que filtra o comando recebido e envia para a função enviarComandoProjetor()
void tratarJsonProjetor(const String &mensagem, EpsonIR &controleProjetor)
{
    JsonDocument doc;
    DeserializationError erro = deserializeJson(doc, mensagem);

    uint8_t comandoTodosProjetores = INDICE_INVALIDO;
    uint8_t comandoProjetor = INDICE_INVALIDO;

    uint8_t indiceComando[] = {comandoTodosProjetores, comandoProjetor};

    if (erro)
    {
        debugErro("Erro na estrutura JSON.");
        debugErro(erro.c_str());
        return;
    }

    if (doc["projetores"]["comando"].is<uint8_t>()) indiceComando[0] = doc["projetores"]["comando"].as<uint8_t>();
    if (doc["projetor_2"]["comando"].is<uint8_t>()) indiceComando[1] = doc["projetor_2"]["comando"].as<uint8_t>();

    for (size_t i = 0; i < 2; i++)
    {
        if (indiceComando[i] != INDICE_INVALIDO)
        {
            debugInfo("Enviando comando: " + String(indiceComando[i]));
            enviarComandoProjetor(indiceComando[i], controleProjetor);
        }
    }
}

// Função que envia o comando hexadecimal para o projetor
void enviarComandoProjetor(uint8_t indiceComando, EpsonIR &controleProjetor)
{
    statusHandshakeProjetor = false;
    if (indiceComando > QUANTIDADE_COMANDOS - 1)
    {
        debugErro("Código inválido. Verifique o Json.");
        return;
    }
    statusHandshakeProjetor = true;

    indiceComandoEnviado = indiceComando;
    uint32_t comando = COMANDOS_PROJETOR[indiceComando];
    controleProjetor.send(comando);
}

bool obterStatusHandshake()
{
    return statusHandshakeProjetor;
}