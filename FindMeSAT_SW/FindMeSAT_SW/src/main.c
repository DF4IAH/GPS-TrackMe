/**
 * \file
 *
 * \brief FindMeSAT
 * main.c
 *
 * Created: 22.01.2017 13:13:47
 * Author : DF4IAH
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>

#include "conf_dac.h"
#include "dds.h"
#include "serial.h"
#include "usb.h"
#include "twi.h"
#include "interpreter.h"
#include "usart_serial.h"

#include "main.h"


/* GLOBAL section */

bool						g_adc_enabled						= true;
bool						g_dac_enabled						= false;
int16_t						g_backlight_mode_pwm				= 0;		// EEPROM
uint8_t						g_bias_pm							= 22;
uint8_t						g_pitch_tone_mode					= 0;		// EEPROM
bool						g_errorBeep_enable					= false;	// EEPROM
bool						g_keyBeep_enable					= false;	// EEPROM

uint64_t					g_milliseconds_cnt64				= 0ULL;

uint16_t					g_1pps_last_lo						= 0U;
uint64_t					g_1pps_last_hi						= 0ULL;
bool						g_1pps_last_new						= false;

bool						g_usb_cdc_stdout_enabled			= false;
bool						g_usb_cdc_printStatusLines_atxmega	= false;	// EEPROM
bool						g_usb_cdc_printStatusLines_sim808	= false;	// EEPROM
bool						g_usb_cdc_rx_received				= false;
bool						g_usb_cdc_transfers_authorized		= false;
bool						g_usb_cdc_access_blocked			= false;
WORKMODE_ENUM_t				g_workmode							= WORKMODE_OFF;

usart_serial_options_t		g_usart1_options					= { 0 };
bool						g_usart1_rx_ready					= false;
uint16_t					g_usart1_rx_idx						= 0;
uint8_t						g_usart1_rx_buf[C_USART1_RX_BUF_LEN]= { 0 };

bool						g_twi1_gsm_valid					= false;
uint8_t						g_twi1_gsm_version					= 0;

bool						g_twi1_gyro_valid					= false;
uint8_t						g_twi1_gyro_1_version				= 0;
volatile int16_t			g_twi1_gyro_1_temp					= 0;
volatile int16_t			g_twi1_gyro_1_temp_RTofs			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_temp_sens				= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_temp_deg_100			= 0;
volatile int16_t			g_twi1_gyro_1_accel_x				= 0;
volatile int16_t			g_twi1_gyro_1_accel_y				= 0;
volatile int16_t			g_twi1_gyro_1_accel_z				= 0;
volatile int16_t			g_twi1_gyro_1_accel_ofsx			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_accel_ofsy			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_accel_ofsz			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_accel_factx			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_accel_facty			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_accel_factz			= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_accel_x_mg			= 0;
volatile int16_t			g_twi1_gyro_1_accel_y_mg			= 0;
volatile int16_t			g_twi1_gyro_1_accel_z_mg			= 0;
volatile int16_t			g_twi1_gyro_1_gyro_x				= 0;
volatile int16_t			g_twi1_gyro_1_gyro_y				= 0;
volatile int16_t			g_twi1_gyro_1_gyro_z				= 0;
volatile int16_t			g_twi1_gyro_1_gyro_ofsx				= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_gyro_ofsy				= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_1_gyro_ofsz				= 0;		// EEPROM
volatile int32_t			g_twi1_gyro_1_gyro_x_mdps			= 0;
volatile int32_t			g_twi1_gyro_1_gyro_y_mdps			= 0;
volatile int32_t			g_twi1_gyro_1_gyro_z_mdps			= 0;
volatile bool				g_twi1_gyro_gyro_offset_set__flag	= false;
volatile bool				g_twi1_gyro_accel_offset_set__flag	= false;
uint8_t						g_twi1_gyro_2_version				= 0;
volatile int8_t				g_twi1_gyro_2_asax					= 0;
volatile int8_t				g_twi1_gyro_2_asay					= 0;
volatile int8_t				g_twi1_gyro_2_asaz					= 0;
volatile int16_t			g_twi1_gyro_2_ofsx					= 0;
volatile int16_t			g_twi1_gyro_2_ofsy					= 0;
volatile int16_t			g_twi1_gyro_2_ofsz					= 0;
volatile int16_t			g_twi1_gyro_2_mag_x					= 0;
volatile int16_t			g_twi1_gyro_2_mag_y					= 0;
volatile int16_t			g_twi1_gyro_2_mag_z					= 0;
volatile int16_t			g_twi1_gyro_2_mag_factx				= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_2_mag_facty				= 0;		// EEPROM
volatile int16_t			g_twi1_gyro_2_mag_factz				= 0;		// EEPROM
volatile int32_t			g_twi1_gyro_2_mag_x_nT				= 0;
volatile int32_t			g_twi1_gyro_2_mag_y_nT				= 0;
volatile int32_t			g_twi1_gyro_2_mag_z_nT				= 0;

bool						g_twi1_baro_valid					= false;
uint16_t					g_twi1_baro_version					= 0;
volatile uint16_t			g_twi1_baro_c[C_TWI1_BARO_C_CNT]	= { 0 };
volatile uint32_t			g_twi1_baro_d1						= 0UL;
volatile uint32_t			g_twi1_baro_d2						= 0UL;
volatile int32_t			g_twi1_baro_temp_100				= 0L;
volatile int32_t			g_twi1_baro_p_100					= 0L;

bool						g_twi1_hygro_valid					= false;
uint8_t						g_twi1_hygro_status					= 0;
volatile uint16_t			g_twi1_hygro_S_T					= 0;
volatile uint16_t			g_twi1_hygro_S_RH					= 0;
volatile int16_t			g_twi1_hygro_T_100					= 0;
volatile int16_t			g_twi1_hygro_RH_100					= 0;

uint8_t						g_twi2_lcd_version					= 0;
volatile bool				g_twi2_lcd_repaint					= false;

volatile int32_t			g_xo_mode_pwm						= 0L;		// EEPROM


struct adc_config			g_adc_a_conf						= { 0 };
struct adc_channel_config	g_adcch_vctcxo_5v0_vbat_conf		= { 0 };
struct adc_channel_config	g_adcch_io_adc4_conf				= { 0 };
struct adc_channel_config	g_adcch_io_adc5_conf				= { 0 };
struct adc_channel_config	g_adcch_silence_conf				= { 0 };

struct adc_config			g_adc_b_conf						= { 0 };
struct adc_channel_config	g_adcch_temp_conf					= { 0 };

volatile uint32_t			g_adc_vctcxo_cur					= 0;
volatile uint32_t			g_adc_vctcxo_sum					= 0;
volatile uint16_t			g_adc_vctcxo_cnt					= 0;
volatile uint32_t			g_adc_5v0_cur						= 0;
volatile uint32_t			g_adc_5v0_sum						= 0;
volatile uint16_t			g_adc_5v0_cnt						= 0;
volatile uint32_t			g_adc_vbat_cur						= 0;
volatile uint32_t			g_adc_vbat_sum						= 0;
volatile uint16_t			g_adc_vbat_cnt						= 0;
volatile uint32_t			g_adc_io_adc4_cur					= 0;
volatile uint32_t			g_adc_io_adc4_sum					= 0;
volatile uint16_t			g_adc_io_adc4_cnt					= 0;
volatile uint32_t			g_adc_io_adc5_cur					= 0;
volatile uint32_t			g_adc_io_adc5_sum					= 0;
volatile uint16_t			g_adc_io_adc5_cnt					= 0;
volatile uint32_t			g_adc_silence_cur					= 0;
volatile uint32_t			g_adc_silence_sum					= 0;
volatile uint16_t			g_adc_silence_cnt					= 0;
volatile uint32_t			g_adc_temp_cur						= 0;
volatile uint32_t			g_adc_temp_sum						= 0;
volatile uint16_t			g_adc_temp_cnt						= 0;
volatile int16_t			g_adc_vctcxo_volt_1000				= 0;
volatile int16_t			g_adc_5v0_volt_1000					= 0;
volatile int16_t			g_adc_vbat_volt_1000				= 0;
volatile int16_t			g_adc_io_adc4_volt_1000				= 0;
volatile int16_t			g_adc_io_adc5_volt_1000				= 0;
volatile int16_t			g_adc_silence_volt_1000				= 0;
volatile int16_t			g_adc_temp_deg_100					= 0;

fifo_desc_t					fifo_sched_desc;
uint32_t					fifo_sched_buffer[FIFO_SCHED_BUFFER_LENGTH];

struct pwm_config			g_pwm_vctcxo_cfg					= { 0 };
struct pwm_config			g_pwm_ctr_pll_cfg					= { 0 };

volatile uint8_t			g_sched_lock						= 0;
volatile uint8_t			g_interpreter_lock					= 0;
volatile uint8_t			g_twi1_lock							= 0;
volatile bool				g_sched_yield						= false;
volatile bool				g_sched_pop_again					= false;
volatile sched_entry_t		g_sched_data[C_SCH_SLOT_CNT]		= { 0 };
volatile uint8_t			g_sched_sort[C_SCH_SLOT_CNT]		= { 0 };

char						g_prepare_buf[C_TX_BUF_SIZE]		= "";


twi_options_t twi1_options = {
	.speed     = TWI1_SPEED,
//	.speed_reg = TWI_BAUD(sysclk_get_cpu_hz(), TWI1_SPEED),
	.speed_reg = TWI_BAUD((BOARD_XOSC_HZ * CONFIG_PLL0_MUL) / 2, TWI1_SPEED),
	.chip      = TWI1_MASTER_ADDR
};

uint8_t twi1_m_data[TWI_DATA_LENGTH] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

twi_package_t twi1_packet = {
	.buffer      = (void *)twi1_m_data,
	.no_wait     = true
};


twi_options_t twi2_options = {
	.speed     = TWI2_SPEED,
//	.speed_reg = TWI_BAUD(sysclk_get_cpu_hz(), TWI2_SPEED),
	.speed_reg = TWI_BAUD((BOARD_XOSC_HZ * CONFIG_PLL0_MUL) / 2, TWI2_SPEED),
	.chip      = TWI2_MASTER_ADDR
};

