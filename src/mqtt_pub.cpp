#include "mqtt_pub.hpp"

#include "MQTTClient.h"
#include <system_error>
#include <sstream>
#include <iostream>
#include <string>


void delivered(void *context, MQTTClient_deliveryToken dt)
{
    printf("Message with token value %d delivery confirmed\n", dt);
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    int i;
    char* payloadptr;
    printf("Message arrived\n");
    printf("     topic: %s\n", topicName);
    printf("   message: ");
    payloadptr = static_cast<char*>(message->payload);
    for(i=0; i<message->payloadlen; i++)
    {
        putchar(*payloadptr++);
    }
    putchar('\n');
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

void connlost(void *context, char *cause)
{
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}


MQTTPublisher::MQTTPublisher()
{
  // just for testing reasons
  // serverUri  = "tcp://192.168.178.99:1883";
  ipServer       = "192.168.178.99";
  topicName      = "/streams/test";
  portServer     = 1883;
  protocolServer = "tcp";
  clientName     = "test_client";
  enabbledSSL    = false;
}

MQTTPublisher::MQTTPublisher(std::string ip_server,
                             unsigned int port_server,
                             std::string protocol_server,
                             std::string topic_name):
    ipServer(ip_server),
    portServer(portServer),
    topicName(topic_name),
    enabbledSSL(false)
{
  if(protocol_server.compare("tcp") == 0){
    protocolServer = "tcp";
  }
  else if(protocol_server.compare("ssl") == 0){
    protocolServer = "ssl";
    enabbledSSL    = true;
  }
  else
  {
    std::cout << "MQTTPuplisher(): protocoll no known, switch to tcp";
    protocolServer = "tcp";
  }
}
                               
                             

MQTTPublisher::~MQTTPublisher()
{
  MQTTClient_destroy(&client);
}

int MQTTPublisher::connect()
{
  // for in-memory persitence
  // gives possibility to save data localy, if connection is droped
  int persistence_type = MQTTCLIENT_PERSISTENCE_NONE;
  char* persistence_context = NULL;

  std::string server_uri = protocolServer + "://" + ipServer + ":" + std::to_string(portServer);
  std::cout << "MQTTPuplisher::connect: protocolServer: " << server_uri << "\n";;
  int rc = MQTTClient_create(&client,
                             server_uri.c_str(),
                             clientName.c_str(),
                             persistence_type,
                             (void*)persistence_context);
  if (rc != MQTTCLIENT_SUCCESS)
  {
    throw std::system_error(rc, std::generic_category(), "MQTTClient_create failed");
  }
  // SSL usage, not used for testing reasons
  // MQTTClient_SSLOptions sslOptions = MQTTClient_SSLOptions_initializer;
  // sslOptions.enableServerCertAuth = false;

  MQTTClient_connectOptions options = MQTTClient_connectOptions_initializer;
  options.username = "test_user";
  options.keepAliveInterval = 20;
  options.cleansession      = 1;

  MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

  //options.ssl = &sslOptions;

  rc = MQTTClient_connect(client, &options);
  if(rc != MQTTCLIENT_SUCCESS)
  {
    MQTTClient_destroy(&client);
    throw std::system_error(rc, std::generic_category(), "MQTTClient_connect failed");
  }
  return 0;
}

int MQTTPublisher::sendMessage(std::string message)
{
  MQTTClient_message mqtt_message;
  mqtt_message = MQTTClient_message_initializer;

  unsigned int message_length = message.length();;
  mqtt_message.payload = const_cast<char*>(message.data());
  mqtt_message.payloadlen = message_length;
  mqtt_message.qos = MQTTPublisher::QOS::at_least_once;
  mqtt_message.retained = 0;
  MQTTClient_deliveryToken token;
  int rc = MQTTClient_publishMessage(client, topicName.c_str(), &mqtt_message, &token);
  if(rc != MQTTCLIENT_SUCCESS){
    throw std::system_error(rc, std::generic_category(), "MQTTClient_publishMessage failed");
  }
  
  rc = MQTTClient_waitForCompletion(client, token, 10000); //wait for 10000millisec
  if(rc != MQTTCLIENT_SUCCESS)
  {
    throw std::system_error(rc, std::generic_category(), "MQTTClient_publishMessage failed,timeout waitForCompletion");
  }
  return 0;
}
                   
int MQTTPublisher::disconnect(){
  MQTTClient_disconnect(client, 10000); // disconnect with timeout of 10000milliseconds
  return 0;
}
                   
                   
                   
