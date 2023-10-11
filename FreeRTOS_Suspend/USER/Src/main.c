#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

//任务优先级
#define START_TASK_PRIO     1
#define LED0_TASK_PRIO      2
#define LED1_TASK_PRIO      3
#define KEY_TASK_PRIO      4

//任务堆栈大小
#define START_STK_SIZE      128
#define LED0_STK_SIZE       128
#define LED1_STK_SIZE       128
#define KEY_STK_SIZE       128

//任务句柄
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED0_Task_Handle;
static TaskHandle_t LED1_Task_Handle;
static TaskHandle_t KEY_Task_Handle;

static void led0_task(void *pvParameters)
{
    u32 task0_num = 0;
    while(1)
    {
        task0_num++;
        LED0=!LED0;
        printf("Task1 Running: %lu!!\r\n", task0_num);
        if (task0_num == 10){
            vTaskDelete(LED1_Task_Handle);
            printf("Task1 Delete Task2!!\r\n");
        }
        vTaskDelay(500);
    }
}

static void led1_task(void *pvParameters)
{
    u32 task1_num = 0;
    while(1)
    {
        task1_num++;
        LED1=!LED1;
        printf("Task2 Running: %lu!!\r\n", task1_num);
        vTaskDelay(200);
    }
}

void AppTaskCreate(void *pvParameters)
{
    taskENTER_CRITICAL();
    printf("AppTaskCreate任务创建中......\r\n");
    //创建任务1
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_TASK_PRIO,
                (TaskHandle_t *) &LED0_Task_Handle);
    if (LED0_Task_Handle)
        printf("led0_task任务创建成功！\r\n");

    //创建任务2
    xTaskCreate((TaskFunction_t )led1_task,
                (const char*    )"led1_task",
                (uint16_t       )LED1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t *) &LED1_Task_Handle);
    if (LED1_Task_Handle)
        printf("led1_task任务创建成功！\r\n");

    //创建任务2
    xTaskCreate((TaskFunction_t )key_task,
                (const char*    )"key_task",
                (uint16_t       )LED1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t *) &KEY_Task_Handle);
    if (KEY_Task_Handle)
        printf("key_task任务创建成功！\r\n");

    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    led_init();
    uart_init(115200);
    PrintfInit(USART1);
    printf("正在启动.....\r\n");
    xTaskCreate((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*   )AppTaskCreate_Handle);
    vTaskStartScheduler();
}