uint8_t twi2_m_data[TWI_DATA_LENGTH] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

twi_package_t twi2_packet = {
	.chip        = TWI2_SLAVE_ADDR,
	.buffer      = (void *)twi2_m_data,
	.no_wait     = true
};

#ifdef TWI1_SLAVE
TWI_Slave_t		twi1_slave;
uint8_t			twi1_recv_data[DATA_LENGTH];
#endif

#ifdef TWI2_SLAVE
TWI_Slave_t		twi2_slave;
uint8_t			twi2_recv_data[DATA_LENGTH];
#endif


/* STATIC section for this module */

static struct dac_config			dac_conf				= { 0 };

static struct dma_channel_config	dmach_dma0_conf			= { 0 };
static struct dma_channel_config	dmach_dma1_conf			= { 0 };

static uint32_t						dds0_freq_mHz			= 2000000UL;		// 2 kHz
static uint32_t						dds0_inc				= 0UL;
static uint32_t						dds0_reg				= 0UL;				// Sine
static uint32_t						dds1_freq_mHz			= 4000010UL;		// 4 kHz
static uint32_t						dds1_inc				= 0UL;
static uint32_t						dds1_reg				= 0x40000000UL;		// Cosine

static dma_dac_buf_t dac_io_dac0_buf[2][DAC_NR_OF_SAMPLES]	= { 0 };


/* Forward declarations */

static void tc_init(void);
static void tc_start(void);

static void adc_init(void);
static void adc_start(void);
static void adc_stop(void);

static void dac_init(void);
static void dac_start(void);
static void dac_stop(void);

//static void isr_10ms(uint32_t now);
static void isr_100ms(uint32_t now);
static void isr_500ms(uint32_t now);
static void isr_sparetime(uint32_t now);

static void dma_init(void);
static void dma_start(void);
static void isr_dma_dac_ch0_A(enum dma_channel_status status);
static void isr_dma_dac_ch0_B(enum dma_channel_status status);

static void task_dac(uint32_t now);
static void task_adc(uint32_t now);


/* UTILS section */

static void init_globals(void)
{
	/* VCTCXO */
	{
		int32_t val_i32 = 0L;

		if (nvm_read(INT_EEPROM, EEPROM_ADDR__VCTCXO, &val_i32, sizeof(val_i32)) == STATUS_OK) {
			irqflags_t flags = cpu_irq_save();
			g_xo_mode_pwm = val_i32;
			cpu_irq_restore(flags);
		}
	}

	/* LCD backlight */
	{
		int16_t val_i16 = 0;

		if (nvm_read(INT_EEPROM, EEPROM_ADDR__LCDBL, &val_i16, sizeof(val_i16)) == STATUS_OK) {
			irqflags_t flags = cpu_irq_save();
			g_backlight_mode_pwm = val_i16;
			cpu_irq_restore(flags);
		}
	}

	/* Beepers */
	{
		uint8_t val_ui8 = 0;

		if (nvm_read(INT_EEPROM, EEPROM_ADDR__BEEP, &val_ui8, sizeof(val_ui8)) == STATUS_OK) {
			irqflags_t flags = cpu_irq_save();
			g_errorBeep_enable	= val_ui8 & 0x01;
			g_keyBeep_enable	= val_ui8 & 0x02;
			cpu_irq_restore(flags);
		}
	}

	/* Pitch tone variants */
	{
		uint8_t val_i8 = 0;

		if (nvm_read(INT_EEPROM, EEPROM_ADDR__PITCHTONE, &val_i8, sizeof(val_i8)) == STATUS_OK) {
			irqflags_t flags = cpu_irq_save();
			g_pitch_tone_mode = val_i8;
			cpu_irq_restore(flags);
		}
	}

	/* Status lines */
	{
		uint8_t val_ui8 = 0;

		if (nvm_read(INT_EEPROM, EEPROM_ADDR__PRINT_STATUS, &val_ui8, sizeof(val_ui8)) == STATUS_OK) {
			irqflags_t flags = cpu_irq_save();
			g_usb_cdc_printStatusLines_atxmega	= val_ui8 & 0x01;
			g_usb_cdc_printStatusLines_sim808	= val_ui8 & 0x02;
			cpu_irq_restore(flags);
		}
	}

	/* 9-axis offset and gain corrections */
	{
		int16_t l_twi1_gyro_1_temp_RTofs	= 0;
		int16_t l_twi1_gyro_1_temp_sens		= 0;
		int16_t l_twi1_gyro_1_accel_ofsx	= 0;
		int16_t l_twi1_gyro_1_accel_ofsy	= 0;
		int16_t l_twi1_gyro_1_accel_ofsz	= 0;
		int16_t l_twi1_gyro_1_accel_factx	= 0;
		int16_t l_twi1_gyro_1_accel_facty	= 0;
		int16_t l_twi1_gyro_1_accel_factz	= 0;
		int16_t l_twi1_gyro_1_gyro_ofsx		= 0;
		int16_t l_twi1_gyro_1_gyro_ofsy		= 0;
		int16_t l_twi1_gyro_1_gyro_ofsz		= 0;
		int16_t l_twi1_gyro_2_mag_factx		= 0;
		int16_t l_twi1_gyro_2_mag_facty		= 0;
		int16_t l_twi1_gyro_2_mag_factz		= 0;

		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_TEMP_RTOFS, (void*)&l_twi1_gyro_1_temp_RTofs, sizeof(l_twi1_gyro_1_temp_RTofs));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_TEMP_SENS,  (void*)&l_twi1_gyro_1_temp_sens, sizeof(l_twi1_gyro_1_temp_sens));

		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_OFS_X, (void*)&l_twi1_gyro_1_accel_ofsx, sizeof(l_twi1_gyro_1_accel_ofsx));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_OFS_Y, (void*)&l_twi1_gyro_1_accel_ofsy, sizeof(l_twi1_gyro_1_accel_ofsy));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_OFS_Z, (void*)&l_twi1_gyro_1_accel_ofsz, sizeof(l_twi1_gyro_1_accel_ofsz));

		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_FACT_X, (void*)&l_twi1_gyro_1_accel_factx, sizeof(l_twi1_gyro_1_accel_factx));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_FACT_Y, (void*)&l_twi1_gyro_1_accel_facty, sizeof(l_twi1_gyro_1_accel_facty));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_FACT_Z, (void*)&l_twi1_gyro_1_accel_factz, sizeof(l_twi1_gyro_1_accel_factz));

		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_GYRO_OFS_X, (void*)&l_twi1_gyro_1_gyro_ofsx, sizeof(l_twi1_gyro_1_gyro_ofsx));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_GYRO_OFS_Y, (void*)&l_twi1_gyro_1_gyro_ofsy, sizeof(l_twi1_gyro_1_gyro_ofsy));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_GYRO_OFS_Z, (void*)&l_twi1_gyro_1_gyro_ofsz, sizeof(l_twi1_gyro_1_gyro_ofsz));

		// EEPROM_ADDR__9AXIS_GYRO_FACT_XYZ
		// EEPROM_ADDR__9AXIS_MAG_OFS_XYZ

		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_MAG_FACT_X, (void*)&l_twi1_gyro_2_mag_factx, sizeof(l_twi1_gyro_2_mag_factx));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_MAG_FACT_Y, (void*)&l_twi1_gyro_2_mag_facty, sizeof(l_twi1_gyro_2_mag_facty));
		nvm_read(INT_EEPROM, EEPROM_ADDR__9AXIS_MAG_FACT_Z, (void*)&l_twi1_gyro_2_mag_factz, sizeof(l_twi1_gyro_2_mag_factz));

		/* In case EEPROM is not set yet - load with default values instead */
		if (!l_twi1_gyro_1_accel_factx || !l_twi1_gyro_1_accel_facty || !l_twi1_gyro_1_accel_factz ||
			!l_twi1_gyro_2_mag_factx || !l_twi1_gyro_2_mag_facty || !l_twi1_gyro_2_mag_factz) {
			calibration_mode(CALIBRATION_MODE_ENUM__DEFAULTS);
			return;
		}

		irqflags_t flags = cpu_irq_save();
		g_twi1_gyro_1_temp_RTofs	= l_twi1_gyro_1_temp_RTofs;
		g_twi1_gyro_1_temp_sens		= l_twi1_gyro_1_temp_sens;

		g_twi1_gyro_1_accel_ofsx	= l_twi1_gyro_1_accel_ofsx;
		g_twi1_gyro_1_accel_ofsy	= l_twi1_gyro_1_accel_ofsy;
		g_twi1_gyro_1_accel_ofsz	= l_twi1_gyro_1_accel_ofsz;

		g_twi1_gyro_1_accel_factx	= l_twi1_gyro_1_accel_factx;
		g_twi1_gyro_1_accel_facty	= l_twi1_gyro_1_accel_facty;
		g_twi1_gyro_1_accel_factz	= l_twi1_gyro_1_accel_factz;

		g_twi1_gyro_1_gyro_ofsx		= l_twi1_gyro_1_gyro_ofsx;
		g_twi1_gyro_1_gyro_ofsy		= l_twi1_gyro_1_gyro_ofsy;
		g_twi1_gyro_1_gyro_ofsz		= l_twi1_gyro_1_gyro_ofsz;

		g_twi1_gyro_2_mag_factx		= l_twi1_gyro_2_mag_factx;
		g_twi1_gyro_2_mag_facty		= l_twi1_gyro_2_mag_facty;
		g_twi1_gyro_2_mag_factz		= l_twi1_gyro_2_mag_factz;
		cpu_irq_restore(flags);
	}
}

