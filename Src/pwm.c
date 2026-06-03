#include "stm32f4xx.h"
#include "pwm.h"

#define PWM_TIMER           TIM2
#define PWM_TIMER_CHANNEL   1U
#define PWM_TIMER_ARR       3599U
#define PWM_TIMER_PSC       0U
#define PWM_GPIO_PORT       GPIOA
#define PWM_GPIO_PIN        5U
#define PWM_GPIO_AF         1U

void PWM_Init(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    PWM_GPIO_PORT->MODER &= ~(3U << (PWM_GPIO_PIN * 2));
    PWM_GPIO_PORT->MODER |=  (2U << (PWM_GPIO_PIN * 2));

    PWM_GPIO_PORT->AFR[0] &= ~(0xFU << (PWM_GPIO_PIN * 4));
    PWM_GPIO_PORT->AFR[0] |=  (PWM_GPIO_AF << (PWM_GPIO_PIN * 4));

    PWM_GPIO_PORT->OSPEEDR &= ~(3U << (PWM_GPIO_PIN * 2));
    PWM_GPIO_PORT->OSPEEDR |=  (2U << (PWM_GPIO_PIN * 2));

    PWM_GPIO_PORT->PUPDR &= ~(3U << (PWM_GPIO_PIN * 2));

    PWM_TIMER->PSC = PWM_TIMER_PSC;
    PWM_TIMER->ARR = PWM_TIMER_ARR;
    PWM_TIMER->CCR1 = (PWM_TIMER_ARR + 1U) / 2U;

    PWM_TIMER->CCMR1 &= ~TIM_CCMR1_CC1S;
    PWM_TIMER->CCMR1 |= ((6U << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE);

    PWM_TIMER->CCER &= ~TIM_CCER_CC1P;
    PWM_TIMER->CCER |= TIM_CCER_CC1E;

    PWM_TIMER->CR1 = TIM_CR1_ARPE;
    PWM_TIMER->EGR = TIM_EGR_UG;
    PWM_TIMER->SR &= ~TIM_SR_UIF;
    PWM_TIMER->CR1 |= TIM_CR1_CEN;
}

void PWM_SetDutyPercent(uint8_t percent) {
    if (percent > 100U) {
        percent = 100U;
    }

    uint32_t pulse = ((uint32_t)percent * (PWM_TIMER_ARR + 1U)) / 100U;
    PWM_TIMER->CCR1 = pulse;
}
