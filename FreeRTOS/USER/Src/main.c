#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

//������
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED0_Task_Handle;

static void led0_task(void *pvParameters)
{
    while(1)
    {
        LED0=!LED0;
        vTaskDelay(500);
    }
}

void AppTaskCreate(void *pvParameters)
{
    //��������
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )120,
                (void*          )NULL,
                (UBaseType_t    )4,
                (TaskHandle_t *) LED0_Task_Handle);

    if (NULL != LED0_Task_Handle){
        printf("LED_Task���񴴽��ɹ���\n");
    } else{
        printf("LED_Task���񴴽�ʧ�ܣ�\n");
    }

    vTaskDelete(AppTaskCreate_Handle);
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    led_init();
    uart_init(115200);
    PrintfInit(USART1);

    xTaskCreate((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint16_t       )120,
                (void*          )NULL,
                (UBaseType_t    )3,
                (TaskHandle_t*   )AppTaskCreate_Handle);
    vTaskStartScheduler();
}





