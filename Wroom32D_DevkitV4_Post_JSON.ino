//ENCODER ROTARY
#include "AiEsp32RotaryEncoder.h"
//REDE WIFI E REQUEST
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiMulti.h>

//ENCODER ROTARY
#define ROTARY_CLK_PIN 34
#define ROTARY_DT_PIN 35
#define ROTARY_SW_PIN 32
#define ROTARY_VCC_PIN -1
#define ROTARY_STEPS 4

//Instanciando objeto encoder
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_CLK_PIN, ROTARY_DT_PIN, ROTARY_SW_PIN, ROTARY_VCC_PIN, ROTARY_STEPS);

//ID DISPOSITIVO
int idDispositivo = 1;

//REDE WIFI
const char *AP_SSID = "IFSC";
const char *AP_PWD = "campuschapeco";

//Instanciando objeto wifi
WiFiMulti wifiMulti;

//Instanciando objeto HTTP
HTTPClient http;

//Funcao que envia json para servidor quando encoder detecta movimento
void postDataToServer(int encoder, float corrente, float tensao) {
  // Block until we are able to connect to the WiFi access point
  if (wifiMulti.run() == WL_CONNECTED) {
    http.begin("http://marcosvirgilio.dev.br/toucan/cadLeitura.php");
    http.addHeader("Content-Type", "application/json");

    StaticJsonDocument<200> doc;
    //idDispositivo é fixo por placa
    doc["idDispositivo"] = idDispositivo;
    // Adicionar valores lidos
    doc["vlEncoder"] = encoder;
    doc["vlCorrente"] = corrente;
    doc["vlTensao"] = tensao;

    String requestBody;
    serializeJson(doc, requestBody);

    Serial.println("Enviando JSON para o servidor...");
    Serial.println(requestBody);
    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {

      String response = http.getString();

      Serial.println(httpResponseCode);
      Serial.println(response);

    } else {

      Serial.printf("Error occurred while sending HTTP POST: %s\n", http.errorToString(httpResponseCode).c_str());
    }
  }
}


void rotary_onButtonClick() {
  Serial.print("Botão acionado");
}

void rotary_loop() {
  //impremi dados quando tem alteração
  if (rotaryEncoder.encoderChanged()) {
    Serial.print("Value: ");
    Serial.println(rotaryEncoder.readEncoder());
    //manda dados para o servidor
    //passando o valor do encoder, corrente e tensão fixos em zero por enquanto
    postDataToServer(rotaryEncoder.readEncoder(), 0,0);
  }
  if (rotaryEncoder.isEncoderButtonClicked()) {
    rotary_onButtonClick();
  }
}

void IRAM_ATTR readEncoderISR() {
  rotaryEncoder.readEncoder_ISR();
}

void setup() {
  Serial.begin(115200);
  //MAC ADDRESS
  String macAddress = WiFi.macAddress();
  Serial.print("MAC Address : ");
  Serial.println(macAddress);
  delay(2000);
  //Cria Access Point rede
  wifiMulti.addAP(AP_SSID, AP_PWD);
  //ENCODER
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  //definindo limites da volta do encoder
  bool circleValues = true;
  //minimo, maximo, voltas true|false
  rotaryEncoder.setBoundaries(0, 40, circleValues);
  //Desativando aceleração
  rotaryEncoder.disableAcceleration();
  //Passando 0 desativa aceleração
  rotaryEncoder.setAcceleration(0);
}

void loop() {
  //função para ações do encoder
  rotary_loop();
  delay(50);
}
