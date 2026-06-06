#include <Arduino.h>
#include <EpsonIR.h>
#include <ArduinoJson.h>
#include "EpsonManager.h"
#include "DebugManager.h"

void tratarJsonProjetor(const String &mensagem, EpsonIR controleProjetor)
{
  JsonDocument doc;
  DeserializationError erro = deserializeJson(doc, mensagem);

  uint8_t projetor = INDICE_INVALIDO;
  uint8_t projetores = INDICE_INVALIDO;

  uint8_t indiceComando[] = {projetores, projetor};

  if (erro)
  {
    debugErro("Erro na estrutura JSON.");
    debugErro(erro.c_str());
    return;
  }

  if (doc["projetor"]["comando"].is<uint8_t>()) indiceComando[0] = doc["projetor"]["comando"].as<uint8_t>();
  if (doc["projetor_2"]["comando"].is<uint8_t>()) indiceComando[1] = doc["projetor_2"]["comando"].as<uint8_t>();

  for(size_t i = 0; i < 2; i ++)
  {
    if(indiceComando[i] != INDICE_INVALIDO)
    {
      debugInfo("Enviando comando: " + String(indiceComando[i]));
      enviarComandoProjetor(indiceComando[i], controleProjetor);
    }
  }
}

void enviarComandoProjetor(uint8_t indiceComando, EpsonIR controleProjetor)
{
    if (indiceComando > QUANTIDADE_COMANDOS - 1)
    {
        debugErro("Código inválido. Verifique o Json.");
        return;
    }

    uint32_t comando = COMANDOS_PROJETOR[indiceComando];
    controleProjetor.send(comando);
}