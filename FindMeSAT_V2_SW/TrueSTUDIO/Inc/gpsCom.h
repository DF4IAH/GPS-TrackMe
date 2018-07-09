/*
 * gpsCom.h
 *
 *  Created on: 04.07.2018
 *      Author: DF4IAH
 */

#ifndef GPSCOM_H_
#define GPSCOM_H_
#ifdef __cplusplus
 extern "C" {
#endif


 /* Bit-mask for the gpscomEventGroup */
typedef enum Gpscom_EGW_BM {
  Gpscom_EGW__QUEUE_IN                = 0x00000001UL,
  Gpscom_EGW__DMA_TX_END              = 0x00000010UL,
  Gpscom_EGW__DMA_RX50                = 0x00000020UL,
  Gpscom_EGW__DMA_RX100_END           = 0x00000040UL,
  Gpscom_EGW__DMA_TXRX_ERROR          = 0x00000080UL,
  Gpscom_EGW__DMA_TX_RUN              = 0x00000100UL,
  Gpscom_EGW__DMA_RX_RUN              = 0x00000200UL,
  Gpscom_EGW__TIMER_SERVICE_RX        = 0x00002000UL,
} Gpscom_EGW_BM_t;

 /* Command types for the gpscomInQueue */
typedef enum gpscomInQueueCmds {
  gpscomInQueueCmds__NOP              = 0,
} gpscomInQueueCmds_t;


#define Gps_Channels                  16

typedef enum GpsStatus {
  GpsStatus__notValid                 = 0,
  GpsStatus__valid
} GpsStatus_t;

typedef enum GpsMode {
  GpsMode__A_Autonomous               = 0,
  GpsMode__D_DGPS,
  GpsMode__N_DatNotValid,
  GpsMode__R_CoarsePosition,
  GpsMode__S_Simulator
} GpsMode_t;

typedef enum GpsMode1 {
  GpsMode1__M_Manual                  = 0,
  GpsMode1__A_Automatic
} GpsMode1_t;

typedef enum GpsMode2 {
  GpsMode2__unset                     = 0,
  GpsMode2__1_NoFix,
  GpsMode2__2_2D,
  GpsMode2__3_3D,
} GpsMode2_t;


typedef struct GpscomGpsCtx {

  /* Last timestamp */
  float                               time;
  int32_t                             data;

  /* Status and modes */
  GpsStatus_t                         status;
  GpsMode_t                           mode;
  GpsMode1_t                          mode1;
  GpsMode2_t                          mode2;

  /* 3D position */
  float                               lat_deg;
  float                               lon_deg;
  float                               alt_m;

  /* Motion */
  float                               speed_kts;                                                // 1 kts = 1 nm / h    &   1 nm = 1,852 m
  float                               course_deg;

  /* Dilution of precision */
  float                               pdop;
  float                               hdop;
  float                               vdop;

  /* Sats count */
  uint8_t                             satsUse;
  uint8_t                             satsView;

  /* Channel array */
  int32_t                             sv[Gps_Channels];
  int32_t                             sElev[Gps_Channels];
  int32_t                             sAzim[Gps_Channels];
  int32_t                             sSNR[Gps_Channels];

} GpscomGpsCtx_t;


void gpscomtRXTimerCallbackImpl(TimerHandle_t argument);

void gpscomGpscomTaskInit(void);
void gpscomGpscomTaskLoop(void);


#ifdef __cplusplus
}
#endif
#endif /* GPSCOM_H_ */