void save_globals(EEPROM_SAVE_BF_ENUM_t bf)
{
	/* Version information */
	{
		uint32_t version_ui32 = ((uint32_t)(20000 + VERSION_HIGH) * 1000) + (uint32_t)VERSION_LOW;
		for (uint8_t idx = 0; idx < 8; ++idx) {
			uint8_t pad = '0' + (version_ui32 % 10);
			nvm_write(INT_EEPROM, EEPROM_ADDR__VERSION + (7 - idx), (void*)&pad, sizeof(pad));
			version_ui32 /= 10;
		}
	}

	/* VCTCXO */
	if (bf & EEPROM_SAVE_BF__VCTCXO) {
		irqflags_t flags = cpu_irq_save();
		int32_t val_i32 = g_xo_mode_pwm;
		cpu_irq_restore(flags);

		nvm_write(INT_EEPROM, EEPROM_ADDR__VCTCXO, (void*)&val_i32, sizeof(val_i32));
	}

	/* LCD backlight */
	if (bf & EEPROM_SAVE_BF__LCDBL) {
		irqflags_t flags = cpu_irq_save();
		int16_t val_i16 = g_backlight_mode_pwm;
		cpu_irq_restore(flags);

		nvm_write(INT_EEPROM, EEPROM_ADDR__LCDBL, (void*)&val_i16, sizeof(val_i16));
	}

	/* Beepers */
	if (bf & EEPROM_SAVE_BF__BEEP) {
		irqflags_t flags = cpu_irq_save();
		uint8_t val_ui8 = (g_keyBeep_enable ?  0x02 : 0x00) | (g_errorBeep_enable ?  0x01 : 0x00);
		cpu_irq_restore(flags);

		nvm_write(INT_EEPROM, EEPROM_ADDR__BEEP, (void*)&val_ui8, sizeof(val_ui8));
	}

	/* Pitch tone variants */
	if (bf & EEPROM_SAVE_BF__PITCHTONE) {
		irqflags_t flags = cpu_irq_save();
		int8_t val_i8 = g_pitch_tone_mode;
		cpu_irq_restore(flags);

		nvm_write(INT_EEPROM, EEPROM_ADDR__PITCHTONE, (void*)&val_i8, sizeof(val_i8));
	}

	/* Status lines */
	if (bf & EEPROM_SAVE_BF__PRINT_STATUS) {
		irqflags_t flags = cpu_irq_save();
		uint8_t val_ui8 = (g_usb_cdc_printStatusLines_sim808 ?  0x02 : 0x00) | (g_usb_cdc_printStatusLines_atxmega ?  0x01 : 0x00);
		cpu_irq_restore(flags);

		nvm_write(INT_EEPROM, EEPROM_ADDR__PRINT_STATUS, (void*)&val_ui8, sizeof(val_ui8));
	}

	/* 9-axis offset and gain corrections */
	if (bf & EEPROM_SAVE_BF__9AXIS_OFFSETS) {
		irqflags_t flags = cpu_irq_save();
		int16_t l_twi1_gyro_1_temp_RTofs	= g_twi1_gyro_1_temp_RTofs;
		int16_t l_twi1_gyro_1_temp_sens		= g_twi1_gyro_1_temp_sens;

		int16_t l_twi1_gyro_1_accel_ofsx	= g_twi1_gyro_1_accel_ofsx;
		int16_t l_twi1_gyro_1_accel_ofsy	= g_twi1_gyro_1_accel_ofsy;
		int16_t l_twi1_gyro_1_accel_ofsz	= g_twi1_gyro_1_accel_ofsz;

		int16_t l_twi1_gyro_1_accel_factx	= g_twi1_gyro_1_accel_factx;
		int16_t l_twi1_gyro_1_accel_facty	= g_twi1_gyro_1_accel_facty;
		int16_t l_twi1_gyro_1_accel_factz	= g_twi1_gyro_1_accel_factz;

		int16_t l_twi1_gyro_1_gyro_ofsx		= g_twi1_gyro_1_gyro_ofsx;
		int16_t l_twi1_gyro_1_gyro_ofsy		= g_twi1_gyro_1_gyro_ofsy;
		int16_t l_twi1_gyro_1_gyro_ofsz		= g_twi1_gyro_1_gyro_ofsz;

		int16_t l_twi1_gyro_2_mag_factx		= g_twi1_gyro_2_mag_factx;
		int16_t l_twi1_gyro_2_mag_facty		= g_twi1_gyro_2_mag_facty;
		int16_t l_twi1_gyro_2_mag_factz		= g_twi1_gyro_2_mag_factz;
		cpu_irq_restore(flags);

		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_TEMP_RTOFS, (void*)&l_twi1_gyro_1_temp_RTofs, sizeof(l_twi1_gyro_1_temp_RTofs));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_TEMP_SENS,  (void*)&l_twi1_gyro_1_temp_sens, sizeof(l_twi1_gyro_1_temp_sens));

		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_OFS_X, (void*)&l_twi1_gyro_1_accel_ofsx, sizeof(l_twi1_gyro_1_accel_ofsx));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_OFS_Y, (void*)&l_twi1_gyro_1_accel_ofsy, sizeof(l_twi1_gyro_1_accel_ofsy));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_OFS_Z, (void*)&l_twi1_gyro_1_accel_ofsz, sizeof(l_twi1_gyro_1_accel_ofsz));

		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_FACT_X, (void*)&l_twi1_gyro_1_accel_factx, sizeof(l_twi1_gyro_1_accel_factx));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_FACT_Y, (void*)&l_twi1_gyro_1_accel_facty, sizeof(l_twi1_gyro_1_accel_facty));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_ACCEL_FACT_Z, (void*)&l_twi1_gyro_1_accel_factz, sizeof(l_twi1_gyro_1_accel_factz));

		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_GYRO_OFS_X, (void*)&l_twi1_gyro_1_gyro_ofsx, sizeof(l_twi1_gyro_1_gyro_ofsx));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_GYRO_OFS_Y, (void*)&l_twi1_gyro_1_gyro_ofsy, sizeof(l_twi1_gyro_1_gyro_ofsy));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_GYRO_OFS_Z, (void*)&l_twi1_gyro_1_gyro_ofsz, sizeof(l_twi1_gyro_1_gyro_ofsz));

		// EEPROM_ADDR__9AXIS_GYRO_FACT_XYZ
		// EEPROM_ADDR__9AXIS_MAG_OFS_XYZ

		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_MAG_FACT_X, (void*)&l_twi1_gyro_2_mag_factx, sizeof(l_twi1_gyro_2_mag_factx));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_MAG_FACT_Y, (void*)&l_twi1_gyro_2_mag_facty, sizeof(l_twi1_gyro_2_mag_facty));
		nvm_write(INT_EEPROM, EEPROM_ADDR__9AXIS_MAG_FACT_Z, (void*)&l_twi1_gyro_2_mag_factz, sizeof(l_twi1_gyro_2_mag_factz));
	}
}


int myStringToVar(char *str, uint32_t format, float out_f[], long out_l[], int out_i[])
{
	int ret = 0;
	int idx = 0;

	uint8_t mode = format & 0x03;
	while (mode) {
		char* item = str + idx;

		switch (mode) {
			case MY_STRING_TO_VAR_FLOAT:
			if (out_f) {
				*(out_f++) = atof(item);
				++ret;
			}
			break;

			case MY_STRING_TO_VAR_LONG:
			if (out_l) {
				*(out_l++) = atol(item);
				++ret;
			}
			break;

			case MY_STRING_TO_VAR_INT:
			if (out_i) {
				*(out_i++) = atoi(item);
				++ret;
			}
			break;

			default:
				return ret;
		}

		/* forward to next string position */
		char* next = strchr(item, ',');
		if (!next) {
			break;
		}
		idx += 1 + next - item;

		format >>= 2;
		mode = format & 0x03;
	}

	return ret;
}


void adc_app_enable(bool enable)
{
	if (g_adc_enabled != enable) {
		if (enable) {
			tc_init();
			adc_init();

			tc_start();
			adc_start();

		} else {
			adc_stop();
		}

		/* each of it is atomic */
		{
			g_adc_enabled = enable;
			g_twi2_lcd_repaint = true;
		}
	}
}

void backlight_mode_pwm(int16_t mode_pwm)
{
	uint8_t l_pwm = mode_pwm & 0xff;

	/* Setting the mode */
	g_backlight_mode_pwm = mode_pwm;
	save_globals(EEPROM_SAVE_BF__LCDBL);

	switch (mode_pwm) {
		case -2:
			;
		break;

		case -1:
			twi2_set_ledbl(1, 0);
		break;

		default:
			twi2_set_ledbl(0, l_pwm);
	}
}

void bias_update(uint8_t bias)
{
	uint8_t l_bias_pm = bias & 0x3f;

	g_bias_pm = l_bias_pm & 0x3f;
	twi2_set_bias(l_bias_pm);
}

