#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "relay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "delay.h"
#include "adc.h"
#include "oled.h"
#include "key.h"
#include "dht11.h"

//任务优先级
#define START_TASK_PRIO     1
#define OLED_PRIO           2
#define ENVIR_PRIO          6
#define BEEP_PRIO           4
#define KEY_PRIO            5

//任务堆栈大小
#define START_STK_SIZE      128
#define KEY_STK_SIZE        128
#define ENVIR_STK_SIZE      128
#define OLED_STK_SIZE       128
#define BEEP_STK_SIZE       128

//任务句柄
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t ENVIR_Handle;
static TaskHandle_t OLED_Handle;
static TaskHandle_t KEY_Handle;
static TaskHandle_t BEEP_Handle;

u8 temp, humi, soil, light;

//报警任务
static void beep_task(void *pvParameters)
{
    while(1)
    {
//        printf("beep_task Running!!！\r\n");
        BEEP = !BEEP;
        LED3 = !LED3;
        delay_ms(pdMS_TO_TICKS(100));
    }
}

//按键检测
static void key_task(void *pvParameters)
{
    u8 key;
    u8 beep_temp = 0;
    LED2= 0;
    while(1)
    {
//        printf("key_task Running!!！\r\n");
        key = key_scan();
        switch (key) {
            case KEY0_P:
                if (beep_temp){
                    vTaskSuspend(BEEP_Handle);
                    BEEP = 1;
                    LED4 = 1;
                    beep_temp = 0;
                } else {
                    vTaskResume(BEEP_Handle);
                    beep_temp = 1;
                }
                break;
            case KEY1_P:
                LIGHT = !LIGHT;
                LED4 = !LED4;
                break;
            case KEY2_P:
                Shuibeng = !Shuibeng;
                LED5 = !LED5;
                break;
            case WK_UP:
                break;
            default:
                break;
        }
        delay_ms(pdMS_TO_TICKS(50));
    }
}

//检测环境任务
static void envir_task(void *pvParameters)
{
    while(1)
    {
        printf("envir_task Running!!！\r\n");
        portENTER_CRITICAL();
        DHT11_Read_Data(&temp,&humi);
        printf("%d    %d\r\n\n",temp,humi);
        portEXIT_CRITICAL();
//        printf("%d    %d\r\n\n",temp,humi);
        soil = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_4, 10)) / 4095));
        light = (int )(100-(100*((double )Get_Adc_Average(ADC_Channel_9, 10)) / 4095));
        LED0 = !LED0;
        delay_ms(pdMS_TO_TICKS(100));
    }
}

//OLED显示任务
static void oled_task(void *pvParameters)
{
    while(1)
    {
        printf("oled_task Running!!\r\n");
        OLED_ShowNum(32,0,temp,2,16);
        OLED_ShowNum(96,0,humi,2,16);
        OLED_ShowNum(32,2,soil,2,16);
        OLED_ShowNum(88,2,light,3,16);
        LED1 = !LED1;
        delay_ms(pdMS_TO_TICKS(500));
    }
}

//总任务初始化
void AppTaskCreate(void *pvParameters)
{
    taskENTER_CRITICAL();
    printf("AppTaskCreate任务创建中......\r\n");
    //创建任务1
    xTaskCreate((TaskFunction_t )envir_task,
                (const char*    )"envir_task",
                (uint16_t       )ENVIR_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )ENVIR_PRIO,
                (TaskHandle_t *) &ENVIR_Handle);
    if (ENVIR_Handle)
        printf("envir_task任务创建成功！\r\n");

    //创建任务2
    xTaskCreate((TaskFunction_t )oled_task,
                (const char*    )"oled_task",
                (uint16_t       )OLED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )OLED_PRIO,
                (TaskHandle_t *) &OLED_Handle);
    if (OLED_Handle)
        printf("oled_task任务创建成功！\r\n");

    //创建任务3
    xTaskCreate((TaskFunction_t )key_task,
                (const char*    )"key_task",
                (uint16_t       )KEY_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )KEY_PRIO,
                (TaskHandle_t *) &KEY_Handle);
    if (KEY_Handle)
        printf("key_task任务创建成功！\r\n");

    //创建任务4 默认挂起
    xTaskCreate((TaskFunction_t )beep_task,
                (const char*    )"beep_task",
                (uint16_t       )BEEP_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )BEEP_PRIO,
                (TaskHandle_t *) &BEEP_Handle);
    vTaskSuspend(BEEP_Handle);
    if (BEEP_Handle)
        printf("beep_task任务创建成功！\r\n");

    vTaskDelete(AppTaskCreate_Handle);
    taskEXIT_CRITICAL();
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    led_init();
    beep_init();
    delay_init();
    uart_init(115200);
    PrintfInit(USART1);
    Adc_Init();
    key_init();
    relay_init();
    OLED_Init();
    OLED_Clear();
    delay_ms(1000);

    while (DHT11_Init()){//DHT11初始化
        OLED_ShowString(24,2,"DHT11 ERROR!!");
        delay_ms(100);
        OLED_Clear();
        delay_ms(200);
        printf("DHT11 ERROR!!\r\n");
    }
    printf("DHT11 OK!!\r\n");

    //OLED
    OLED_ShowString(16,0,":   '   :   %");
    OLED_ShowString(16,2,":   %   :   H");
    OLED_ShowCHinese(0,0,0);
    OLED_ShowCHinese(64,0,1);
    OLED_ShowCHinese(0,2,2);
    OLED_ShowCHinese(64,2,3);

    printf("正在启动.....\r\n");
    xTaskCreate((TaskFunction_t ) AppTaskCreate,
                (const char*    )"AppTaskCreate",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*   )AppTaskCreate_Handle);
    vTaskStartScheduler();
}