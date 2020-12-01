/***************************************************************************//**
 *   @file   AD7792.c
 *   @brief  Implementation of AD7792 Driver.
 *   @author Bancisor MIhai
********************************************************************************
 * Copyright 2012(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
********************************************************************************
 *   SVN Revision: 501
*******************************************************************************/

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "ad7792.h"
#include "stdio.h"
#include "stm32f2xx.h"
#include "delay.h"

#define AD7792_DEBUG(fmt, args...)    printf(fmt, ##args)
//#define AD7792_DEBUG(fmt, args...) 

#define ADI_PART_CS_LOW         GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define ADI_PART_CS_HIGH        GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define AD7792_RDY_STATE        GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)
#define SCLOCK1                 GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define SCLOCK0                 GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define CS1                     ADI_PART_CS_HIGH
#define CS0                     ADI_PART_CS_LOW
#define MOSI1                   GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define MOSI0                   GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define MISO                    AD7792_RDY_STATE
#define AD7792_ClrRst()	
#define AD7792_SetRst()
#define AD7792_SPI_DELAY   delay_us(50)

unsigned char spi_gpio_readwrite_byte(unsigned char bytedata)
{
    unsigned char temp = 0x00;
    int i;

    for (i = 0; i < 8; i++) {
        if(bytedata & 0x80)     
            MOSI1;   
        else
            MOSI0;
        SCLOCK0;
        AD7792_SPI_DELAY;
        temp = temp << 1;
        if (MISO)
            temp |= 0x01;
        SCLOCK1;
        AD7792_SPI_DELAY;
        bytedata = bytedata << 1;
    }

    return temp;
}

unsigned char lowlevel_init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockPha)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
	/* CS  MOSI  CLK */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* DRDY/MISO */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADI_PART_CS_HIGH;
    SCLOCK1;

    return 1;
}

unsigned char ad7792_write(unsigned char* data,
                        unsigned char bytesNumber)
{
	unsigned char chipSelect    = data[0];
    unsigned char byte          = 0;

    if (chipSelect == 1)
        ADI_PART_CS_LOW;

    for(byte = 0; byte < bytesNumber; byte++)
        spi_gpio_readwrite_byte(data[byte + 1]);

    if (chipSelect == 1)
        ADI_PART_CS_HIGH;

	return(bytesNumber);
}

unsigned char ad7792_read(unsigned char* data,
                       unsigned char bytesNumber)
{
 	unsigned char chipSelect    = data[0];
    unsigned char byte          = 0;

    if (chipSelect == 1)
        ADI_PART_CS_LOW;

    for (byte = 0; byte < bytesNumber; byte++)
        data[byte] = spi_gpio_readwrite_byte(data[byte + 1]);

    if (chipSelect == 1)
        ADI_PART_CS_HIGH;
    
	return(bytesNumber);
}

/***************************************************************************//**
 * @brief Initializes the AD7792 and checks if the device is present.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful (ID is 0x0B).
 *                           0 - if initialization was unsuccessful.
*******************************************************************************/
unsigned char AD7792_Init(void)
{ 
	unsigned char status = 0x1;
    unsigned char id;
    
    lowlevel_init(0, 1000000, 1, 1);
    AD7792_Reset();
    id = AD7792_GetRegisterValue(AD7792_REG_ID, 1, 1) & 0x0f;
    if(id != AD7792_ID) {
		AD7792_DEBUG("%s: read id fail, id = 0x%02x\r\n", __FUNCTION__, id);
        status = 0x0;
	} else {
        AD7792_DEBUG("%s: AD7792 init succeed!\r\n", __FUNCTION__);
    }
    
	return(status);
}

