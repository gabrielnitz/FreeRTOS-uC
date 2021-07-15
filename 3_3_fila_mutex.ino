#include <Arduino_FreeRTOS.h> // Biblioteca do FreeRTOS para o Arduino
#include "task.h" // Biblioteca para utilização das tasks
#include "queue.h" // Biblioteca para utilização de filas
#include "semphr.h" // Biblioteca para utilização de semáforos

// Declarar pinos
#define led1 6
#define led2 5
#define LDR A0
#define potenciometro A5

// Identificadores de filas e semáforos
QueueHandle_t pot;
QueueHandle_t ldr;
SemaphoreHandle_t mutex;

void setup() {
  Serial.begin(9600);

  // Configurar pinos
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(LDR,INPUT);
  pinMode(potenciometro,INPUT);

  // Criar filas
  pot = xQueueCreate( 10, // Tamanho da fila
                      sizeof(int) // Tamanho dos itens da fila
                    );
  ldr = xQueueCreate(10, sizeof(int));

  // Criar semáforo (Mutex)
  mutex = xSemaphoreCreateMutex();
  if (mutex != NULL) {
    Serial.println("Mutex criado");
  }
   
  // Criar tarefas
  xTaskCreate(TaskLeitura, "Leitura", 128, NULL, 1, NULL);
  xTaskCreate(TaskLED1, "LED1", 128, NULL, 2, NULL);
  xTaskCreate(TaskLED2, "LED2", 128, NULL, 2, NULL);    
}

void loop() {}

/* Tarefa que faz a leitura analógica do potênciometro e 
 * do LDR e envia para as filas. */
void TaskLeitura(void *parametro)
{
  (void) parametro;
  
  for (;;){
    int valor_pot = analogRead(potenciometro)/4;
    int valor_ldr = analogRead(LDR)/4;

    xQueueSend(pot, &valor_pot, portMAX_DELAY);
    xQueueSend(ldr, &valor_ldr, portMAX_DELAY);

    // Delay de 1 tick (15 ms) para estabilidade
    vTaskDelay(1);
  }
}

/* Tarefa que faz a escrita analógica no LED 1 com os valores 
 * recebido da fila que armazenou os valores do potenciômetro. */
void TaskLED1(void *parametro) {
  (void) parametro;  
  int valor_fila_pot = 0;
  
  for (;;) 
  {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE){
      if (xQueueReceive(pot, &valor_fila_pot, portMAX_DELAY) == pdPASS) {
      Serial.println(valor_fila_pot);
      analogWrite(led1,valor_fila_pot);
      }
      xSemaphoreGive(mutex);
    }
  vTaskDelay(1);
  }
}

/* Tarefa que faz a escrita analógica no LED 2 com os valores 
 * recebido da fila que armazenou os valores do LDR. */
void TaskLED2(void *parametro) {
  (void) parametro;
  int valor_fila_ldr = 0;
  
  for (;;) 
  {
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE){
      if (xQueueReceive(ldr, &valor_fila_ldr, portMAX_DELAY) == pdPASS) {
      Serial.println(valor_fila_ldr);
      analogWrite(led2,valor_fila_ldr);
      }
      xSemaphoreGive(mutex);
    }
  vTaskDelay(1);
  }
}
