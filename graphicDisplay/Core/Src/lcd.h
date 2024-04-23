/*
 * lcd.h
 *
 *  Created on: Apr 22, 2024
 *      Author: DELL
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_

#include <stdint.h>
#define KS0108_SCREEN_WIDTH		128
#define KS0108_SCREEN_HEIGHT	64

#define DISPLAY_SET_Y			0x40
#define DISPLAY_SET_X			0xB8
#define DISPLAY_START_LINE		0xC0
#define DISPLAY_ON_CMD			0x3E
//#define ON						0x01
//#define OFF						0x00
#define DISPLAY_STATUS_BUSY		0x80

#define ON (1)
#define OFF (0)

#define KS0108_DATA_PORT GPIOA
//#define KS0108_CTRL_PORT GPIOA // Assuming KS0108_CTRL_PORT is connected to GPIOA
//#define KS0108_CS_CTRL_PORT GPIOA // Assuming KS0108_CS_CTRL_PORT is connected to GPIOA

#define KS0108_CTRL_DIR     GPIOA->MODER
#define KS0108_CTRL_PORT    GPIOA->ODR
#define KS0108_CS_CTRL_DIR    GPIOA->MODER
#define KS0108_CS_CTRL_PORT    GPIOA->ODR
#define KS0108_RS            (8) // PA8
#define KS0108_EN            (9) // PA9
#define KS0108_CS1            (11) // PA11
#define KS0108_CS2            (10) // PA10


void GLCD_InitalizePorts(void);
void GLCD_EnableController(unsigned char);
void GLCD_DisableController(unsigned char);
void GLCD_WriteCommand(unsigned char, unsigned char);
void GLCD_WriteData(unsigned char);
unsigned char GLCD_ReadByteFromROMMemory(char *);
void GLCD_Initalize(void);
void GLCD_GoTo(unsigned char, unsigned char);
void GLCD_ClearScreen(void);
void GLCD_WriteChar(char);
void GLCD_WriteString(char *);
void GLCD_WriteString_inv(char *);

void delay_us(uint32_t us);
#endif /* SRC_LCD_H_ */
