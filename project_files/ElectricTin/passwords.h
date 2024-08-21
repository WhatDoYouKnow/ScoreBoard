// HiveMQ address is a string of 32 alphanumerics + .s1.eu.hivemq.cloud
#define MQTT_BROKER     "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX.s1.eu.hivemq.cloud"
#define MQTT_PORT       8883                        // secure port
// The two lines below are needed for secure comms with HiveMQ
#define MQTT_USERNAME   "your_subscriber_name"      // HiveMQ subscriber name
#define MQTT_PASSWORD   "your_subsciber_password"   // HiveMQ subscriber password 

// The lines below are for public subscription to HiveMQ
//#define MQTT_BROKER     "broker.hivemq.com"
//#define MQTT_PORT       1883

#define MQTT_TOPIC      "your_topic_name1"
#define MQTT_TOPIC1     "your_topic_name2"
#define MQTT_TOPIC2     "your_topic_name3"

#define WIFI_SSID1      "ssid1"
#define WIFI_PWORD1  	  "password1"

#define WIFI_SSID2      "ssid2"
#define WIFI_PWORD2  	  "password2"

#define WIFI_SSID3		  "ssid3"
#define WIFI_PWORD3  	  "password3"

#define WIFI_SSID4		  "ssid_4"
#define WIFI_PWORD4  	  "pword_4"

// This array is made up of the username/pwords above
// and is used to after the scan of available networks
// The first match will be used to connect
const char              SSID_ARRAY[][30] = {WIFI_SSID1, WIFI_SSID2, WIFI_SSID3, WIFI_SSID4};
const char              PASSWORD_ARRAY[][30] = {WIFI_PWORD1, WIFI_PWORD2, WIFI_PWORD3, WIFI_PWORD4};
const uint8_t           ARRAY_LENGTH = sizeof(SSID_ARRAY)/sizeof(SSID_ARRAY[0]);
