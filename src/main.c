#include <stdbool.h>
#include <stm8s.h>
#include "main.h"
#include "milis.h"
#include "uart1.h"
#include "daughterboard.h"

void init(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);      // taktovani MCU na 16MHz

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_G_PORT, PWM_G_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_B_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);
    

    init_milis();
    init_uart1();

    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 500 - 1);
    TIM2_OC1Init(  // konfigurace OutputCHannel1
        TIM2_OCMODE_PWM1,  // mód PWM1
        TIM2_OUTPUTSTATE_ENABLE,  // povolím
        5000, //nastavení šířky impulzu
        TIM2_OCPOLARITY_HIGH // nastavení polarity
        );
    TIM2_OC2Init(  TIM2_OCMODE_PWM1,  TIM2_OUTPUTSTATE_ENABLE,  1000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(  TIM2_OCMODE_PWM1,  TIM2_OUTPUTSTATE_ENABLE,  9000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);

}


int main(void)
{
  
    uint32_t time = 0;
    uint8_t r=1, b=1, g=1;
    bool S1memory = false;
    bool S2memory = false;
    bool S3memory = false;
    uint8_t color_index;

    init();

    while (1) {

        if (milis() - time > 33 ) {
            time = milis();
            

            if (PUSH(S1) && ! S1memory) {
                color_index++;
                if (color_index >= 3) {
                    color_index = 0;
                }
            }
                
            S1memory = PUSH(S1);

            if (PUSH(S2) && ! S2memory) {
                if (color_index == 0) {
                    r += 10;
                }

                if (color_index == 1) {
                    g += 10;
                }

                if (color_index == 2) {
                    b += 10;
                }
            }
            S2memory = PUSH(S2);

            if (PUSH(S3) && ! S3memory) {
                if (color_index == 0) {
                    r -= 10;
                }

                if (color_index == 1) {
                    g -= 10;
                }

                if (color_index == 2) {
                    b -= 10;
                }
            }
            S3memory = PUSH(S3);
            

            TIM2_SetCompare1(r);
            TIM2_SetCompare2(g);
            TIM2_SetCompare3(b);
            

        }
    }
}
/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
