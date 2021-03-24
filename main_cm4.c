/* UNPUBLISHED, LICENSED SOFTWARE.
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

void isr_bouton()
{
    xSemaphoreGiveFromISR(bouton_semph,NULL);
    Cy_GPIO_ClearInterrupt(Bouton_0_PORT,Bouton_0_NUM);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
}

void bouton_task()
{
    UART_Start();        // Initialisation de l'unité UART 1
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

int main(void)
{
    __enable_irq(); /* Enable global interrupts. */    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysInt_Init(&Bouton_ISR_cfg, isr_bouton);
    NVIC_ClearPendingIRQ(Bouton_ISR_cfg.intrSrc);
    NVIC_EnableIRQ(Bouton_ISR_cfg.intrSrc);

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    xTaskCreate(inverseLED,"inverseLED",100,NULL,1,0);
    bouton_semph = xSemaphoreCreateBinary();
    xTaskCreate(bouton_task,"bouton_task",100,NULL,1,0);
    xTaskCreate(isr_bouton,"isr_bouton",100,NULL,1,0);
    vTaskStartScheduler();
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
