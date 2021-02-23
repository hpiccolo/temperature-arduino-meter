//Carregando biblioteca do sensor de temperatura
#include <Adafruit_MAX31865.h>

// Use software SPI: CS, DI, DO, CLK
//para Arduino usar as duas linhas de baixo
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(10, 11, 12, 13);
//Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(9, 11, 12, 13);

//para ESP usar as duas linhas de baixo
Adafruit_MAX31865 thermo = Adafruit_MAX31865(D8, D7, D6, D5);
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(D9, D7, D6, D5);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(10);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.51
#define RNOMINAL2  100.19
//Carregando biblioteca para LCD controlado por I2C

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Carregando biblioteca wifi
#include <ESP8266WiFi.h> 
// Alterar a linha anterior por #include <WiFi.h> se estiver usando ESP32
// #include <WiFiClientSecure.h> // Incluir esta biblioteca se estiver usando ESP32
WiFiClientSecure client;//Cria um cliente seguro (para ter acesso ao HTTPS)
String textFix = "GET /forms/d/e/1FAIpQLScPzzsfLaW3fcf5g-s3uNT9Kmq0fQcgAAmbEzAVTB94KFeTvQ/formResponse?ifq&";
//Essa String sera uma auxiliar contendo o link utilizado pelo GET, para nao precisar ficar re-escrevendo toda hora

int i =60;
void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando");
  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  thermo2.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary
  lcd.init();
  
  WiFi.mode(WIFI_STA);//Habilita o modo estaçao
  Serial.println("tentando conectar wifi");
  //colocar o SSID da rede e senha
  WiFi.begin("SSID", "senha");//Conecta na rede
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 // if connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  }

void loop() {
  uint16_t rtd = thermo.readRTD();
  uint16_t rtd2 = thermo2.readRTD();
  Serial.print("RTD value: "); Serial.println(rtd);
  Serial.print("RTD2 value: "); Serial.println(rtd);
  float ratio = rtd;
  ratio /= 32768;
  float ratio2 = rtd2;
  ratio2 /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio,8);
  Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
  Serial.print("Temperature = "); Serial.println(thermo.temperature(RNOMINAL, RREF));
  Serial.print("Resistance2 = "); Serial.println(RREF*ratio2,8);
  Serial.print("Temperature2 = "); Serial.println(thermo2.temperature(RNOMINAL, RREF));
  // Print a message to the LCD.
  lcd.backlight();
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Temp1=");
  lcd.setCursor(6,0);
  lcd.print(thermo.temperature(RNOMINAL, RREF));
  lcd.setCursor(0,1);
  lcd.print("Temp2=");
  lcd.setCursor(6,1);
  lcd.print(thermo2.temperature(RNOMINAL2, RREF));
  lcd.setCursor(12,0);
  lcd.print("WiFi");
  if (i==60)
    {
      i=0;
      client.setInsecure();
        if (client.connect("docs.google.com", 443) == 1)//Tenta se conectar ao servidor do Google docs na porta 443 (HTTPS)
        { 
          lcd.setCursor(13,1);
          lcd.print("Ok");
          String toSend = textFix;//Atribuimos a String auxiliar na nova String que sera enviada
          sheet1="entry.COLOCAR O NUMERO DA PLANILHA="
          toSend += sheet1;
          toSend += thermo.temperature(RNOMINAL, RREF);
          sheet2="entry.COLOCAR O NUMERO DA PLANILHA="
          toSend += sheet2;
          toSend += thermo2.temperature(RNOMINAL2, RREF);
          toSend += "&submit=Submit HTTP/1.1";//Completamos o metodo GET para nosso formulario.
          client.println(toSend);//Enviamos o GET ao servidor-
          client.println("Host: docs.google.com");//-
          client.println();//-
          client.stop();//Encerramos a conexao com o servidor
          Serial.println("Dados enviados.");//Mostra no monitor que foi enviado
        }
          else
        {
          lcd.setCursor(12,1);
          lcd.print("Erro");
          Serial.println("Erro ao se conectar");//Se nao for possivel conectar no servidor, ira avisar no monitor.
        }
    }
  else
  {
   i=i+1;
  }
  
  Serial.println();
  delay(1000);
}
