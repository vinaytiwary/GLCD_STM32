
#include "lcd.h"
#include "main.h"
#include "font5x8.h"

unsigned char screen_x = 0;
unsigned char screen_y = 0;
unsigned char color = 1;


void GLCD_InitalizePorts(void)
{
    // Assuming data pins are on port A, set them as outputs (PA0-PA7)
    GPIOA->MODER = 0xFFFFFFFF; // All pins on port A are set as outputs
    GPIOA->ODR = 0x00000000; // All pins on port A are set to 0 initially
    // Set GPIO pins for control lines and CS as outputs
    //KS0108_CS_CTRL_DIR |= ((GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0) << (KS0108_CS2 * 2));
   // KS0108_CTRL_DIR |= ((GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0) << (KS0108_RS * 2));

    // Set CS1 and CS2 pins as outputs
    KS0108_CS_CTRL_DIR |= ((GPIO_MODER_MODE11_0 | GPIO_MODER_MODE10_0) << (KS0108_CS1 * 2));
    KS0108_CS_CTRL_DIR |= ((GPIO_MODER_MODE11_0 | GPIO_MODER_MODE10_0) << (KS0108_CS2 * 2));

    // Set RS and EN pins as outputs
    KS0108_CTRL_DIR |= ((GPIO_MODER_MODE9_0 | GPIO_MODER_MODE8_0) << (KS0108_RS * 2));
    KS0108_CTRL_DIR |= ((GPIO_MODER_MODE9_0 | GPIO_MODER_MODE8_0) << (KS0108_EN * 2));

    // Set GPIO pins for control lines and CS to initial states
    KS0108_CS_CTRL_PORT |= ((1 << KS0108_CS1) | (1 << KS0108_CS2));
    KS0108_CTRL_PORT |= ((1 << KS0108_RS) | (1 << KS0108_EN));

}

void GLCD_EnableController(unsigned char controller)
{
	switch(controller){
		case 0 : KS0108_CS_CTRL_PORT &= ~(1 << KS0108_CS1); break;
		case 1 : KS0108_CS_CTRL_PORT &= ~(1 << KS0108_CS2); break;
	}
}
//-------------------------------------------------------------------------------------------------
// Disable Controller (0-2)
//-------------------------------------------------------------------------------------------------
void GLCD_DisableController(unsigned char controller)
{
	switch(controller){
		case 0 : KS0108_CS_CTRL_PORT |= (1 << KS0108_CS1); break;
		case 1 : KS0108_CS_CTRL_PORT |= (1 << KS0108_CS2); break;
	}
}

void GLCD_WriteCommand(uint8_t commandToWrite, uint8_t controller)
{
    KS0108_CTRL_PORT &= ~(1 << KS0108_RS);
    GLCD_EnableController(controller);
    GPIOA->ODR = commandToWrite;
    KS0108_CTRL_PORT |= (1 << KS0108_EN);
    delay_us(2);
    KS0108_CTRL_PORT &= ~(1 << KS0108_EN);
    GLCD_DisableController(controller);
    delay_us(2);
}

void GLCD_WriteData(uint8_t dataToWrite)
{
    KS0108_CTRL_PORT |= (1 << KS0108_RS);
    GLCD_EnableController(screen_x / 64);
    GPIOA->ODR = dataToWrite; // Assuming data port is GPIOA
    KS0108_CTRL_PORT |= (1 << KS0108_EN);

    delay_us(2);
    KS0108_CTRL_PORT &= ~(1 << KS0108_EN);
    GLCD_DisableController(screen_x / 64);
    screen_x++;
    if (screen_x >= KS0108_SCREEN_WIDTH)
        screen_x = 0;
    delay_us(2);
}

void GLCD_Initalize(void)
{
	unsigned char i;
	GLCD_InitalizePorts();

	for(i = 0; i < 2; i++)
	{
		GLCD_WriteCommand((DISPLAY_ON_CMD | ON), i);
	}
}

void GLCD_GoTo(unsigned char x, unsigned char y)
{
	unsigned char i;
	screen_x = x;
	screen_y = y;

	for(i = 0; i < (KS0108_SCREEN_WIDTH / 32); i++)//64
	{
		GLCD_WriteCommand((DISPLAY_SET_Y | 0), i);
		GLCD_WriteCommand((DISPLAY_SET_X | y), i);
		GLCD_WriteCommand((DISPLAY_START_LINE | 0), i);
	}
	GLCD_WriteCommand((DISPLAY_SET_Y | (x % 64)), (x / 64));
	GLCD_WriteCommand((DISPLAY_SET_X | y), (x / 64));

}

void GLCD_ClearScreen(void)
{
	unsigned char i, j;
	for(j = 0; j < (KS0108_SCREEN_HEIGHT / 8); j++)
	{
		GLCD_GoTo(0,j);
		for(i = 0; i < KS0108_SCREEN_WIDTH; i++)
			GLCD_WriteData(0x00);
	}
	//GLCD_GoTo(0, 0);
}

void GLCD_WriteChar(char charToWrite)
{
    int i;
    charToWrite -= ' ';     //' ' == 32
    for (i = 0; i < 5; i++) {
        uint8_t data = GLCD_ReadByteFromROMMemory((char *)(font5x8 + (5 * charToWrite) + i));
        GLCD_WriteData(data);
    }
    GLCD_WriteData(0x00);
}


void GLCD_WriteString(char * stringToWrite)
{
	while(*stringToWrite)
		GLCD_WriteChar(*stringToWrite++);
}
uint8_t GLCD_ReadByteFromROMMemory(char *ptr)
{
    return *ptr;
}

void delay_us(uint32_t us)
{
    uint32_t start = SysTick->VAL;
    uint32_t ticks = (SystemCoreClock / 1000000) * us;
    while ((start - SysTick->VAL) < ticks);
}
