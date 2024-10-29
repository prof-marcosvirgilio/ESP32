#include "AiEsp32RotaryEncoder.h"

#define ROTARY_CLK_PIN 34
#define ROTARY_DT_PIN 35
#define ROTARY_SW_PIN 32
#define ROTARY_VCC_PIN -1
#define ROTARY_STEPS 4

//Instanciando objeto
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_CLK_PIN, ROTARY_DT_PIN, ROTARY_SW_PIN, ROTARY_VCC_PIN, ROTARY_STEPS);

void rotary_onButtonClick()
{
    Serial.print("Botão acionado");
  
}

void rotary_loop()
{
    //impremi dados quando tem alteração
    if (rotaryEncoder.encoderChanged())
    {
            Serial.print("Value: ");
            Serial.println(rotaryEncoder.readEncoder());
    }
    if (rotaryEncoder.isEncoderButtonClicked())
    {
            rotary_onButtonClick();
    }
}

void IRAM_ATTR readEncoderISR()
{
    rotaryEncoder.readEncoder_ISR();
}

void setup()
{
    Serial.begin(115200);
    //inicializando encoder
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

void loop()
{
    //função para ações do encoder
    rotary_loop();
    delay(50); 
}