/***************************************************************************//**
 * @brief Sends 32 consecutive 1's on SPI in order to reset the part.
 *
 * @return  None.    
*******************************************************************************/
void AD7792_Reset(void)
{
	unsigned char dataToSend[5] = {0x0, 0xff, 0xff, 0xff, 0xff};
	
    ADI_PART_CS_LOW;
	ad7792_write(dataToSend, 4);
	ADI_PART_CS_HIGH;
}
/***************************************************************************//**
 * @brief Reads the value of the selected register
 *
 * @param regAddress - The address of the register to read.
 * @param size - The size of the register to read.
 *
 * @return data - The value of the selected register register.
*******************************************************************************/
unsigned long AD7792_GetRegisterValue(unsigned char regAddress, 
                                      unsigned char size,
                                      unsigned char modifyCS)
{
	unsigned char data[5]      = {0x00, 0x00, 0x00, 0x00, 0x00};
	unsigned long receivedData = 0x00;
    unsigned char i            = 0x00; 
    
	data[0] = modifyCS;
	data[1] = AD7792_COMM_READ |  AD7792_COMM_ADDR(regAddress); 
	ad7792_read(data,(1 + size));
	for(i = 1;i < size + 1;i ++)
    {
        receivedData = (receivedData << 8) + data[i];
    }
    
    return (receivedData);
}
/***************************************************************************//**
 * @brief Writes the value to the register
 *
 * @param -  regAddress - The address of the register to write to.
 * @param -  regValue - The value to write to the register.
 * @param -  size - The size of the register to write.
 *
 * @return  None.    
*******************************************************************************/
void AD7792_SetRegisterValue(unsigned char regAddress,
                             unsigned long regValue, 
                             unsigned char size,
                             unsigned char modifyCS)
{
	unsigned char data[5]      = {0x00, 0x00, 0x00, 0x00, 0x00};	
	unsigned char* dataPointer = (unsigned char*)&regValue;
    unsigned char bytesNr      = size + 1;
    
    data[0] = modifyCS;
    data[1] = AD7792_COMM_WRITE |  AD7792_COMM_ADDR(regAddress);
    while(bytesNr > 1)
    {
        data[bytesNr] = *dataPointer;
        dataPointer ++;
        bytesNr --;
    }	    
	ad7792_write(data,(1 + size));
}
/***************************************************************************//**
 * @brief  Waits for RDY pin to go low.
 *
 * @return None.
*******************************************************************************/
void AD7792_WaitRdyGoLow(void)
{
    unsigned long status;

    while(1) {
        status = AD7792_GetRegisterValue(AD7792_REG_STAT, 1, 1);
        if ((status & 0x80) == 0x0)
            return;
        printf("status = %x\r\n", status);
    }
}

/***************************************************************************//**
 * @brief Sets the operating mode of AD7792.
 *
 * @param mode - Mode of operation.
 *
 * @return  None.    
*******************************************************************************/
void AD7792_SetMode(unsigned long mode)
{
    unsigned long command;
    
    command = AD7792_GetRegisterValue(AD7792_REG_MODE,
                                      2,
                                      1); // CS is modified by SPI read/write functions.
    command &= ~AD7792_MODE_SEL(0xFF);
    command |= AD7792_MODE_SEL(mode);
    AD7792_SetRegisterValue(
            AD7792_REG_MODE,
            command,
            2, 
            1); // CS is modified by SPI read/write functions.
}

void AD7792_SetUpdateRate(unsigned long rate)
{
    unsigned long command;
    
    command = AD7792_GetRegisterValue(AD7792_REG_MODE,
                                      2,
                                      1); // CS is modified by SPI read/write functions.
    command &= ~AD7792_UPDATE_RATE(0xF);
    command |= AD7792_UPDATE_RATE(rate);
    AD7792_SetRegisterValue(
            AD7792_REG_MODE,
            command,
            2, 
            1); // CS is modified by SPI read/write functions.
}
/***************************************************************************//**
 * @brief Selects the channel of AD7792.
 *
 * @param  channel - ADC channel selection.
 *
 * @return  None.    
*******************************************************************************/
void AD7792_SetChannel(unsigned long channel)
{
    unsigned long command;
    
    command = AD7792_GetRegisterValue(AD7792_REG_CONF,
                                      2,
                                      1); // CS is modified by SPI read/write functions.
    command &= ~AD7792_CONF_CHAN(0xFF);
    command |= AD7792_CONF_CHAN(channel);
    AD7792_SetRegisterValue(
            AD7792_REG_CONF,
            command,
            2,
            1); // CS is modified by SPI read/write functions.
}

