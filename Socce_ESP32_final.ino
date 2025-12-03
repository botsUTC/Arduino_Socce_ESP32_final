#include "BluetoothSerial.h"

// Instancia de Bluetooth Serial
BluetoothSerial SERIAL_BT;

// Pines para el Driver TB6612FNG
const int PWMA = 2;
const int PWMB = 15;
const int AIN1 = 4;
const int AIN2 = 18;
const int BIN1 = 17;
const int BIN2 = 5;

// LED indicador de conexión
const int LED = 32;

// Variables para la velocidad
int SPEED_A = 0; // Velocidad motor A
int SPEED_B = 0; // Velocidad motor B

// Control de parpadeo del LED
unsigned long PREVIOUS_MILLIS = 0;
const long INTERVAL = 500; // Intervalo de parpadeo en ms
bool LED_STATE = false;

void setup() 
{
  // Configuración de pines
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(LED, OUTPUT);

  // Inicializar Bluetooth con el nombre "SOCCER 1"
  if (!SERIAL_BT.begin("SOCCER 2")) 
  {
    Serial.println("¡ERROR AL INICIAR BLUETOOTH!");
    while (true);
  }

  Serial.begin(115200);
  Serial.println("BLUETOOTH INICIADO COMO 'SOCCER 1'. ESPERANDO CONEXIÓN...");
}

void loop() 
{
  // Verificar si el dispositivo está vinculado
  if (SERIAL_BT.hasClient()) 
  {
    BLINK_LED();  // Parpadear si está vinculado
  } 
  else
  {
    digitalWrite(LED, LOW);  // Apagar LED si no hay conexión
  }

  // Leer datos de Bluetooth
  if (SERIAL_BT.available()) 
  {
    String COMMAND = SERIAL_BT.readStringUntil('\n'); // Leer hasta nueva línea
    COMMAND.trim(); // Eliminar espacios o saltos de línea extras
    PROCESS_COMMAND(COMMAND);
  }
}

// Función para procesar comandos
void PROCESS_COMMAND(String COMMAND) 
{
  Serial.println("COMANDO RECIBIDO: " + COMMAND);
  // Adelante
  if (COMMAND == "W") 
  { 
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, SPEED_A);
    analogWrite(PWMB, SPEED_B);
  } 
  // Atrás
  else if (COMMAND == "S") 
  { 
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, SPEED_A);
    analogWrite(PWMB, SPEED_B);
  } 
  // Izquierda
  else if (COMMAND == "A") 
  { 
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, SPEED_A);
    analogWrite(PWMB, SPEED_B);
  } 
  // Derecha
  else if (COMMAND == "D") 
  { 
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMA, SPEED_A);
    analogWrite(PWMB, SPEED_B);
  } 
  else if (COMMAND.startsWith("F")) 
  { // Ajustar velocidad de PWMA
    int VALUE = COMMAND.substring(1).toInt();
    SPEED_A = constrain(VALUE, 0, 255);
    analogWrite(PWMA, SPEED_A);
    Serial.println("VELOCIDAD PWMA: " + String(SPEED_A));
  } 
  else if (COMMAND.startsWith("G")) 
  { // Ajustar velocidad de PWMB
    int VALUE = COMMAND.substring(1).toInt();
    SPEED_B = constrain(VALUE, 0, 255);
    analogWrite(PWMB, SPEED_B);
    Serial.println("VELOCIDAD PWMB: " + String(SPEED_B));
  } 
  else if (COMMAND == "X") 
  { // Apagar motores
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  } else {
    Serial.println("COMANDO NO RECONOCIDO: " + COMMAND);
  }
}

// Función para hacer parpadear el LED si el dispositivo está vinculado
void BLINK_LED() 
{
  unsigned long CURRENT_MILLIS = millis();
  if (CURRENT_MILLIS - PREVIOUS_MILLIS >= INTERVAL) 
  {
    PREVIOUS_MILLIS = CURRENT_MILLIS;
    LED_STATE = !LED_STATE;
    digitalWrite(LED, LED_STATE);
  }
}
