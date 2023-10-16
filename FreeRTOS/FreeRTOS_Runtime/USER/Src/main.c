#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "exti.h"

//�������ȼ�
#define START_TASK_PRIO     1
#define LED0_TASK_PRIO      2
#define LED1_TASK_PRIO      2

//�����ջ��С
#define START_STK_SIZE      128
#define LED0_STK_SIZE       128
#define LED1_STK_SIZE       128

//������
TaskHandle_t AppTaskCreate_Handle;
TaskHandle_t LED0_Task_Handle;
TaskHandle_t LED1_Task_Handle;

void led0_task(void *pvParameters)
{
    u32 task0_num = 0;
    while(1)
    {
        task0_num++;
        LED0=!LED0;
        taskENTER_CRITICAL();
        printf("Task1 Running: %lu!!\r\n", task0_num);
        taskEXIT_CRITICAL();
        delay_xms(1000);
    }
}

void led1_task(void *pvParameters)
{
    u32 task1_num = 0;
    while(1)
    {
        task1_num++;
        LED1=!LED1;
        taskENTER_CRITICAL();
        printf("Task2 Running: %lu!!\r\n", task1_num);
        taskEXIT_CRITICAL();
        delay_xms(1000);
    }
}

void AppTaskCreate(void *pvParameters)
{
    taskENTER_CRITICAL();
    printf("AppTaskCreate���񴴽���......\r\n");
    //��������1
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_TASK_PRIO,
                (TaskHandle_t *) &LED0_Task_Handle);
    if (LED0_Task_Handle)
        printf("led0_task���񴴽��ɹ���\r\n");

    //��������2
    xTaskCreate((TaskFunction_t )led1_task,
                (const char*    )"led1_task",
                (uint16_t       )LED1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t *) &LED1_Task_Handle);
    if (LED1_Task_Handle)
        printf("led1_task���񴴽��ɹ���\r\n");

    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

int main(void)
{
    //0-15��ռ���ȼ���0�����ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    uart_init(115200);
    PrintfInit(USART1);
    led_init();
////    key_init();
//    delay_init();
//    EXTIX_Init();

    printf("��������.....\r\n");
    xTaskCreate((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*   )AppTaskCreate_Handle);
    vTaskStartScheduler();
}
