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
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    xTaskCreate(inverseLED,"inverseLED",100,NULL,1,0);
    vTaskStartScheduler();
    
    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

