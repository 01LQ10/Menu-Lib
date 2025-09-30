/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      driver_ssd1306_basic.c
 * @brief     driver ssd1306 basic source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-03-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/03/30  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/12/10  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ssd1306_basic.h"
#include "i2c.h"
#include <math.h>
#include <stdlib.h>

static ssd1306_handle_t gs_handle; /**< ssd1306 handle */

void ssd1306_basic_test(void)
{

    ssd1306_basic_init();
    for (int i = 0; i < 50; i++)
    {
        // ssd1306_basic_clear();
        ssd1306_gram_fill_rect(&gs_handle, 0, i, 128, i + 8, 0);
        // ssd1306_gram_write_string(&gs_handle, 0, i, "Hello World!", 12, 1, SSD1306_FONT_12);
        ssd1306_basic_string(0, i, "Hello World!", 12);
        ssd1306_basic_update();
        HAL_Delay(2);
    }
}

/**
 * @brief     basic example init
 * @param[in] interface interface type
 * @param[in] addr iic device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t ssd1306_basic_init(void)
{
    uint8_t res;

    /* link functions */
    DRIVER_SSD1306_LINK_INIT(&gs_handle, ssd1306_handle_t);
    DRIVER_SSD1306_LINK_IIC_INIT(&gs_handle, ssd1306_interface_iic_init);
    DRIVER_SSD1306_LINK_IIC_DEINIT(&gs_handle, ssd1306_interface_iic_deinit);
    DRIVER_SSD1306_LINK_IIC_WRITE(&gs_handle, ssd1306_interface_iic_write);
    DRIVER_SSD1306_LINK_SPI_INIT(&gs_handle, ssd1306_interface_spi_init);
    DRIVER_SSD1306_LINK_SPI_DEINIT(&gs_handle, ssd1306_interface_spi_deinit);
    DRIVER_SSD1306_LINK_SPI_WRITE_COMMAND(&gs_handle, ssd1306_interface_spi_write_cmd);
    DRIVER_SSD1306_LINK_SPI_COMMAND_DATA_GPIO_INIT(&gs_handle, ssd1306_interface_spi_cmd_data_gpio_init);
    DRIVER_SSD1306_LINK_SPI_COMMAND_DATA_GPIO_DEINIT(&gs_handle, ssd1306_interface_spi_cmd_data_gpio_deinit);
    DRIVER_SSD1306_LINK_SPI_COMMAND_DATA_GPIO_WRITE(&gs_handle, ssd1306_interface_spi_cmd_data_gpio_write);
    DRIVER_SSD1306_LINK_RESET_GPIO_INIT(&gs_handle, ssd1306_interface_reset_gpio_init);
    DRIVER_SSD1306_LINK_RESET_GPIO_DEINIT(&gs_handle, ssd1306_interface_reset_gpio_deinit);
    DRIVER_SSD1306_LINK_RESET_GPIO_WRITE(&gs_handle, ssd1306_interface_reset_gpio_write);
    DRIVER_SSD1306_LINK_DELAY_MS(&gs_handle, ssd1306_interface_delay_ms);
    DRIVER_SSD1306_LINK_DEBUG_PRINT(&gs_handle, ssd1306_interface_debug_print);

    /* set interface */
    res = ssd1306_set_interface(&gs_handle, SSD1306_INTERFACE_IIC);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set interface failed.\n");

        return 1;
    }

    /* set addr pin */
    res = ssd1306_set_addr_pin(&gs_handle, SSD1306_ADDR_SA0_0);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set addr failed.\n");

        return 1;
    }

    /* ssd1306 init */
    res = ssd1306_init(&gs_handle);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: init failed.\n");

        return 1;
    }

    /* close display */
    res = ssd1306_set_display(&gs_handle, SSD1306_DISPLAY_OFF);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set display failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set column address range */
    res = ssd1306_set_column_address_range(&gs_handle, SSD1306_BASIC_DEFAULT_COLUMN_ADDRESS_RANGE_START, SSD1306_BASIC_DEFAULT_COLUMN_ADDRESS_RANGE_END);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set column address range failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set page address range */
    res = ssd1306_set_page_address_range(&gs_handle, SSD1306_BASIC_DEFAULT_PAGE_ADDRESS_RANGE_START, SSD1306_BASIC_DEFAULT_PAGE_ADDRESS_RANGE_END);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set page address range failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set low column start address */
    res = ssd1306_set_low_column_start_address(&gs_handle, SSD1306_BASIC_DEFAULT_LOW_COLUMN_START_ADDRESS);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set low column start address failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set high column start address */
    res = ssd1306_set_high_column_start_address(&gs_handle, SSD1306_BASIC_DEFAULT_HIGH_COLUMN_START_ADDRESS);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set high column start address failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set display start line */
    res = ssd1306_set_display_start_line(&gs_handle, SSD1306_BASIC_DEFAULT_DISPLAY_START_LINE);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set display start line failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set fade blinking mode */
    res = ssd1306_set_fade_blinking_mode(&gs_handle, SSD1306_BASIC_DEFAULT_FADE_BLINKING_MODE, SSD1306_BASIC_DEFAULT_FADE_FRAMES);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set fade blinking failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* deactivate scroll */
    res = ssd1306_deactivate_scroll(&gs_handle);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set deactivate scroll failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set zoom in */
    res = ssd1306_set_zoom_in(&gs_handle, SSD1306_BASIC_DEFAULT_ZOOM_IN);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set set zoom in failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set contrast */
    res = ssd1306_set_contrast(&gs_handle, SSD1306_BASIC_DEFAULT_CONTRAST);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set contrast failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set segment remap */
    res = ssd1306_set_segment_remap(&gs_handle, SSD1306_BASIC_DEFAULT_SEGMENT);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set segment remap failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set scan direction */
    res = ssd1306_set_scan_direction(&gs_handle, SSD1306_BASIC_DEFAULT_SCAN_DIRECTION);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set scan direction failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set display mode */
    res = ssd1306_set_display_mode(&gs_handle, SSD1306_BASIC_DEFAULT_DISPLAY_MODE);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set display mode failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set multiplex ratio */
    res = ssd1306_set_multiplex_ratio(&gs_handle, SSD1306_BASIC_DEFAULT_MULTIPLEX_RATIO);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set multiplex ratio failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set display offset */
    res = ssd1306_set_display_offset(&gs_handle, SSD1306_BASIC_DEFAULT_DISPLAY_OFFSET);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set display offset failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set display clock */
    res = ssd1306_set_display_clock(&gs_handle, SSD1306_BASIC_DEFAULT_OSCILLATOR_FREQUENCY, SSD1306_BASIC_DEFAULT_CLOCK_DIVIDE);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set display clock failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set pre charge period */
    res = ssd1306_set_precharge_period(&gs_handle, SSD1306_BASIC_DEFAULT_PHASE1_PERIOD, SSD1306_BASIC_DEFAULT_PHASE2_PERIOD);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set pre charge period failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set hardware pins conf */
    res = ssd1306_set_com_pins_hardware_conf(&gs_handle, SSD1306_BASIC_DEFAULT_PIN_CONF, SSD1306_BASIC_DEFAULT_LEFT_RIGHT_REMAP);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set com pins hardware conf failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set deselect level 0.77 */
    res = ssd1306_set_deselect_level(&gs_handle, SSD1306_BASIC_DEFAULT_DESELECT_LEVEL);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set deselect level failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* set page memory addressing mode */
    res = ssd1306_set_memory_addressing_mode(&gs_handle, SSD1306_MEMORY_ADDRESSING_MODE_PAGE);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set memory addressing level failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* enable charge pump */
    res = ssd1306_set_charge_pump(&gs_handle, SSD1306_CHARGE_PUMP_ENABLE);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set charge pump failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* entire display off */
    res = ssd1306_set_entire_display(&gs_handle, SSD1306_ENTIRE_DISPLAY_OFF);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set entire display failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* enable display */
    res = ssd1306_set_display(&gs_handle, SSD1306_DISPLAY_ON);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: set display failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    /* clear screen */
    res = ssd1306_clear(&gs_handle);
    if (res != 0)
    {
        ssd1306_interface_debug_print("ssd1306: clear failed.\n");
        (void)ssd1306_deinit(&gs_handle);

        return 1;
    }

    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ssd1306_basic_deinit(void)
{
    /* deinit ssd1306 */
    if (ssd1306_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example display on
 * @return status code
 *         - 0 success
 *         - 1 display on failed
 * @note   none
 */
uint8_t ssd1306_basic_display_on(void)
{
    uint8_t res;

    /* display on */
    res = ssd1306_set_display(&gs_handle, SSD1306_DISPLAY_ON);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example display off
 * @return status code
 *         - 0 success
 *         - 1 display off failed
 * @note   none
 */
uint8_t ssd1306_basic_display_off(void)
{
    uint8_t res;

    /* display off */
    res = ssd1306_set_display(&gs_handle, SSD1306_DISPLAY_OFF);
    if (res != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  basic example clear
 * @return status code
 *         - 0 success
 *         - 1 clear failed
 * @note   none
 */
uint8_t ssd1306_basic_clear(void)
{
    /* clear */
    memset(gs_handle.gram, 0, 128 * 64 / 8);
    // if (ssd1306_clear(&gs_handle) != 0)
    // {
    //     return 1;
    // }
    // else
    // {
        return 0;
    // }
}

/**
 * @brief     basic example write a point
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] data written data
 * @return    status code
 *            - 0 success
 *            - 1 write point failed
 * @note      none
 */
uint8_t ssd1306_basic_write_point(uint8_t x, uint8_t y, uint8_t data)
{
    uint8_t res;

    /* write point */
    res = ssd1306_write_point(&gs_handle, x, y, data);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief      basic example read a point
 * @param[in]  x coordinate x
 * @param[in]  y coordinate y
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read point failed
 * @note       none
 */
uint8_t ssd1306_basic_read_point(uint8_t x, uint8_t y, uint8_t *data)
{
    uint8_t res;

    /* read point in gram */
    res = ssd1306_read_point(&gs_handle, x, y, data);
    if (res != 0)
    {
        return 1;
    }

    return 0;
}

/**
 * @brief     basic example draw a string
 * @param[in] x coordinate x
 * @param[in] y coordinate y
 * @param[in] *str pointer to a written string address
 * @param[in] len length of the string
 * @param[in] color display color
 * @param[in] font display font size
 * @return    status code
 *            - 0 success
 *            - 1 write string failed
 * @note      none
 */
uint8_t ssd1306_basic_string(uint8_t x, uint8_t y, char *str, uint16_t len)
{
    uint8_t res;

    /* write string in gram */
    res = ssd1306_gram_write_string(&gs_handle, x, y, str, len, 1, SSD1306_FONT_12);
    if (res != 0)
    {
        return 1;
    }

    /* update gram */
    // if (ssd1306_gram_update(&gs_handle) != 0)
    // {
    //     return 1;
    // }
    else
    {
        return 0;
    }
}

void ssd1306_basic_update(void)
{
    ssd1306_gram_update(&gs_handle);
}

/**
 * @brief     basic example fill a rectangle
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] color display color
 * @return    status code
 *            - 0 success
 *            - 1 fill rect failed
 * @note      none
 */
uint8_t ssd1306_basic_rect(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t color)
{
    uint8_t res;

    /* fill rect in gram */
    res = ssd1306_gram_fill_rect(&gs_handle, left, top, right, bottom, color);
    if (res != 0)
    {
        return 1;
    }

    /* update gram */
    if (ssd1306_gram_update(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief     basic example draw a picture
 * @param[in] left left coordinate x
 * @param[in] top top coordinate y
 * @param[in] right right coordinate x
 * @param[in] bottom bottom coordinate y
 * @param[in] *img pointer to a image buffer
 * @return    status code
 *            - 0 success
 *            - 1 draw picture failed
 * @note      none
 */
uint8_t ssd1306_basic_picture(uint8_t left, uint8_t top, uint8_t right, uint8_t bottom, uint8_t *img)
{
    uint8_t res;

    /* draw picture in gram */
    res = ssd1306_gram_draw_picture(&gs_handle, left, top, right, bottom, img);
    if (res != 0)
    {
        return 1;
    }

    /* update gram */
    if (ssd1306_gram_update(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void ssd1306_basic_draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    int8_t dx = abs(x1 - x0);       // x 方向的差值
    int8_t dy = abs(y1 - y0);       // y 方向的差值
    int8_t sx = (x0 < x1) ? 1 : -1; // x 方向步进符号
    int8_t sy = (y0 < y1) ? 1 : -1; // y 方向步进符号
    int8_t err = dx - dy;           // 误差项

    while (1)
    {
        // 在当前坐标画点
        ssd1306_gram_write_point(&gs_handle, x0, y0, 1);

        // 如果到达终点，退出循环
        if (x0 == x1 && y0 == y1)
        {
            break;
        }

        int e2 = 2 * err; // 计算误差项的 2 倍
        if (e2 > -dy)     // 如果误差项大于 -dy，沿 x 方向步进
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) // 如果误差项小于 dx，沿 y 方向步进
        {
            err += dx;
            y0 += sy;
        }
    }

    /* update gram */
    // return ssd1306_gram_update(&gs_handle);
}
