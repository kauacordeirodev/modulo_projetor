#ifndef EPSON_MANAGER.H
#define EPSON_MANAGER.H

#include <Arduino.h>
#include <EpsonIR.h>

inline constexpr uint8_t INDICE_INVALIDO = 100;
inline constexpr char TOPICO_COMANDO[] = {"senai134/equipe/mario/devices/projetor"};

inline constexpr uint32_t COMANDOS_PROJETOR[] = {
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

inline constexpr uint8_t QUANTIDADE_COMANDOS = sizeof(COMANDOS_PROJETOR) / sizeof(COMANDOS_PROJETOR[0]);

void enviarComandoProjetor(uint8_t indiceComando, EpsonIR controleProjetor);
void tratarJsonProjetor(const String &mensagem, EpsonIR controleProjetor);

#endif