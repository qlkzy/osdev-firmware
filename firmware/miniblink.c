/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
 * Copyright (C) 2012 Karl Palsson <karlp@tweak.net.au>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/l1/nvic.h>

static void init(void)
{
	/* Enable GPIOB clock. */
	/* Manually: */
	//RCC_AHBENR |= RCC_AHBENR_GPIOBEN;
	/* Using API functions: */
	rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_TIM2);

	/* Set GPIO6 (in GPIO port B) to 'output push-pull'. */
	/* Using API functions: */
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO6);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO7);

    /* 
     * timer_reset(TIM2);
     * timer_set_period(TIM2, 240);
     */

    timer_reset(TIM2);
    timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT_MUL_2,
                   TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    
    timer_set_prescaler(TIM2, 4000);
    timer_set_period(TIM2, 1000);
    timer_enable_counter(TIM2);
    
    timer_enable_update_event(TIM2);
    timer_enable_irq(TIM2,TIM_DIER_UIE);
    nvic_enable_irq(NVIC_TIM2_IRQ);
    gpio_set(GPIOB, GPIO7);
}

void tim2_isr(void)
{
    timer_clear_flag(TIM2, TIM_SR_UIF);
    gpio_toggle(GPIOB, GPIO6);	/* LED on/off */
}

int main(void)
{
	/* int i; */

	init();

	/* Blink the LED (PB6) on the board. */
	while (1) {
		/* Manually: */
		// GPIOB_BSRR = GPIO6;		/* LED off */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");
		// GPIOB_BRR = GPIO6;		/* LED on */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");

		/* Using API functions gpio_set()/gpio_clear(): */
		// gpio_set(GPIOB, GPIO6);	/* LED off */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");
		// gpio_clear(GPIOB, GPIO6);	/* LED on */
		// for (i = 0; i < 1000000; i++)	/* Wait a bit. */
		//	__asm__("nop");

		/* Using API function gpio_toggle(): */
		/* gpio_toggle(GPIOB, GPIO6);	/\* LED on/off *\/ */
		/* for (i = 0; i < 500000; i++) {	/\* Wait a bit. *\/ */
			/* __asm__("nop"); */
		/* } */
	}

	return 0;
}
