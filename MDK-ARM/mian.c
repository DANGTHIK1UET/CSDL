test_mode1.c
#include "stm32f10x.h"

// Define PPI8255 control pins
#define PPI8255_CTRL_PORT GPIOB
#define PPI8255_CTRL_PIN_WR 1<<4
#define PPI8255_CTRL_PIN_A0 1<<0
#define PPI8255_CTRL_PIN_A1 1<<1
#define PPI8255_CTRL_PIN_RD 1<<3

// Define PPI8255 data port
#define PPI8255_DATA_PORT GPIOA

void GPIO_Init(void);
void Delay_ms(uint32_t);
void EXTI_Config(void);
void EXTI15_10_IRQHandler(void);

void PPI_Control_Word(void);
void PPI8255_WriteData_A(uint8_t data);
void PPI8255_WriteData_B(uint8_t data);
void PPI8255_WriteData_C(uint8_t data);
void PPI8255_Read_Port(char port);
	
void PPI8255_Write(void);

void PPI8255_MODE0_A(void);
void PPI8255_MODE0_B(void);
void PPI8255_MODE1_A(void);
void PPI8255_MODE1_B(void);
void PPI8255_MODE2(void);

void PPI8255_Input_PortSet(char);
void PPI8255_Output_PortSet(char);

void BSR_Set(uint8_t);
void BSR_Reset(uint8_t);
void PinConf(void);

int main(void) {
    // Initialize GPIO pins
  GPIO_Init();
	EXTI_Config();
	
	PPI8255_MODE1_A();
	Delay_ms(100);
	PPI8255_Input_PortSet('A');
	Delay_ms(100);
	BSR_Set(6); // C6 = INTE A EN.
	Delay_ms(100);
	uint8_t data;
    while(1){
			
    }
}

void PPI8255_MODE0_A(void){
	PPI_Control_Word();
	PPI8255_DATA_PORT->ODR &= ~(0x3<<5); 
	Delay_ms(500);
	PPI8255_Write();
}

vo
