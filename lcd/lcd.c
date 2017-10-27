#include "lcd.h"
#include "lcd_interface.h"


void lcd_init(void)
{
    _delay_ms(15);                  // lcd power-up time
    LCD_DDR |= 0b11110111;          // set lcd port to be output
    LCD_PORT &= ~(1<<LCD_EN);       // clear the enable pin
    
    // NOTE: cant afford to remove those two lines
    // these two lines is to get the attention of the lcd controller
    lcd_command(0x33);
    lcd_command(0x32);
    
    // config the display for 4-bit mode
    lcd_command(LCD_4BIT);
    lcd_command(LCD_DISP_OFF);
    lcd_command(LCD_CURSOR_BLINK_ON);
    lcd_command(LCD_MODE_CURSOR_RIGHT);        // here display comes back on           
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
    _delay_us(100);     // t execution + tAh + tH
    // send the low nibble
    val0 = command << 4;
    val1 = LCD_PORT & 0X0F;
    LCD_PORT = val0 | val1;
    LCD_PORT |= (1<<LCD_EN);
    _delay_us(1);       // tPWH
    LCD_PORT &= ~(1<<LCD_EN);
    _delay_us(0.1);     // tH + tAH
    /*
    if(command == 0x01 || command == 0x02)          // command execution time
        _delay_ms(2);
    else
        _delay_us(100);
        * */
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
    //_delay_us(100);     // tH + tAH + execution time
    lcd_busy();     // waits for the lcd to finish
}

// scheme starts at [0,0]
void lcd_goxy(uint8_t x, uint8_t y)
{
    uint8_t addr[]= {LCD_LINE1, LCD_LINE2};     // first ddram addr. 0x00 which can be accessed by command 0b10000000
    lcd_command(LCD_USE_DDRAM | (addr[x]+(y)));    // 2nd line ddram addr. 0x40 which can be accessed by command 0b11000000
}


void lcd_disp(unsigned char *string)
{
    uint8_t i=0;
    
    // check to make sure we have a good pointer
    if(!string) return;
    
    while(*(string+i) != 0){
        lcd_data(*(string+i));
        i++;
        #ifdef __LCD_DISP_SEQ
            _delay_ms(300);
        #endif
    }
}

