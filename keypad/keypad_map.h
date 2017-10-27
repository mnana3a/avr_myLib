
#ifndef __KEYPAD_MAP_H__
#define __KEYPAD_MAP_H__

#define ROW1 0
#define COL1 4

#define PORTx C
#define _CONCAT(a,b) a##b
#define PORT(x) _CONCAT(PORT,x)
#define PIN(x) _CONCAT(PIN,x)
#define DDR(x) _CONCAT(DDR,x)

#define KEYPAD_DDR  DDR(PORTx)
#define KEYPAD_PORT  PORT(PORTx)
#define KEYPAD_PIN  PIN(PORTx)

#ifdef __ASCII
unsigned char keypadMap[_MAX_KEYS] = {'1','2','3','A', 
                                      '4','5','6','B', 
                                      '7','8','9','C', 
                                      '*','0','#','D'};
#elif defined (__NUMERICAL)
unsigned char keypadMap[_MAX_KEYS] = {1, 2, 3, 0x0a, 
                                      4, 5, 6, 0x0b, 
                                      7, 8, 9, 0x0c, 
                                      0x0e, 0, 0x0f, 0x0d};
#endif

#endif
