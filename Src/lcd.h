#ifndef _LCD_H
#define _LCD_H

/**
 * this module is adapted from Renan Stark's lcd module of avr 8-bit classes.
 * https://github.com/xtarke/avrlibs
 * */

#include "stm32f1xx_hal.h"

//Definições para facilitar a troca dos pinos do hardware e facilitar a re-programação

#define NIBBLE_DADOS	0		//0 para via de dados do LCD nos 4 LSBs do PORT empregado (Px0-D4, Px1-D5, Px2-D6, Px3-D7)
								//1 para via de dados do LCD nos 4 MSBs do PORT empregado (Px4-D4, Px5-D5, Px6-D6, Px7-D7) 
#define DADOS_LCD		GPIOA
#define CONTR_LCD		GPIOA
#define E    			GPIO_PIN_8     //pino de habilitação do LCD (enable)
#define RS   			GPIO_PIN_10     //pino para informar se o dado é uma instrução ou caractere



/**
  * @brief  Configura hardware.
  * @param	Nenhum
  *
  * @retval Nenhum.
  */
void inic_LCD_4bits();


/**
  * @brief  Envia um dado estático para o display: caractere ou comando.
  * @param c: valor do comando.
  * @param cd: 0 para comando. 1 para caractere.
  *
  * @retval Nenhum
  */
void cmd_LCD(uint8_t c, uint8_t cd);

/**
  * @brief  Escreve um string estática (sem printf) no LCD. 
  * @param c: ponteiro para a string em RAM
  *
  * @retval Nenhum
  */
void escreve_LCD(char *c);

/**
  * @brief Escreve um caractere no stream. Função utilizada pela libc.
  * @param c: caractere a ser escrito.
  * @param fp: ponteiro do stream inicilizado que receberá o dado.
  *
  * @retval int maior que zero se sucesso.
  */
int lcd_putchar(char c, FILE *fp);

#endif
