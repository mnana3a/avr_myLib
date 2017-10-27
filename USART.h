#ifndef UART_H_
#define UART_H_

#ifndef DATA_TYPES
#define DATA_TYPES
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
#endif

/*ADDRESSES*/
#define ADD_BH 0X40		//USART BAUD RATE REGISTER
#define ADD_BL 0X29		//..
#define ADD_A 0X2B		//USART CONTROL REGISTER A
#define ADD_B 0X2A		//.. B
#define ADD_C 0X40		//.. C
#define ADD_DR 0X2C		//USART DATA REGISTER
/*REGISTERS*/
#define BAUD_H *(u8* const)ADD_BH	//POINTER TO USART BAUD_RATE REGISTER H
#define BAUD_L *(u8* const)ADD_BL	//..	L
#define CONTROL_A *(volatile u8* const)ADD_A//POINTER TO VOLATILE CHAR POINTS TO USART CONTROL REGISTER A
#define CONTROL_B *(u8* const)ADD_B			//POINTER TO CHAR POINTS TO USART CONTROL REGISTER B
#define CONTROL_C *(u8* const)ADD_C			//.. C
#define DATA_REG *(volatile u8* const)ADD_DR	//POINTER TO VOLATILE CHAR POINTS TO USART DATA REGISTER

#define SET_BIT(REG,POS) REG|=(1<<POS)	//SET BIT
#define CLR_BIT(REG,POS) REG&=~(1<<POS)	//CLEAR BIT
#define TOG_BIT(REG,POS) REG^=(1<<POS)	//TOGGLE BIT

/*CONTROL REGISTER A*/
#define RX_COMP 7		//RX COMPLETE FLAG
#define TX_COMP 6		//TX COMP..
#define DATA_REG_EMPTY 5//USART DATA REGISTER EMPTY FLAG
#define FE 4			//FRAME ERROR FLAG
#define DOR 3			//DATA OVER RUN FLAG
#define PE 2			//PARITY ERROR FLAG
/*CONTROL REGISTER B*/
#define RX_IEN 7		//RX COMPLETE INTERRUPT ENABLE
#define TX_IEN 6		//TX COMP..
#define UDR_EMPTY_IEN 5	//USART DATA REGISTER EMPTY INTERRUPT ENABLE
#define RX_EN 4			//ENABLE TRANSMITTER
#define TX_EN 3			//.. RECIEVER
/*CONTROL REGISTER C*/
#define REG_SEL 7		//REGISTER SELECT 0-BAUD_H, 1-CR_C
#define MOD_SEL 6		//SELECT BETWEEN ASYNCH-0, SYNCH-1
#define PARITY_M1 5		//SET PARITY MODE 00-DIS, 10-EVEN, 11-ODD
#define PARITY_M0 4		
#define STOP_BIT 3		//STOP BITS MODE 0-ONE_BIT, 1-TWO_BITS
#define FRAME_SIZE_2 2	//FRAME SIZE SETTING BITS
#define FRAME_SIZE_1 1
#define FRAME_SIZE_0 0

#define BAUD_RATE 3		//BAUD RATE 250K @16MHz CLOCK
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

void Initialize_USART(void);
void Send_by_USART(u8 data);
u8 Receive_from_USART(void);

#endif /* UART_H_ */