void calibration_mode(CALIBRATION_MODE_ENUM_t mode)
{
	switch (mode) {
		case CALIBRATION_MODE_ENUM__DEFAULTS:
			{
				irqflags_t flags = cpu_irq_save();

				g_twi1_gyro_1_temp_RTofs	= 0;
				g_twi1_gyro_1_temp_sens		= 413;

				g_twi1_gyro_1_accel_ofsx	= ((int16_t) (-46672L / 16));	// 16LSB / OFS
				g_twi1_gyro_1_accel_ofsy	= ((int16_t) (+41120L / 16));	// 16LSB / OFS
				g_twi1_gyro_1_accel_ofsz	= ((int16_t) (+76672L / 16));	// 16LSB / OFS
				g_twi1_gyro_1_accel_factx	=  9980;						// X = Xchip * factx / 10000
				g_twi1_gyro_1_accel_facty	=  9975;						// Y = Ychip * facty / 10000
				g_twi1_gyro_1_accel_factz	=  9950;						// Z = Zchip * factz / 10000

				g_twi1_gyro_1_gyro_ofsx		= ( -32 / 4);					//  4LSB / OFS
				g_twi1_gyro_1_gyro_ofsy		= ( -80 / 4);					//  4LSB / OFS
				g_twi1_gyro_1_gyro_ofsz		= (+148 / 4);					//  4LSB / OFS

				g_twi1_gyro_2_mag_factx		=  9250;						// X = Xchip * factx / 10000
				g_twi1_gyro_2_mag_facty		=  9250;						// Y = Ychip * facty / 10000
				g_twi1_gyro_2_mag_factz		= 14440;						// Z = Zchip * factz / 10000

				/* Update the offset registers in the I2C device */
				g_twi1_gyro_gyro_offset_set__flag	= true;
				g_twi1_gyro_accel_offset_set__flag	= true;

				cpu_irq_restore(flags);

				/* Write back current offset values to the EEPROM */
				save_globals(EEPROM_SAVE_BF__9AXIS_OFFSETS);
			}
		break;

		case CALIBRATION_MODE_ENUM__GYRO:
			{
				irqflags_t flags = cpu_irq_save();

				/* Adjust the settings */
				g_twi1_gyro_1_gyro_ofsx -= (g_twi1_gyro_1_gyro_x >> 2);
				g_twi1_gyro_1_gyro_ofsy -= (g_twi1_gyro_1_gyro_y >> 2);
				g_twi1_gyro_1_gyro_ofsz -= (g_twi1_gyro_1_gyro_z >> 2);

				/* Update the offset registers in the I2C device */
				g_twi1_gyro_gyro_offset_set__flag = true;

				cpu_irq_restore(flags);

				/* Write back current offset values to the EEPROM */
				save_globals(EEPROM_SAVE_BF__9AXIS_OFFSETS);
			}
		break;

		case CALIBRATION_MODE_ENUM__ACCEL_X:
			{
				irqflags_t flags = cpu_irq_save();

				/* Adjust X factor */
				g_twi1_gyro_1_accel_factx = (int16_t) (((int32_t)g_twi1_gyro_1_accel_factx * 1000L) / g_twi1_gyro_1_accel_x_mg);

				/* Adjust Y/Z offsets */
				g_twi1_gyro_1_accel_ofsy -= (g_twi1_gyro_1_accel_y >> 4);
				g_twi1_gyro_1_accel_ofsz -= (g_twi1_gyro_1_accel_z >> 4);

				/* Update the offset registers in the I2C device */
				g_twi1_gyro_accel_offset_set__flag = true;

				cpu_irq_restore(flags);

				/* Write back current offset values to the EEPROM */
				save_globals(EEPROM_SAVE_BF__9AXIS_OFFSETS);
			}
		break;

		case CALIBRATION_MODE_ENUM__ACCEL_Y:
			{
				irqflags_t flags = cpu_irq_save();

				/* Adjust Y factor */
				g_twi1_gyro_1_accel_facty = (int16_t) (((int32_t)g_twi1_gyro_1_accel_facty * 1000L) / g_twi1_gyro_1_accel_y_mg);

				/* Adjust X/Z offsets */
				g_twi1_gyro_1_accel_ofsx -= (g_twi1_gyro_1_accel_x >> 4);
				g_twi1_gyro_1_accel_ofsz -= (g_twi1_gyro_1_accel_z >> 4);

				/* Update the offset registers in the I2C device */
				g_twi1_gyro_accel_offset_set__flag = true;

				cpu_irq_restore(flags);

				/* Write back current offset values to the EEPROM */
				save_globals(EEPROM_SAVE_BF__9AXIS_OFFSETS);
			}
		break;

		case CALIBRATION_MODE_ENUM__ACCEL_Z:
			{
				irqflags_t flags = cpu_irq_save();

				/* Adjust Z factor */
				g_twi1_gyro_1_accel_factz = (int16_t) (((int32_t)g_twi1_gyro_1_accel_factz * 1000L) / g_twi1_gyro_1_accel_z_mg);

				/* Adjust X/Y offsets */
				g_twi1_gyro_1_accel_ofsx -= (g_twi1_gyro_1_accel_x >> 4);
				g_twi1_gyro_1_accel_ofsy -= (g_twi1_gyro_1_accel_y >> 4);

				/* Update the offset registers in the I2C device */
				g_twi1_gyro_accel_offset_set__flag = true;

				cpu_irq_restore(flags);

				/* Write back current offset values to the EEPROM */
				save_globals(EEPROM_SAVE_BF__9AXIS_OFFSETS);
			}
		break;
	}
}

void dac_app_enable(bool enable)
{
	if (g_dac_enabled != enable) {
		if (enable) {
			/* Setting the values */
			{
				irqflags_t flags	= cpu_irq_save();
				dds0_freq_mHz		= 2000000UL;		// 2 kHz
				dds0_reg			= 0UL;				// Sine
				dds1_freq_mHz		= 4000010UL;		// 4 kHz
				dds1_reg			= 0x40000000UL;		// Cosine
				cpu_irq_restore(flags);
			}

			dac_init();
			tc_start();
			dac_start();

		} else {
			dac_stop();
		}

		/* atomic */
		g_dac_enabled = enable;
	}
}

void dds_update(float dds0_hz, float dds1_hz, float phase)
{
	uint32_t l_dds0_freq_mHz = 0UL;
	uint32_t l_dds1_freq_mHz = 0UL;
	uint32_t l_dds1_reg = 0UL;

	/* Update only when mHz value for DDS0 is given */
	if (dds0_hz >= 0.f) {
		l_dds0_freq_mHz = (uint32_t) (dds0_hz * 1000.f);
	}

	/* Update only when mHz value for DDS1 is given */
	if (dds1_hz >= 0.f) {
		l_dds1_freq_mHz = (uint32_t) (dds1_hz * 1000.f);
	}

	/* Set the phase between two starting oscillators */
	if (phase >= 0.f) {
		l_dds1_reg = (uint32_t) (0x40000000UL * (phase / 90.f));
	}

	/* Modifying the DDS registers */
	{
		irqflags_t flags = cpu_irq_save();

		/* Update only when mHz value for DDS0 is given */
		if (dds0_hz >= 0.f) {
			dds0_freq_mHz = l_dds0_freq_mHz;
		}

		/* Update only when mHz value for DDS1 is given */
		if (dds1_hz >= 0.f) {
			dds1_freq_mHz = l_dds1_freq_mHz;
		}

		/* Set the phase between two starting oscillators */
		if (phase >= 0.f) {
			dds0_reg = 0UL;
			dds1_reg = l_dds1_reg;
		}

		cpu_irq_restore(flags);
	}

	/* Calculate new increment values */
	sched_push(task_dac, SCHED_ENTRY_CB_TYPE__LISTTIME, 10, true, false, false);
}

void errorBeep_enable(bool enable)
{
	/* atomic */
	g_errorBeep_enable = enable;

	save_globals(EEPROM_SAVE_BF__BEEP);
}

void keyBeep_enable(bool enable)
{
	/* atomic */
	g_keyBeep_enable = enable;

	save_globals(EEPROM_SAVE_BF__BEEP);
}

void pitchTone_mode(uint8_t mode)
{
	/* atomic */
	g_pitch_tone_mode = mode;

	save_globals(EEPROM_SAVE_BF__PITCHTONE);
}

void printStatusLines_bitfield(PRINT_STATUS_BF_ENUM_t bf)
{
	/* atomic */
	g_usb_cdc_printStatusLines_atxmega	= bf & PRINT_STATUS_LINES__ATXMEGA	?  true : false;
	g_usb_cdc_printStatusLines_sim808	= bf & PRINT_STATUS_LINES__SIM808	?  true : false;

	save_globals(EEPROM_SAVE_BF__PRINT_STATUS);
}

void xoPwm_set(int32_t mode_pwm)
{
	int32_t l_xo_mode_pwm = INT16_MIN;

	if (mode_pwm >= 0) {
		/* Set the PWM value for the VCTCXO pull voltage (should be abt. 1.5 V) */
		l_xo_mode_pwm = mode_pwm & C_XO_VAL_MASK;			// Flags removed
		tc_write_cc_buffer(&TCC0, TC_CCC, (uint16_t) l_xo_mode_pwm);

	} else {
		switch (mode_pwm) {
			case -2:
				/* Preset value */
				l_xo_mode_pwm = ((int32_t) (0.5f + g_pwm_vctcxo_cfg.period * C_VCTCXO_DEFAULT_VOLTS / C_VCTCXO_PWM_HI_VOLTS)) & UINT16_MAX;
				tc_write_cc_buffer(&TCC0, TC_CCC, (uint16_t) l_xo_mode_pwm);	// Preset value with no flags
			break;

			case -1:
				/* PLL mode - get current PWM value and set PLL bit */
				{
					irqflags_t flags = cpu_irq_save();
					l_xo_mode_pwm = g_xo_mode_pwm;
					cpu_irq_restore(flags);
				}
				l_xo_mode_pwm &= C_XO_VAL_MASK;				// Mask out all flags
				l_xo_mode_pwm |= C_XO_BF_PLL;				// Flag: use PLL feature
			break;
		}
	}

	/* Write back global var */
	if (l_xo_mode_pwm != INT16_MIN) {
		irqflags_t flags = cpu_irq_save();
		g_xo_mode_pwm = l_xo_mode_pwm;
		cpu_irq_restore(flags);

		save_globals(EEPROM_SAVE_BF__VCTCXO);
	}
}

void halt(void)
{
	/* MAIN Loop Shutdown */
	/* atomic */
	{
		g_workmode = WORKMODE_END;
	}
}


static void calc_next_frame(dma_dac_buf_t buf[DAC_NR_OF_SAMPLES], uint32_t* dds0_reg_p, uint32_t* dds0_inc_p, uint32_t* dds1_reg_p, uint32_t* dds1_inc_p)
{
	/* Filling the DMA block for a dual connected DAC channel */
	for (uint8_t idx = 0; idx < DAC_NR_OF_SAMPLES; ++idx, *dds0_reg_p += *dds0_inc_p, *dds1_reg_p += *dds1_inc_p) {
		uint16_t dds0_phase = *dds0_reg_p >> 16;
		buf[idx].ch0 = get_interpolated_sine(dds0_phase);

		uint16_t dds1_phase = *dds1_reg_p >> 16;
		buf[idx].ch1 = get_interpolated_sine(dds1_phase);
	}
}


/* Simple scheduler concept */

bool sched_getLock(volatile uint8_t* lockVar)
{
	bool status = false;

	/* IRQ secured access */	
	{
		irqflags_t flags = cpu_irq_save();
		barrier();
		if (!*lockVar) {	// No use before
			++*lockVar;
			barrier();
			status = true;
		}
		cpu_irq_restore(flags);
	}
	return status;
}

void sched_freeLock(volatile uint8_t* lockVar)
{
	irqflags_t flags	= cpu_irq_save();
	*lockVar			= 0;
	cpu_irq_restore(flags);
}

