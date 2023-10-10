#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED0_Task_Handle;

static StackType_t AppTaskCreate_Stack[128];
static StackType_t LED0_Task_Stack[128];

static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED0_Task_TCB;

static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];

static StaticTask_t Idle_Task_TCP;
static StaticTask_t LED0_Task_TCB;

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
    taskENTER_CRITICAL();
    //创建任务
    LED0_Task_Handle = xTaskCreateStatic((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint32_t       )128,
                (void*          )NULL,
                (UBaseType_t    )4,
                (StackType_t*   )LED0_Task_Stack,
                (StaticTask_t*  )&LED0_Task_TCB);

    if (NULL != LED0_Task_Handle){
        printf("LED_Task任务创建成功！\n");
    } else{
        printf("LED_Task任务创建失败！\n");
    }

    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    led_init();
    uart_init(115200);
    PrintfInit(USART1);

    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint32_t       )128,
                (void*          )NULL,
                (UBaseType_t    )3,
                (StackType_t*   )AppTaskCreate_Stack,
                (StaticTask_t*  )&AppTaskCreate_TCB);
    if (NULL != AppTaskCreate_Handle){
        vTaskStartScheduler();
    }
    while (1);
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer=&Idle_Task_TCP;
    *ppxIdleTaskStackBuffer=Idle_Task_Stack;
    *pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
}





