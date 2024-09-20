/**
 * @file xf_main.c
 * @author dotc (dotchan@qq.com)
 * @brief
 * @version 0.1
 * @date 2024-07-16
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ==================== [Includes] ========================================== */

#include "xf_hal.h"
#include "xf_task.h"

/* ==================== [Defines] =========================================== */

#define DEFAULT_SPEED               (1 * 1000 * 1000)

#define DEFAULT_SPI_ROLE            XF_HAL_SPI_HOSTS_SLAVE
#define DEFAULT_SPI_NUM             (0)
#define DEFAULT_SPI_SAMPLE_MODE     XF_HAL_SPI_MODE_0

#define DEFAULT_SPI_MOSI_IO_NUM     (9)
#define DEFAULT_SPI_MISO_IO_NUM     (11)
#define DEFAULT_SPI_SCLK_IO_NUM     (7)
#define DEFAULT_SPI_CS_IO_NUM       (10)

#define TASK_PRIORITY               5
#define TASK_DELAY_MS               2000

#define TAG "example_spi"

/* ==================== [Typedefs] ========================================== */

/* ==================== [Static Prototypes] ================================= */

static void spi_task(xf_task_t task);

/* ==================== [Static Variables] ================================== */

static bool is_next = false;

/* ==================== [Macros] ============================================ */

/* ==================== [Global Functions] ================================== */

void xf_main(void)
{
    xf_hal_spi_gpio_t spi_io_cfg = {
        .mosi_num = DEFAULT_SPI_MOSI_IO_NUM,
        .miso_num = DEFAULT_SPI_MISO_IO_NUM,
        .sclk_num = DEFAULT_SPI_SCLK_IO_NUM,
        .cs_num = DEFAULT_SPI_CS_IO_NUM,
        .quadhd_num = XF_HAL_GPIO_NUM_NONE,
        .quadwp_num = XF_HAL_GPIO_NUM_NONE,
    };

    xf_hal_spi_init(DEFAULT_SPI_NUM, DEFAULT_SPI_ROLE, DEFAULT_SPEED);
    xf_hal_spi_set_gpio(DEFAULT_SPI_NUM, &spi_io_cfg);
    xf_hal_spi_set_mode(DEFAULT_SPI_NUM, DEFAULT_SPI_SAMPLE_MODE);
    xf_hal_spi_enable(DEFAULT_SPI_NUM);

    xf_ntask_create_loop(spi_task, NULL, TASK_PRIORITY, TASK_DELAY_MS);
}

/* ==================== [Static Functions] ================================== */

static void spi_task(xf_task_t task)
{
    UNUSED(task);
    if(is_next == false)
    {
        XF_LOGI(TAG, "SPI slave read ... ...");
        uint8_t rbuf[32]={0};
        int ret = xf_hal_spi_read(DEFAULT_SPI_NUM, rbuf, sizeof(rbuf), 1000);
        if(ret < 0)
        {
            return;
        }
        is_next = true;
        XF_LOGI(TAG, "READ cmpl:%s", rbuf);
    }
    if(is_next == true)
    {
        XF_LOGI(TAG, "SPI slave write ... ...");
        uint8_t wbuf[]="I M SLAVE";
        int ret = xf_hal_spi_write(DEFAULT_SPI_NUM, wbuf, sizeof(wbuf), 1000);
        if(ret < 0)
        {
            return;
        }
        is_next = false;
        XF_LOGI(TAG, "WRITE CMPL");
    }
    return;
}
