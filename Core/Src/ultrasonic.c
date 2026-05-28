#include "ultrasonic.h"
#include "main.h"
#include "Servo.h"

extern TIM_HandleTypeDef htim1;

#define ULTRASONIC_TIMER_PERIOD 20000U
#define ULTRASONIC_TIMEOUT_US   18000U

static uint16_t Get_us(uint16_t start)
{
    uint16_t now = __HAL_TIM_GET_COUNTER(&htim1);

    if (now >= start)
    {
        return now - start;
    }

    return ULTRASONIC_TIMER_PERIOD - start + now;
}

static void Delay_us(uint16_t us)
{
    uint16_t start = __HAL_TIM_GET_COUNTER(&htim1);

    while (Get_us(start) < us)
    {
    }
}

void ultrasonic_Send(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
    Delay_us(2);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
    Delay_us(10);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

uint32_t ultrasonic_Get_Time(void)
{
    uint16_t start;
    uint32_t time = 0;

    start = __HAL_TIM_GET_COUNTER(&htim1);
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_RESET)
    {
        if (Get_us(start) > ULTRASONIC_TIMEOUT_US)
        {
            return 0;
        }
    }

    start = __HAL_TIM_GET_COUNTER(&htim1);
    while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
    {
        if (Get_us(start) > ULTRASONIC_TIMEOUT_US)
        {
            return 0;
        }
    }

    time = Get_us(start);

    return time;
}

uint16_t Get_distance(void)
{
    uint32_t time = 0;
    uint16_t distance = 0;

    ultrasonic_Send();

    time = ultrasonic_Get_Time();

    if (time == 0)
    {
        return 0;
    }

    distance = time / 58;

    return distance;
}
void ultrasonic_Scan(uint16_t *left,uint16_t *front,uint16_t *right)
{
    Servo_SetAngle(90);
    HAL_Delay(300);
    *front = Get_distance();

    Servo_SetAngle(30);
    HAL_Delay(300);
    *left = Get_distance();

    Servo_SetAngle(150);
    HAL_Delay(300);
    *right = Get_distance();

    Servo_SetAngle(90);
    HAL_Delay(300);
}