#include <Arduino_FreeRTOS.h> // Biblioteca do FreeRTOS para o Arduino
#include "task.h" // Biblioteca para utilização das tasks
#include "semphr.h" // Biblioteca para utilização de semáforos

// Declarar pinos dos relés
#define rele1 8
#define rele2 9

// Protótipos das tarefas
void Task_Direita(void *param);
void Task_Esquerda(void *param);

// Identificadores 
TaskHandle_t HandleDireita;
TaskHandle_t HandleEsquerda;
SemaphoreHandle_t mutex;

void setup() {
  Serial.begin(9600);

  // Configurar os pinos
  pinMode(rele1,OUTPUT);
  pinMode(rele2,OUTPUT);

  // Criar Mutex
  mutex = xSemaphoreCreateMutex();
  if (mutex != NULL) {
    Serial.println("Mutex criado");
  }
  
  // Criar as tarefas
  xTaskCreate(Task_Direita, "TaskDireita", 128, NULL, 1, &HandleDireita);
  xTaskCreate(Task_Esquerda, "TaskEsquerda", 128, NULL, 1, &HandleEsquerda);
}

void loop() {
  // Sem código aqui
}

/* Tarefa que faz o motor girar para a 
 * direita por 0,5 s e parar por 1 s. */
void Task_Direita(void *param){
  for (;;){
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE){
      digitalWrite(rele1,LOW);
      digitalWrite(rele2,LOW);
      vTaskDelay(500/portTICK_PERIOD_MS);
      digitalWrite(rele1,HIGH);
      digitalWrite(rele2,LOW);
      vTaskDelay(1000/portTICK_PERIOD_MS);
    
      xSemaphoreGive(mutex);
    }
      vTaskDelay(1);
  }
}

/* Tarefa que faz o motor girar para a 
 * esquerda por 0,5 s e parar por 1 s. */
void Task_Esquerda(void *param){
  for (;;){
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE){
      digitalWrite(rele1,HIGH);
      digitalWrite(rele2,HIGH);
      vTaskDelay(500/portTICK_PERIOD_MS);
      digitalWrite(rele1,LOW);
      digitalWrite(rele2,HIGH);
      vTaskDelay(1000/portTICK_PERIOD_MS);
    
      xSemaphoreGive(mutex);
    }
      vTaskDelay(1);
  }
}
