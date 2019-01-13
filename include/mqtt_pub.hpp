#include <string>
#include "MQTTClient.h"

class MQTTPublisher
{
 public:
  enum QOS : int {at_most_once = 0 ,at_least_once = 1 ,exactly_once = 2};
  
  MQTTPublisher();
  MQTTPublisher(std::string ip_server,
                unsigned int port_server,
                std::string protocol_server,
                std::string topic_name);
  ~MQTTPublisher();
  int connect();
  int disconnect();
  int sendMessage(std::string message);

 private:
  MQTTClient client;    
  std::string serverUri; //depricated
  std::string ipServer;
  std::string protocolServer;
  unsigned int portServer;
  std::string clientName;
  std::string topicName;

  bool enabbledSSL;
};
