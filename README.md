# tesseraktCarArduino

Bienvenue sur le projet de voiture de Tesserakt

Ce projet consite à controller une voiture motorisé à l'aide de cubes Tesserakt (Kireta.pythonanywhere.com)

Ce projet est constitué de :
  
  -1* esp 8266 NODEMCU
  -2* moteur brushed
  -1* controlleurs de vitesse (esc)
  -1* led verte
  -1* led rouge
  
Pour écrire sur l'esp, j'utilise l'ide d'Arduino.
La configuration des nombreuses carte peut être compliqué, voici la configuration correspondante pour notr esp :

-file>préférence>url : https://github.com/esp8266/Arduino/releases/download/2.3.0/package_esp8266com_index.json
-tool>board>board manager : mettre 'esp8266' dans la barre de recherche, et télécharger la librairie esp8266
-tool>board> selectionner la boarde NODEMCU 1.0
-tool> baudrate 11000
-tool> freq 80hz

C'est le moment de brancher l'esp ! 

après avoir télécharger les drivers (silicone driver esp8266), branché l'esp,
et vérifier que le com spécifique au driver silicone soit présent dans tool>port (il s'aggit en général du 5)

Il ne reste plus qu'a appuyer sur téléverser et ... Succès !

![image](https://user-images.githubusercontent.com/22155045/94266397-61aaf000-ff3a-11ea-973e-4f32981e78de.png)
