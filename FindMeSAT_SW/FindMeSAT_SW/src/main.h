/*
 * main.h
 *
 * Created: 25.01.2017 09:17:52
 *  Author: DF4IAH
 */


#ifndef MAIN_H_
#define MAIN_H_


#define C_0DEGC_K				273.15f

#define C_ADC_STEPS				8192
#define C_ADC_SUM_CNT			256
#define C_ADC_SUM_SHIFT			7

#define C_ADC_0V0_DELTA			190
#define C_VCC_3V3_VOLTS			3.318f
#define C_VCC_3V0_AREF_VOLTS	3.000f

#define C_VCTCXO_PWM_HI_VOLTS	3.253f
#define C_VCTCXO_DEFAULT_VOLTS	1.500f
#define C_VCTCXO_DELTA_VOLTS	0.047f

#define C_VCC_5V0_VOLTS			4.810f
#define C_VCC_5V0_MULT			2.50248756f
#define C_VCC_VBAT_MULT			2.50248756f
#define C_TEMPSENSE_MULT		0.00175993f


typedef enum ADC_CH0_SCAN_ENUM {
	ADC_CH0_SCAN_3V0 = 255,									// PIN = PA0, ADC0 - used as AREFA
	ADC_CH0_SCAN_VCTCXO = 0,								// PIN = PA1, ADC1
	ADC_CH0_SCAN_5V0,										// PIN = PA2, ADC2
	ADC_CH0_SCAN_VBAT,										// PIN = PA3, ADC3
} ADC_CH0_SCAN_t;

typedef enum DMA_CHANNEL_ENUM {
	DMA_CHANNEL_DACB_CH0_A = 0,
	DMA_CHANNEL_DACB_CH0_B,
} DMA_CHANNEL_ENUM_t;


typedef struct dma_dac_buf_s {
	uint16_t	ch0;
	uint16_t	ch1;
} dma_dac_buf_t;



/* INIT section */

void cb_rtc_alarm(uint32_t rtc_time);
void cb_adc_a(ADC_t* adc, uint8_t ch_mask, adc_result_t res);
void cb_tce1_ovfl(void);

void usb_callback_suspend_action(void);
void usb_callback_resume_action(void);

void usb_callback_remotewakeup_enable(void);
void usb_callback_remotewakeup_disable(void);
void usb_send_wakeup_event(void);

bool usb_callback_cdc_enable(void);
void usb_callback_cdc_disable(void);
void usb_callback_config(uint8_t port, usb_cdc_line_coding_t * cfg);
void usb_callback_cdc_set_dtr(uint8_t port, bool b_enable);
void usb_callback_cdc_set_rts(uint8_t port, bool b_enable);
void usb_callback_rx_notify(uint8_t port);
void usb_callback_tx_empty_notify(uint8_t port);


/* RUNNING section */

void halt(void);

int main(void);


#endif /* MAIN_H_ */
