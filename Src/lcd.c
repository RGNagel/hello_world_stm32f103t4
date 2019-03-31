#include "lcd.h"
#include "atraso.h"
/* sinal de habilitação para o LCD */

#define PULSO_ENABLE() \
	atraso_us(1);\
	HAL_GPIO_WritePin(CONTR_LCD, E, GPIO_PIN_SET);\
	atraso_us(1);\
	HAL_GPIO_WritePin(CONTR_LCD, E, GPIO_PIN_RESET);\
	atraso_us(45)

/**
  * @brief  Envia um dado estático para o display: caractere ou comando.
  * @param c: valor do comando/dado.
  * @param cd: 0 para comando. 1 para caractere.
  *
  * @retval Nenhum
  */
void cmd_LCD(uint8_t c, uint8_t cd)
{
	HAL_GPIO_WritePin(CONTR_LCD, RS, cd);

    //primeiro nibble de dados - 4 MSB
    //compila código para os pinos de dados do LCD nos 4 MSB do PORT
    #if (NIBBLE_DADOS)
    	HAL_GPIO_WritePin(DADOS_LCD, 0x00F0 & c, GPIO_PIN_SET);
    #else
        //compila código para os pinos de dados do LCD nos 4 LSB do POR
    	HAL_GPIO_WritePin(DADOS_LCD, c >> 4, GPIO_PIN_SET);
    #endif

    PULSO_ENABLE();

    //segundo nibble de dados - 4 LSB
    #if (NIBBLE_DADOS)								
        //compila código para os pinos de dados do LCD nos 4 MSB do PORT
    	HAL_GPIO_WritePin(DADOS_LCD, 0x00F0 & (c << 4), GPIO_PIN_SET);
    #else
        //compila código para os pinos de dados do LCD nos 4 LSB do PORT
    	HAL_GPIO_WritePin(DADOS_LCD, 0x00F & c, GPIO_PIN_SET);
    #endif
    
    PULSO_ENABLE();
    
    //se for instrução de retorno ou limpeza espera LCD estar pronto
    if((cd==0) && (c<4))
    	HAL_Delay(2); // ms
}

/**
  * @brief  Configura hardware: verificar lcd.h para mapa de pinos e nible de dados.
  * @param	Nenhum
  *
  * @retval Nenhum.
  */
void inic_LCD_4bits()
{
    //sequência ditada pelo fabricando do circuito integrado HD44780
    //o LCD será só escrito. Então, R/W é sempre zero.
    
    /* Configura pinos de controle */
	HAL_GPIO_WritePin(CONTR_LCD, RS, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CONTR_LCD, E, GPIO_PIN_SET);

    //RS em zero indicando que o dado para o LCD será uma instrução
	HAL_GPIO_WritePin(CONTR_LCD, RS, GPIO_PIN_RESET);
    //pino de habilitação em zero
	HAL_GPIO_WritePin(CONTR_LCD, E, GPIO_PIN_RESET);
    
    //tempo para estabilizar a tensão do LCD, após VCC ultrapassar 4.5 V (na prática pode
    //ser maior). 
    HAL_Delay(20); // ms

    //habilitação respeitando os tempos de resposta do LCD
    PULSO_ENABLE();
    HAL_Delay(5);
    PULSO_ENABLE();
    atraso_us(200);
    PULSO_ENABLE();
    
    /* Até aqui ainda é uma interface de 8 bits.
    * Muitos programadores desprezam os comandos acima, respeitando apenas o tempo de
    * estabilização da tensão (geralmente funciona). Se o LCD não for inicializado primeiro no 
    * modo de 8 bits, haverá problemas se o microcontrolador for inicializado e o display já o tiver sido.*/
    
    //interface de 4 bits, deve ser enviado duas vezes (a outra está abaixo)
    #if (NIBBLE_DADOS)
    	HAL_GPIO_WritePin(DADOS_LCD, 0x0020, GPIO_PIN_SET);
    #else
    	HAL_GPIO_WritePin(DADOS_LCD, 0x0002, GPIO_PIN_SET);
    #endif
    
    PULSO_ENABLE();
    //interface de 4 bits 2 linhas (aqui se habilita as 2 linhas) 
    cmd_LCD(0x28,0);

    //são enviados os 2 nibbles (0x2 e 0x8)
    
    //desliga o display
    cmd_LCD(0x08,0);
    //limpa todo o display
    cmd_LCD(0x01,0);
    //mensagem aparente cursor inativo não piscando   
    cmd_LCD(0x0C,0);
    //inicializa cursor na primeira posição a esquerda - 1a linha
    cmd_LCD(0x80,0);
}

/**
  * @brief  Escreve um string estática (sem printf) no LCD. 
  * @param c: ponteiro para a string em RAM
  *
  * @retval Nenhum
  */
void escreve_LCD(char *c)
{
   for (; *c!=0;c++) cmd_LCD(*c,1);
}

/**
  * @brief Escreve um caractere no stream. Função utilizada pela libc.
  * @param c: caractere a ser escrito.
  * @param fp: ponteiro do stream inicilizado que receberá o dado.
  *
  * @retval int maior que zero se sucesso.
  */
int lcd_putchar(char c, FILE *fp){
	cmd_LCD(c,1);

	return 1;
}
