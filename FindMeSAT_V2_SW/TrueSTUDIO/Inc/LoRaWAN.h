/*
 * LoRaWAN.h
 *
 *  Created on: 12.05.2018
 *      Author: DF4IAH
 */

#ifndef LORAWAN_H_
#define LORAWAN_H_

#include "stm32l4xx_hal.h"


#define LoRaWAN_MsgLenMax             51
#define LoRaWAN_FRMPayloadMax         48


/* Bit-mask for the loRaWANEventGroup */
typedef enum LORAWAN_EGW_BM {
  LORAWAN_EGW__DO_INIT                = 0x00000001UL,
  LORAWAN_EGW__QUEUE_IN               = 0x00000010UL,
  LORAWAN_EGW__QUEUE_OUT              = 0x00000020UL,
} LORAWAN_EGW_BM_t;

/* Command types for the loraInQueue */
typedef enum loraInQueueCmds {
  loraInQueueCmds__NOP                = 0,
  loraInQueueCmds__Init,
} loraInQueueCmds_t;

/* FSM states of the loRaWANLoRaWANTaskLoop */
typedef enum Fsm_States {
  Fsm_NOP                             = 0,

  Fsm_RX1                             = 0x01,
  Fsm_RX2,
  Fsm_JoinRequestRX1,
  Fsm_JoinRequestRX2,

  Fsm_TX                              = 0x09,

  Fsm_MAC_Decode                      = 0x10,

  Fsm_MAC_JoinRequest,
  Fsm_MAC_JoinAccept,

  Fsm_MAC_LinkCheckReq,
  Fsm_MAC_LinkCheckAns,

  Fsm_MAC_LinkADRReq,
  Fsm_MAC_LinkADRAns,

  Fsm_MAC_DutyCycleReq,
  Fsm_MAC_DutyCycleAns,

  Fsm_MAC_RXParamSetupReq,
  Fsm_MAC_RXParamSetupAns,

  Fsm_MAC_DevStatusReq,
  Fsm_MAC_DevStatusAns,

  Fsm_MAC_NewChannelReq,
  Fsm_MAC_NewChannelAns,

  Fsm_MAC_RXTimingSetupReq,
  Fsm_MAC_RXTimingSetupAns,

  Fsm_MAC_TxParamSetupReq,
  Fsm_MAC_TxParamSetupAns,

  Fsm_MAC_DlChannelReq,
  Fsm_MAC_DlChannelAns,
} Fsm_States_t;


/* LoRaWAN RX windows */
typedef enum LoRaWAN_RX_windows {
  LORAWAN_RX_PREPARE_MS               =   10,
  LORAWAN_EU868_MAX_TX_DURATION_MS    = 3000,                                                   // TODO: Search for right number

  LORAWAN_EU868_DELAY1_MS             = 1000,
  LORAWAN_EU868_DELAY2_MS             = 2000,

  LORAWAN_EU868_JOIN_ACCEPT_DELAY1_MS = 5000,
  LORAWAN_EU868_JOIN_ACCEPT_DELAY2_MS = 6000,
} LoRaWAN_RX_windows_t;

/* LoRaWAN version information */
typedef enum LoRaWANVersion {
  LoRaWANVersion_10                   = 0,
  LoRaWANVersion_11                   = 1,
} LoRaWANVersion_t;

/* LoRaWAN direction of transmission - Up: device --> gateway / Dn: device <-- gateway */
typedef enum LoRaWANctxDir {
  Up                                  = 0,
  Dn                                  = 1
} LoRaWANctxDir_t;


/* Memory assignment of the non-volatile memory section of the timer */
typedef struct LoRaWANctxBkpRam {

  /* Skip first entries */
  uint32_t                            _skip[16];

  /* CRC */
  uint32_t                            LoRaWANcrc;

  /* Counters */
  uint32_t                            FCntUp;
  uint32_t                            NFCntDwn;                                                 // LW 1.0: FCntDwn
  uint32_t                            AFCntDwn;

  uint32_t                            n8_n8_DLSettings8_RxDelay8;
  uint32_t                            CFList_03_02_01_00;
  uint32_t                            CFList_07_06_05_04;
  uint32_t                            CFList_11_10_09_08;
  uint32_t                            CFList_15_14_13_12;

  uint32_t                            _end;

} LoRaWANctxBkpRam_t;

