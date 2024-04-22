#include <DHT.h>
int pinsobrepoblacion = 9; // Define el pin donde el LED está conectado
int pinbajapoblacion = 10;
int pinestabilidad = 11;
#define buttonPin 3
#define DHTPin 5
#define DHTTYPE DHT11

DHT dht(DHTPin, DHTTYPE);
 
bool oldVal = HIGH;
 
void setup() {
  Serial.begin(9600);        // Inicia la comunicación serial a 9600 bps
  pinMode(pinsobrepoblacion, OUTPUT);
  pinMode(pinbajapoblacion, OUTPUT);
  pinMode(pinestabilidad, OUTPUT);   // Configura el pin del LED como salida
  pinMode(buttonPin, INPUT_PULLUP);
  dht.begin();

}
 
void loop() {
  bool val = digitalRead(buttonPin);
 
  if (oldVal == HIGH && val == LOW) {
    Serial.println("reset");  // Envía el mensaje de reseteo al programa Python
  }
  oldVal = val;

  float temp = dht.readTemperature();

  Serial.print("a-");
  Serial.print((int)temp);
  Serial.println();

  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');
    if (data.startsWith("live_cells:")) { // Verifica si el mensaje es un conteo de células vivas
      int live_cells = data.substring(11).toInt(); // Extrae el número de células vivas
      // Asumiendo algunos umbrales ejemplo para demostración
      if (live_cells > 200) {
        setColor(255, 0, 0); // Rojo: Sobrepoblación
      } else if (live_cells < 50) {
        setColor(0, 255, 0); // Verde: Subpoblación
      } else {
        setColor(0, 0, 255); // Azul: Estabilidad
      }
    } else {
      // El procesamiento de las condiciones existentes sigue igual
      int primeracoma = data.indexOf(',');
      int segundacoma = data.indexOf(',', primeracoma + 1);
      int sobrepoblacion = Serial.parseInt();  
      int bajapoblacion = Serial.parseInt();   
      int estabilidad = Serial.parseInt();
     
      if (sobrepoblacion > 0) {
        setColor(255, 0, 0);  // Rojo: Sobrepoblación
      } else if (bajapoblacion > 0) {
        setColor(0, 255, 0);  // Verde: Subpoblación
      } else if (estabilidad > 0) {
        setColor(0, 0, 255);  // Azul: Estabilidad
      } else {
        setColor(0, 0, 0);    // Apagado si no hay condiciones
      }
    }
  }
}
 
void setColor(int red, int green, int blue) {
  analogWrite(pinsobrepoblacion, red);
  analogWrite(pinbajapoblacion, green);
  analogWrite(pinestabilidad, blue);
}