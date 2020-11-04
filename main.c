#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void USART1_Init(void);
void USART2_Init(void);
void NVIC_Configure(void); //interrupt priority

void Delay(void);

void sendDataUART(USART_TypeDef* USARTx, uint16_t data);

int sendFlag = 0; //User s1

//---------------------------------------------------------------------------------------------------

void RCC_Configure(void) //complete
{
   // TODO: Enable the APB2 peripheral clock using the function 'RCC_APB2PeriphClockCmd'
   
   
   /* Alternate Function IO clock enable */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //usart2
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //usart1
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); //interrupt
}

void GPIO_Configure(void) //complete
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // TODO: Initialize the GPIO pins using the structure 'GPIO_InitTypeDef' and the function 'GPIO_Init'
   
    
    /* UART1 pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* UART2 pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
   
}

void USART1_Init(void)
{
   USART_InitTypeDef USART1_InitStructure;
        
        USART1_InitStructure.USART_BaudRate = 9600;
   USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART1_InitStructure.USART_StopBits = USART_StopBits_1;
        USART1_InitStructure.USART_Parity = USART_Parity_No;
        USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART1_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_Init(USART1, &USART1_InitStructure);
        
   USART_Cmd(USART1, ENABLE);
         USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //RX를 인터럽트로 사용 설정
        // Enable the USART1 peripheral
   
   // TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
   // TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
   
}

void USART2_Init(void)
{
   USART_InitTypeDef USART2_InitStructure;
        
        USART2_InitStructure.USART_BaudRate = 9600;
   USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART2_InitStructure.USART_StopBits = USART_StopBits_1;
        USART2_InitStructure.USART_Parity = USART_Parity_No;
        USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
        USART_Init(USART2, &USART2_InitStructure);
        
        // Enable the USART1 peripheral
   USART_Cmd(USART2, ENABLE);
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //RX를 인터럽트로 사용 설정
   
   // TODO: Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
   // TODO: Enable the USART1 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
   
}

//interrupt간의 우선순위 결정
void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    
    // TODO: fill the arg you want
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

   // TODO: Initialize the NVIC using the structure 'NVIC_InitTypeDef' and the function 'NVIC_Init'
    // UART1
   // 'NVIC_EnableIRQ' is only required for USART setting
    //NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // TODO
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler() {
   unsigned char word;
    if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)!=RESET){
       // the most recent received data by the USART1 peripheral
        word = (unsigned char)USART_ReceiveData(USART1); //1문자씩 받는다(char 1개당 2byte)
        //스마트폰에 출력
        USART_SendData(USART2, word);
        // clear 'Read data register not empty' flag
       USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

void USART2_IRQHandler() {
   unsigned char word;
      if(USART_GetFlagStatus(USART2,USART_FLAG_TXE)!=RESET){
       // the most recent received data by the USART1 peripheral
        word = (unsigned char)USART_ReceiveData(USART2); //1문자씩 받는다(char 1개당 2byte) USART_GetFlagStatus
        //putty에출력
        USART_SendData(USART1, word);
        // clear 'Read data register not empty' flag
       USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}

void Delay(void) {
   int i;

   for (i = 0; i < 1000000; i++) {}
}

int main(void)
{

    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    USART1_Init();
    USART2_Init();
    NVIC_Configure();

    while (1) {     
    }
    return 0;
}