#include "Arduino_FreeRTOS.h" // Biblioteca do FreeRTOS para o Arduino
#include "task.h" // Biblioteca para utilização das tasks
#include <math.h> // Biblioteca para funções matemáticas

// Definir os pinos dos leds
#define led_amarelo   4
#define led_vermelho  6
#define led_verde     8
 
//Tasks
TaskHandle_t    led1TaskH;
TaskHandle_t    led2TaskH;
TaskHandle_t    sineTaskH;
  
void setup() {
    // Inicializa Serial
    Serial.begin(9600);
    
    // Relaciona o tempo com número de ticks
    Serial.print("1 segundo é ");
    Serial.print(configTICK_RATE_HZ);
    Serial.print(" ticks em ");
    Serial.print(F_CPU);
    Serial.print(" Hz\n\n");
    
    // Define os leds como saída
    pinMode(led_verde, OUTPUT);
    pinMode(led_amarelo, OUTPUT);
    pinMode(led_vermelho, OUTPUT);
     
    // Cria tarefa led1Task
    xTaskCreate(led1Task,     // Função
                "led1Task",   // Nome
                128,          // Pilha
                NULL,         // Parâmetro
                1,            // Prioridade
                &led1TaskH);  // Identificador

    // Cria tarefa led2Task
    xTaskCreate(led2Task,     // Função
                "led2Task",   // Nome
                128,          // Pilha
                NULL,         // Parâmetro
                1,            // Prioridade
                &led2TaskH);  // Identificador
     
    // Cria tarefa sineTask
    xTaskCreate(SineTask,     // Função
                "SineTask",   // Nome
                128,          // Pilha
                NULL,         // Parâmetro
                1,            // Prioridade
                &sineTaskH);  // Identificador
}
 
void loop() {
    // Nada é executado aqui, as tarefas estão nas funções abaixo
}
 
  // Led1Task
  // Pisca led verde com frequência de 1Hz
void led1Task(void *arg) {
    while(1) { // Loop infinito
        digitalWrite(led_verde, HIGH);    // Liga o led
        vTaskDelay(pdMS_TO_TICKS(500));   // Espera 0.5s
        digitalWrite(led_verde, LOW);     // Desliga o led
        vTaskDelay(pdMS_TO_TICKS(500));   // Espera 0.5s
    }
}

  // Led2Task
  // Pisca led amarelo com frequência de 3Hz
void led2Task(void *arg) {
    while(1) {  // Loop infinito
        digitalWrite(led_amarelo, HIGH);  // Liga o led
        vTaskDelay(pdMS_TO_TICKS(166));   // Espera 0.166s
        digitalWrite(led_amarelo, LOW);   // Desliga o led
        vTaskDelay(pdMS_TO_TICKS(166));   // Espera 0.166s
    }
}
 
  // SineTask
  // Descreve uma senoide de 2Hz na saida analógica (PWM)
void SineTask(void *arg) {
    unsigned int tempo = 0;
    unsigned int saida = 0;
    while(1) {  // Loop infinito
        for(unsigned int periodo = 0; periodo < 16; ++periodo){
            tempo = millis();
            saida = (unsigned int)((sin(2*PI*2*tempo*0.001)+1)*127.5);  // seno(2*pi*f*t)
            analogWrite(led_vermelho, saida); // Acende o led seguindo a senoide
            vTaskDelay(pdMS_TO_TICKS(33));    // Espera taxa de atualização
        }
    }
}
