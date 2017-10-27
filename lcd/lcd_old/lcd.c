
#include "lcd.h"

/*
	dis_command(0x0C);		//display no cursor - no blink
	
	dis_command(0x06);		//DD RAM counter automatic increment - no display shift
	
	dis_command(0x80);		//address DD RAM with no offset
*/

void lcd_init(void)
{
  _delay_ms(2);		// lcd power-up time
	LCD_DDR = 0XFF;     // set lcd port to be output
	LCD_PORT &= ~(1<<LCD_EN);       // clear the enable pin
	lcd_command(0x33);
	lcd_command(0x32);
	lcd_command(0x28);
	lcd_command(0x0f);
	lcd_command(LCD_CLEAR);
	lcd_busy();
}

void lcd_command(uint8_t command)
{
	uint8_t val0, val1;     // vars used to manipulate the nibbles
	// send the high nibble
	val0 = command & 0xf0;
	val1 = LCD_PORT & 0x0f;
	LCD_PORT = val0 | val1;
	LCD_PORT &= ~(1<<LCD_RS);
	LCD_PORT &= ~(1<<LCD_RW);
	_delay_us(0.2);     // tAS from data sheet (140 ns min.)
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(1);       // tPWH from data sheet (450 ns min.)
	LCD_PORT &= ~(1<<LCD_EN);
	_delay_us(100);		// t execution + tAh + tH
	// send the low nibble
	val0 = command << 4;
	val1 = LCD_PORT & 0X0F;
	LCD_PORT = val0 | val1;
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(1);       // tPWH
	LCD_PORT &= ~(1<<LCD_EN);
	_delay_us(0.1);     // tH + tAH
	if(command == 0x01 || command == 0x02)          // command execution time
        _delay_ms(2);
    else
        _delay_us(100);
   lcd_busy();     // waits for the lcd to finish
}

void lcd_data(uint8_t data)
{
	uint8_t val0, val1;
	// send the high nibble
	val0 = data & 0xf0;
	val1 = LCD_PORT & 0x0f;
	LCD_PORT = val0 | val1;
	LCD_PORT |= (1<<LCD_RS);
	LCD_PORT &= ~(1<<LCD_RW);
    _delay_us(0.2);     // tAS from data sheet (140 ns min.)
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_EN);
    _delay_us(100);     // tAH + tH from data sheet (10 ns min.) + (10 ns min.)
	// send the low nibble
	val0 = data << 4;
	val1 = LCD_PORT & 0X0F;
	LCD_PORT = val0 | val1;
	LCD_PORT |= (1<<LCD_EN);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_EN);
	_delay_us(100);     // tH + tAH + execution time
	lcd_busy();     // waits for the lcd to finish
}

void lcd_goxy(uint8_t x, uint8_t y)
{
	uint8_t addr[]= {LCD_LINE1, LCD_LINE2};			// first ddram addr. 0x00 which can be accessed by command 0b10000000
	lcd_command(addr[x-1]+(y-1));			// 2nd line ddram addr. 0x40 which can be accessed by command 0b11000000
}


void lcd_disp(unsigned char *string)
{
	uint8_t i=0;
	while(*(string+i) != 0){
		lcd_data(*(string+i));
		i++;
		//_delay_ms(300);       // tag this if you want the string to show sequentially
	}
}


void lcd_busy(void)
{
    // here we will make the lcd output and check the busy flag of it
	
    uint8_t status, temp, busy;
    LCD_DDR = 0x0f;    // make pin-7 input from the mc perspective (the busy flag bit) --> d$
    LCD_PORT &= ~(1<<LCD_RS);
	LCD_PORT |= (1<<LCD_RW);        // read mode
	_delay_us(0.5);     // tAS from data sheet (140 ns min.)
	do
    {
        LCD_PORT |= (1<<LCD_EN);
        _delay_us(1);
        status = LCD_PIN & 0xf0;
        LCD_PORT &= ~(1<<LCD_EN);
		_delay_us(100);
		
		LCD_PORT |= (1<<LCD_EN);
		_delay_us(1);
		temp = LCD_PIN & 0xf0;
		temp = temp >> 4;
		busy = status | temp;
		LCD_PORT &= ~(1<<LCD_EN);
		_delay_us(100);	
    }while(busy & 0b10000000);
    LCD_DDR = 0xff;  
}

// the data sent to the lcd must be ASCII coded so if we provide input as integer we must convert it first to ASCII
void lcd_int(int32_t value, int8_t field_length)
{
    // the number max digits is 5
    // must provide the length of the display to be displayed on
    // if the field is -1 then it will disp the actual digits
    // else the digits is displayed and the rest is 0 (max field is 5)
    uint8_t str[8] = {0,0,0,0,0};
	uint8_t i = 7, j = 0;
	uint32_t value1;
	if(value == 0){
			lcd_data('0');
			return;
	}
	if(value < 0)
		value1 = (~value) + 1;		// take the positive part and overflow the sign
		// or just value * -1 ;
	else
		value1 = value;
	while( value1 ){
        str[i] = value1 % 10;
        value1 = value1 / 10;
        i--;
	}   // now we have the single digits of the int number, next we convert each to ascii and send it one by one
	if(field_length == -1){
        while(str[j]==0)    j++;
	}
	else{
        j = 8 - field_length;
	}
	if(value < 0)   lcd_data('-');
	for(i = j;i < 8;i++){
        lcd_data(48+str[i]);
	}
}


void lcd_float(float value, int8_t field_length)
{
    // the number max digits is 5
    // must provide the length of the display to be displayed on
    // if the field is -1 then it will disp the actual digits
    // else the digits is displayed and the rest is 0 (max field is 5)
    uint8_t exponent[8] = {0,0,0,0,0};
    uint8_t fraction[4] = {0,0,0};
	uint8_t i = 7, j = 0,k=3,z=0;
	uint32_t value1;
	uint32_t Pvalue;
	uint16_t value2=0;
	value1 = value;		// the integer part of the number
	value2 = (value - value1)* 1000U;		// the fraction part of the number	up to 3 decimal places
	
	if(value == 0){
			lcd_data('0');
			lcd_data('.');
			lcd_data('0');
			return;
	}
	if(value < 0)
		Pvalue = (~value1) + 1;		// take the positive part and overflow the sign
	else
		Pvalue = value1;
	while( Pvalue ){
        exponent[i] = Pvalue % 10;
        Pvalue = Pvalue / 10;
        i--;
	}   // now we have the single digits of the int number, next we get the mantissa part
	while( value2 ){
        fraction[k] = value2 % 10;
        value2 = value2 / 10;
        k--;
	}   // now we have the single digits of the mantissa number, next we convert each to ascii and send it one by one
	
	if(field_length == -1){
        while(exponent[j]==0)    j++;
		while(fraction[z] == 0)	z++;
	}
	else{
        j = 8 - field_length;
	}
	if(value < 0)   lcd_data('-');
	for(i = j;i < 8;i++){
        lcd_data(48+exponent[i]);
	}
	lcd_data('.');
	if(z == 4){
		lcd_data('0');
		lcd_data('0');
	}
	else {
		for(i = z;i < 4;i++){
			lcd_data(48+fraction[i]);
		}
	}
}


