// I used HiveMQ as my broker on a free account
// Broker address is in the format "ALPHANUMERICTEXT32CHARS.s1.eu.hivemq.cloud"
#define MQTT_BROKER     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.s1.eu.hivemq.cloud"

// As this is a secure connection, the port is 8883
#define MQTT_PORT       8883

// HiveMQ allows a username/password to keep your communications private
#define MQTT_USERNAME   "MQTT_username"
#define MQTT_PASSWORD   "MQTT_password"

// MQTT has a "Topic" which differentiates the comms from this app from any others
#define MQTT_TOPIC      "SquashScore"
#define MQTT_TOPIC1     "SquashScore1"
#define MQTT_TOPIC2     "SquashScore2"

// enter the details of your SSID and password for WiFi comms
#define WIFI_SSID1      "your_ssid"
#define WIFI_PASSWORD1  "your_password"

// enter the details of any other WiFi networks the display may need to access
#define WIFI_SSID2      "ssid_2"
#define WIFI_PASSWORD2  "password_2"  

// these arrays hold the ssid/password so that on power-up, ssids are scanned
// to find a matching value which is then used. 
const char              SSID_ARRAY[][30] = {"your_ssid", "ssid_2"};
const char              PASSWORD_ARRAY[][30] = {"your_password", "password_2"};
const uint8_t           ARRAY_LENGTH = sizeof(SSID_ARRAY)/sizeof(SSID_ARRAY[0]);