void sched_set_alarm(uint32_t alarmTime)
{
	/* Set next time to wake up */
	uint32_t checkTime = rtc_get_time() + 2;
	if (alarmTime < checkTime) {
		alarmTime = checkTime;
	}
	rtc_set_alarm(alarmTime);
}

void sched_doSleep(void)
{
#if 1
	/* Power down until IRQ or event calls in */
	sleepmgr_enter_sleep();
#endif
}

void sched_push(void* cb, SCHED_ENTRY_CB_TYPE_ENUM_t cbType, uint32_t wakeTime, bool isDelay, bool isIntDis, bool isSync)
{
	const uint32_t pushTime = rtc_get_time();
	uint32_t alarmTime = 0UL;
	uint32_t sfCb = (isIntDis ?  0x8F000000UL : 0x0F00000UL) | (isSync ?  0x40000000UL : 0x0000000UL) | ((uint32_t)cbType << 24) | ((uint32_t)(uint16_t)cb);

	if (isDelay) {
		/* Sanity checks */
		if (wakeTime < 2) {
			wakeTime = 2;														// Min value to use to work properly
		} else if (wakeTime > 30000U) {
			wakeTime = ((30000U << 10) / 1000);									// Max value 30 sec
		} else {
			wakeTime = ((wakeTime << 10) / 1000);								// Time correction for 1024 ticks per second
		}

		/* Absolute time ticks */
		wakeTime += pushTime;
	}

	/* Lock access to the scheduler entries */
	if (!sched_getLock(&g_sched_lock)) {
		/* Push entry to the stack due to blocked access */
		if (!fifo_is_full(&fifo_sched_desc)) {
			irqflags_t flags = cpu_irq_save();
			fifo_push_uint32(&fifo_sched_desc, wakeTime);
			fifo_push_uint32(&fifo_sched_desc, sfCb);
			cpu_irq_restore(flags);
		}
		return;
	}

// -- single entry section for g_sched_data[] / g_sched_sort[]
// v

	/* Get next free slot */
	bool dataEntryStored = false;
	uint8_t slot = 0;

	for (int idx = 0; idx < C_SCH_SLOT_CNT; ++idx) {
		if (!(g_sched_data[idx].occupied)) {
			g_sched_data[idx].occupied	= true;
			g_sched_data[idx].callback	= cb;
			g_sched_data[idx].cbType	= cbType & 0x03;
			g_sched_data[idx].isIntDis	= (isIntDis ?  1 : 0);
			g_sched_data[idx].isSync	= (isSync	?  1 : 0);
			g_sched_data[idx].wakeTime	= wakeTime;
			slot = idx + 1;
			dataEntryStored = true;
			break;
		}
	}

	if (!dataEntryStored) {
		goto sched_push_out;  // should not happen
	}

	/* Bind to sort list */
	for (int pos = 0; pos < C_SCH_SLOT_CNT; ++pos) {
		uint8_t idx = g_sched_sort[pos];
		if (!idx) {
			/* Fill in after last entry */
			g_sched_sort[pos] = slot;
			break;
		}
		--idx;

		/* Place new item in front of that entry */
		if (g_sched_data[idx].occupied && (wakeTime < g_sched_data[idx].wakeTime)) {
			/* Move all entries of position list one up to give space for our new entry */
			for (int mvidx = C_SCH_SLOT_CNT - 2; pos <= mvidx; --mvidx) {
				g_sched_sort[mvidx + 1] = g_sched_sort[mvidx];
			}

			/* Fill in new item */
			g_sched_sort[pos] = slot;
			break;
		}
	}

	/* Get next time for wake-up */
	alarmTime = g_sched_data[g_sched_sort[0] - 1].wakeTime;

sched_push_out:

// ^
// -- single entry section for g_sched_data[] / g_sched_sort[]

	/* Release lock */
	sched_freeLock(&g_sched_lock);

	/* Pop back all FIFO entries */
	while (!fifo_is_empty(&fifo_sched_desc)) {
		uint32_t sfCb = 0UL;
		wakeTime = 0UL;

		/* Get next entries */
		{
			irqflags_t flags = cpu_irq_save();
			fifo_pull_uint32(&fifo_sched_desc, &wakeTime);
			fifo_pull_uint32(&fifo_sched_desc, &sfCb);
			cpu_irq_restore(flags);
		}

		/* FIFO check */
		if ((sfCb & 0x0f000000UL) != 0x0f000000UL) {
			/* Signature not found - clear all entries to synchronize again */
			irqflags_t flags = cpu_irq_save();
			while (!fifo_is_empty(&fifo_sched_desc)) {
				fifo_pull_uint32(&fifo_sched_desc, &sfCb);
			}
			sfCb = 0UL;
			cpu_irq_restore(flags);
		}

		if ((sfCb & 0x0000ffffUL) && wakeTime) {
			sched_push((void*)(uint16_t)(sfCb & 0x0000ffffUL), ((sfCb & 0x00030000) >> 24), wakeTime, false, (sfCb & 0x80000000UL ?  true : false), (sfCb & 0x40000000UL ?  true : false));
		}
	}

	/* Set next time to wake up */
	if (alarmTime) {
		sched_set_alarm(alarmTime);
	}
}

void sched_pop(uint32_t wakeNow)
{
	uint32_t alarmTime = 0UL;

	if (!sched_getLock(&g_sched_lock)) {
		/* Locked by another one, sched_pop() later */
		g_sched_pop_again = true;
		return;
	}

// -- single entry section for g_sched_data[] / g_sched_sort[]
// v

	uint8_t idx = g_sched_sort[0];
	if (!idx) {
		/* No jobs at the scheduler */
		goto sched_pop_out;
	}
	if (!(g_sched_data[idx - 1].occupied)) {
		/* Sanity failed */
		goto sched_pop_out;
	}

	/* Process each entry until the current timestamp */
	alarmTime = g_sched_data[idx - 1].wakeTime;
	while (alarmTime <= rtc_get_time()) {
		/* Get callback */
		void* cbVal		= g_sched_data[idx - 1].callback;
		uint8_t cbType	= g_sched_data[idx - 1].cbType;
		bool isIntDis	= g_sched_data[idx - 1].isIntDis;
		bool isSync		= g_sched_data[idx - 1].isSync;

		/* Free entry */
		g_sched_data[idx - 1].occupied = false;

		/* Move all items down by one */
		for (int mvidx = 0; mvidx < (C_SCH_SLOT_CNT - 1); ++mvidx) {
			g_sched_sort[mvidx] = g_sched_sort[mvidx + 1];
		}
		g_sched_sort[C_SCH_SLOT_CNT - 1] = 0;	// clear top-most index

		/* Call the CB function */
		if (cbVal) {
			irqflags_t flags = cpu_irq_save();
			if (isIntDis) {
				cpu_irq_disable();
			} else {
				cpu_irq_enable();
			}
			switch (cbType) {
				case SCHED_ENTRY_CB_TYPE__LISTTIME:
				{
					sched_callback_type0 cb = cbVal;
					cb(alarmTime);
				}
				break;

				case SCHED_ENTRY_CB_TYPE__LISTTIME_ISSYNC:
				{
					sched_callback_type1 cb = cbVal;
					cb(alarmTime, isSync);
				}
				break;
			}
			cpu_irq_restore(flags);
		}

		/* Get the next alarm time */
		{
			idx = g_sched_sort[0];
			if (!idx) {
				/* No jobs at the scheduler */
				goto sched_pop_out;
			}
			if (!(g_sched_data[idx - 1].occupied)) {
				/* Sanity failed */
				goto sched_pop_out;
			}
			alarmTime = g_sched_data[idx - 1].wakeTime;
		}
	}

sched_pop_out:

// ^
// -- single entry section for g_sched_data[] / g_sched_sort[]

	sched_freeLock(&g_sched_lock);

	/* Set next time to wake up */
	if (alarmTime) {
		sched_set_alarm(alarmTime);
	}

	/* Restart due to another guest rang the door bell */
	if (g_sched_pop_again) {
		g_sched_pop_again = false;
		sched_pop(wakeNow);
	}
}

void yield_ms(uint16_t ms)
{
	irqflags_t flags = cpu_irq_save();
	cpu_irq_enable();

	/* A yield job is on the scheduler */
	g_sched_yield = true;

	/* Push ourself to the scheduler */
	sched_push(yield_ms_cb, SCHED_ENTRY_CB_TYPE__LISTTIME, ms, true, false, false);

	/* Continued sleep until our callback is done */
	do {
		/* Enter sleep mode */
		sched_doSleep();

		/* Woke up for any reason - check if we were called */
	} while (g_sched_yield);

	cpu_irq_restore(flags);
}

void yield_ms_cb(uint32_t listTime)
{
	g_sched_yield = false;
}


/* INIT section */

static void interrupt_init(void)
{
	PORTR_DIRCLR	= (1 << 0);													// Pin R0 direction is cleared = INPUT
	PORTR_PIN0CTRL	= PORT_ISC_RISING_gc;										// GNSS 1PPS has a rising edge signal

	PORTR_INT0MASK	= (1 << 0);													// Port R0	--> INT0
	PORTR_INT1MASK	= 0;														// (none)	--> INT1

	PORTR_INTCTRL	= PORT_INT1LVL_OFF_gc | PORT_INT0LVL_LO_gc;					// Enable interrupt for port R0 with low level
}

ISR(PORTR_INT0_vect)
{
	/* Take the time */
	g_1pps_last_lo	= TCC1_CNT;
	g_1pps_last_hi	= g_milliseconds_cnt64;
	g_1pps_last_new	= true;
}


