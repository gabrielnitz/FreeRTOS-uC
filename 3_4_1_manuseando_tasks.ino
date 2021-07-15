#include <Arduino_FreeRTOS.h> // Biblioteca do FreeRTOS para o Arduino
#include "task.h" // Biblioteca para utilização das tasks

// Protótipos das tarefas
void Task_Contador(void *param);
void Task_1(void *param);
void Task_2(void *param);
void Task_3(void *param);
void Task_4(void *param);
void Task_5(void *param);

// Identificadores para manusear tarefas
TaskHandle_t HandleContador;
TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;
TaskHandle_t TaskHandle_3;
TaskHandle_t TaskHandle_4;
TaskHandle_t TaskHandle_5;

// Variáveis
int contador = 0;
int finalizar = 0;

void setup() {
  Serial.begin(9600);
  // Criar as tarefas
  xTaskCreate(Task_Contador, "Contador", 128, NULL, 1, &HandleContador);
  xTaskCreate(Task_1, "Task1", 128, NULL, 1, &TaskHandle_1);
  xTaskCreate(Task_2, "Task2", 128, NULL, 1, &TaskHandle_2);
  xTaskCreate(Task_3, "Task3", 128, NULL, 1, &TaskHandle_3);
  xTaskCreate(Task_4, "Task4", 128, NULL, 1, &TaskHandle_4);
  xTaskCreate(Task_5, "Task5", 128, NULL, 1, &TaskHandle_5);
}

void loop() {
  // Sem código aqui
}

void Task_Contador(void *param){
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();
  /*  A função xTaskGetTickCount() retorna 
   *  o número total de interrupções de ticks 
   *  que ocorreram desde que o escalonador
   *  foi iniciado. Com isso, podemos usar a
   *  função vTaskDelayUntil(), que torna o 
   *  tempo de pausa da tarefa mais preciso. */
  while(1){
    if(contador == 6){
      vTaskDelete(HandleContador);
    }
    Serial.print("Contador");Serial.print(": ");
    Serial.println(contador);
    contador++;
    vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);
  }
}

void Task_1(void *param){
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();
  while(1){
    Serial.println("Task 1");
    vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);
    vTaskSuspend(TaskHandle_2);
    vTaskSuspend(TaskHandle_3);
    vTaskSuspend(TaskHandle_4);
    vTaskSuspend(TaskHandle_5);

    if(contador == 3){
      vTaskResume(TaskHandle_2);
      vTaskSuspend(TaskHandle_1);
    }
  }
}

void Task_2(void *param){
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();
  while(1){
    Serial.println("Task 2");
    vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);
    
    if(contador == 4){
      vTaskResume(TaskHandle_3);
      vTaskSuspend(TaskHandle_2);
    }
  }
}

void Task_3(void *param){
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();
  while(1){
    Serial.println("Task 3");
    vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);

    if(contador == 5){
      vTaskResume(TaskHandle_4);
      vTaskSuspend(TaskHandle_3);
    }
  }
}

void Task_4(void *param){
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();
  while(1){
    Serial.println("Task 4");
    vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);

    if(contador == 6){
      vTaskResume(TaskHandle_5);
      vTaskSuspend(TaskHandle_4);
    }
  }
}

void Task_5(void *param){
  (void) param;
  TickType_t getTick;
  getTick = xTaskGetTickCount();  
  while(1){
    if(finalizar == 6){
      vTaskDelete(TaskHandle_1);
      vTaskDelete(TaskHandle_2);
      vTaskDelete(TaskHandle_3);
      vTaskDelete(TaskHandle_4);
      vTaskDelete(TaskHandle_5);
    }
    Serial.println("Task 5");
    vTaskDelayUntil(&getTick, 1000/portTICK_PERIOD_MS);

    finalizar++;
  }
}
