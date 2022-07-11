import time

import paho.mqtt.client as MQTT

msg_count = 90
broker = "mqtt.eclipseprojects.io"
port = 1883
topic = "ocean23"
# generate client ID with pub prefix randomly
client_id = 'opecvOcean'
topicName   =   "ocean24"
# username = 'emqx'
# password = 'public'
global starus_braço
starus_braço = "OFF"

def connect_mqtt():
    def on_connect(client, userdata, flags, rc):
        if rc == 0:
            print("Connected to MQTT Broker!")
            result = client.subscribe(topicName)  # getting the Tuple from the call back
        else:
            print("Failed to connect, return code %d\n", rc)

    client = MQTT.Client(client_id)
    #client.username_pw_set(username, password)
    client.on_connect = on_connect
    client.connect(broker, port)
    client.on_message   =   on_message
    
    return client


def publish(client,toppub,msg):
    #msg_count = 180
    #while True:
        #time.sleep(10)
        #msg = f"counter: {msg_count}"
    result = client.publish(toppub, msg)
    time.sleep(0.001)
    #print(msg)
        # result: [0, 1]
        #status = result[0]
        #if status == 0:
        #    print(f"Send `{msg}` to topic `{topic}`")
        #else:
        #    print(f"Failed to send message to topic {topic}")                    clienteMqtt.loop_forever()

        #msg_count += 1

def on_message(pvtClient, userdata, msg):
    global starus_braço

            # here we are extracting details from the msg parameter,
            #print("\n============================================")
            #print("Payload       : " +str(msg.payload.decode()))
            #print("Qos of message: "+str(msg.qos))
            #print("Message Topic : "+str(msg.topic))
            #print("Message retain: "+ str(msg.retain))
            #print("============================================\n")
    if(msg.payload.decode() == "exit(0)" ):
         client.disconnect()
    starus_braço = str(msg.payload.decode())
    

        
        


#def run():
    #msg_count = 180


    #publish(client)
 #   print(msg_count)


#if __name__ == '__main__':
#    run()