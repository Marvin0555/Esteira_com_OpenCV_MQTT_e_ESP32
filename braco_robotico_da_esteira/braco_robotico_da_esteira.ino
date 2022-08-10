#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>

Servo eixo1;
Servo eixo2;
Servo eixo3;
Servo eixo4;
Servo eixo5;
Servo eixo6; // create servo object to control a servo

String msg;

const char* ssid = "Ocean_Adm";
const char* password = "@artico2020!";

const char* topic_sub1 = "eixo1-ocean";
const char* topic_sub2 = "eixo2-ocean";
const char* topic_sub3 = "eixo3-ocean";
const char* topic_sub4 = "eixo4-ocean";
const char* topic_sub5 = "eixo5-ocean";
const char* topic_sub6 = "eixo6-ocean";

//const char* topic_pub = "laps-ocean-iot-1";
const char* mqtt_id = "esp32-ocean";

const char* BROKER_MQTT = "mqtt.eclipseprojects.io"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT

WiFiClient espClient;
PubSubClient MQTT(espClient);

void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void VerificaConexaoMQTT(void);


void initMQTT(void)
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
    String msg;
    /* obtem a string do payload recebido */
    for(int i = 0; i < length; i++)
    {
       char c = (char)payload[i];
       msg += c;
    }
    Serial.println("Chegou a seguinte string via MQTT: ");
    Serial.println(topic);
    if(String(topic) == topic_sub1){
       eixo1.write(msg.toInt());
       Serial.println(msg);
       //delay(15);
    }
    else if(String(topic) == topic_sub2){
       eixo2.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) == topic_sub3){
       eixo3.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) == topic_sub4){
       eixo4.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) == topic_sub5){
       eixo5.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) == topic_sub6){
       eixo6.write(msg.toInt());
       Serial.println(msg);
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
            MQTT.subscribe(topic_sub5);
            MQTT.subscribe(topic_sub6);
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
 

void setup() {

  Serial.begin(115200);
   WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  Serial.println("WiFi Connected....IP Address:");
  Serial.println(WiFi.localIP());
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  eixo1.setPeriodHertz(200);    
  eixo1.attach(2); 
  eixo2.setPeriodHertz(200);    
  eixo2.attach(4, 1000, 2000); 
  eixo3.setPeriodHertz(200);    
  eixo3.attach(5, 1000, 2000); 
  eixo4.setPeriodHertz(50);    
  eixo4.attach(18, 1000, 2000);
  eixo5.setPeriodHertz(50);    
  eixo5.attach(19, 1000, 2000);
  eixo6.setPeriodHertz(50);    
  eixo6.attach(21, 1000, 2000);
  initMQTT();
}

void loop() {
    VerificaConexaoMQTT();
    MQTT.loop();
}