typedef struct LoRaWANctx {

  /* Non-volatile counters */
  volatile LoRaWANctxBkpRam_t*        bkpRAM;

  /* FSM */
  volatile Fsm_States_t               FsmState;

  /* Device specific */
  uint8_t                             DevEUI_LE[8];
  volatile uint8_t                    DevAddr_LE[4];                                            // JOIN-ACCEPT
  volatile uint8_t                    DevNonce_LE[2];                                           // JOIN-REQUEST, REJOIN-REQUEST - V1.02: random value
  volatile uint8_t                    Home_NetID_LE[3];                                         // JOIN-ACCEPT
  volatile uint32_t                   LastIqBalTimeUs;                                          // Last point of system time when procedure was done
  volatile uint32_t                   LastIqBalDurationUs;                                      // Duration of balancing procedure
  volatile int8_t                     LastIqBalTemp;                                            // Temperature degC when last I/Q balancing took place
  volatile float                      CrystalPpm;                                               // PPM value to correct local device crystal drift
  volatile float                      GatewayPpm;                                               // PPM value to adjust RX for remote gateway crystal drift

  /* Network / MAC specific */
  LoRaWANVersion_t                    LoRaWAN_ver;
//uint8_t                             NwkKey_1V1[16];                                           // Network root key (for OTA devices)
  volatile uint8_t                    NwkSKey_1V02[16];                                         // Session key - JOIN-ACCEPT (derived from: AppKey)
//uint8_t                             FNwkSKey_1V1[16];                                         // JOIN-ACCEPT
//uint8_t                             FNwkSIntKey_1V1[16];                                      // JOIN-ACCEPT (derived from: NwkKey)
//uint8_t                             SNwkSIntKey_1V1[16];                                      // JOIN-ACCEPT (derived from: NwkKey)
//uint8_t                             NwkSEncKey_1V1[16];                                       // JOIN-ACCEPT (derived from: NwkKey)
//uint8_t                             JSIntKey_1V1[16];                                         // (for OTA devices)
//uint8_t                             JSEncKey_1V1[16];                                         // (for OTA devices)
  volatile uint8_t                    NetID_LE[3];                                              // JOIN-RESPONSE
  volatile uint8_t                    DLSettings;                                               // JOIN-ACCEPT
  volatile uint8_t                    RXDelay;                                                  // JOIN-ACCEPT
  volatile uint8_t                    CFList[16];                                               // JOIN-ACCEPT - EU-868: Freq Ch3[2:0], Freq Ch4[2:0], Freq Ch5[2:0], Freq Ch6[2:0], Freq Ch7[2:0], CFListType[0]
  volatile float                      Ch_Frequencies_MHz[8];
  volatile uint8_t                    Ch_EnabledMsk;

  /* Join Server specific */
//uint8_t                             JoinEUI_LE_1V1[8];                                        // JOIN-REQUEST, REJOIN-REQUEST
//volatile uint8_t                    JoinNonce_LE_1V1[4];                                      // JOIN-ACCEPT
//volatile uint8_t                    ServerNonce_LE_1V1[3];

  /* Application specific */
  uint8_t                             AppEUI_LE[8];
  uint8_t                             AppKey[16];                                               // Application root key (for OTA devices)
  volatile uint8_t                    AppNonce_LE[3];                                           // JOIN-RESPONSE
  volatile uint8_t                    AppSKey_1V02[16];                                         // Session key - JOIN-ACCEPT (derived from: AppKey)

  /* Current transmission */
  volatile LoRaWANctxDir_t            Dir;
  volatile uint8_t                    FCtrl_ADR;
  volatile uint8_t                    FCtrl_ADRACKReq;
  volatile uint8_t                    FCtrl_ACK;
  volatile uint8_t                    FCtrl_ClassB;
  volatile uint8_t                    FCtrl_FPending;
  volatile uint8_t                    FPort;
  volatile uint32_t                   FCntUp;
  volatile uint32_t                   FCntDown;
  volatile uint8_t                    SpreadingFactor;
  volatile float                      FrequencyMHz;
  volatile uint8_t                    TxDr;
  volatile uint8_t                    TxCh;
  volatile int16_t                    LastRSSIDbm;
//volatile int16_t                    LastPacketStrengthDBm;
  volatile int8_t                     LastPacketSnrDb;
  volatile float                      LastFeiHz;
  volatile float                      LastFeiPpm;
} LoRaWANctx_t;


