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

#include "MyButton_task.h"
#include "MyButton.h"
#include "LED.h"
#include <stdio.h>
#include "menu_basic.h"
extern menu_handle_t menu_handle;
void move_up(void *btn_p)
{
    printf("up\r\n");
    menu_opt(&menu_handle, 'u');
    menu_handle.focus_line_state = 1;
}

void move_down(void *btn_p)
{
    printf("down\r\n");
    menu_opt(&menu_handle, 'd');
    menu_handle.focus_line_state = 1;
}

void move_left(void *btn_p)
{
    printf("left\r\n");
    menu_opt(&menu_handle, 'l');
    menu_handle.focus_line_state = 1;
}

void move_right(void *btn_p)
{
    printf("right\r\n");
    menu_opt(&menu_handle, 'r');
    menu_handle.focus_line_state = 1;
}

void MyButton_task_init(void)
{
    button_attach(&btn[BUTTON_ID_UP], PRESS_DOWN, move_up);
    button_attach(&btn[BUTTON_ID_DN], PRESS_DOWN, move_down);
    button_attach(&btn[BUTTON_ID_LF], PRESS_DOWN, move_left);
    button_attach(&btn[BUTTON_ID_RT], PRESS_DOWN, move_right);
}
