// STM32F4DISCOVERY

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

#define GREEN_LED_PIN  GPIO_Pin_12
#define ORANGE_LED_PIN GPIO_Pin_13
#define RED_LED_PIN    GPIO_Pin_14
#define BLUE_LED_PIN   GPIO_Pin_15

void wait()
{
    static int i=0; // without it being static, it gets compiled out !?
    for (i=0; i < 100000000; ++i)
        ;
}

int main(void)
{
    GPIO_InitTypeDef gpio;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GREEN_LED_PIN;
    gpio.GPIO_Mode  = GPIO_Mode_OUT;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOD, &gpio);

    gpio.GPIO_Pin = ORANGE_LED_PIN;
    GPIO_Init(GPIOD, &gpio);

    gpio.GPIO_Pin = RED_LED_PIN;
    GPIO_Init(GPIOD, &gpio);

    gpio.GPIO_Pin = BLUE_LED_PIN;
    GPIO_Init(GPIOD, &gpio);

    int bitmap[4] = {1,0,1,1};

    while(1)
    {
        int saved = bitmap[0];
        for( int x = 0; x < 3; x++ )
            bitmap[x] = bitmap[x + 1];
        bitmap[3] = saved;

        GPIO_WriteBit( GPIOD, GREEN_LED_PIN,  bitmap[0] ? Bit_SET : Bit_RESET );
        GPIO_WriteBit( GPIOD, BLUE_LED_PIN,   bitmap[1] ? Bit_SET : Bit_RESET );
        GPIO_WriteBit( GPIOD, RED_LED_PIN,    bitmap[2] ? Bit_SET : Bit_RESET );
        GPIO_WriteBit( GPIOD, ORANGE_LED_PIN, bitmap[3] ? Bit_SET : Bit_RESET );

        wait();
    }
}
