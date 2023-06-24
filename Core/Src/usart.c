/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
volatile uint8_t rxBuffer[BUFFER_SIZE];
volatile uint32_t rxIndex = 0;
volatile bool usartflag = false;
volatile uint32_t msg_length = 0;
/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  // Enable UART receive interrupt
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);

  /* USER CODE END USART2_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, USART_TX_Pin|USART_RX_Pin);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2)
  {
//    // Copy received data into the buffer
//    rxBuffer[rxIndex++] = huart2.Instance->DR;
//
//    // Check if the buffer is full
//    if (rxIndex >= BUFFER_SIZE)
//    {
//      // Handle buffer overflow error
//      // ...
//    }
//    HAL_UART_Transmit (&huart2, rxBuffer, rxIndex, 10);
  }
}
//
//void USART2_IRQHandler(void)
//{
//  /* USER CODE BEGIN USART1_IRQn 0 */
////    if ( USART1->ISR & UART_IT_TXE) {
////
////    }
////
////    if ( USART1->ISR & UART_IT_RXNE) {
////        HAL_UART_Receive_IT(&huart1,rx_data,buff_size_rx);
////        if(rx_data[pointer]=='\0') {
////              pointer=0;
////              readCommand(rx_data);
////              clearBuffer(rx_data,buff_size_rx);
////        } else {
////          pointer++;
////          if(pointer>=buff_size_rx) {
////              pointer=0;
////          }
////        }
////    }
////    /* USER CODE END USART1_IRQn 0 */
////    HAL_UART_IRQHandler(&huart1);
////    /* USER CODE BEGIN USART1_IRQn 1 */
////
//
//
//  /* USER CODE END USART1_IRQn 1 */
//}
bool hasNewMessage(void)
{
  if (usartflag)
  {
    // Reset the message complete flag
    usartflag = false;
    return true;
  }
  return false;
}
void ProcessReceivedData(volatile uint8_t* buffer, volatile uint16_t length)
{
  // Parse and process the received data here
  // Separate the numeric values (ints) and store them as needed
  // You can use strtok or sscanf functions to parse the buffer

  // Example: Assuming the received data format is "int1,int2,int3\n"
  int base, top, rising, falling, hold;
  sscanf((char*)buffer, "%d,%d,%d,%d,%d", &base, &top, &rising, &falling, &hold);
  SetCharacteristic(&characteristic_new, top, base, rising, falling, hold);

  // Use the parsed integer values as needed
  // ...
}
/* USER CODE END 1 */
