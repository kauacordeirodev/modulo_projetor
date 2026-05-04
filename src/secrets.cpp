#include "secrets.h"

// Configurações do WiFi

const char *WIFI_SSID = "SALA 09";
const char *WIFI_SENHA = "info@134";

//============================ MQTT ============================//

const bool USAR_AWS_IOT = false;

const char* MQTT_BROKER = "broker.hivemq.com";
const int MQTT_PORTA = 1883;

const char* MQTT_CLIENT_ID = "esp32_kaua_cordeiro";

const char* MQTT_USUARIO = "";
const char* MQTT_SENHA = "";

const bool MQTT_TLS = false;

const char* MQTT_CERTIFICADO_CA = "";

const char* TOPICOS_PUBLICAR[] = {"senai134/kauac/esp32/status", "senai134/kauac/esp32/log", "senai134/kauac/esp32/resposta"};
const int TOTAL_TOPICOS_PUBLICAR = 3;

const char* TOPICOS_RECEBER[] = {"senai134/kauac/esp32/comando", "senai134/kauac/esp32/config", "senai134/kauac/esp32/display"};
const int TOTAL_TOPICOS_RECEBER = 3;

//============================ DEBUG ============================//

// 0 = Sem mensagens
// 1 = Apenas erros
// 2 = Todas as mensagens
const int DEBUG_NIVEL_INICIAL = 2;

// Pino usado para forçar todas as mensagens
const int PINO_HABILITA_DEBUG_COMPLETO = 4;