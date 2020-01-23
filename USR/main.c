#include "DELAY/Delay.h"
#include "ADC/ADC.h"
#include "LED/LED.h"
#include "KEY/key.h"
#include "UART/uart.h"
#include "IIC/IIC.h"
#include "OLED/OLED.h"
#include "SPIx/SPIx.h"
#include "SX1278/SX1278.h"
#include "NodeBus.h"

DeviceBlock DeviceBlock_StructureArray[4];

int main(void)
{
	extern unsigned char receivebuffer[9];
    u8 sys=0;
    u8 i = 0;
 u8 j = 0;

	/*初始化各外设*/ 
    initSysTick();  
	initUART();
	initIIC();
    initOLED();
    initSPIx(SPI2);
    initSX1278();
	//initADC();
	//initLED();
	//initKey();

	formatScreen(0x00);
	showString(0,2,"Hum :",FONT_16_EN);
    showString(0,4,"Temp:",FONT_16_EN);
    showString(0,6,"Bpm :",FONT_16_EN);

    while (1)
    {
		
        if(++i==40) //3S采集一次从机数据,3.3s
        {
			j++;
			switch(j){
				case 10:{
			sendMasterAsk(SLAVE1_ADDR,OP_R_SENSOR,PRAM_R_ALL);//主机发送指令，DeviceBlock类型数组1
			sys=receiveSlaveAck(SLAVE1_ADDR,OP_R_SENSOR,PRAM_R_ALL,DeviceBlock_StructureArray);//接收从机数据			
			Delay_ms(100);
			while(sys != FRAME_OK && ++i<10)//等待从机响应
			{
				Delay_ms(100);
			}
			i = 0;
					   }break;
			case 20:{
			sendMasterAsk(SLAVE2_ADDR,OP_R_SENSOR,PRAM_R_ALL);//主机发送指令，DeviceBlock类型数组2
			sys=receiveSlaveAck(SLAVE2_ADDR,OP_R_SENSOR,PRAM_R_ALL,DeviceBlock_StructureArray);//接收从机数据			
			Delay_ms(100);
			while(sys != FRAME_OK && ++i<10)//等待从机响应
			{
				Delay_ms(100);
			}
			i = 0;
					}break;
			case 40:{
			sendMasterAsk(SLAVE3_ADDR,OP_R_SENSOR,PRAM_R_ALL);//主机发送指令，DeviceBlock类型数组3
			sys=receiveSlaveAck(SLAVE3_ADDR,OP_R_SENSOR,PRAM_R_ALL,DeviceBlock_StructureArray);//接收从机数据			
			Delay_ms(100);
		    while(sys != FRAME_OK && ++i<10)//等待从机响应
			{
				Delay_ms(100);
			}
			i = 0;
					}break;
			//i=0;
			}
		}
#if 0
        keyvalue = getKeyValue(KEY_RELEASE);//获取键值

        /*根据键值做出相应控制动作*/
        if(keyvalue==KEY_UP)
        {
			j=0;
			DeviceBlock_StructureArray[SLAVE1_ADDR].Coils ^= 0x01; 
            sendMasterAsk(SLAVE1_ADDR,OP_W_COILS,DeviceBlock_StructureArray[SLAVE1_ADDR].Coils);
			receiveSlaveAck(SLAVE1_ADDR,OP_R_SENSOR,PRAM_R_ALL,DeviceBlock_StructureArray);		
			Delay_ms(100);
			while(receiveSlaveAck(SLAVE1_ADDR,OP_W_COILS,0,DeviceBlock_StructureArray)!=FRAME_OK && ++j<30)
			{
				Delay_ms(100);
			}
        }else if(keyvalue==KEY_DOWN)
        {
			j=0;
			DeviceBlock_StructureArray[SLAVE1_ADDR].Coils ^= 0x02; 
            sendMasterAsk(SLAVE1_ADDR,OP_W_COILS,DeviceBlock_StructureArray[SLAVE1_ADDR].Coils);
			receiveSlaveAck(SLAVE1_ADDR,OP_R_SENSOR,PRAM_R_ALL,DeviceBlock_StructureArray);
			Delay_ms(100);
			while(receiveSlaveAck(SLAVE1_ADDR,OP_W_COILS,0,DeviceBlock_StructureArray)!=FRAME_OK && ++j<30)
			{
				Delay_ms(100);
			}
        }else if(keyvalue==KEY_ALL)
        {
			j=0;
			DeviceBlock_StructureArray[SLAVE1_ADDR].Coils ^= 0x03; 
            sendMasterAsk(SLAVE1_ADDR,OP_W_COILS,DeviceBlock_StructureArray[SLAVE1_ADDR].Coils);
			receiveSlaveAck(SLAVE1_ADDR,OP_R_SENSOR,PRAM_R_ALL,DeviceBlock_StructureArray);
			Delay_ms(100);
			while(receiveSlaveAck(SLAVE1_ADDR,OP_W_COILS,0,DeviceBlock_StructureArray)!=FRAME_OK && ++j<30)
			{
				Delay_ms(100);
			}
        }else
        {

        }
#endif
		j=0;j++;if(j==40){formatScreen(0x00);}
		//formatScreen(0x00);
        /*显示传感器数据*/
		showString(100,2,"Slave:",FONT_16_EN);showNumber(120,2,receivebuffer[1] ,DEC,3,FONT_16_EN);
		switch(sys){
			case 0:{
        showNumber(40,2,DeviceBlock_StructureArray[receivebuffer[1]].Humidity,DEC,3,FONT_16_EN);
        showNumber(40,4,DeviceBlock_StructureArray[receivebuffer[1]].Temperature,DEC,3,FONT_16_EN);
        showNumber(40,6,DeviceBlock_StructureArray[receivebuffer[1]].Bpm,DEC,5,FONT_16_EN);  
       // toggleLED();
		Delay_ms(50);}break;
		case 1:  formatScreen(0x00);showString(0,2,"NETADDR_ERR",FONT_16_EN);formatScreen(0x00);break;
		case 2:  formatScreen(0x00);showString(0,4,"SLAVEADDR_ERR",FONT_16_EN);formatScreen(0x00);break;
		case 3:  formatScreen(0x00);showString(0,6,"CRC_ERR",FONT_16_EN);formatScreen(0x00);break;
		case 255:  formatScreen(0x00);showString(0,6,"DATA_EMPTY",FONT_16_EN);formatScreen(0x00);break;
		default : formatScreen(0x00);showString(0,6,"UNKOWN_CODE",FONT_16_EN);formatScreen(0x00);break;
		}
	}
	}
		
		
