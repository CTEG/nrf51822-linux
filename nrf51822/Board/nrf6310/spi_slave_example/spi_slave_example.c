/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
 
#include "spi_slave_example.h"
#include "spi_slave.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "boards.h"

#define TX_BUF_SIZE   128u              /**< SPI TX buffer size. */      
#define RX_BUF_SIZE   TX_BUF_SIZE       /**< SPI RX buffer size. */      
#define DEF_CHARACTER 0xAAu             /**< SPI default character. Character clocked out in case of an ignored transaction. */      
#define ORC_CHARACTER 0x55u             /**< SPI over-read character. Character clocked out after an over-read of the transmit buffer. */      

static uint8_t m_tx_buf[TX_BUF_SIZE];   /**< SPI TX buffer. */      
static uint8_t m_rx_buf[RX_BUF_SIZE];   /**< SPI RX buffer. */          


/**@brief Function for toggling LED1.
 */
static __INLINE void led_toggle(void)
{
    nrf_gpio_pin_toggle(LED_1);
}


/**@brief Function for SPI slave event callback.
 *
 * Upon receiving an SPI transaction complete event, LED1 will blink and the buffers will be set.
 *
 * @param[in] event SPI slave driver event.  
 */
static void spi_slave_event_handle(spi_slave_evt_t event)
{
    uint32_t err_code;
    
    if (event.evt_type == SPI_SLAVE_XFER_DONE)
    { 
        led_toggle();
        
        err_code = spi_slave_buffers_set(m_tx_buf, m_rx_buf, sizeof(m_tx_buf), sizeof(m_rx_buf));
        APP_ERROR_CHECK(err_code);          
    }
}


uint32_t spi_slave_example_init(void)
{
    uint32_t           err_code;
    spi_slave_config_t spi_slave_config;
        
    err_code = spi_slave_evt_handler_register(spi_slave_event_handle);
    APP_ERROR_CHECK(err_code);    

    spi_slave_config.pin_miso         = SPIS_MISO_PIN;
    spi_slave_config.pin_mosi         = SPIS_MOSI_PIN;
    spi_slave_config.pin_sck          = SPIS_SCK_PIN;
    spi_slave_config.pin_csn          = SPIS_CSN_PIN;
    spi_slave_config.mode             = SPI_MODE_0;            
    spi_slave_config.bit_order        = SPIM_LSB_FIRST;            
    spi_slave_config.def_tx_character = DEF_CHARACTER;      
    spi_slave_config.orc_tx_character = ORC_CHARACTER;      
    
    err_code = spi_slave_init(&spi_slave_config);
    APP_ERROR_CHECK(err_code);
       
    err_code = spi_slave_buffers_set(m_tx_buf, m_rx_buf, sizeof(m_tx_buf), sizeof(m_rx_buf));
    APP_ERROR_CHECK(err_code);            

    return NRF_SUCCESS;
}