/***************************************************************************//**
 * @brief  Sets the gain of the In-Amp.
 *
 * @param  gain - Gain.
 *
 * @return  None.    
*******************************************************************************/
void AD7792_SetGain(unsigned long gain)
{
    unsigned long command;
    
    command = AD7792_GetRegisterValue(AD7792_REG_CONF,
                                      2,
                                      1); // CS is modified by SPI read/write functions.
    command &= ~AD7792_CONF_GAIN(0xFF);
    command |= AD7792_CONF_GAIN(gain);
    AD7792_SetRegisterValue(
            AD7792_REG_CONF,
            command,
            2,
            1); // CS is modified by SPI read/write functions.
}
/***************************************************************************//**
 * @brief Sets the reference source for the ADC.
 *
 * @param type - Type of the reference.
 *               Example: AD7792_REFSEL_EXT	- External Reference Selected
 *                        AD7792_REFSEL_INT	- Internal Reference Selected.
 *
 * @return None.    
*******************************************************************************/
void AD7792_SetIntReference(unsigned char type)
{
    unsigned long command = 0;
    
    command = AD7792_GetRegisterValue(AD7792_REG_CONF,
                                      2,
                                      1); // CS is modified by SPI read/write functions.
    command &= ~AD7792_CONF_REFSEL(AD7792_REFSEL_INT);
    command |= AD7792_CONF_REFSEL(type);
    AD7792_SetRegisterValue(AD7792_REG_CONF,
							command,
							2,
                            1); // CS is modified by SPI read/write functions.
}

/***************************************************************************//**
 * @brief Performs the given calibration to the specified channel.
 *
 * @param mode - Calibration type.
 * @param channel - Channel to be calibrated.
 *
 * @return none.
*******************************************************************************/
void AD7792_Calibrate(unsigned char mode, unsigned char channel)
{
    unsigned short oldRegValue = 0x0;
    unsigned short newRegValue = 0x0;
    
    AD7792_SetChannel(channel);
    oldRegValue &= AD7792_GetRegisterValue(AD7792_REG_MODE, 2, 1); // CS is modified by SPI read/write functions.
    oldRegValue &= ~AD7792_MODE_SEL(0x7);
    newRegValue = oldRegValue | AD7792_MODE_SEL(mode);
    ADI_PART_CS_LOW; 
    AD7792_SetRegisterValue(AD7792_REG_MODE, newRegValue, 2, 0); // CS is not modified by SPI read/write functions.
    AD7792_WaitRdyGoLow();
    ADI_PART_CS_HIGH;
    
}

/***************************************************************************//**
 * @brief Returns the result of a single conversion.
 *
 * @return regData - Result of a single analog-to-digital conversion.
*******************************************************************************/
unsigned long AD7792_SingleConversion(void)
{
    unsigned long command = 0x0;
    unsigned long regData = 0x0;
    
    command  = AD7792_MODE_SEL(AD7792_MODE_SINGLE);
    //ADI_PART_CS_LOW;
    AD7792_SetRegisterValue(AD7792_REG_MODE, 
                            command,
                            2,
                            1);// CS is not modified by SPI read/write functions.
    AD7792_WaitRdyGoLow();
    regData = AD7792_GetRegisterValue(AD7792_REG_DATA, 2, 1); // CS is not modified by SPI read/write functions.
    //ADI_PART_CS_HIGH;

    return(regData);
}

