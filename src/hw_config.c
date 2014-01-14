/**
 ******************************************************************************
 * @file    hw_config.c
 * @author  MCD Application Team
 * @version V4.0.0
 * @date    21-January-2013
 * @brief   Hardware Configuration & Setup
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/

#include "stm32f30x_it.h"

#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
// EXTI_InitTypeDef EXTI_InitStructure;


static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
/* Extern variables ----------------------------------------------------------*/

extern LINE_CODING linecoding;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//void USB_Pins_Config(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//
//    /*Set PA11,12 as IN - USB_DM,DP*/
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//    /*SET PA11,12 for USB: USB_DM,DP*/
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_14);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_14);
//
//    /* USB_DISCONNECT used as USB pull-up */
//    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
//
//    /* Configure the EXTI line 18 connected internally to the USB IP */
//    EXTI_ClearITPendingBit(EXTI_Line18);
//    EXTI_InitStructure.EXTI_Line = EXTI_Line18;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);
//}



/*******************************************************************************
 * Function Name  : Set_USBClock
 * Description    : Configures USB Clock input (48MHz)
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
//void Set_USBClock(void)
//{
//    /* Select USBCLK source */
//    RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
//
//    /* Enable the USB clock */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
//}



/*******************************************************************************
 * Function Name  : Enter_LowPowerMode
 * Description    : Power-off system clocks and power while entering suspend mode
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Enter_LowPowerMode(void)
{
    /* Set the device state to suspend */
    bDeviceState = SUSPENDED;
}



/*******************************************************************************
 * Function Name  : Leave_LowPowerMode
 * Description    : Restores system clocks and power while exiting suspend mode
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
void Leave_LowPowerMode(void)
{
    DEVICE_INFO *pInfo = &Device_Info;

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);

    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();


    /* Wait till HSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
    {}

    /* Enable PLL1 */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL1 is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}

    /* Set the device state to the correct state */
    if (pInfo->Current_Configuration != 0)
    {
        /* Device configured */
        bDeviceState = CONFIGURED;
    }
    else
    {
        bDeviceState = ATTACHED;
    }
}



/*******************************************************************************
 * Function Name  : USB_Interrupts_Config
 * Description    : Configures the USB interrupts
 * Input          : None.
 * Return         : None.
 *******************************************************************************/
//void USB_Interrupts_Config(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure;
//
//    /* 2 bit for pre-emption priority, 2 bits for subpriority */
//    /* NOTE: This goes against the FreeRTOS requirements.... check and remove!!! */
//    /* NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); */
//
//    /* Enable the USB interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SYS_IRQ_PRIO_LOW;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//
//    /* Enable the USB Wake-up interrupt */
//    NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SYS_IRQ_PRIO_HIGH;
//    NVIC_Init(&NVIC_InitStructure);
//}



void NVIC_Configuration(void)
{
    /* This is required for FreeRTOS (Refer: http://www.freertos.org/RTOS-Cortex-M3-M4.html) */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* Set HCLK as the source for Systick */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}



/*******************************************************************************
 * Function Name  : USB_Cable_Config
 * Description    : Software Connection/Disconnection of USB Cable
 * Input          : None.
 * Return         : Status
 *******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
//    if (NewState != DISABLE)
//    {
//        GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
//    }
//    else
//    {
//        GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
//    }
}



/*******************************************************************************
 * Function Name  : Get_SerialNum.
 * Description    : Create the serial number string descriptor.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Get_SerialNum(void)
{
    uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

    Device_Serial0 = *(uint32_t*)ID1;
    Device_Serial1 = *(uint32_t*)ID2;
    Device_Serial2 = *(uint32_t*)ID3;

    Device_Serial0 += Device_Serial2;

    if (Device_Serial0 != 0)
    {
        IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
        IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
    }
}



/*******************************************************************************
 * Function Name  : HexToChar.
 * Description    : Convert Hex 32Bits value into char.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
    uint8_t idx = 0;

    for( idx = 0 ; idx < len ; idx ++)
    {
        if( ((value >> 28)) < 0xA )
        {
            pbuf[ 2* idx] = (value >> 28) + '0';
        }
        else
        {
            pbuf[2* idx] = (value >> 28) + 'A' - 10;
        }

        value = value << 4;

        pbuf[ 2* idx + 1] = 0;
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
