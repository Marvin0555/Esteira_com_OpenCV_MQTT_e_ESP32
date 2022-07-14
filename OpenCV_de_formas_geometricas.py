import cv2
import numpy as np
import time
import MQTT_communication.PUB_SUB_MQTT as pubmqtt
import paho.mqtt.client 


cap = cv2.VideoCapture(1)
cap.set(cv2.CAP_PROP_FRAME_WIDTH,1280)
cap.set(cv2.CAP_PROP_FRAME_HEIGHT,720)
clienteMqtt = pubmqtt.connect_mqtt()
clienteMqtt.loop_start()
Starus_esteria = "OFF"
variavel = "e"


while True:

    _, frame = cap.read()
    mask = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    height, width, dd = frame.shape
    cx = int(width / 2)
    cy = int(height / 2)
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    cv2.circle(frame, (cx, cy), 5, (255, 0, 0), 3)


    # Red color
    low_red = np.array([161, 155, 84])
    high_red = np.array([179, 255, 255])
    red_mask = cv2.inRange(mask, low_red, high_red)

    # Blue color
    low_blue = np.array([97, 175, 74])
    high_blue = np.array([117, 254, 190])
    blue_mask = cv2.inRange(mask, low_blue, high_blue)


    # Green color
    #low_green = np.array([25, 52, 72])
    #high_green = np.array([102, 255, 255])
    low_green = np.array([70, 67, 175])
    high_green = np.array([97, 165, 255])
    green_mask = cv2.inRange(mask, low_green, high_green)
    _,green_mask = cv2.threshold(green_mask, 250, 255, cv2.THRESH_BINARY)
    #green_mask = cv2.dilate(green_mask, None, iterations=1)
    #green_mask = cv2.erode(green_mask, None, iterations=1)


    erodeElement = cv2.getStructuringElement(cv2.MORPH_RECT,(3,3))
    #dilateElement = cv2.getStructuringElement(cv2.MORPH_RECT,(8,8))

    #erosão de rgb
    green_mask = cv2.erode(green_mask,erodeElement)
    #blue_mask = cv2.erode(blue_mask,erodeElement)
    #red_mask = cv2.erode(red_mask,erodeElement)
    #dilatação de rgb
    #green_mask = cv2.dilate(green_mask,dilateElement)
    #blue_mask = cv2.dilate(blue_mask,dilateElement)
    #red_mask = cv2.erode(red_mask,dilateElement)

    green_contours, _ = cv2.findContours(green_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    blue_contours, _ = cv2.findContours(blue_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    red_contours, _ = cv2.findContours(red_mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    
    for cnt in green_contours :
        (x, y, w, h) = cv2.boundingRect(cnt)

        area = cv2.contourArea(cnt)
        if area > 400:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
            #cv2.putText(belt, str(area), (x, y), 1, 1, (0,255,0))
            moment = cv2.moments(cnt)
            area = moment['m00']
            x = int(moment['m10']/area)
            y = int(moment['m01']/area)
            cv2.circle(frame, (x , y), 10, (0,0,255), -1)
            cv2.putText(frame,str(x)+","+str(y),(x,y+30),2,1,(0,255,255),2)
            erro = 640 - x

            if erro < 0:
                cv2.putText(frame,"Passou2 !!!!",(0,50),2,1,(255,0,0),2)
                approx = cv2.approxPolyDP(cnt, 0.02* cv2.arcLength(cnt, True), True)
                cv2.drawContours(frame, [approx], 0, (0, 0, 0), 5)
                if pubmqtt.starus_braço != "ON":
                    if len(approx) == 3:
                        cv2.putText(frame, "Triangulo", (400, 40), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 255, 0),2)
                        pubmqtt.publish(clienteMqtt,"oceanTriangulo","triangulo") 
                    elif len(approx) == 4:
                        cv2.putText(frame, "Quadrado", (400, 40), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 255, 0),2)
                        pubmqtt.publish(clienteMqtt,"oceanQuadrado","quadrado") 
                    elif  7 < len(approx)  < 20:
                        cv2.putText(frame, "circulo", (400, 40), cv2.FONT_HERSHEY_COMPLEX, 2, (0, 255, 0),2)
                        pubmqtt.publish(clienteMqtt,"oceanCirculo","circulo")
                    pubmqtt.starus_braço = "ON"

                                
  
        

    if variavel != pubmqtt.starus_braço:
        variavel = pubmqtt.starus_braço
        if pubmqtt.starus_braço == "OFF":
            Starus_esteria = "ON"
            pubmqtt.publish(clienteMqtt,"oceanEsteira",Starus_esteria) #braço
            print("esteira movendo")
        else:
            Starus_esteria = "OFF"
            pubmqtt.publish(clienteMqtt,"oceanEsteira",Starus_esteria) #braço
            print("esteira parada")

    #cv2.imshow("Frame", frame)
    #cv2.imshow("Red", red)
    #cv2.imshow("Blue", blue)
    cv2.imshow("Green", frame)
    cv2.imshow("Grween", green_mask)


    #cv2.imshow("Result", result)

    key = cv2.waitKey(1)
    if key == 27:
        break