/***************************************************************************//**
 * @brief Returns the average of several conversion results.
 *
 * @return samplesAverage - The average of the conversion results.
*******************************************************************************/
unsigned long AD7792_ContinuousReadAvg(unsigned char sampleNumber)
{
    unsigned long samplesAverage = 0x0;
    unsigned long command        = 0x0;
    unsigned char count          = 0x0;
    
    command = AD7792_MODE_SEL(AD7792_MODE_CONT);
    ADI_PART_CS_LOW;
    AD7792_SetRegisterValue(AD7792_REG_MODE,
                            command, 
                            2,
                            0);// CS is not modified by SPI read/write functions.
    for(count = 0;count < sampleNumber;count ++)
    {
        AD7792_WaitRdyGoLow();
        samplesAverage += AD7792_GetRegisterValue(AD7792_REG_DATA, 2, 0);  // CS is not modified by SPI read/write functions.
    }
    ADI_PART_CS_HIGH;
    samplesAverage = samplesAverage / sampleNumber;
    
    return(samplesAverage);
}

void ad7792_get_volt(void)
{
    unsigned long samplesAverage = 0x0;
    unsigned short volt;
    float diff, result;
    unsigned long regValue = 0xD;
    AD7792_SetRegisterValue(AD7792_REG_IO, regValue, 1, 1);
    AD7792_SetGain(AD7792_GAIN_1);
    AD7792_SetChannel(AD7792_CH_AIN2P_AIN2M);
    // AD7792_SetIntReference(AD7792_REFSEL_INT);
    // AD7792_Calibrate(AD7792_MODE_CAL_INT_ZERO, AD7792_CH_AIN1P_AIN1M);
    // delay_us_ms(50);
    // AD7792_Calibrate(AD7792_MODE_CAL_INT_FULL, AD7792_CH_AIN1P_AIN1M);
    // delay_us_ms(50);
    AD7792_SetIntReference(AD7792_REFSEL_EXT);
    while (1) {
        samplesAverage = AD7792_SingleConversion();
        volt = (unsigned short)(samplesAverage & 0xffff);
        if (volt >= 0x8000)
            diff = (float)(volt - 0x8000);
        else
            diff = -(float)(0x8000 - volt);
        result = (diff / 0x8000) * 2500;
        printf("%s: 1 read value 0x%x, volt = %f\r\n", __FUNCTION__, volt, result);
        delay_ms(1000);
    }
}



unsigned char DataRead[3];





void WriteToReg(unsigned char ByteData) // write ByteData to the register
{
	unsigned char temp;
	int i;	
	ADI_PART_CS_LOW;
	temp=0x80;
	for(i=0;i<8;i++)
	{
 		if((temp & ByteData)==0)
		{		
      		MOSI0;
		}	
 		else
		{
			 MOSI1;
     	}
		SCLOCK0;
		delay_us(10);
	   	SCLOCK1;
		delay_us(10);
 		temp=temp>>1;
	}
	ADI_PART_CS_HIGH;
}


void ReadFromReg(unsigned char nByte) // nByte is the number of bytes which need to be read
{
	int i,j;
   	unsigned char temp;
   	MOSI1;
 	ADI_PART_CS_LOW;
    temp=0;

	for(i=0; i<nByte; i++)
	{
		for(j=0; j<8; j++)
	    {
	     	SCLOCK0;
	     	if(MISO==0)
	     	{
				temp=temp<<1;
		 	}else
		 	{
				temp=temp<<1;
		 		temp=temp+0x01;
			}
			delay_us(10);
	        SCLOCK1;
			delay_us(10);
		  }
		  DataRead[i]=temp;
		  temp=0;
	}
    ADI_PART_CS_HIGH;
}

