/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

File    : main.c
Autor   : strefapic.blogspot.com
MCU     : STM32G071KBT6

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stm32g071xx.h>

void PinInit(void);

int main(void) {

SystemInit();
RCC->IOPENR |= RCC_IOPENR_GPIOAEN; //Open clock for GPIOA
RCC->IOPENR |= RCC_IOPENR_GPIOBEN; //Open clock for GPIOB
PinInit();
NVIC_SetPriority(EXTI0_1_IRQn, 0);
NVIC_EnableIRQ(EXTI0_1_IRQn);
NVIC_SetPriority(EXTI4_15_IRQn, 0);
NVIC_EnableIRQ(EXTI4_15_IRQn);

while(1){}

}

void PinInit(void){

/********** PA8 set Out for LED **********************/
GPIOA->MODER &= ~GPIO_MODER_MODE8_1 ;  //MODE8 -> 0b01

/********** PA1 set for EXTI ************/
/*set Input (0b00)*/
GPIOA->MODER &= ~(GPIO_MODER_MODE1_0 | GPIO_MODER_MODE1_1); 
/*set low speed (0b01)*/
GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED1_0 ;
GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED1_1 ;
/*set Pull-down (0b10)*/
GPIOA->PUPDR |= GPIO_PUPDR_PUPD1_1 ;
GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1_0;
/*set EXTI for PA1 - 1(EXTI1) PA(0b00)*/
EXTI->EXTICR[0] &= ~EXTI_EXTICR1_EXTI1 ;
/*set EXTI Rising triger for Line no. 1 (PA1)*/
EXTI->RTSR1 |= EXTI_RTSR1_RT1 ;
/*set EXTI Interrupt no-masked for Line no.1 (PA1)*/
EXTI->IMR1 |= EXTI_IMR1_IM1 ;

/********** PB9 set for EXTI ************/
/*set Input (0b00)*/
GPIOB->MODER &= ~(GPIO_MODER_MODE9_0 | GPIO_MODER_MODE9_1); 
/*set low speed (0b01)*/
GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED9_0 ;
GPIOB->OSPEEDR &= ~GPIO_OSPEEDR_OSPEED9_1 ;
/*set Pull-up (0b01)*/
GPIOB->PUPDR |= GPIO_PUPDR_PUPD9_0 ;
GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD9_1;
/*set EXTI for PB9 - 9(EXTI9) PB(0b01)*/
EXTI->EXTICR[2] &= ~EXTI_EXTICR3_EXTI9 ; //clear
EXTI->EXTICR[2] |=  EXTI_EXTICR3_EXTI9_0 ; //set 0b01
/*set EXTI Falling triger for Line no. 9 (PB9)*/
EXTI->FTSR1 |= EXTI_FTSR1_FT9 ;
/*set EXTI Interrupt no-masked for Line no.9 (PB9)*/
EXTI->IMR1 |= EXTI_IMR1_IM9 ;
}

/*Handling Interrupt from Line 0..1*/
void EXTI0_1_IRQHandler(void){
if(EXTI->RPR1 & EXTI_RPR1_RPIF1){ //interrupt pending from Line no.1 (Rising)?
EXTI->RPR1 |= EXTI_RPR1_RPIF1; //clear pending
GPIOA->ODR |= GPIO_ODR_OD8; //LED ON
}
}
/*Handling Interrupt from Line 4..15*/
void EXTI4_15_IRQHandler(void){
if(EXTI->FPR1 & EXTI_FPR1_FPIF9){//interrupt pending from Line no.9 (Falling) ?
EXTI->FPR1 |= EXTI_FPR1_FPIF9; //clear pending
GPIOA->ODR &= ~GPIO_ODR_OD8; //LED OFF
}
}