/*****************************************************************************
* | File      	:   DEV_Config.c
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-03-16
* | Info        :   
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of theex Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
******************************************************************************/
#include "DEV_Config.h"

#define SPI_PORT spi1
#define I2C_PORT spi1

/******************************************************************************
function:	Define the GPIO
parameter:
******************************************************************************/
int EPD_RST_PIN;
int EPD_DC_PIN;
int EPD_CS_PIN;
int EPD_BL_PIN;
int EPD_CLK_PIN;
int EPD_MOSI_PIN;
int EPD_SCL_PIN;
int EPD_SDA_PIN;


uint slice_num;
/******************************************************************************
function:	GPIO read and write
parameter:
		Pin		： GPIO 
		Value	： 0 level  or  1 level 
******************************************************************************/
void DEV_Digital_Write(UWORD Pin, UBYTE Value)
{
    gpio_put(Pin, Value);
}

UBYTE DEV_Digital_Read(UWORD Pin)
{
    return gpio_get(Pin);
}

/******************************************************************************
function:	SPI write
parameter:
		Value	： 8 bits of data
		pData	： data array
		Len		： The length of the array
******************************************************************************/
void DEV_SPI_WriteByte(uint8_t Value)
{
    spi_write_blocking(SPI_PORT, &Value, 1);
}

void DEV_SPI_Write_nByte(uint8_t pData[], uint32_t Len)
{
    spi_write_blocking(SPI_PORT, pData, Len);
}



/******************************************************************************
function:	I2C write
parameter:
		addr 	: I2C address
		reg		: 0x00  Write command    0x40  Write data
		Value	: command  or  data
		pData	: data array
		Len		：The length of the array
******************************************************************************/

void DEV_I2C_Write(uint8_t addr, uint8_t reg, uint8_t Value)
{
    uint8_t data[2] = {reg, Value};
    i2c_write_blocking(i2c1, addr, data, 2, false);
}

void DEV_I2C_Write_nByte(uint8_t addr, uint8_t *pData, uint32_t Len)
{
    i2c_write_blocking(i2c1, addr, pData, Len, false);
}

uint8_t DEV_I2C_ReadByte(uint8_t addr, uint8_t reg)
{
    uint8_t buf;
    i2c_write_blocking(i2c1,addr,&reg,1,true);
    i2c_read_blocking(i2c1,addr,&buf,1,false);
    return buf;
}

/******************************************************************************
function:	GPIO Mode
parameter:
		Pin		: GPIO 
		Mode	; 0 Input   1 output
******************************************************************************/
void DEV_GPIO_Mode(UWORD Pin, UWORD Mode)
{
    gpio_init(Pin);
    if(Mode == 0 || Mode == GPIO_IN) {
        gpio_set_dir(Pin, GPIO_IN);
    } else {
        gpio_set_dir(Pin, GPIO_OUT);
    }
}

/******************************************************************************
function:	delay
parameter:
		xms		：  Delay millisecond * xms
		xus		：  Delay microseconds * xus
******************************************************************************/
void DEV_Delay_ms(UDOUBLE xms)
{
    sleep_ms(xms);
}

void DEV_Delay_us(UDOUBLE xus)
{
    sleep_us(xus);
}


/******************************************************************************
function:	GPIO initialization
parameter:
Info:
******************************************************************************/
void DEV_GPIO_Init(void)
{
    DEV_GPIO_Mode(EPD_RST_PIN, 1);
    DEV_GPIO_Mode(EPD_DC_PIN, 1);
    DEV_GPIO_Mode(EPD_CS_PIN, 1);
    DEV_GPIO_Mode(EPD_BL_PIN, 1);
    
    
    DEV_GPIO_Mode(EPD_CS_PIN, 1);
    DEV_GPIO_Mode(EPD_BL_PIN, 1);

    DEV_Digital_Write(EPD_CS_PIN, 1);
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_BL_PIN, 1);
}
/******************************************************************************
function:	Module Initialize, the library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_Module_Init(void)
{
    stdio_init_all();
    
    //GPIO PIN
    EPD_RST_PIN     = 12;
    EPD_DC_PIN      = 8;
    EPD_BL_PIN    = 13;
    
    EPD_CS_PIN      = 9;
    EPD_CLK_PIN     = 10;
    EPD_MOSI_PIN    = 11;
    
    EPD_SCL_PIN    = 7;
    EPD_SDA_PIN    = 6;
    
    // SPI Config
    spi_init(SPI_PORT, 62000 * 1000);
    gpio_set_function(EPD_CLK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(EPD_MOSI_PIN, GPIO_FUNC_SPI);
    
    // GPIO Config
    DEV_GPIO_Init();
    
    
    // PWM Config
    gpio_set_function(EPD_BL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(EPD_BL_PIN);
    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 1);
    pwm_set_clkdiv(slice_num,50);
    pwm_set_enabled(slice_num, true);
    
    
    //I2C Config
    i2c_init(i2c1,300*1000);
    gpio_set_function(EPD_SDA_PIN,GPIO_FUNC_I2C);
    gpio_set_function(EPD_SCL_PIN,GPIO_FUNC_I2C);
    gpio_pull_up(EPD_SDA_PIN);
    gpio_pull_up(EPD_SCL_PIN);
    
    printf("DEV_Module_Init OK \r\n");
    return 0;
}


/******************************************************************************
function:	Backlight brightness control
parameter:
		Value 	: 0 ~ 100
******************************************************************************/
void DEV_SET_PWM(uint8_t Value){
    if(Value<0 || Value >100){
        printf("DEV_SET_PWM Error \r\n");
    }else {
        pwm_set_chan_level(slice_num, PWM_CHAN_B, Value);
    }
}

/******************************************************************************
function:	Module exits, closes SPI and BCM2835 library
parameter:
******************************************************************************/
void DEV_Module_Exit(void)
{

}
