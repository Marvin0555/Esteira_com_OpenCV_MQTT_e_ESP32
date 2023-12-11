#include <Wire.h>
#include <PubSubClient.h> //Biblioteca para utilizanção do MQTT
#include <ESP8266WiFi.h> //Biblioteca do ESP8266 wifi

const char* ssid = "Fablab"; //Nome da Rede WIFI que está usando
const char* password = "@fablab2020!"; // Senha da Rede Wifi que está usando

const char* topic_sub1 = "ocean-esp01"; //Topico onde o esp é um assinant
const char* topic_sub2 = "ocean-esp02"; //Topico onde o esp é um assinant
const char* topic_sub3 = "ocean-esp03"; //Topico onde o esp é um assinant
const char* topic_sub4 = "ocean-esp04"; //Topico onde o esp é um assinant
const char* topic_sub5 = "ocean-esp05"; //Topico onde o esp é um assinant


const char* mqtt_id = "esp8266-Ocean";//ID do usuario, deve ser unico para cada esp8266

const char* BROKER_MQTT = "mqtt.eclipseprojects.io"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; //Porta do Broker MQTT

WiFiClient espClient; //Cria o objeto espClient do tipo WiFiclient
PubSubClient MQTT(espClient); //Instancia o cliente Mqtt passando o objeto espClient

void initMQTT(void); //Função responsavel por iniciar a comunicação com o broker mqtt
void mqtt_callback(char* topic, byte* payload, unsigned int length); //Função responsavel receber os dados do broker mqtt
void reconnectMQTT(void); //Função responsavel por realizar a conexão com o broker mqtt
void VerificaConexaoMQTT(void); //Função responsavel por verificar a conexão com o broker mq
void setup()
{
  Serial.begin(115200); // inicia a serial
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { //Verifica se a conexão com a rede foi realizada e só vai sair do laço quando a conexão foi realizada
    delay(500);
    Serial.print(".");
   }
  Serial.println("WiFi Connected .... IP Address:"); //IP da rede
  Serial.println(WiFi.localIP());
  
  Wire.begin();
  
  initMQTT(); // Chama a função initMQTT
}

void loop()
{
  VerificaConexaoMQTT(); //Chama a função VerificaConexaoMQTT
  MQTT.loop(); //Necessario para ficar em contato comunicação com o broker

}

void initMQTT(void)
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    String mensagem; // Obtem a string do payload recebido 
    for(int i = 0; i < length; i++) // laço de repetição responsável por converter a mensagem
    {
       char c = (char)payload[i];
       mensagem += c;
    }
    int valorMqtt = mensagem.toInt();
    
    Serial.println("Topico da mensagem");
    Serial.println(topic); //Topico da mensagem do Broker
    Serial.println("Mensagem recebida do broker");

    if(String(topic) == "ocean-esp01"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      Wire.beginTransmission(0x0A);
      Wire.write("servo1");  
      Wire.write(valorMqtt);           
      Wire.endTransmission();  
      delay(1000);
    }
    if(String(topic) == "ocean-esp02"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      Wire.beginTransmission(0x0A);
      Wire.write("servo2");  
      Wire.write(valorMqtt);           
      Wire.endTransmission();  
      delay(1000);
    }
    if(String(topic) == "ocean-esp03"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      Wire.beginTransmission(0x0A);
      Wire.write("servo3");  
      Wire.write(valorMqtt);           
      Wire.endTransmission();  
      delay(1000);
    }
    if(String(topic) == "ocean-esp04"){ //Condição que verifica qual é o tópico e passa a mensagem para seu correspondente
      Wire.beginTransmission(0x0A);
      Wire.write("servo4");  
      Wire.write(valorMqtt);           
      Wire.endTransmission();  
      delay(1000);
    }
}

void reconnectMQTT(void)
{
    while (!MQTT.connected())
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(mqtt_id))
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            MQTT.subscribe(topic_sub1);
            MQTT.subscribe(topic_sub2);
            MQTT.subscribe(topic_sub3);
            MQTT.subscribe(topic_sub4);
 
        }
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

void VerificaConexaoMQTT(void)
{
  if (!MQTT.connected())
  reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
  

}