typedef enum LoRaWAN_MTypeBF {
  JoinRequest                         = 0,
  JoinAccept                          = 1,
  UnconfDataUp                        = 2,
  UncondDataDn                        = 3,
  ConfDataUp                          = 4,
  ConfDataDn                          = 5,
  RejoinReq                           = 6,
  Proprietary                         = 7
} LoRaWAN_MTypeBF_t;

typedef enum LoRaWAN_MajorBF {
  LoRaWAN_R1                          = 0,
} LoRaWAN_MajorBF_t;


#define LoRaWAN_MHDR_MType_SL         5
#define LoRaWAN_MHDR_Major_SL         0

#define LoRaWAN_FCtl_ADR_SL           7
#define LoRaWAN_FCtl_ADRACKReq_SL     6
#define LoRaWAN_FCtl_ACK_SL           5
#define LoRaWAN_FCtl_ClassB_SL        4
#define LoRaWAN_FCtl_FPending_SL      4
#define LoRaWAN_FCtl_FOptsLen_SL      0


uint8_t GET_BYTE_OF_WORD(uint32_t word, uint8_t pos);


typedef enum LoRaWANMAC_CID {

  ResetInd_UP                         = 0x01,                                                   // ABP devices only, LW 1.1
  ResetConf_DN                        = 0x01,                                                   // ABP devices only, LW 1.1

  LinkCheckReq_UP                     = 0x02,
  LinkCheckAns_DN                     = 0x02,

  LinkADRReq_DN                       = 0x03,
  LinkADRAns_UP                       = 0x03,

  DutyCycleReq_DN                     = 0x04,
  DutyCycleAns_UP                     = 0x04,

  RXParamSetupReq_DN                  = 0x05,
  RXParamSetupAns_UP                  = 0x05,

  DevStatusReq_DN                     = 0x06,
  DevStatusAns_UP                     = 0x06,

  NewChannelReq_DN                    = 0x07,
  NewChannelAns_UP                    = 0x07,

  RXTimingSetupReq_DN                 = 0x08,
  RXTimingSetupAns_UP                 = 0x08,

  TxParamSetupReq_DN                  = 0x09,
  TxParamSetupAns_UP                  = 0x09,

  DlChannelReq_DN                     = 0x0A,
  DlChannelAns_UP                     = 0x0A,

  RekeyInd_UP                         = 0x0B,
  RekeyConf_DN                        = 0x0B,

  ADRParamSetupReq_DN                 = 0x0C,
  ADRParamSetupAns_UP                 = 0x0C,

  DeviceTimeReq_UP                    = 0x0D,
  DeviceTimeAns_DN                    = 0x0D,

  ForceRejoinReq_DN                   = 0x0E,

  RejoinParamSetupReq_DN              = 0x0F,
  RejoinParamSetupAns_UP              = 0x0F,

} LoRaWANMAC_CID_t;


typedef struct FRMPayloadBlockA_Up {
  uint8_t                             variant;
  uint8_t                             _noUse[4];
  uint8_t                             Dir;
  uint8_t                             DevAddr[4];
  uint8_t                             FCntUp[4];
  uint8_t                             _pad;
  uint8_t                             idx;
} FRMPayloadBlockA_Up_t;

typedef struct MICBlockB0_Up {
  uint8_t                             variant;
  uint8_t                             _noUse[4];
  uint8_t                             Dir;
  uint8_t                             DevAddr[4];
  uint8_t                             FCntUp[4];
  uint8_t                             _pad;
  uint8_t                             len;
} MICBlockB0_Up_t;