void just_test()
{

	int ResetTime;
    uint16_t volt = 0;
    float diff, result, ref;
	
	printf("Hello\r\n");

	/* PRECONFIGURE...*/
	ResetTime=32;
	SCLOCK1;

 	ADI_PART_CS_LOW;		  //to keep MOSI1 for 32 sclock to reset the part
 	MOSI1;
 	while(ResetTime--)
	{
		delay_us(10);
		SCLOCK0;
		delay_us(10);
 		SCLOCK1;
	}
 	ADI_PART_CS_HIGH;	
	
    printf("Reset\r\n");

    WriteToReg(0x60);
    ReadFromReg(1);

    printf("ID = %x\r\n", DataRead[0]);
	
    // internal ref, channel=3, gain=1
	WriteToReg(0x10); //write to Communication register.The next step is writing to Configuration register.
	WriteToReg(0x00); //set the Configuration bipolar mode.Gain=1.
	WriteToReg(0x80); //Configuration internal reference selected.

    WriteToReg(0x28);
    WriteToReg(0x2);

    // waiting for convert flat.
    ResetTime = 300;
    while(ResetTime--) {
		WriteToReg(0x08);//write to Communication register.The next step is writing to Mode register.
		WriteToReg(0x20);//set the mode register as single conversion mode.
		WriteToReg(0x0A);//inter 64 kHZ clock.internal clock is not available at the clk pin.

        while (MISO);

		WriteToReg(0x58);//write to Communication register.The next step is to read from Data register.
		ReadFromReg(2);
        volt = DataRead[0];
        volt <<= 8;
        volt |= DataRead[1];
        if (volt >= 0x8000)
            diff = (float)(volt - 0x8000);
        else
            diff = -(float)(0x8000 - volt);
        result = (diff / 0x8000) * 1170;
        printf("%s: read value 0x%x, volt = %f\r\n", __FUNCTION__, volt, result);
        delay_ms(1000);
    }
   
    ref = (diff / 0x8000) * 1170.0;
    printf("get ref = %f\r\n", ref);

    printf("read channel 1\r\n");
    WriteToReg(0x10); //write to Communication register.The next step is writing to Configuration register.
	WriteToReg(0x00); //set the Configuration bipolar mode.Gain=1.
	WriteToReg(0x00); //Configuration internal reference selected.

    while (1) {
        WriteToReg(0x08);//write to Communication register.The next step is writing to Mode register.
		WriteToReg(0x20);//set the mode register as single conversion mode.
		WriteToReg(0x0A);//inter 64 kHZ clock.internal clock is not available at the clk pin.

        while (MISO);

		WriteToReg(0x58);//write to Communication register.The next step is to read from Data register.
		ReadFromReg(2);
        volt = DataRead[0];
        volt <<= 8;
        volt |= DataRead[1];
        if (volt >= 0x8000)
            diff = (float)(volt - 0x8000);
        else
            diff = -(float)(0x8000 - volt);
        result = (diff / 0x8000) * ref;
        printf("%s: read value 0x%x, volt = %f\r\n", __FUNCTION__, volt, result);
        delay_ms(2000);
	}


	// while(1)
	// {
	
	// 	WriteToReg(0x08);//write to Communication register.The next step is writing to Mode register.
	// 	WriteToReg(0x20);//set the mode register as single conversion mode.
	// 	WriteToReg(0x0A);//inter 64 kHZ clock.internal clock is not available at the clk pin.
		
	   
		
		
	// 	// WriteToReg(0x40);//write to Communication register.The next step is to read from Status register.
	// 	// ReadFromReg(1);	
	// 	// while((DataRead[0]&0x80)==0x80)//wait for the end of convertion by polling the status register RDY bit
	 
	// 	// {
		  
	// 	// 	WriteToReg(0x40); 
	// 	// 	ReadFromReg(1);	
	// 	// }

    //     while (MISO);

    //     //printf("READ status = %x\r\n", DataRead[0]);
		 
		 
		
	// 	WriteToReg(0x58);//write to Communication register.The next step is to read from Data register.
	// 	ReadFromReg(2);

    //     volt = DataRead[0];
    //     volt <<= 8;
    //     volt |= DataRead[1];
    //     printf("VOLT = %x\r\n", volt);
    //     if (volt >= 0x8000)
    //         diff = (float)(volt - 0x8000);
    //     else
    //         diff = -(float)(0x8000 - volt);
    //     result = (diff / 0x8000) * 1170;
    //     printf("%s: read value 0x%x, volt = %f\r\n", __FUNCTION__, volt, result);
    //     delay_ms(2000);
	// }

}    