static void evsys_init(void)
{
	sysclk_enable_module(SYSCLK_PORT_GEN, SYSCLK_EVSYS);

	/* ADC - event channels 0, 1, 2, 3 */
	EVSYS.CH0MUX  = EVSYS_CHMUX_TCC0_CCC_gc;									// TCC0 CC-C goes to EVSYS CH0
	EVSYS.CH0CTRL = EVSYS_DIGFILT_1SAMPLE_gc;									// EVSYS CH0 no digital filtering
	EVSYS.CH1MUX  = EVSYS_CHMUX_TCC0_CCC_gc;									// TCC0 CC-C goes to EVSYS CH1
	EVSYS.CH1CTRL = EVSYS_DIGFILT_1SAMPLE_gc;									// EVSYS CH1 no digital filtering
	EVSYS.CH2MUX  = EVSYS_CHMUX_TCC0_CCC_gc;									// TCC0 CC-C goes to EVSYS CH2
	EVSYS.CH2CTRL = EVSYS_DIGFILT_1SAMPLE_gc;									// EVSYS CH2 no digital filtering
	EVSYS.CH3MUX  = EVSYS_CHMUX_TCC0_CCC_gc;									// TCC0 CC-C goes to EVSYS CH3
	EVSYS.CH3CTRL = EVSYS_DIGFILT_1SAMPLE_gc;									// EVSYS CH3 no digital filtering

	/* DAC - event 4 */
	EVSYS.CH4MUX  = EVSYS_CHMUX_TCE1_OVF_gc;									// TCE1 overflow goes to EVSYS CH4
	EVSYS.CH4CTRL = EVSYS_DIGFILT_1SAMPLE_gc;									// EVSYS CH4 no digital filtering
}


static void tc_init(void)
{
	/* Get the PWM value */
	int32_t l_xo_mode_pwm;
	{
		irqflags_t flags = cpu_irq_save();
		l_xo_mode_pwm = g_xo_mode_pwm;
		cpu_irq_restore(flags);
	}

	/* TCC0: VCTCXO PWM signal generation and ADCA & ADCB */
	pwm_init(&g_pwm_vctcxo_cfg, PWM_TCC0, PWM_CH_C, 2048);						// Init PWM structure and enable timer - running with 2048 Hz --> 2 Hz averaged data
	pwm_start(&g_pwm_vctcxo_cfg, 45);											// Start PWM here. Percentage with 1% granularity is to coarse, use driver access instead
	tc_write_cc_buffer(&TCC0, TC_CCC, (uint16_t) (l_xo_mode_pwm & C_XO_VAL_MASK));	// Setting the PWM value

	/* TCC1: Free running clock for 30 MHz PLL */
	g_pwm_ctr_pll_cfg.tc = &TCC1;
	tc_enable(&TCC1);															// Enable TCC1 and power up
	tc_set_wgm(&TCC1, TC_WG_NORMAL);											// Normal counting up
	tc_write_clock_source(&TCC1, PWM_CLK_OFF);									// Disable counter until all is ready
	pwm_set_frequency(&g_pwm_ctr_pll_cfg, 1000);								// Prepare structure for 1 ms overflow frequency
	tc_write_period(&TCC1, g_pwm_ctr_pll_cfg.period);							// Calculate period count
	tc_write_period_buffer(&TCC1, C_TCC1_PERIOD - 1);							// Overflows every 1 ms

	/* TCE1: DAC clock */
	tc_enable(&TCE1);
	tc_set_wgm(&TCE1, TC_WG_NORMAL);											// Internal clock for DAC convertion
	tc_write_period(&TCE1, (sysclk_get_per_hz() / DAC_RATE_OF_CONV) - 1);		// DAC clock of 100 kHz for DDS (Direct Digital Synthesis)
}

static void tc_start(void)
{
	/* ADC clock */
	tc_write_clock_source(&TCC0, TC_CLKSEL_DIV1_gc);							// VCTCXO PWM start, output still is Z-state
	tc_set_overflow_interrupt_callback(&TCC0, isr_tcc0_ovfl);
	tc_set_overflow_interrupt_level(&TCC0, TC_INT_LVL_LO);

	/* Free running clock for 30 MHz PLL */
	tc_write_clock_source(&TCC1, TC_CLKSEL_DIV1_gc);
	tc_set_overflow_interrupt_callback(&TCC1, isr_tcc1_ovfl);
	tc_set_overflow_interrupt_level(&TCC1, TC_INT_LVL_HI);

//	tc_write_clock_source(&TCD0, TC_CLKSEL_DIV1_gc);
//	tc_write_clock_source(&TCD1, TC_CLKSEL_DIV1_gc);
//	tc_write_clock_source(&TCE0, TC_CLKSEL_DIV1_gc);

	/* DAC clock */
	tc_write_clock_source(&TCE1, TC_CLKSEL_DIV1_gc);

//	tc_write_clock_source(&TCF0, TC_CLKSEL_DIV1_gc);
//	tc_write_clock_source(&TCF1, TC_CLKSEL_DIV1_gc);
}

//ISR(TCC0_OVF_vect)
void isr_tcc0_ovfl(void)
{	// This ISR is called 2048 per second
//	static uint32_t	last_10ms  = 0UL;
	static uint32_t	last_100ms = 0UL;
	static uint32_t	last_500ms = 0UL;
	
	/* Time downscaling */
	uint32_t now = rtc_get_time();

	/* Clear IF bit to allow interrupt enabled section */
	TCC0_INTFLAGS = TC0_OVFIF_bm;

	if (g_workmode == WORKMODE_RUN) {
#if 0
		/* Group, which needs to be called about 100x per second */
		if (((now - last_10ms) >= 10) || (now < last_10ms)) {
			last_10ms = now;
			isr_10ms(now);
			return;
		}
#endif

		/* Group, which needs to be called about 10x per second */
		if (((now - last_100ms) >= 102) || (now < last_100ms)) {
			last_100ms = now;
			isr_100ms(now);
			return;
		}

		/* Group, which needs to be called about 2x per second */
		if (((now - last_500ms) >= 512) || (now < last_500ms)) {
			last_500ms = now;
			isr_500ms(now);
			return;
		}

		isr_sparetime(now);
	}
}

#if 0
static void isr_10ms(uint32_t now)
{
	isr_10ms_twi1_onboard(now);
}
#endif

static void isr_100ms(uint32_t now)
{
	isr_100ms_twi1_onboard(now);
}

static void isr_500ms(uint32_t now)
{
	isr_500ms_twi1_onboard(now);

	/* CPU ADC values */
	sched_push(task_adc, SCHED_ENTRY_CB_TYPE__LISTTIME, 100, true, false, false);

	/* CPU DAC reconfiguration */
	sched_push(task_dac, SCHED_ENTRY_CB_TYPE__LISTTIME, 100, true, false, false);

	/* Kick RTC32 */
	rtc_set_alarm(rtc_get_time() + 2);
}

static void isr_sparetime(uint32_t now)
{
	isr_sparetime_twi1_onboard(now);
}

//ISR(TCC1_OVF_vect)
void isr_tcc1_ovfl(void)
{	// This ISR is called every millisecond for tracking the PLL
	++g_milliseconds_cnt64;
}

static void rtc_start(void)
{
	rtc_set_callback(isr_rtc_alarm);
}

void isr_rtc_alarm(uint32_t rtc_time)
{	// Alarm call-back with the current time
	cpu_irq_enable();
	sched_pop(rtc_time);
}


static void adc_init(void)
{
	/* Disable digital circuits of ADC pins */
	PORTA_PIN0CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN1CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN2CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN4CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTA_PIN5CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTB_PIN2CTRL |= PORT_ISC_INPUT_DISABLE_gc;
	PORTB_PIN3CTRL |= PORT_ISC_INPUT_DISABLE_gc;

	/* Prepare the structures */
	adc_read_configuration(&ADC_VCTCXO_5V0_VBAT,							&g_adc_a_conf);
	adcch_read_configuration(&ADC_VCTCXO_5V0_VBAT, ADC_VCTCXO_5V0_VBAT_CH,	&g_adcch_vctcxo_5v0_vbat_conf);
	adcch_read_configuration(&ADC_IO_ADC4, ADC_IO_ADC4_CH,					&g_adcch_io_adc4_conf);
	adcch_read_configuration(&ADC_IO_ADC5, ADC_IO_ADC5_CH,					&g_adcch_io_adc5_conf);
	adcch_read_configuration(&ADC_SILENCE, ADC_SILENCE_CH,					&g_adcch_silence_conf);
	adc_read_configuration(&ADC_TEMP,										&g_adc_b_conf);
	adcch_read_configuration(&ADC_TEMP, ADC_TEMP_CH,						&g_adcch_temp_conf);

	/* ADC-clock request */
	adc_set_clock_rate(&g_adc_a_conf, 1000000UL);												// External signals: 100kHz .. 2000kHz
	adc_set_clock_rate(&g_adc_b_conf,  115000UL);												// Internal signals: 100kHz ..  125kHz

	/* Enable internal ADC-B input for temperature measurement */
	adc_disable_internal_input(&g_adc_a_conf,		ADC_INT_TEMPSENSE				   );
	adc_enable_internal_input (&g_adc_a_conf,							ADC_INT_BANDGAP);
	adc_enable_internal_input (&g_adc_b_conf,		ADC_INT_TEMPSENSE | ADC_INT_BANDGAP);

	/* Current limitation */
	adc_set_current_limit(&g_adc_a_conf,			ADC_CURRENT_LIMIT_MED);
	adc_set_current_limit(&g_adc_b_conf,			ADC_CURRENT_LIMIT_HIGH);

	/* ADC impedance */
	adc_set_gain_impedance_mode(&g_adc_a_conf,		ADC_GAIN_HIGHIMPEDANCE);
	adc_set_gain_impedance_mode(&g_adc_b_conf,		ADC_GAIN_HIGHIMPEDANCE);

	/* PIN assignment */
	adcch_set_input(&g_adcch_vctcxo_5v0_vbat_conf,	ADCCH_POS_PIN1,			ADCCH_NEG_NONE, 1);
	adcch_set_input(&g_adcch_io_adc4_conf,			ADCCH_POS_PIN4,			ADCCH_NEG_NONE, 1);
	adcch_set_input(&g_adcch_io_adc5_conf,			ADCCH_POS_PIN5,			ADCCH_NEG_NONE, 1);
	adcch_set_input(&g_adcch_silence_conf,			ADCCH_POS_BANDGAP,		ADCCH_NEG_NONE, 1);
	adcch_set_input(&g_adcch_temp_conf,				ADCCH_POS_TEMPSENSE,	ADCCH_NEG_NONE, 1);

	/* Convertion and reference */
	adc_set_conversion_parameters(&g_adc_a_conf,	ADC_SIGN_OFF, ADC_RES_12, ADC_REF_AREFA);	// ADC-A: ADC0 (Pin 62 3V0 as reference pin)
	adc_set_conversion_parameters(&g_adc_b_conf,	ADC_SIGN_OFF, ADC_RES_12, ADC_REF_BANDGAP);	// ADC-B: bandgap diode (1V0)

	/* PIN scan on ADC-channel 0 */
	adcch_set_pin_scan(&g_adcch_vctcxo_5v0_vbat_conf, 0, 2);									// ADC-A: scan between ADC1 .. ADC3

	/* Trigger */
	adc_set_conversion_trigger(&g_adc_a_conf, ADC_TRIG_EVENT_SINGLE, 4, 1);
	adc_set_conversion_trigger(&g_adc_b_conf, ADC_TRIG_EVENT_SINGLE, 1, 0);

	/* Interrupt service routine */
	adc_set_callback(&ADCA, isr_adc_a);
	adc_set_callback(&ADCB, isr_adc_b);

	/* Interrupt type */
	adcch_set_interrupt_mode(&g_adcch_vctcxo_5v0_vbat_conf,	ADCCH_MODE_COMPLETE);
	adcch_set_interrupt_mode(&g_adcch_io_adc4_conf,			ADCCH_MODE_COMPLETE);
	adcch_set_interrupt_mode(&g_adcch_io_adc5_conf,			ADCCH_MODE_COMPLETE);
	adcch_set_interrupt_mode(&g_adcch_silence_conf,			ADCCH_MODE_COMPLETE);
	adcch_set_interrupt_mode(&g_adcch_temp_conf,			ADCCH_MODE_COMPLETE);

	/* Interrupt enable */
	adcch_enable_interrupt(&g_adcch_vctcxo_5v0_vbat_conf);
	adcch_enable_interrupt(&g_adcch_io_adc4_conf);
	adcch_enable_interrupt(&g_adcch_io_adc5_conf);
	adcch_enable_interrupt(&g_adcch_silence_conf);
	adcch_enable_interrupt(&g_adcch_temp_conf);

	/* Execute the new settings */
	adc_write_configuration(&ADCA,											&g_adc_a_conf);
	adcch_write_configuration(&ADC_VCTCXO_5V0_VBAT,	ADC_VCTCXO_5V0_VBAT_CH,	&g_adcch_vctcxo_5v0_vbat_conf);
	adcch_write_configuration(&ADC_IO_ADC4,			ADC_IO_ADC4_CH,			&g_adcch_io_adc4_conf);
	adcch_write_configuration(&ADC_IO_ADC5,			ADC_IO_ADC5_CH,			&g_adcch_io_adc5_conf);
	adcch_write_configuration(&ADC_SILENCE,			ADC_SILENCE_CH,			&g_adcch_silence_conf);
	adc_write_configuration(&ADCB,											&g_adc_b_conf);
	adcch_write_configuration(&ADC_TEMP,			ADC_TEMP_CH,			&g_adcch_temp_conf);

	/* Get production signature for calibration */
	ADCA_CAL = adc_get_calibration_data(ADC_CAL_ADCA);
	ADCB_CAL = adc_get_calibration_data(ADC_CAL_ADCB);
}