// NOTE: here the R/W line must not be grounded
// reads the lcd busy-flag and the lcd returns the location of the cursor
void lcd_busy(void)
{
    uint8_t status, temp, busy;
    //LCD_DDR = 0x0f;    // make pin-7 input from the mc perspective (the busy flag bit) --> d$
    LCD_DDR &= ~(1<<4);
    LCD_DDR &= ~(1<<5);
    LCD_DDR &= ~(1<<6);
    LCD_DDR &= ~(1<<7);

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
    LCD_DDR |= 0xf0;  
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
        value1 = (~value) + 1;      // take the positive part and overflow the sign
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
    value1 = value;     // the integer part of the number
    value2 = (value - value1)* 1000U;       // the fraction part of the number  up to 3 decimal places
    
    if(value == 0){
            lcd_data('0');
            lcd_data('.');
            lcd_data('0');
            return;
    }
    if(value < 0)
        Pvalue = (~value1) + 1;     // take the positive part and overflow the sign
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
        while(fraction[z] == 0) z++;
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

// this is the same as lcd_disp() but works with flash memory
void lcd_disp_flash(const uint8_t *flashLocation)
{
    uint8_t i=0;
    uint8_t string = 0;
    
    // check to make sure we have a good pointer
    if(!flashLocation) return;
    
    while((string = (uint8_t)pgm_read_byte(flashLocation+i)) != '\0'){
        lcd_data(string);
        i++;
        #ifdef __LCD_DISP_SEQ
            _delay_ms(300);
        #endif
    }
}


//defines char symbol in CGRAM
/*
const uint8_t backslash[] PROGMEM= 
{
0b00000000,//back slash
0b00010000,
0b00001000,
0b00000100,
0b00000010,
0b00000001,
0b00000000,
0b00000000
};
lcd_defineChar(backslash,0);
*/

#ifdef __USE_CUSTOM_CHAR
void lcd_customChars_init(void)
{
    //init 8 custom chars
    uint8_t ch = 0, chn = 0;
    
    while(ch < 64)
    {
        lcd_defineChar((lcdCustomChar+ch), chn++);
        ch = ch + 8;
    }
    //cursorloc = lcd_cursorLocation(); 
    //lcd_command(LCD_USE_DDRAM);
}

// NOTE: this works only with characters defined in the flash memory of the mc
void lcd_defineChar(const uint8_t *pc,uint8_t char_code)
{
    uint8_t CGRAMLocation, pcc;
    uint16_t i;
    
    CGRAMLocation = (char_code<<3) | 0x40;
    
    for (i = 0; i < 8; i++)
    {
        pcc = pgm_read_byte(&pc[i]);
        lcd_command(CGRAMLocation++);
        lcd_data(pcc);
    }
    lcd_command(LCD_USE_DDRAM);
}
#endif


uint8_t lcd_cursorLocation(void)
{
    uint8_t status, temp, cursorLocation;
    LCD_DDR &= ~(1<<4);
    LCD_DDR &= ~(1<<5);
    LCD_DDR &= ~(1<<6);
    LCD_DDR &= ~(1<<7);

    LCD_PORT &= ~(1<<LCD_RS);
    LCD_PORT |= (1<<LCD_RW);        // read mode
    _delay_us(0.5);                 // tAS from data sheet (140 ns min.)
    LCD_PORT |= (1<<LCD_EN);
    _delay_us(1);
    
    status = LCD_PIN & 0xf0;
    LCD_PORT &= ~(1<<LCD_EN);
    _delay_us(100);
    LCD_PORT |= (1<<LCD_EN);
    _delay_us(1);
    
    temp = LCD_PIN & 0xf0;
    temp = temp >> 4;
    cursorLocation = status | temp;
    cursorLocation &= 0x7F;
    LCD_PORT &= ~(1<<LCD_EN);
    _delay_us(100); 
    LCD_DDR |= 0xf0;
    return cursorLocation;  
}

// NOTE: this function depend on the native custom characters defined in this lib.
// this function draws a progress bar that can be used to make animation inside a for loop
// for(int i = 0; i < 10; i++)
//{
//      lcd_progressBar(i, 10, 16);
//       _delay_ms(700);
//      lcd_command(LCD_CLEAR);    
//}

// progress: defines the current progress
// maxprogress: defines the upper limit
// length: used to draw the bar
// the progress displayed as a percentage of the maxprogress to the length of the bar
// so if length = 5, maxprogress = 5; then each progress is displayed as a single segment
// but if length = 10, maxprogress = 5; then each progress is displayed a 2 segments
void lcd_progressBar(uint8_t progress, uint8_t maxprogress, uint8_t length)
{
    uint8_t i;
    uint16_t pixelprogress;
    uint8_t c;

    // draw a progress bar displaying (progress / maxprogress)
    // starting from the current cursor position
    // with a total length of "length" characters
    // ***note, LCD chars 0-5 must be programmed as the bar characters
    // char 0 = empty ... char 5 = full

    // total pixel length of bargraph equals length*PROGRESSPIXELS_PER_CHAR;
    // pixel length of bar itself is
    pixelprogress = ((progress*(length*PROGRESSPIXELS_PER_CHAR))/maxprogress);
    
    // print exactly "length" characters
    for(i=0; i<length; i++)
    {
        // check if this is a full block, or partial or empty
        // (u16) cast is needed to avoid sign comparison warning
        if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)+5) > pixelprogress )
        {
            // this is a partial or empty block
            if( ((i*(uint16_t)PROGRESSPIXELS_PER_CHAR)) > pixelprogress )
            {
                // this is an empty block
                // use space character?
                c = 0;
            }
            else
            {
                // this is a partial block
                c = pixelprogress % PROGRESSPIXELS_PER_CHAR;
            }
        }
        else
        {
            // this is a full block
            c = 5;
        }
        
        // write character to display
        lcd_data(c);
    }

}
