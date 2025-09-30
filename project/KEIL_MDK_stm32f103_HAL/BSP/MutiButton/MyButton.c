/**
 ******************************************************************************
 * @file    MyKEY.c
 * @author  LQ
 * @version V1.0
 * @date    2024-12-16
 * @brief   ����״̬��
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */

#include "MyButton.h"
#include "main.h"

struct Button btn[4];

uint8_t BUTN_GPIO_read(uint8_t id)
{
    uint8_t ret;
    switch (id)
    {
    case BUTTON_ID_UP:
        ret = (uint8_t)HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12);
        break;
    case BUTTON_ID_DN:
        ret = (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3);
        break;
    case BUTTON_ID_LF:
        ret = (uint8_t)HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
        break;
    case BUTTON_ID_RT:
        ret = (uint8_t)HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
        break;
    default:
        break;
    }
    return ret;
}

void MyButton_start(void)
{
    button_start(&btn[BUTTON_ID_UP]);
    button_start(&btn[BUTTON_ID_DN]);
    button_start(&btn[BUTTON_ID_RT]);
    button_start(&btn[BUTTON_ID_LF]);
}

void MyButton_Init(void)
{
    button_init(&btn[BUTTON_ID_UP], BUTN_GPIO_read, 0, BUTTON_ID_UP);
    button_init(&btn[BUTTON_ID_DN], BUTN_GPIO_read, 0, BUTTON_ID_DN);
    button_init(&btn[BUTTON_ID_RT], BUTN_GPIO_read, 0, BUTTON_ID_RT);
    button_init(&btn[BUTTON_ID_LF], BUTN_GPIO_read, 0, BUTTON_ID_LF);

    MyButton_task_init();

    MyButton_start();
}