static void adc_start(void)
{
	adc_enable(&ADCA);
	adc_enable(&ADCB);
}

static void adc_stop(void)
{
	adc_disable(&ADCA);
	adc_disable(&ADCB);
}

void isr_adc_a(ADC_t* adc, uint8_t ch_mask, adc_result_t res)
{
	uint8_t scan_ofs_next = (ADCA_CH0_SCAN >> 4);
	int16_t val = res - C_ADC_0V0_DELTA;

	if ((ch_mask & ADC_VCTCXO_5V0_VBAT_CH)) {
		switch (scan_ofs_next) {
			case ADC_CH0_SCAN_5V0:
				g_adc_vctcxo_sum += val;
				if (++g_adc_vctcxo_cnt >= C_ADC_SUM_CNT) {
					g_adc_vctcxo_cur = g_adc_vctcxo_sum;
					g_adc_vctcxo_sum = g_adc_vctcxo_cnt = 0;
				}
			break;

			case ADC_CH0_SCAN_VBAT:
				g_adc_5v0_sum += val;
				if (++g_adc_5v0_cnt >= C_ADC_SUM_CNT) {
					g_adc_5v0_cur = g_adc_5v0_sum;
					g_adc_5v0_sum = g_adc_5v0_cnt = 0;
				}
			break;

			case ADC_CH0_SCAN_VCTCXO:
				g_adc_vbat_sum += val;
				if (++g_adc_vbat_cnt >= C_ADC_SUM_CNT) {
					g_adc_vbat_cur = g_adc_vbat_sum;
					g_adc_vbat_sum = g_adc_vbat_cnt = 0;
				}
			break;
		}

	} else if (ch_mask & ADC_IO_ADC4_CH) {
		g_adc_io_adc4_sum += val;
		if (++g_adc_io_adc4_cnt >= C_ADC_SUM_CNT) {
			g_adc_io_adc4_cur = g_adc_io_adc4_sum;
			g_adc_io_adc4_sum = g_adc_io_adc4_cnt = 0;
		}

	} else if (ch_mask & ADC_IO_ADC5_CH) {
		g_adc_io_adc5_sum += val;
		if (++g_adc_io_adc5_cnt >= C_ADC_SUM_CNT) {
			g_adc_io_adc5_cur = g_adc_io_adc5_sum;
			g_adc_io_adc5_sum = g_adc_io_adc5_cnt = 0;
		}

	} else if (ch_mask & ADC_SILENCE_CH) {
		g_adc_silence_sum += val;
		if (++g_adc_silence_cnt >= C_ADC_SUM_CNT) {
			g_adc_silence_cur = g_adc_silence_sum;
			g_adc_silence_sum = g_adc_silence_cnt = 0;
		}
	}
}

void isr_adc_b(ADC_t* adc, uint8_t ch_mask, adc_result_t res)
{
	int16_t val = res - C_ADC_0V0_DELTA;

	if (ch_mask & ADC_TEMP_CH) {
		g_adc_temp_sum += val;
		if (++g_adc_temp_cnt >= C_ADC_SUM_CNT) {
			g_adc_temp_cur = g_adc_temp_sum;
			g_adc_temp_sum = g_adc_temp_cnt = 0;
		}
	}
}


static void dac_init(void)
{
	dac_read_configuration(&DAC_DAC, &dac_conf);
	dac_set_conversion_parameters(&dac_conf, DAC_REF_BANDGAP, DAC_ADJ_LEFT);
	dac_set_active_channel(&dac_conf, DAC_DAC1_CH | DAC_DAC0_CH, 0);
	dac_set_conversion_trigger(&dac_conf, DAC_DAC1_CH | DAC_DAC0_CH, 4);
	dac_write_configuration(&DAC_DAC, &dac_conf);

	/* Get production signature for calibration */
	DACB_CH0OFFSETCAL	= dac_get_calibration_data(DAC_CAL_DACB0_OFFSET);
	DACB_CH0GAINCAL		= dac_get_calibration_data(DAC_CAL_DACB0_GAIN);
	DACB_CH1OFFSETCAL	= dac_get_calibration_data(DAC_CAL_DACB1_OFFSET);
	DACB_CH1GAINCAL		= dac_get_calibration_data(DAC_CAL_DACB1_GAIN);

	dma_init();
}

static void dac_start(void)
{
	dac_enable(&DACB);

	/* Connect the DMA to the DAC periphery */
	dma_start();

	/* IRQ disabled section */
	{
		irqflags_t flags = cpu_irq_save();

		/* Prepare DMA blocks */
		calc_next_frame(&dac_io_dac0_buf[0][0], &dds0_reg, &dds0_inc, &dds1_reg, &dds1_inc);
		calc_next_frame(&dac_io_dac0_buf[1][0], &dds0_reg, &dds0_inc, &dds1_reg, &dds1_inc);

		/* DMA channels activation */
		dma_channel_enable(DMA_CHANNEL_DACB_CH0_A);

		cpu_irq_restore(flags);
	}
}

static void dac_stop(void)
{
	dma_disable();
	dac_disable(&DACB);
}


