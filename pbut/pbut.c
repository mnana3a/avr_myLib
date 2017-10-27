#include "pbut.h"


uint8_t bFlag = 0;

enum STATE _state = RELEASED;

uint8_t *pbut_state(void)
{
    switch(_state){
        case RELEASED:
            if(BUT_PORT & (1<<BUT_PIN)) _state = MAYBE_PUSHED;
            else    _state = RELEASED;
        break;

        case MAYBE_PUSHED:
            if(BUT_PORT & (1<<BUT_PIN)){
                _state = PUSHED;
                bFlag = 1;
            }
            else    _state = RELEASED;
        break;

        case PUSHED:
            if(BUT_PORT & (1<<BUT_PIN)) _state = PUSHED;
            else _state = MAYBE_RELEASED;
        break;

        case MAYBE_RELEASED:
            if(BUT_PORT & (1<<BUT_PIN)){
                _state = PUSHED;
            }
            else
            {
                _state = RELEASED;
                //bFlag = 0;
            }
            break;
    }
    return &bFlag;
}

