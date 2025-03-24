#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

void five_s_timer_handler(struct k_timer *timer);
void eight_s_timer_handler(struct k_timer *timer);

K_TIMER_DEFINE(five_s_timer, five_s_timer_handler, NULL);
K_TIMER_DEFINE(eight_s_timer, eight_s_timer_handler, NULL);

void five_s_timer_handler(struct k_timer *timer)
{
    printk("It has been five seconds.\n");
}

void eight_s_timer_handler(struct k_timer *timer)
{
	printk("now its been 8 seconds\n");
}

void main(void)
{
	k_timer_start(&five_s_timer, K_SECONDS(5), K_SECONDS(5));
	printk("Started five second timer. Please wait.\n");
	k_timer_start(&eight_s_timer, K_SECONDS(8), K_SECONDS(8));
	printk("started the 8 second timer \n");
}
