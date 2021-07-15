#include <Arduino_FreeRTOS.h> // Biblioteca do FreeRTOS para o Arduino
#include "task.h"   // Biblioteca para utilização das tasks
#include "Servo.h"  // Inclui a Biblioteca Servo.h

// Definir os pinos dos leds
#define botao_1 6
#define botao_2 7
#define botao_3 8

// Tasks
TaskHandle_t HandleServo;
TaskHandle_t HandleControle;

// Variáveis
int contador = 0;
int posicao = 0;
int angulo = 0;
int variacao = 3;
boolean liga_desliga = false;
boolean velocidade = false;

// Nome para o servo
Servo meuservo; 

void setup() {
  // Define os botões como entrada
  pinMode(botao_1,INPUT_PULLUP);
  pinMode(botao_2,INPUT_PULLUP);
  pinMode(botao_3,INPUT_PULLUP);
  
  // Define pino do servo
  meuservo.attach(10);

  // Cria tarefa Task_Servo
  xTaskCreate(Task_Servo, "Servo", 128, NULL, 1, &HandleServo);
  // Cria tarefa Task_Controle
  xTaskCreate(Task_Controle, "Controle", 128, NULL, 1, &HandleControle);
}

void loop() {
  // Nada é executado aqui, as tarefas estão nas funções abaixo
}

// Task_Servo
// Faz o servo motor girar
void Task_Servo(void *param){
  while(1){ // Loop infinito
    // Gira o servo de 0° até 180°
    for (angulo = 0; angulo < 180; angulo += variacao) {
      meuservo.write(angulo); // Altera o ângulo
      vTaskDelay(10/portTICK_PERIOD_MS); // Delay de 10 ms
    }
    // Gira o servo de 180° até 0°
    for (angulo = 180; angulo >= 1; angulo -= variacao) {
      meuservo.write(angulo); // Altera o ângulo
      vTaskDelay(10/portTICK_PERIOD_MS); // Delay de 10 ms
    }
  }
}

// Task_Controle
// Lê botões que podem alterar a Task_Servo
void Task_Controle(void *param){
  while(1){ // Loop infinito

    // O botão 1 suspende e libera a Task_Servo
    if(digitalRead(botao_1) == LOW){ 
      while(digitalRead(botao_1) == LOW); // Aguarda soltar o botão
      if(liga_desliga == false){
        vTaskSuspend(HandleServo); // Suspende a Task_Servo
      }
      else if(liga_desliga == true){
        vTaskResume(HandleServo); // Libera a Task_Servo
      }
      liga_desliga = !liga_desliga; // Altera o controle
    }
    // O botão 2 altera a velocidade do servo
    if(digitalRead(botao_2) == LOW){
      while(digitalRead(botao_2) == LOW); // Aguarda soltar o botão
      if(velocidade == false){
        variacao = 10; // Aumenta a velocidade
      }
      else if(velocidade == true){
        variacao = 3; // Diminui a velocidade
      }
      velocidade = !velocidade; // Altera o controle
    }
    // O botão 3 deleta a Task_Servo
    if(digitalRead(botao_3) == LOW){        
      vTaskDelete(HandleServo); // Deleta a Task_Servo
    }
  }
}
