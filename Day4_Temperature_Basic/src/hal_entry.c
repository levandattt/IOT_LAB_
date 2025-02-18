#include "hal_data.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);
void user_uart_callback(uart_callback_args_t *p_args);
void console_write(const char *buffer);
float read_celsius(void);

static volatile bool is_transfer_complete = false;
static volatile bool g_MAX6675_complete = false;
char write_buffer[200] = {};

#define SPI_SSL_ASSERT(x) R_IOPORT_PinWrite(&g_ioport_ctrl, x, BSP_IO_LEVEL_HIGH)
#define SPI_SSL_DEASSERT(x) R_IOPORT_PinWrite(&g_ioport_ctrl, x, BSP_IO_LEVEL_LOW)

void MAX6675_callback (spi_callback_args_t * p_args);

extern bsp_leds_t g_bsp_leds;
uint8_t MAX6675_data_values[2];

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif

    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = 2000;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;

    float MAX6675_temperature_value = 0;

    R_SCI_SPI_Open(&g_spi0_ctrl, &g_spi0_cfg);
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    sprintf(write_buffer, "SPI entry\r\n");
    console_write(write_buffer);
    R_BSP_SoftwareDelay(200, BSP_DELAY_UNITS_MILLISECONDS);
    while (1)
    {
        // Read temperature in Celsius
        float temperature = read_celsius();

        // Print the temperature value
        if (isnan(temperature))
        {
            sprintf(write_buffer, "No thermocouple attached!\r\n");
        }
        else
        {
            uint8_t integer_part = temperature / 1;
            uint8_t decimal_part = (temperature - integer_part) * 100;
            sprintf(write_buffer, "Temperature: %d.%02d C\r\n", integer_part, decimal_part);
        }
        console_write(write_buffer);

        // Delay
        R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}

void user_uart_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        case UART_EVENT_TX_COMPLETE:
        {
            is_transfer_complete = true;
            break;
        }
        default:
        {

        }
    }
}

void console_write(const char *buffer) {
    is_transfer_complete = false;
    R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *) buffer, strlen(buffer));
    while (!is_transfer_complete)
    {
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }
}

void MAX6675_callback (spi_callback_args_t * p_args) {
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event) {
        g_MAX6675_complete = true;
    }
}

float read_celsius(void)
{
    uint16_t v;

    // Assert SPI SSL
    SPI_SSL_DEASSERT(BSP_IO_PORT_04_PIN_13);
    SPI_SSL_ASSERT(BSP_IO_PORT_04_PIN_10);
    SPI_SSL_ASSERT(BSP_IO_PORT_04_PIN_12);
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);

    // Perform SPI Read operation with 16-bit width
    R_SCI_SPI_WriteRead(&g_spi0_ctrl, NULL, MAX6675_data_values, 2, SPI_BIT_WIDTH_8_BITS);
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);
    // Combine the two bytes into a 16-bit value
    v = (MAX6675_data_values[0] << 8) | MAX6675_data_values[1];
    R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MICROSECONDS);

    // Deassert SPI SSL
    SPI_SSL_ASSERT(BSP_IO_PORT_04_PIN_13);
    SPI_SSL_DEASSERT(BSP_IO_PORT_04_PIN_10);
    SPI_SSL_DEASSERT(BSP_IO_PORT_04_PIN_12);

    // Check if the thermocouple is attached
    if (v & 0x4)
    {
        // No thermocouple attached
        return NAN;
    }

    // Shift to remove the status bits
    v >>= 3;

    // Convert to temperature in Celsius (each bit represents 0.25 degrees Celsius)
    return v * 0.25;
}
