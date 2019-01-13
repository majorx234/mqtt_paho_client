#include "mqtt_pub.hpp"
#include <system_error>
#include <iostream>

int main(int argc, char** argv)
{
  MQTTPublisher my_client;
  
  try
  {
    my_client.connect();
  }
  catch (std::system_error& exc)
  {
    std::cerr << exc.what() << " (" << exc.code() << ")" << std::endl;
  }
  std::string message = "{\n"
                        "   \"protocol\": \"v2\",\n"
                        "   \"device\": \"test_device\",\n"
                        "   \"at\": \"now\",\n"
                        "   \"data\": {\n"
                        "   \"value\": 234 \n"
                        "   }\n"
                        " }";
  try {
    my_client.sendMessage(message);
  } catch (std::system_error& exc) {
    std::cerr << exc.what() << " (" << exc.code() << ")" << std::endl;
  }

  return 0;
}
