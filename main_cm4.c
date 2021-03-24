/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "params.h"
#include "queue.h"


SemaphoreHandle_t bouton_semph;
QueueHandle_t print_queue;
BaseType_t queue_send;
task_params_t task_A = {
    .delay=1000,
    .message="Tache A en cours\n\r"
};
task_params_t task_B = {
    .delay = 999,
    .message = "Tache B en cours\n\r"
};

void print_loop(task_params_t*params)
{
    for(;;)
    {
    // Avant la question bonus : vTaskDelay(pdMS_TO_TICKS(params->delay));
  //Avant la question bonus :  UART_PutString(params->message);
    xQueueSend(print_queue, params->message,pdMS_TO_TICKS(params->delay));
    }
    
}

void isr_bouton()
{
    xSemaphoreGiveFromISR(bouton_semph,NULL);
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT,Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
}

void bouton_task()
{
    
    for(;;)
    {
        xSemaphoreTake(bouton_semph,portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(20));
       
        if(Cy_GPIO_Read(Bouton_0_PORT,Bouton_0_NUM))
        {
            UART_PutString("Boutonappuye");
        }
        else
        {
            UART_PutString("Bouton relache");
        }
    }
}

void inverseLED ()
{
    for(;;)
    {
    Cy_GPIO_Write(LED_PORT,LED_NUM,1);
    vTaskDelay(pdMS_TO_TICKS(500));
    Cy_GPIO_Write(LED_PORT,LED_NUM,0);
    vTaskDelay(pdMS_TO_TICKS(500));
    }

}

void print()
{
    char*message;
    for(;;){
        xQueueReceive(print_queue,&message,portMAX_DELAY);
        UART_PutString(message);
    }
}

int main(void)
{
    UART_Start();        // Initialisation de l'unité UART 1
    __enable_irq(); /* Enable global interrupts. */    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    xTaskCreate(inverseLED,"inverseLED",100,NULL,1,0);
    bouton_semph = xSemaphoreCreateBinary();
    xTaskCreate(bouton_task,"bouton_task",100,NULL,1,0);
    xTaskCreate(isr_bouton,"isr_bouton",100,NULL,1,0);
    task_params_t* tacheA=NULL;
    tacheA=&task_A;
    xTaskCreate(
        print_loop(tacheA),
        "task A",
        configMINIMAL_STACK_SIZE,
        (void*) &task_A,
        1,
        NULL);
    task_params_t* tacheB=NULL;
    tacheB=&task_B;
    xTaskCreate(
        print_loop(task_B),
        "task B",
        configMINIMAL_STACK_SIZE,
        (void*) &task_B,
        1,
        NULL);
    vTaskStartScheduler();
    
    print_queue = xQueueCreate(2, sizeof(char*));
    
    
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
