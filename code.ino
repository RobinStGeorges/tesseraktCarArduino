#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>

/* Definition des wifi */

const char* ssid = "kireta";
const char* password = "mdpdepasse";

//const char* ssid = "Xana";
//const char* password = "lyoko2468";

/* Definition de l'URL de l'api
 *  pour savoir l'adresse IP, taper dans un terminal de la raspberry "ifconfig", et prendre la valeur dans Wlan0*/
const char* serverName = "http://192.168.43.28:5000/getCarCommande";

/*valeur du timer, defini le temps d'execution entre les requetes*/
unsigned long lastTime = 0;
// 6 minutes (600000)
unsigned long timerDelay = 600000;
// 5 seconds (5000)
//unsigned long timerDelay = 5000;

/* Definition des variables*/

String commande;


int leftMotorForward = 2;     
int rightMotorForward = 15;   
int leftMotorBackward = 0;    
int rightMotorBackward = 13;  
int rightMotorENB = 14; /* active le moteur */
int leftMotorENB = 12;  /*active le moteur */

int isFirstTIme = 0; /* check si la requete a déjà eu lieu une fois */

void setup() {
  Serial.begin(115200);

  pinMode(leftMotorForward, OUTPUT);
  pinMode(rightMotorForward, OUTPUT); 
  pinMode(leftMotorBackward, OUTPUT);  
  pinMode(rightMotorBackward, OUTPUT);

  pinMode(leftMotorENB, OUTPUT); 
  pinMode(rightMotorENB, OUTPUT);
  
  WiFi.begin(ssid, password);
  Serial.println("Connexion en cours ... ");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecte au WIFI sur l'ip : ");
  Serial.println(WiFi.localIP());
 
}

void loop() {

  if (isFirstTIme == 0){
    manageLoop();
    isFirstTIme = 1;
  }
  else{
    if((millis() - lastTime) > timerDelay){
      manageLoop();
    }
  }
}

void manageLoop(){
  String arrayCmd[3][3];
  if(WiFi.status()== WL_CONNECTED){
      commande = httpGETRequest(serverName);
      JSONVar myObject = JSON.parse(commande);
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Le JSON n'a pas ete correctement recupere ! ");
        return;
      }
      
      int cpt = 0;
      int ctn = 1;
      int x = 0;
      int y = 0;
      String val;

      /*Definie les contour de la commande*/
      arrayCmd[0][0]= "a";
      arrayCmd[0][1]= "a";
      arrayCmd[0][2]= "a";
      arrayCmd[0][3]= "a";
      arrayCmd[0][4]= "a";
      
      while(ctn > 0){
        if(myObject[cpt]==null){
          Serial.println("cpt is null");
          ctn=0;
        }
        else{
          /* Construit une matrice à partir de la requete*/
          y=myObject[cpt][0];
          x=myObject[cpt][1];
          val=JSON.stringify(myObject[cpt][2]);
          arrayCmd[x][y] = val;
          cpt = cpt+1;
        }
      }
      
      x = 0;
      y = 0;
      ctn = 1;
      
      /***************************/
      /* Lectures de la commande */
      /***************************/
      
      while(ctn > 0){
        Serial.println("reding cmd");
        if(arrayCmd[x][y] == "\"AVANCER\""){
          Serial.println("avancer");
          if(arrayCmd[x+1][y] == "\"EGAL\""){
            Serial.println("egal");
            if(arrayCmd[x+2][y] == "\"UN\""){
              Serial.println("un");
              MotorForward();
            }
            if(arrayCmd[x+2][y] == "\"DEUX\""){
              Serial.println("deux");
              MotorForward();
              MotorForward();
            }
            if(arrayCmd[x+2][y] == "\"TROIS\""){
              Serial.println("trois");
              MotorForward();
              MotorForward();
              MotorForward();
            }
          }
          else{
            Serial.println("error");
            /* lumière rouge */
          }
        }
        else if(arrayCmd[x][y] == "\"VIRAGEGAUCHE\""){
          Serial.println("virage gauche");
          TurnLeft();
        }
        else if(arrayCmd[x][y] == "\"VIRAGEDROITE\""){
          Serial.println("virage gauche");
          TurnRight();
        }
        /*Passe a la ligne suivante */
        y = y+1;
        /*S'il n'y a plus de commande*/
        if(arrayCmd[0][y] == "a"){
          ctn = 0;
        }
        
      }
      
  }
  else {
    Serial.println("WiFi deconecte");
  }
  lastTime = millis();
}

String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  http.begin(serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

void MotorForward(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorBackward,LOW);
  delay(1000);
  MotorStop();
  delay(1000);
}

void MotorBackward(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  digitalWrite(leftMotorBackward,HIGH);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,LOW);
  delay(1000);
  MotorStop();
  delay(1000);
}

void TurnLeft(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH); 
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(rightMotorForward,HIGH);
  digitalWrite(rightMotorBackward,LOW);
  digitalWrite(leftMotorBackward,HIGH);  
  delay(450);
  MotorStop();
  delay(1000);
}

void TurnRight(void)   
{
  digitalWrite(leftMotorENB,HIGH);
  digitalWrite(rightMotorENB,HIGH);
  digitalWrite(leftMotorForward,HIGH);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,HIGH);
  digitalWrite(leftMotorBackward,LOW);
  delay(450);
  MotorStop();
  delay(1000);
}

void MotorStop(void)   
{
  digitalWrite(leftMotorENB,LOW);
  digitalWrite(rightMotorENB,LOW);
  digitalWrite(leftMotorForward,LOW);
  digitalWrite(leftMotorBackward,LOW);
  digitalWrite(rightMotorForward,LOW);
  digitalWrite(rightMotorBackward,LOW);
}
/*Rajoutez vos fonctions personalisées ici ! */