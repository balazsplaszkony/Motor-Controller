///*
// * usb.c
// *
// *  Created on: May 19, 2023
// *      Author: plasz
// */
//
//#include "usb.h"
//
//uint16_t rx_buffer_index = 0;
//uint8_t rx_complete = 0;
//uint8_t rx_buffer[BUFFER_SIZE] = {'\0'};
//void ProcessReceivedData(uint8_t* buffer, uint16_t length)
//{
//  // Parse and process the received data here
//  // Separate the numeric values (ints) and store them as needed
//  // You can use strtok or sscanf functions to parse the buffer
//
//  // Example: Assuming the received data format is "int1,int2,int3\n"
//  int base, top, rising, falling, hold;
//  sscanf((char*)buffer, "%d,%d,%d,%d,%d", &base, &top, &rising, &falling, &hold);
//  SetCharacteristic(&characteristic_new, top, base, rising, falling, hold);
//
//  // Use the parsed integer values as needed
//  // ...
//}
//
//void CDC_ReceiveCallback(uint8_t* buffer, uint32_t length)
//{
//  for (uint32_t i = 0; i < length; i++)
//  {
//    if (buffer[i] == '\n')
//    {
//      rx_buffer[rx_buffer_index] = '\0'; // Add null termination at the end of the received string
//      rx_complete = 1;
//    }
//    else
//    {
//      rx_buffer[rx_buffer_index] = buffer[i];
//      rx_buffer_index++;
//      if (rx_buffer_index >= BUFFER_SIZE)
//      {
//        // Buffer overflow, handle error or wrap around to start of buffer
//        rx_buffer_index = 0;
//      }
//    }
//  }
//}