static void dma_init(void)
{
	memset(&dmach_dma0_conf, 0, sizeof(dmach_dma0_conf));	// DACB channel 0 - linked with dma1
	memset(&dmach_dma1_conf, 0, sizeof(dmach_dma1_conf));	// DACB channel 1 - linked with dma0

	dma_channel_set_burst_length(&dmach_dma0_conf, DMA_CH_BURSTLEN_4BYTE_gc);
	dma_channel_set_burst_length(&dmach_dma1_conf, DMA_CH_BURSTLEN_4BYTE_gc);

	dma_channel_set_transfer_count(&dmach_dma0_conf, DAC_NR_OF_SAMPLES * sizeof(dma_dac_buf_t));
	dma_channel_set_transfer_count(&dmach_dma1_conf, DAC_NR_OF_SAMPLES * sizeof(dma_dac_buf_t));

	dma_channel_set_src_reload_mode(&dmach_dma0_conf, DMA_CH_SRCRELOAD_TRANSACTION_gc);
	dma_channel_set_src_dir_mode(&dmach_dma0_conf, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_source_address(&dmach_dma0_conf, (uint16_t)(uintptr_t) &dac_io_dac0_buf[0][0]);
	dma_channel_set_dest_reload_mode(&dmach_dma0_conf, DMA_CH_DESTRELOAD_BURST_gc);
	dma_channel_set_dest_dir_mode(&dmach_dma0_conf, DMA_CH_DESTDIR_INC_gc);
	dma_channel_set_destination_address(&dmach_dma0_conf, (uint16_t)(uintptr_t) &DACB_CH0DATA);		// Access to CH0 and CH1

	dma_channel_set_src_reload_mode(&dmach_dma1_conf, DMA_CH_SRCRELOAD_TRANSACTION_gc);
	dma_channel_set_src_dir_mode(&dmach_dma1_conf, DMA_CH_SRCDIR_INC_gc);
	dma_channel_set_source_address(&dmach_dma1_conf, (uint16_t)(uintptr_t) &dac_io_dac0_buf[1][0]);
	dma_channel_set_dest_reload_mode(&dmach_dma1_conf, DMA_CH_DESTRELOAD_BURST_gc);
	dma_channel_set_dest_dir_mode(&dmach_dma1_conf, DMA_CH_DESTDIR_INC_gc);
	dma_channel_set_destination_address(&dmach_dma1_conf, (uint16_t)(uintptr_t) &DACB_CH0DATA);		// Access to CH0 and CH1

	dma_channel_set_trigger_source(&dmach_dma0_conf, DMA_CH_TRIGSRC_DACB_CH0_gc);
	dma_channel_set_single_shot(&dmach_dma0_conf);

	dma_channel_set_trigger_source(&dmach_dma1_conf, DMA_CH_TRIGSRC_DACB_CH0_gc);
	dma_channel_set_single_shot(&dmach_dma1_conf);

	task_dac(rtc_get_time());																		// Calculate DDS increments
}

static void dma_start(void)
{
	dma_enable();

	dma_set_callback(DMA_CHANNEL_DACB_CH0_A, isr_dma_dac_ch0_A);
	dma_channel_set_interrupt_level(&dmach_dma0_conf, DMA_INT_LVL_MED);

	dma_set_callback(DMA_CHANNEL_DACB_CH0_B, isr_dma_dac_ch0_B);
	dma_channel_set_interrupt_level(&dmach_dma1_conf, DMA_INT_LVL_MED);

	dma_set_priority_mode(DMA_PRIMODE_CH01RR23_gc);
	dma_set_double_buffer_mode(DMA_DBUFMODE_CH01_gc);

	dma_channel_write_config(DMA_CHANNEL_DACB_CH0_A, &dmach_dma0_conf);
	dma_channel_write_config(DMA_CHANNEL_DACB_CH0_B, &dmach_dma1_conf);
}

static void isr_dma_dac_ch0_A(enum dma_channel_status status)
{
	dma_channel_enable(DMA_CHANNEL_DACB_CH0_B);

	cpu_irq_enable();
	calc_next_frame(&dac_io_dac0_buf[0][0], &dds0_reg, &dds0_inc, &dds1_reg, &dds1_inc);
}

static void isr_dma_dac_ch0_B(enum dma_channel_status status)
{
	dma_channel_enable(DMA_CHANNEL_DACB_CH0_A);

	cpu_irq_enable();
	calc_next_frame(&dac_io_dac0_buf[1][0], &dds0_reg, &dds0_inc, &dds1_reg, &dds1_inc);
}


/* The LOOP section */

static void task_dac(uint32_t now)
{	/* Calculation of the DDS increments */
	static uint32_t s_dds0_freq_mHz = 0UL;
	static uint32_t s_dds1_freq_mHz = 0UL;
	uint32_t l_dds0_freq_mHz, l_dds1_freq_mHz;

	/* Setting the pair of frequencies */
	{
		irqflags_t flags		= cpu_irq_save();
		l_dds0_freq_mHz			= dds0_freq_mHz;
		l_dds1_freq_mHz			= dds1_freq_mHz;
		cpu_irq_restore(flags);
	}

	if ((l_dds0_freq_mHz != s_dds0_freq_mHz) || (l_dds1_freq_mHz != s_dds1_freq_mHz)) {
		/* DDS increment calculation */
		uint32_t l_dds0_inc = (uint32_t) (((uint64_t)dds0_freq_mHz * UINT32_MAX) / (DAC_RATE_OF_CONV * 1000UL));
		uint32_t l_dds1_inc = (uint32_t) (((uint64_t)dds1_freq_mHz * UINT32_MAX) / (DAC_RATE_OF_CONV * 1000UL));
		s_dds0_freq_mHz = l_dds0_freq_mHz;
		s_dds1_freq_mHz = l_dds1_freq_mHz;

		/* Setting the pair of increments */
		{
			irqflags_t flags	= cpu_irq_save();
			dds0_inc			= l_dds0_inc;
			dds1_inc			= l_dds1_inc;
			cpu_irq_restore(flags);
		}
	}
}

static void task_adc(uint32_t now)
{	/* Calculations of the ADC values for the presentation layer */
	static uint32_t adc_last = 0;

	if ((now - adc_last) >= 512 || (now < adc_last)) {
		uint32_t l_adc_vctcxo_cur, l_adc_5v0_cur, l_adc_vbat_cur, l_adc_io_adc4_cur, l_adc_io_adc5_cur;
		uint32_t l_adc_silence_cur, l_adc_temp_cur;

		adc_last = now;

		/* Getting a copy of the values */
		{
			irqflags_t flags	= cpu_irq_save();
			l_adc_vctcxo_cur	= g_adc_vctcxo_cur;
			l_adc_5v0_cur		= g_adc_5v0_cur;
			l_adc_vbat_cur		= g_adc_vbat_cur;
			l_adc_io_adc4_cur	= g_adc_io_adc4_cur;
			l_adc_io_adc5_cur	= g_adc_io_adc5_cur;
			l_adc_silence_cur	= g_adc_silence_cur;
			l_adc_temp_cur		= g_adc_temp_cur;
			cpu_irq_restore(flags);
		}

		int16_t l_adc_vctcxo_volt_1000	= (int16_t) (((( 1000UL * l_adc_vctcxo_cur  * C_VCC_3V0_AREF_VOLTS                   ) / C_ADC_STEPS))  - 1000 * C_VCTCXO_DELTA_VOLTS);
		int16_t l_adc_5v0_volt_1000		= (int16_t) (((  1000UL * l_adc_5v0_cur     * C_VCC_3V0_AREF_VOLTS * C_VCC_5V0_MULT  ) / C_ADC_STEPS));
		int16_t l_adc_vbat_volt_1000	= (int16_t) (((  1000UL * l_adc_vbat_cur    * C_VCC_3V0_AREF_VOLTS * C_VCC_VBAT_MULT ) / C_ADC_STEPS));
		int16_t l_adc_io_adc4_volt_1000	= (int16_t) (((  1000UL * l_adc_io_adc4_cur * C_VCC_3V0_AREF_VOLTS                   ) / C_ADC_STEPS));
		int16_t l_adc_io_adc5_volt_1000	= (int16_t) (((  1000UL * l_adc_io_adc5_cur * C_VCC_3V0_AREF_VOLTS                   ) / C_ADC_STEPS));
		int16_t l_adc_silence_volt_1000	= (int16_t) (((  1000UL * l_adc_silence_cur * C_VCC_3V0_AREF_VOLTS                   ) / C_ADC_STEPS));
		int16_t l_adc_temp_deg_100		= (int16_t) ((((  100UL * l_adc_temp_cur                           * C_TEMPSENSE_MULT) / C_ADC_STEPS))  -  100 * C_0DEGC_K);

		/* Writing back the values */
		{
			irqflags_t flags		= cpu_irq_save();
			g_adc_vctcxo_volt_1000	= l_adc_vctcxo_volt_1000;
			g_adc_5v0_volt_1000		= l_adc_5v0_volt_1000;
			g_adc_vbat_volt_1000	= l_adc_vbat_volt_1000;
			g_adc_io_adc4_volt_1000	= l_adc_io_adc4_volt_1000;
			g_adc_io_adc5_volt_1000	= l_adc_io_adc5_volt_1000;
			g_adc_silence_volt_1000	= l_adc_silence_volt_1000;
			g_adc_temp_deg_100		= l_adc_temp_deg_100;
			cpu_irq_restore(flags);
		}
	}
}

static void task(void)
{
	if (g_workmode == WORKMODE_RUN) {
		uint32_t now = rtc_get_time();

		/* TASK when woken up and all ISRs are done */
		/* note: ADC and DAC are handled by the scheduler */
		task_serial(now);									// Handle serial communication with the SIM808
		task_twi(now);										// Handle (TWI1 and) TWI2 communications
		task_usb(now);										// Handling the USB connection
	}
}


int main(void)
{
	uint8_t retcode = 0;

	/* Init the IOPORT */
	ioport_init();

	/* Init the FIFO buffers */
	fifo_init(&fifo_sched_desc, fifo_sched_buffer, FIFO_SCHED_BUFFER_LENGTH);

	/* Init of interrupt system */
	g_workmode = WORKMODE_INIT;
	irq_initialize_vectors();
	pmic_init();
	pmic_set_scheduling(PMIC_SCH_FIXED_PRIORITY);

	sysclk_init();		// Clock configuration set-up

	sleepmgr_init();	// Unlocks all sleep mode levels

	rtc_init();
	rtc_start();

	init_globals();

	interrupt_init();	// Port interrupts
	evsys_init();		// Event system
	tc_init();			// Timers
	serial_init();		// Set up serial connection to the SIM808
	if (g_adc_enabled) {
		adc_init();		// ADC
	}
	if (g_dac_enabled) {
		dac_init();		// DAC
	}
	twi_init();			// I2C / TWI

	board_init();		// Activates all in/out pins not already handled above - transitions from Z to dedicated states

	nvm_init(INT_FLASH);

	/* All interrupt sources & PMIC are prepared until here - IRQ activation follows */
	cpu_irq_enable();

	/* Start of sub-modules */
	tc_start();			// All clocks and PWM timers start here
	if (g_dac_enabled) {
		dac_start();	// Start DA convertions
	}
	if (g_adc_enabled) {
		adc_start();	// Start AD convertions
	}

	/* Init of USB system */
	usb_init();			// USB device stack start function to enable stack and start USB

	/* Start TWI channels */
	twi_start();		// Start TWI

	/* Start serial1 */
	serial_start();		// Start communication with the SIM808 */

	/* Show help page of command set */
	printHelp();

	/* LED green */
	twi2_set_leds(0x02);

	/* The application code */
	g_twi2_lcd_repaint = true;
	g_workmode = WORKMODE_RUN;
    while (g_workmode) {
		/* Process all user space tasks */
		task();

		/* Work on the pushed back jobs */
		yield_ms(0);
    }

	/* LED off */
	twi2_set_leds(0x00);

	cpu_irq_disable();
	sleepmgr_enter_sleep();

	return retcode;
}