typedef struct MICBlockB1_Up {
  uint8_t                             variant;
  uint8_t                             ConfFCnt[2];
  uint8_t                             TxDr;
  uint8_t                             TxCh;
  uint8_t                             Dir;
  uint8_t                             DevAddr[4];
  uint8_t                             FCntUp[4];
  uint8_t                             _pad;
  uint8_t                             len;
} MICBlockB1_Up_t;


typedef struct MICBlockB0_Dn {
  uint8_t                             variant;
  uint8_t                             ConfFCnt[2];
  uint8_t                             _noUse[2];
  uint8_t                             Dir;
  uint8_t                             DevAddr[4];
  uint8_t                             FCntDn[4];
  uint8_t                             _pad;
  uint8_t                             len;
} MICBlockB0_Dn_t;


typedef enum LoRaWAN_CalcMIC_JOINREQUEST {
  MIC_JOINREQUEST                     = 0x01,
  MIC_DATAMESSAGE                     = 0x11,
} LoRaWAN_CalcMIC_VARIANT_t;


typedef struct LoRaWAN_TX_Message {

/* Ready for transport section */
volatile uint8_t  msg_encoded_EncDone;
volatile uint8_t  msg_encoded_Len;
volatile uint8_t  msg_encoded_Buf[LoRaWAN_MsgLenMax];


/* Prepare section */
uint8_t           msg_prep_MHDR;
uint8_t           msg_prep_FCtrl;
uint16_t          msg_prep_FCnt;
//
uint8_t           msg_prep_FOpts_Len;
uint8_t           msg_prep_FOpts_Buf[16];
#ifdef LORAWAN_1V1
uint8_t           msg_prep_FOpts_Encoded[16];
#endif
//
uint8_t           msg_prep_FPort_absent;
uint8_t           msg_prep_FPort;
//
uint8_t           msg_prep_FRMPayload_Len;
uint8_t           msg_prep_FRMPayload_Buf[LoRaWAN_FRMPayloadMax];
uint8_t           msg_prep_FRMPayload_Encoded[LoRaWAN_FRMPayloadMax];

} LoRaWAN_TX_Message_t;

typedef struct LoRaWAN_RX_Message {

/* Received data section */
volatile uint8_t  msg_encoded_Len;
volatile uint8_t  msg_encoded_Buf[LoRaWAN_MsgLenMax];

} LoRaWAN_RX_Message_t;


typedef struct LoraliveApp {

  uint8_t                             voltage_32_v;

  /* Big endian */
  uint8_t                             dust025_10_hi;
  uint8_t                             dust025_10_lo;

  /* Big endian */
  uint8_t                             dust100_10_hi;
  uint8_t                             dust100_10_lo;

  char  id;

  union {
    struct loraliveApp_len6 {
    } l6;

    struct loraliveApp_len8 {
      int8_t                          temperature;
      uint8_t                         humidity_100;
    } l8;

    struct loraliveApp_len14 {
      uint8_t                         latitude_1000_sl24;
      uint8_t                         latitude_1000_sl16;
      uint8_t                         latitude_1000_sl08;
      uint8_t                         latitude_1000_sl00;

      uint8_t                         longitude_1000_sl24;
      uint8_t                         longitude_1000_sl16;
      uint8_t                         longitude_1000_sl08;
      uint8_t                         longitude_1000_sl00;
    } l14;

    struct loraliveApp_len16 {
      int8_t                          temperature;
      uint8_t                         humidity_100;

      uint8_t                         latitude_1000_sl24;
      uint8_t                         latitude_1000_sl16;
      uint8_t                         latitude_1000_sl08;
      uint8_t                         latitude_1000_sl00;

      uint8_t                         longitude_1000_sl24;
      uint8_t                         longitude_1000_sl16;
      uint8_t                         longitude_1000_sl08;
      uint8_t                         longitude_1000_sl00;
    } l16;
  } u;

} LoraliveApp_t;



void LoRaWANctx_applyKeys_trackMeApp(void);

uint8_t LoRaWAN_calc_randomChannel(LoRaWANctx_t* ctx);
float LoRaWAN_calc_Channel_to_MHz(LoRaWANctx_t* ctx, uint8_t channel, uint8_t dflt);

void loRaWANLoRaWANTaskInit(void);
void loRaWANLoRaWANTaskLoop(void);

#endif /* LORAWAN_H_ */
