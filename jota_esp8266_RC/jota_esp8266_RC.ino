/*
 * WebSocket Server Remote Control
 *
 *  Created on: 15.04.2016
 *
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/// VAMOS A CREAR UN PUNTO DE ACCESO 
/// Luego conectamos a http://192.168.4.1 con un navegador
const char *ssid = "J_ESP_RC";
const char *password = "";

#define adelante     12
#define atras        13
#define izquierda    14
#define derecha      16
#define USE_SERIAL   Serial

ESP8266WebServer server    = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void loop() {
    webSocket.loop();
    server.handleClient();
}

void m_adelante()
 {
   delay(10);
   USE_SERIAL.println("adelante");
   digitalWrite(adelante, HIGH);
   digitalWrite(atras,    LOW);
 }

 void m_atras()
 {
   delay(10);
   USE_SERIAL.println("atras");
   digitalWrite(adelante, LOW);
   digitalWrite(atras,    HIGH);
 }

 void m_izquierda()
 {
   delay(10);
   USE_SERIAL.println("izquieda");
   digitalWrite(izquierda, HIGH);
   digitalWrite(derecha,   LOW);
 }
 
 void m_derecha()
 {
   delay(10);
   USE_SERIAL.println("derecha");
   digitalWrite(izquierda, LOW);
   digitalWrite(derecha,   HIGH);
 }

void m_Stop()
 {
   delay(10);
   USE_SERIAL.println("STOP");
   digitalWrite(adelante,  LOW);
   digitalWrite(atras,     LOW);
   digitalWrite(izquierda, LOW);
   digitalWrite(derecha,   LOW);
 }

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
            // send message to client
            webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT: {
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

            //if(payload[0] == '#') {
                int comando = int (payload[1])-int('0');
            //     USE_SERIAL.println(payload[1]);
            //     USE_SERIAL.println( comando);
                switch(comando){
                   case 1: m_adelante();
                           break;
                   case 2: m_atras();
                           break;
                   case 3: m_izquierda();
                           break;
                   case 4: m_derecha();
                           break;
                   case 0: m_Stop();
                           break;                 
                } //switch comando
            //}    // if payload
           }      //case wstype_text
           break;
    } //switch type
}     //webSocketEvent

void setup() {
    USE_SERIAL.begin(115200);
    //USE_SERIAL.setDebugOutput(true);
    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.print("Iniciando el AP ...");
 
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    USE_SERIAL.print  (" IP del AP:");
    USE_SERIAL.println(myIP);

    pinMode(adelante,  OUTPUT);
    pinMode(atras,     OUTPUT);
    pinMode(izquierda, OUTPUT);
    pinMode(derecha,   OUTPUT);
    digitalWrite(adelante,  LOW);
    digitalWrite(atras,     LOW);
    digitalWrite(izquierda, LOW);
    digitalWrite(derecha,   LOW);

    USE_SERIAL.println("Iniciando el Servdor web");
    server.on("/", []() {
        // send index.html
        server.send(200, "text/html", "<!DOCTYPE HTML><html><head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0'><style>* {-webkit-user-select: none;}</style><title>RC</title><script> var connection; window.addEventListener('load', function () { connection = new WebSocket('ws://192.168.4.1:81');connection.onopen = function () { console.log('Connection opened'); }; connection.onclose = function () { console.log('Connection closed'); }; connection.onerror = function () { console.error('Connection error'); }; connection.onmessage = function (event) { var div = document.getElementById('salida'); div.textContent = event.data; }; document.getElementById('up').addEventListener('touchstart', function () { connection.send('#1');}, false); document.getElementById('up').addEventListener('touchend', function () { connection.send('#0');}, false); document.getElementById('down').addEventListener('touchstart', function () { connection.send('#2');}, false); document.getElementById('down').addEventListener('touchend', function () { connection.send('#0');}, false); document.getElementById('left').addEventListener('touchstart', function () { connection.send('#3');}, false); document.getElementById('left').addEventListener('touchend', function () { connection.send('#0');}, false); document.getElementById('right').addEventListener('touchstart', function () { connection.send('#4');}, false); document.getElementById('right').addEventListener('touchend', function () { connection.send('#0');}, false); document.getElementById('center').addEventListener('touchstart', function () { connection.send('#0');}, false); document.getElementById('center').addEventListener('touchend', function () { connection.send('#0');}, false); });</script></head><body><div id='joystick'><svg height='360' width='360'><path id='up' d='M0,0 360,0 180,180Z' fill='rgba(0,0,0,0.5)' /><path id='left' d='M0,0 180,180 0,360Z' fill='rgba(0,0,0,0.4)' /><path id='right' d='M360,0 360,360 180,180Z' fill='rgba(0,0,0,0.3)' /><path id='down' d='M0,360 180,180 360,360Z' fill='rgba(0,0,0,0.2)' /><circle id='center' cx='180' cy='180' r='50' fill='#cccccc'/></svg></div><div id='salida'>Aqui se muestran los movimientos...</div></body></html>");
    });
    
    server.begin();   

    USE_SERIAL.println("Iniciando el Servdor de websockets");
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);
}


