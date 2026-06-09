# ModuloProjetorESP32

Biblioteca/aplicação Arduino ESP32 para controle remoto de projetores Epson via **MQTT + Infravermelho**, permitindo integração com sistemas de automação, dashboards e controladores centralizados.

> Autor: [kauacordeirodev](https://github.com/kauacordeirodev) 
>
> Depende de:
>
> * EpsonIR
> * PubSubClient
> * ArduinoJson

---

## Visão geral

O módulo conecta-se a uma rede Wi-Fi e a um broker MQTT.

Ao receber mensagens JSON em um tópico MQTT configurado, converte os comandos recebidos em sinais infravermelhos compatíveis com projetores Epson.

Fluxo básico:

```text
Sistema de Automação
          │
          ▼
      MQTT Broker
          │
          ▼
        ESP32
          │
          ▼
    Biblioteca EpsonIR
          │
          ▼
   Projetor Epson
```

---

## Funcionalidades

* ✅ Controle de projetores Epson por IR.
* ✅ Integração MQTT.
* ✅ Recebimento de comandos via JSON.
* ✅ Reconexão automática de Wi-Fi.
* ✅ Reconexão automática de MQTT.
* ✅ Compatível com MQTT simples, TLS e AWS IoT Core.
* ✅ Estrutura modularizada.
* ✅ Sistema de logs para diagnóstico.

---

## Instalação

### PlatformIO

```ini
lib_deps =
    bblanchon/ArduinoJson
    knolleary/PubSubClient
```

Além das dependências:

* EpsonIR
* WiFi (ESP32)

---

## Estrutura do projeto

```text
src/
├── main.cpp
├── WiFiManager.cpp
├── MQTTManager.cpp
├── EpsonManager.cpp
├── DebugManager.cpp

include/
├── WiFiManager.h
├── MQTTManager.h
├── EpsonManager.h
├── DebugManager.h
├── secrets.h
```

---

## Configuração

As credenciais e parâmetros são definidos em:

```cpp
secrets.h
```

### Wi-Fi

```cpp
const char WIFI_SSID[]  = "MinhaRede";
const char WIFI_SENHA[] = "MinhaSenha";
```

### MQTT

```cpp
const char MQTT_BROKER[] = "broker.exemplo.com";
const uint16_t MQTT_PORTA = 1883;

const char MQTT_CLIENT_ID[] = "esp32_projetor";
```

### Tópicos

```cpp
senai134/shared/projeto/projetor
```

---

## Inicialização

O sistema é iniciado em:

```cpp
void setup()
{
    configurarDebug();

    conectarWiFi();

    configurarMQTT();

    registrarCallbackMensagem(
        tratarMensagemRecebida
    );

    conectarMQTT();

    controleProjetor.begin();
}
```

Loop principal:

```cpp
void loop()
{
    garantirWiFiConectado();

    garantirMQTTConectado();

    loopMQTT();
}
```

---

## Formato das mensagens MQTT

### Comando para todos os projetores

```json
{
    "projetor": {
        "comando": 0
    }
}
```

### Comando para projetor específico

```json
{
    "projetor_1": {
        "comando": 15
    }
}
```

---

## Tópico de comando

```cpp
senai134/shared/projeto/projetor
```

Toda mensagem recebida nesse tópico é processada por:

```cpp
tratarJsonProjetor()
```

---

## Comandos suportados

| Índice | Comando       |
| ------ | ------------- |
| 0      | Power         |
| 1      | Freeze        |
| 2      | AV/Mute       |
| 3      | ESC           |
| 4      | Enter         |
| 5      | Up            |
| 6      | Down          |
| 7      | Right         |
| 8      | Left          |
| 9      | Home          |
| 10     | Menu          |
| 11     | Volume +      |
| 12     | Volume -      |
| 13     | Zoom +        |
| 14     | Zoom -        |
| 15     | HDMI          |
| 16     | Computer      |
| 17     | USB           |
| 18     | LAN           |
| 19     | Source Search |
| 20     | Color Mode    |
| 21     | Aspect        |
| 22     | Split         |
| 23     | Tecla 0       |
| 24     | Tecla 1       |
| 25     | Tecla 2       |
| 26     | Tecla 3       |
| 27     | Tecla 4       |
| 28     | Tecla 5       |
| 29     | Tecla 6       |
| 30     | Tecla 7       |
| 31     | Tecla 8       |
| 32     | Tecla 9       |
| 33     | ID            |
| 34     | User          |
| 35     | Default       |

---

## Exemplo de publicação MQTT

### Ligar projetor

```json
{
    "projetor": {
        "comando": 0
    }
}
```

### Selecionar HDMI

```json
{
    "projetor": {
        "comando": 15
    }
}
```

### Abrir menu

```json
{
    "projetor": {
        "comando": 10
    }
}
```

---

## Sistema de MQTT

### Conexão

```cpp
conectarMQTT();
```

### Reconexão automática

```cpp
garantirMQTTConectado();
```

### Publicação

```cpp
publicarMensagem(
    "topico",
    "mensagem"
);
```

ou

```cpp
publicarMensagemNoTopico(
    indice,
    "mensagem"
);
```

---

## Sistema de Wi-Fi

### Conectar

```cpp
conectarWiFi();
```

### Verificar conexão

```cpp
WiFiEstaConectado();
```

### Reconexão automática

```cpp
garantirWiFiConectado();
```

---

## Logs

O sistema utiliza o módulo DebugManager.

Exemplo:

```text
[INFO] WiFi conectado com sucesso
[INFO] MQTT conectado com sucesso
[INFO] Mensagem MQTT recebida
```

Erros:

```text
[ERRO] Falha ao conectar ao WiFi
[ERRO] MQTT desconectado
[ERRO] Código inválido
```

---

## Fluxo de processamento

```text
MQTT
 │
 ▼
Mensagem JSON
 │
 ▼
ArduinoJson
 │
 ▼
Identificação do comando
 │
 ▼
Tabela COMANDOS_PROJETOR[]
 │
 ▼
EpsonIR.send()
 │
 ▼
Projetor Epson
```

---

## Exemplo completo

Publicação MQTT:

```json
{
    "projetor": {
        "comando": 15
    }
}
```

Processamento:

```cpp
tratarJsonProjetor(
    mensagem,
    controleProjetor
);
```

Resultado:

```cpp
controleProjetor.send(
    EPSON_CMD_HDMI
);
```

O projetor muda para a entrada HDMI.

---