

#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;// create servo object to control a servo

String msg;

const char* ssid = "Ocean_Adm";
const char* password = "@artico2020!";

const char* topic_sub1 = "arm-ocean-1";
const char* topic_sub2 = "arm-ocean-2";
const char* topic_sub3 = "arm-ocean-3";
const char* topic_sub4 = "arm-ocean-4";
const char* topic_sub5 = "arm-ocean-5";
//const char* topic_pub = "laps-ocean-iot-1";
const char* mqtt_id = "esp-44";

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
    if(String(topic) == "arm-ocean-1"){
       myservo1.write(msg.toInt());
       Serial.println(msg);
       //delay(15);
    }
    else if(String(topic) == topic_sub2){
       myservo2.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) =="arm-ocean-3"){
       myservo3.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) =="arm-ocean-4"){
       myservo4.write(msg.toInt());
       Serial.println(msg);
    }
    else if(String(topic) =="arm-ocean-5"){
       myservo5.write(msg.toInt());
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

  myservo1.setPeriodHertz(50);    
  myservo1.attach(2); 
  myservo2.setPeriodHertz(50);    
  myservo2.attach(4); 
  myservo3.setPeriodHertz(50);    
  myservo3.attach(5); 
  myservo4.setPeriodHertz(50);    
  myservo4.attach(19);
  myservo5.setPeriodHertz(50);    
  myservo5.attach(21);
  initMQTT();
}

void loop() {


      VerificaConexaoMQTT();

    MQTT.loop();



}
