#include <board.h>
#include <periph/gpio.h>
#include <xtimer.h>


uint32_t last_down = 0; // mcs since last handler calling
void btn_handler(void *arg) {
    (void)arg; 
				
	if (gpio_read(UNWD_CONNECT_BTN) == 0)// button is pressed
	last_down = xtimer_now_usec();

	else { // button is released
        uint32_t downtime = xtimer_now_usec() - last_down;
        
		if (downtime < 500*1000) { // short push < 0.5 sec
		printf("Led on-off\n");
		gpio_toggle(LED0_PIN);

		} else if (downtime < 2000*1000) { //hold for a sec
		printf("Fast flashing\n");
			while(gpio_read(UNWD_CONNECT_BTN) == 1){ // another press will change the mode
			gpio_toggle(LED0_PIN);//or can use gpio_set(LED0_PIN) ...gpio_clear(LED0_PIN);
			xtimer_usleep(20*1000);//delay in mcs
			}
		} else if (downtime > 2000*1000) { //hold longer than 2 sec
		printf("Slow flashing\n");
			while(gpio_read(UNWD_CONNECT_BTN) == 1){
			gpio_toggle(LED0_PIN);
			xtimer_usleep(60*1000);
			}
		}
    	}	
}

int main(void) {
	printf("Led with changing flashing rate\r\n");    
	gpio_init_int(UNWD_CONNECT_BTN, GPIO_IN_PU, GPIO_BOTH, btn_handler, NULL);
    while(1) {}

    return 0;
}

//if ((xtimer_usec_from_ticks(xtimer_now()) - debounce_timer) > 100000){ // contact bounce
	//debounce_timer = xtimer_usec_from_ticks(xtimer_now());    }

