/**
  ******************************************************************************
  * @file           : MotionSP.c
  * @author         : System Research & Applications Team - Catania Lab
  * @version        : v2.2.2
  * @date           : 20-Nov-2024
  * @brief          : This file contains definitions for the Motion Signal Processing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018-2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include "MotionSP.h"

/** @addtogroup MIDDLEWARES Middlewares
  * @{
  */

/** @addtogroup ST ST
  * @{
  */

/** @addtogroup STM32_MOTIONSP_LIB STM32 Motion Signal Processing Library
  * @{
  */

/** @addtogroup STM32_MOTIONSP_LIB_PUBLIC_VARIABLES STM32 Motion Signal Processing Library Public Variables
  * @{
  */

sMotionSP_Parameter_t MotionSP_Parameters;      //!< Algorithm Parameters
sCircBuffer_t AccCircBuffer;                    //!< Circular buffer for storing input values for FFT
arm_rfft_fast_instance_f32 fftS;                //!< Instance structure for the floating-point RFFT/RIFFT function
uint16_t magSize = 0;                           //!< to store the actual size of the FFT magnitude elements
uint16_t accCircBuffIndexForFft;                //!< Position index in circular buffer to perform FFT
sAcceleroParam_t sTimeDomain;                   //!< Time Domain Structure with parameters to use
sAcceleroODR_t AcceleroODR;                     //!< Real Accelerometer ODR evaluated
uint8_t FinishAvgFlag;                          //!< Flag to monitor the FFT Timing
uint8_t fftIsEnabled;                           //!< Flag to enable FFT computation
float Filter_Params[FFT_SIZE_MAX];              //!< Array of window filter parameters
float Window_Scale_Factor;                      //!< Scale factor to correct amplitude
sSumCnt_t AccSumCnt;                            //!< Sum counter for FFT during averaging
sAxesMagResults_t AccAxesMagResults;

sMotionSP_Data_t MotionSP_Data;                 //!< Algorithm Data
sTimeDomainData_t TimeDomainData;               //!< Time Domain Structure with parameters to use
sAccMagResults_t AccMagResults;                 //!< FFT magnitude data

#ifdef USE_SUBRANGE
sSubrange_t SRAmplitude;                        //!< X-Y-Z Threshold Amplitude Subrange Arrays
sSubrange_t SRBinVal;                           //!< X-Y-Z Threshold Bin Frequency Subrange Arrays
#endif /* USE_SUBRANGE */

/**
  * @}
  */

/** @addtogroup STM32_MOTIONSP_LIB_PRIVATE_VARIABLES STM32 Motion Signal Processing Library Private Variables
  * @{
  */

sAxesMagBuff_t AccAxesAvgMagBuff;               //!< Array for storing accelerometer magnitude average values
static SensorVal_f_t SpeedTimeDomain;           //!< Time Domain Speed Arrays to use during integration
static SensorVal_f_t SpeedTimeDomain_noDC;      //!< Time Domain Speed Arrays without DC Offset to use during integration

/**
  * @}
  */

/** @addtogroup STM32_MOTIONSP_LIB_PRIVATE_FUNCTIONS STM32 Motion Signal Processing Library Private Functions
  * @{
  */

static void MotionSP_speedDelOffset(SensorVal_f_t *pDstArr, SensorVal_f_t *pSrcArr, float Smooth, uint8_t Restart);
static void MotionSP_evalSpeedFromAccelero(SensorVal_f_t *pDstArr, sCircBuffer_t *pSrcArr, uint8_t Restart);
static void MotionSP_SwSpeedRmsFilter(SensorVal_f_t *pDstArr, SensorVal_f_t *pSrcArr, float ExpTau, uint8_t start);
static void MotionSP_SwAccRmsFilter(SensorVal_f_t *pDstArr, sCircBuffer_t *pSrcArr, float Lambda, uint8_t start);
static void MotionSP_SwAccPkEval(SensorVal_f_t *pDstArr, sCircBuffer_t *pSrcArr);

static void MotionSP_TD_PeakEvalFromCircBuff(sTimeDomainData_t *pDst, sCircBuff_t *pSrc, uint16_t SrcId);
static void MotionSP_TD_SpeedEvalFromCircBuff(sTimeDomainData_t *pDst, sCircBuff_t *pSrc, uint16_t SrcId, sAcceleroODR_t  AccOdr, uint8_t Rst);
static void MotionSP_TD_AccRmsEvalFromCircBuff(sTimeDomainData_t *pDst, sCircBuff_t *pSrc, uint16_t SrcId, sAcceleroODR_t  AccOdr, uint8_t Rst);

/**
  *  @brief  High Pass Filter to delete Speed Offset
  *  @param  pDstArr pointer to Speed Array without offset
  *  @param  pSrcArr pointer to Speed Array with offset
  *  @param  Smooth constant
  *  @param  Restart flag to reInit internal value
  *  @return none
  */
static void MotionSP_speedDelOffset(SensorVal_f_t *pDstArr, SensorVal_f_t *pSrcArr, float Smooth, uint8_t Restart)
{
  static SensorVal_f_t DstArrPre;
  static SensorVal_f_t SrcArrPre;
  
  if (Restart == 1)
  {
    pDstArr->AXIS_X = 0.0;
    pDstArr->AXIS_Y = 0.0;
    pDstArr->AXIS_Z = 0.0;
    DstArrPre.AXIS_X = pSrcArr->AXIS_X;
    DstArrPre.AXIS_Y = pSrcArr->AXIS_Y;
    DstArrPre.AXIS_Z = pSrcArr->AXIS_Z;
    SrcArrPre.AXIS_X = pSrcArr->AXIS_X;
    SrcArrPre.AXIS_Y = pSrcArr->AXIS_Y;
    SrcArrPre.AXIS_Z = pSrcArr->AXIS_Z;
  }
  else
  {
    pDstArr->AXIS_X = (Smooth * DstArrPre.AXIS_X) + Smooth * (pSrcArr->AXIS_X - SrcArrPre.AXIS_X);
    pDstArr->AXIS_Y = (Smooth * DstArrPre.AXIS_Y) + Smooth * (pSrcArr->AXIS_Y - SrcArrPre.AXIS_Y);
    pDstArr->AXIS_Z = (Smooth * DstArrPre.AXIS_Z) + Smooth * (pSrcArr->AXIS_Z - SrcArrPre.AXIS_Z);
    DstArrPre.AXIS_X = pDstArr->AXIS_X;
    DstArrPre.AXIS_Y = pDstArr->AXIS_Y;
    DstArrPre.AXIS_Z = pDstArr->AXIS_Z;
    SrcArrPre.AXIS_X = pSrcArr->AXIS_X;
    SrcArrPre.AXIS_Y = pSrcArr->AXIS_Y;
    SrcArrPre.AXIS_Z = pSrcArr->AXIS_Z;
  }
}

/**
  *  @brief  Integration Algorithm to evaluate Speed starting from Accelerometer data
  *  @param  pDstArr speed value output
  *  @param  pSrcArr accelero value from external circular buffer
  *  @param  Restart flag to reInit internal value
  *  @return none
  */
static void MotionSP_evalSpeedFromAccelero(SensorVal_f_t *pDstArr,
                                           sCircBuffer_t *pSrcArr,
                                           uint8_t Restart)
{
  uint16_t IndexCurr, IndexPre;
  float DeltaT;
  static SensorVal_f_t DstArrPre;
  
  DeltaT = AcceleroODR.Period;
  IndexCurr = pSrcArr->IdPos;
  IndexPre  = IndexCurr-1;
  
  if (IndexCurr == 0) 
    IndexPre  = pSrcArr->Size-1;

  if (Restart == 1) 
  {  
    memset((void *)pDstArr, 0, sizeof(SensorVal_f_t));
    memset((void *)&DstArrPre, 0, sizeof(SensorVal_f_t));
  }
  
  else
  {     // vi+1 = vi +[(1-GAMMA)*DELTA_T]*ai + (GAMMA*DELTA_T)*ai+1 /* in mm/s

    pDstArr->AXIS_X = DstArrPre.AXIS_X +
                      (((1-GAMMA)*DeltaT)*(pSrcArr->Data.AXIS_X[IndexPre]))+
                      (GAMMA*DeltaT*(pSrcArr->Data.AXIS_X[IndexCurr]));

    pDstArr->AXIS_Y = DstArrPre.AXIS_Y +
                      (((1-GAMMA)*DeltaT)*(pSrcArr->Data.AXIS_Y[IndexPre]))+
                      (GAMMA*DeltaT*(pSrcArr->Data.AXIS_Y[IndexCurr]));
 
    pDstArr->AXIS_Z = DstArrPre.AXIS_Z +
                      (((1-GAMMA)*DeltaT)*(pSrcArr->Data.AXIS_Z[IndexPre]))+
                      (GAMMA*DeltaT*(pSrcArr->Data.AXIS_Z[IndexCurr]));
    
    memcpy((void *)&DstArrPre, (void *)pDstArr, sizeof(SensorVal_f_t));
  }
}

/**
  * @}
  */

/** @addtogroup STM32_MOTIONSP_LIB_EXPORTED_FUNCTIONS STM32 Motion Signal Processing Library Exported Functions
  * @{
  */

/**
  * @brief  Filter to estimate the Speed Moving RMS Values using FAST Lambda
  * @param  pDstArr pointer to RMS Output Arrays
  * @param  pSrcArr pointer to Input Speed Values
  * @param  Lambda  smoothing factor
  * @param  start flag to re-init first value
  * @return none
  *
  * @details More details
  * Reference by MATLAB DSP Toolbox modified with Y(n-1)^2
  */
static void MotionSP_SwSpeedRmsFilter(SensorVal_f_t *pDstArr, SensorVal_f_t *pSrcArr, float Lambda, uint8_t start)
{
  SensorVal_f_t SquareData = {0, 0, 0};
  SensorVal_f_t PrevSquareData  = {0, 0, 0};
  static float WN = 0.0;
  static float WN_1 = 0.0;

  if (start == 1)
  {
    pDstArr->AXIS_X = pSrcArr->AXIS_X;
    pDstArr->AXIS_Y = pSrcArr->AXIS_Y;
    pDstArr->AXIS_Z = pSrcArr->AXIS_Z;
    WN = 1;
  }
  else
  {
    SquareData.AXIS_X = pSrcArr->AXIS_X * pSrcArr->AXIS_X;
    SquareData.AXIS_Y = pSrcArr->AXIS_Y * pSrcArr->AXIS_Y;
    SquareData.AXIS_Z = pSrcArr->AXIS_Z * pSrcArr->AXIS_Z;

    PrevSquareData.AXIS_X = pDstArr->AXIS_X * pDstArr->AXIS_X;
    PrevSquareData.AXIS_Y = pDstArr->AXIS_Y * pDstArr->AXIS_Y;
    PrevSquareData.AXIS_Z = pDstArr->AXIS_Z * pDstArr->AXIS_Z;

    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_X + (1 / WN) * SquareData.AXIS_X), &pDstArr->AXIS_X);
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_Y + (1 / WN) * SquareData.AXIS_Y), &pDstArr->AXIS_Y);
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_Z + (1 / WN) * SquareData.AXIS_Z), &pDstArr->AXIS_Z);

    WN_1 =  WN;
    WN =  Lambda * WN_1 + 1;
  }
}

/**
  * @brief  Filter to estimate the Accelerometer Moving RMS Values using FAST Lambda
  * @param  pDstArr pointer to related RMS Output
  * @param  pSrcArr pointer to accelerometer circular buffer
  * @param  Lambda fast smoothing factor
  * @param  start flag to Re-Init first value
  * @return none
  */
static void MotionSP_SwAccRmsFilter(SensorVal_f_t *pDstArr, sCircBuffer_t *pSrcArr, float Lambda, uint8_t start)
{
  uint16_t Index = 0;
  SensorVal_f_t SquareData = {0, 0, 0};
  SensorVal_f_t PrevSquareData  = {0, 0, 0};
  static float WN = 0.0;
  static float WN_1 = 0.0;

  Index = pSrcArr->IdPos;
  
  if (start == 1)
  {
    pDstArr->AXIS_X = pSrcArr->Data.AXIS_X[Index];
    pDstArr->AXIS_Y = pSrcArr->Data.AXIS_Y[Index];
    pDstArr->AXIS_Z = pSrcArr->Data.AXIS_Z[Index];
    WN = 1;
  }
  else
  {
    SquareData.AXIS_X = pSrcArr->Data.AXIS_X[Index] * pSrcArr->Data.AXIS_X[Index];
    SquareData.AXIS_Y = pSrcArr->Data.AXIS_Y[Index] * pSrcArr->Data.AXIS_Y[Index];;
    SquareData.AXIS_Z = pSrcArr->Data.AXIS_Z[Index] * pSrcArr->Data.AXIS_Z[Index];;

    PrevSquareData.AXIS_X = pDstArr->AXIS_X * pDstArr->AXIS_X;
    PrevSquareData.AXIS_Y = pDstArr->AXIS_Y * pDstArr->AXIS_Y;
    PrevSquareData.AXIS_Z = pDstArr->AXIS_Z * pDstArr->AXIS_Z;

    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_X + (1 / WN) * SquareData.AXIS_X), &pDstArr->AXIS_X);
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_Y + (1 / WN) * SquareData.AXIS_Y), &pDstArr->AXIS_Y);
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_Z + (1 / WN) * SquareData.AXIS_Z), &pDstArr->AXIS_Z);

    WN_1 =  WN;
    WN =  Lambda * WN_1 + 1;
  }
}

/**
  * @brief  Peak evaluation for Accelerometer Value stored inside the Circular Buffer
  * @param  pDstArr pointer to AccPeak Output
  * @param  pSrcArr pointer to input accelero circular buffer
  * @return none
  */
static void MotionSP_SwAccPkEval(SensorVal_f_t *pDstArr, sCircBuffer_t *pSrcArr)
{
  uint16_t Index = 0;

  Index = pSrcArr->IdPos;

  if (pDstArr->AXIS_X < fabsf(pSrcArr->Data.AXIS_X[Index]))
  {
    pDstArr->AXIS_X = fabsf(pSrcArr->Data.AXIS_X[Index]);
  }
  if (pDstArr->AXIS_Y < fabsf(pSrcArr->Data.AXIS_Y[Index]))
  {
    pDstArr->AXIS_Y = fabsf(pSrcArr->Data.AXIS_Y[Index]);
  }
  if (pDstArr->AXIS_Z < fabsf(pSrcArr->Data.AXIS_Z[Index]))
  {
    pDstArr->AXIS_Z = fabsf(pSrcArr->Data.AXIS_Z[Index]);
  }
}

/**
  * @brief  Peak evaluation for accelerometer value stored inside the circular buffer
  * @param  pDst Time Domain Data as destination
  * @param  pSrc Circular Buffer as source
  * @param  SrcId Actual Circular Buffer index
  * @return none
  */
static void MotionSP_TD_PeakEvalFromCircBuff(sTimeDomainData_t *pDst, sCircBuff_t *pSrc, uint16_t SrcId)
{
  if (pDst->AccPeak.AXIS_X < fabsf(pSrc->Array.X[SrcId]))
    pDst->AccPeak.AXIS_X = fabsf(pSrc->Array.X[SrcId]);
  if (pDst->AccPeak.AXIS_Y < fabsf(pSrc->Array.Y[SrcId]))
    pDst->AccPeak.AXIS_Y = fabsf(pSrc->Array.Y[SrcId]);
  if (pDst->AccPeak.AXIS_Z < fabsf(pSrc->Array.Z[SrcId]))
    pDst->AccPeak.AXIS_Z = fabsf(pSrc->Array.Z[SrcId]);      
}

/**
  * @brief  Speed evaluation for accelerometer value stored inside the circular buffer
  * @param  pDst Time Domain Data as destination
  * @param  pSrc Circular Buffer as source
  * @param  SrcId Actual Circular Buffer index
  * @param AccOdr Accelerometer ODR info
  * @param Rst Restart flag
  * @return none
  */
static void MotionSP_TD_SpeedEvalFromCircBuff(sTimeDomainData_t *pDst, sCircBuff_t *pSrc, uint16_t SrcId, sAcceleroODR_t  AccOdr, uint8_t Rst)
{
  uint16_t previd = 0;
  
  if (Rst)
  {
    pDst->Speed.AXIS_X = 0.0;
    pDst->Speed.AXIS_Y = 0.0;
    pDst->Speed.AXIS_Z = 0.0;
  }
  else
  {
    if (SrcId == 0)
      previd = (pSrc->Size)-1;
    else
      previd = SrcId-1;
    
    /* vi = vi-1 +[(1-GAMMA)*DELTA_T]*ai-1 + (GAMMA*DELTA_T)*ai (in mm/s) */
    pDst->Speed.AXIS_X = pDst->Speed.AXIS_X +\
      (((1-GAMMA)*AccOdr.Period)*(pSrc->Array.X[previd]))+\
        (GAMMA*AccOdr.Period*(pSrc->Array.X[SrcId]));
    pDst->Speed.AXIS_Y = pDst->Speed.AXIS_Y +\
      (((1-GAMMA)*AccOdr.Period)*(pSrc->Array.Y[previd]))+\
        (GAMMA*AccOdr.Period*(pSrc->Array.Y[SrcId]));
    pDst->Speed.AXIS_Z = pDst->Speed.AXIS_Z +\
      (((1-GAMMA)*AccOdr.Period)*(pSrc->Array.Z[previd]))+\
        (GAMMA*AccOdr.Period*(pSrc->Array.Z[SrcId]));
  }
}

/**
  * @brief  Acceleration RMS evaluation for accelerometer value stored inside the circular buffer
  * @param  pDst Time Domain Data as destination
  * @param  pSrc Circular Buffer as source
  * @param  SrcId Actual Circular Buffer index
  * @param AccOdr Accelerometer ODR info
  * @param Rst Restart flag
  * @return none
  */
static void MotionSP_TD_AccRmsEvalFromCircBuff(sTimeDomainData_t *pDst, sCircBuff_t *pSrc, uint16_t SrcId, sAcceleroODR_t  AccOdr, uint8_t Rst)
{
  SensorVal_f_t SquareData = {0, 0, 0};
  SensorVal_f_t PrevSquareData = {0, 0, 0};
  static float WN = 0.0;
  static float WN_1 = 0.0;
  
  if (Rst)
  {
    pDst->AccRms.AXIS_X = pSrc->Array.X[SrcId];
    pDst->AccRms.AXIS_Y = pSrc->Array.Y[SrcId];
    pDst->AccRms.AXIS_Z = pSrc->Array.Z[SrcId];
    
    WN = 1;
  }
  else
  {
    SquareData.AXIS_X = pSrc->Array.X[SrcId] * pSrc->Array.X[SrcId];
    SquareData.AXIS_Y = pSrc->Array.Y[SrcId] * pSrc->Array.Y[SrcId];
    SquareData.AXIS_Z = pSrc->Array.Z[SrcId] * pSrc->Array.Z[SrcId];;
    
    PrevSquareData.AXIS_X = pDst->AccRms.AXIS_X * pDst->AccRms.AXIS_X;
    PrevSquareData.AXIS_Y = pDst->AccRms.AXIS_Y * pDst->AccRms.AXIS_Y;
    PrevSquareData.AXIS_Z = pDst->AccRms.AXIS_Z * pDst->AccRms.AXIS_Z;
    
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_X + (1 / WN) * SquareData.AXIS_X), &pDst->AccRms.AXIS_X);
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_Y + (1 / WN) * SquareData.AXIS_Y), &pDst->AccRms.AXIS_Y);
    arm_sqrt_f32(((1 - 1 / WN) * PrevSquareData.AXIS_Z + (1 / WN) * SquareData.AXIS_Z), &pDst->AccRms.AXIS_Z);
    
    WN_1 =  WN;
    WN =  AccOdr.Tau * WN_1 + 1;
  }
}

/**
  *  @brief High Pass Filter to delete Accelerometer Offset
  *
  *  @param pDstArr: pointer to Accelero Array without offset
  *  @param pSrcArr: pointer to Accelero Array with offset
  *  @param Smooth: smoothing factor
  *  @param Restart: flag to Re-Init internal value
  */
void MotionSP_accDelOffset(SensorVal_f_t *pDstArr, SensorVal_f_t *pSrcArr, float Smooth, uint16_t Restart)
{
  static SensorVal_f_t DstArrPre;
  static SensorVal_f_t SrcArrPre;
  
  if (Restart == 1)
  {
    pDstArr->AXIS_X = 0.0;
    pDstArr->AXIS_Y = 0.0;
    pDstArr->AXIS_Z = 0.0;
    DstArrPre.AXIS_X = pSrcArr->AXIS_X;
    DstArrPre.AXIS_Y = pSrcArr->AXIS_Y;
    DstArrPre.AXIS_Z = pSrcArr->AXIS_Z;
    SrcArrPre.AXIS_X = pSrcArr->AXIS_X;
    SrcArrPre.AXIS_Y = pSrcArr->AXIS_Y;
    SrcArrPre.AXIS_Z = pSrcArr->AXIS_Z;
  }
  else
  {
    pDstArr->AXIS_X = (Smooth * DstArrPre.AXIS_X) + Smooth * (pSrcArr->AXIS_X - SrcArrPre.AXIS_X);
    pDstArr->AXIS_Y = (Smooth * DstArrPre.AXIS_Y) + Smooth * (pSrcArr->AXIS_Y - SrcArrPre.AXIS_Y);
    pDstArr->AXIS_Z = (Smooth * DstArrPre.AXIS_Z) + Smooth * (pSrcArr->AXIS_Z - SrcArrPre.AXIS_Z);
    DstArrPre.AXIS_X = pDstArr->AXIS_X;
    DstArrPre.AXIS_Y = pDstArr->AXIS_Y;
    DstArrPre.AXIS_Z = pDstArr->AXIS_Z;
    SrcArrPre.AXIS_X = pSrcArr->AXIS_X;
    SrcArrPre.AXIS_Y = pSrcArr->AXIS_Y;
    SrcArrPre.AXIS_Z = pSrcArr->AXIS_Z;
  }
}

/**
  *  @brief Create a circular Buffer from the Accelerometer data to get after
  *         the FFT Input Arrays
  *  @param pCircBuff Description for pCircBuff
  *  @param buffType Description for buffType
  *  @return Return description
  *
  */
void MotionSP_CreateAccCircBuffer(sCircBuffer_t *pCircBuff, SensorVal_f_t buffType)
{
  pCircBuff->IdPos += 1;
  
  if (pCircBuff->IdPos == pCircBuff->Size)
  {
    pCircBuff->IdPos = 0;
    pCircBuff->Ovf = 1;
  }

  pCircBuff->Data.AXIS_X[pCircBuff->IdPos] = buffType.AXIS_X*G_CONV;
  pCircBuff->Data.AXIS_Y[pCircBuff->IdPos] = buffType.AXIS_Y*G_CONV;
  pCircBuff->Data.AXIS_Z[pCircBuff->IdPos] = buffType.AXIS_Z*G_CONV;
}

/**
  * @brief Time Domain Processing
  * @brief From accelerometer to speed estimation to target the final RMS value processing
  * @param pTimeDomain Pointer to time domain parameter to evaluate
  * @param td_type Time domain analysis type
  * @param Restart Flag
  * @return none
  */
void MotionSP_TimeDomainProcess(sAcceleroParam_t *pTimeDomain, Td_Type_t td_type, uint8_t Restart)
{
  MotionSP_SwAccPkEval(&pTimeDomain->AccPeak, &AccCircBuffer);

  if (td_type == TD_SPEED)
  {
    /* TIME DOMAIN ANALYSIS: Speed RMS Moving AVERAGE */
    MotionSP_evalSpeedFromAccelero(&SpeedTimeDomain, &AccCircBuffer, Restart);
    // Delete the Speed DC components
    MotionSP_speedDelOffset(&SpeedTimeDomain_noDC, &SpeedTimeDomain, DC_SMOOTH, Restart);
    // Evaluate SwExponential Filter by TAU_FILTER on Speed data
    MotionSP_SwSpeedRmsFilter(&sTimeDomain.SpeedRms, &SpeedTimeDomain_noDC, AcceleroODR.Tau, Restart);
  }

  if (td_type == TD_ACCELERO)
  {
    /* TIME DOMAIN ANALYSIS: Accelerometer RMS Moving AVERAGE */
    // Evaluate SwExponential Filter by TAU_FILTER on Accelerometer data
    MotionSP_SwAccRmsFilter(&sTimeDomain.AccRms, &AccCircBuffer, AcceleroODR.Tau, Restart);
  }

  if (td_type == TD_BOTH_TAU)
  {
    /* TIME DOMAIN ANALYSIS: Speed and both RMS Moving AVERAGE TAU */
    MotionSP_evalSpeedFromAccelero(&SpeedTimeDomain, &AccCircBuffer, Restart);
    // Delete the Speed DC components
    MotionSP_speedDelOffset(&SpeedTimeDomain_noDC, &SpeedTimeDomain, DC_SMOOTH, Restart);
    // Evaluate SwExponential Filter by TAU_FILTER on Speed data
    MotionSP_SwSpeedRmsFilter(&sTimeDomain.SpeedRms, &SpeedTimeDomain_noDC, AcceleroODR.Tau, Restart);
    // Evaluate SwExponential Filter by TAU_FILTER on Accelerometer data
    MotionSP_SwAccRmsFilter(&sTimeDomain.AccRms, &AccCircBuffer, AcceleroODR.Tau, Restart);
  }
}

/**
  * @brief Time Domain Data Evaluation from a stored accelerations
  * @param pTimeDomainData Time domain data to be filled
  * @param pAccCircBuff Stored accelerations
  * @param NewDataSamples New samples of stored accelerations
  * @param td_type Time domain analysis type
  * @param AccOdr Accelerometer ODR info
  * @param Rst Restart flag
  * @return none
  */
void MotionSP_TimeDomainEvalFromCircBuff(sTimeDomainData_t *pTimeDomainData, sCircBuff_t *pAccCircBuff, uint16_t NewDataSamples, Td_Type_t td_type, sAcceleroODR_t  AccOdr, uint8_t Rst)
{
  int16_t id = 0;
  uint16_t s = 0;
  
  /* Evaluate the initial IdPos for theese new data samples */
  id = (pAccCircBuff->IdPos) - (NewDataSamples - 1);
  if (id < 0)
    id += (pAccCircBuff->Size);
  
  for (s=0; s<NewDataSamples; s++)
  {
    /* Peak evaluation */
    MotionSP_TD_PeakEvalFromCircBuff(pTimeDomainData, pAccCircBuff, id);
    
    /* RMS ********************************************************************/
    switch (td_type)
    {
    case TD_SPEED:
      /* Speed evaluation */
      MotionSP_TD_SpeedEvalFromCircBuff(pTimeDomainData, pAccCircBuff, id, AccOdr, Rst);
      
      /* Remove the speed DC components */
      MotionSP_speedDelOffset(&pTimeDomainData->Speed_noDC, &pTimeDomainData->Speed, DC_SMOOTH, Rst);
      
      /* Evaluate SwExponential Filter by TAU_FILTER on Speed data */
      MotionSP_SwSpeedRmsFilter(&pTimeDomainData->SpeedRms, &pTimeDomainData->Speed_noDC, AccOdr.Tau, Rst);
      break;
      
    case TD_ACCELERO:
      /* Acceleration RMS evaluation */
      MotionSP_TD_AccRmsEvalFromCircBuff(pTimeDomainData, pAccCircBuff, id, AccOdr, Rst);
      break;
      
    case TD_BOTH_TAU:
      /* Speed evaluation */
      MotionSP_TD_SpeedEvalFromCircBuff(pTimeDomainData, pAccCircBuff, id, AccOdr, Rst);
      
      /* Remove the speed DC components */
      MotionSP_speedDelOffset(&pTimeDomainData->Speed_noDC, &pTimeDomainData->Speed, DC_SMOOTH, Rst);
      
      /* Evaluate SwExponential Filter by TAU_FILTER on Speed data */
      MotionSP_SwSpeedRmsFilter(&pTimeDomainData->SpeedRms, &pTimeDomainData->Speed_noDC, AccOdr.Tau, Rst);

      /* Acceleration RMS evaluation */
      MotionSP_TD_AccRmsEvalFromCircBuff(pTimeDomainData, pAccCircBuff, id, AccOdr, Rst);
      break;
      
    default:
      break;
    }
    /**************************************************************************/
    
    /* Prepare next id */
    id++;
    id %= (pAccCircBuff->Size);
    
    if (Rst)
      Rst = 0;
  }
}

/**
  * @brief  Perform a FFT just for one Axis
  * @param  pfftS  pointer to the instance structure for the floating-point RFFT/RIFFT function
  * @param  pfftIn pointer to the FFT-In array
  * @param  pfftOut pointer to the FFT-Out array
  * @return none
  */
void MotionSP_fftCalc(arm_rfft_fast_instance_f32 *pfftS, float *pfftIn, float *pfftOut)
{
  static float fftTmp[FFT_SIZE_MAX];

  // Compute the Fourier transform of the signal.
  arm_rfft_fast_f32(pfftS, pfftIn, fftTmp, 0);

  // Compute the two-sided spectrum
  arm_cmplx_mag_f32(fftTmp, pfftOut, pfftS->fftLenRFFT / 2);
}

/**
  * @brief  Re-scaling the FFT Output after the RAW frequency Domain processing
  * @param  pfftCmplxMag description for pfftCmplxMag
  * @param  size description for size
  * @return none
  */
void MotionSP_fftAdapt(sAxesMagBuff_t *pfftCmplxMag, uint16_t size)
{
  for (int i = 0; i < size; i++)
  {
    if (i == 0) /* Adjust DC component */
    {
      pfftCmplxMag->AXIS_X[i] = (pfftCmplxMag->AXIS_X[i] / (2 * size)) * Window_Scale_Factor;
      pfftCmplxMag->AXIS_Y[i] = (pfftCmplxMag->AXIS_Y[i] / (2 * size)) * Window_Scale_Factor;
      pfftCmplxMag->AXIS_Z[i] = (pfftCmplxMag->AXIS_Z[i] / (2 * size)) * Window_Scale_Factor;
    }
    else /* Adjust all the elements with i > 0 */
    {
      pfftCmplxMag->AXIS_X[i] = (pfftCmplxMag->AXIS_X[i] / size) * Window_Scale_Factor;
      pfftCmplxMag->AXIS_Y[i] = (pfftCmplxMag->AXIS_Y[i] / size) * Window_Scale_Factor;
      pfftCmplxMag->AXIS_Z[i] = (pfftCmplxMag->AXIS_Z[i] / size) * Window_Scale_Factor;
    }
  }
}

/**
  * @brief  Re-scaling the FFT Output after the RAW frequency Domain processing
  * @param  pAccMagResults The structure for FFT Results
  * @param  WSF Scale factor to correct amplitude
  * @return none
  */
void MotionSP_fftAdapting(sAccMagResults_t *pAccMagResults, float WSF)
{
  uint16_t cnt = pAccMagResults->MagSizeTBU;
  uint16_t size = pAccMagResults->MagSize;
  
  for (int i = 0; i < cnt; i++)
  {
    if (i == 0) /* Adjust DC component */
    {
      pAccMagResults->Array.X[i] = 0.0;
      pAccMagResults->Array.Y[i] = 0.0;
      pAccMagResults->Array.Z[i] = 0.0;
    }
    else /* Adjust all the elements with i > 0 */
    {
      pAccMagResults->Array.X[i] = (pAccMagResults->Array.X[i] * WSF) / size;
      pAccMagResults->Array.Y[i] = (pAccMagResults->Array.Y[i] * WSF) / size;
      pAccMagResults->Array.Z[i] = (pAccMagResults->Array.Z[i] * WSF) / size;
    }
  }
}

/**
  * @brief  Find the biggest peak in the spectrum
  * @param  pfftCmplxMag description for pfftCmplxMag
  * @param  size description for size
  * @param  AccAxesMagResults description for AccAxesMagResults
  * @return none
  */
void MotionSP_fftFindPeak(sAxesMagBuff_t *pfftCmplxMag, uint16_t size, sAxesMagResults_t *AccAxesMagResults)
{
  arm_max_f32(pfftCmplxMag->AXIS_X, size, &AccAxesMagResults->X_Value, &AccAxesMagResults->X_Index);
  arm_max_f32(pfftCmplxMag->AXIS_Y, size, &AccAxesMagResults->Y_Value, &AccAxesMagResults->Y_Index);
  arm_max_f32(pfftCmplxMag->AXIS_Z, size, &AccAxesMagResults->Z_Value, &AccAxesMagResults->Z_Index);
}

/**
  * @brief  Find the biggest peak in the spectrum
  * @param  pAccMagResults The structure for FFT Results
  * @return none
  */
void MotionSP_fftPeakFinding(sAccMagResults_t *pAccMagResults)
{
  uint16_t size = pAccMagResults->MagSizeTBU;
   
  arm_max_f32(pAccMagResults->Array.X, size, &pAccMagResults->Max.X.value, &pAccMagResults->Max.X.loc);
  arm_max_f32(pAccMagResults->Array.Y, size, &pAccMagResults->Max.Y.value, &pAccMagResults->Max.Y.loc);
  arm_max_f32(pAccMagResults->Array.Z, size, &pAccMagResults->Max.Z.value, &pAccMagResults->Max.Z.loc);
}

/**
  * @brief  Initialize Windowing Coefficient Arrays
  * @param  Filter_Params pointer to filtering parameters array
  * @param  size filtering parameters array size
  * @param  Ftype filtering method
  * @return none
  */
void MotionSP_SetWindFiltArray(float *Filter_Params, uint16_t size, Filt_Type_t Ftype)
{
  for (int i = 0; i < size; i++)
  {
    if (Ftype == RECTANGULAR)
    {
      Filter_Params[i] = 1.0f;
    }

    if (Ftype == HANNING)
    {
      Filter_Params[i] = (0.5f * (1 - arm_cos_f32((2 * PI * i) / (size - 1))));
    }

    if (Ftype == HAMMING)
    {
      Filter_Params[i] = 0.54f - (0.46f * (arm_cos_f32((2 * PI * i) / (size - 1))));
    }

    if (Ftype == FLAT_TOP)
      Filter_Params[i] = 0.21557895f - \
                         (0.41663158f * arm_cos_f32((2 * PI * i) / (size - 1))) + \
                         0.277263158f * (arm_cos_f32((4 * PI * i) / (size - 1))) - \
                         0.083578947f * (arm_cos_f32((6 * PI * i) / (size - 1))) + \
                         0.006947368f * (arm_cos_f32((8 * PI * i) / (size - 1)));
  }

  switch (Ftype)
  {
    case RECTANGULAR:
      Window_Scale_Factor = 1.0f;
      break;

    case HANNING:
      Window_Scale_Factor = 2.0f;
      break;

    case HAMMING:
      Window_Scale_Factor = 1.85f;
      break;

    case FLAT_TOP:
      Window_Scale_Factor = 4.55f;
      break;
  }
}

/**
  * @brief Windowing Filter applied to the input data before to perform FFT
  * @param pDstArr pointer to output arrays of sata filtered
  * @param pSrcArr pointer to input arrays of data to be filtered
  * @param SizeArr input/output array size
  * @param Window_Params oointer to windowing coefficients array
  * @return none
  */
void motionSP_fftUseWindow(float *pDstArr, float *pSrcArr, uint16_t SizeArr, float *Window_Params)
{
  arm_mult_f32(pSrcArr, Window_Params, pDstArr, SizeArr);
}

/**
  * @brief  Get the FFT-In array from the circular buffer
  * @param  pDst pointer to the destination array
  * @param  DstSize destination array size
  * @param  pSrc pointer to the source array
  * @param  SrcSize source array size
  * @param  SrcLastPos last index of data to be taken
  * @retval 0 in case of success
  * @retval 1 in case of failure
  */
uint8_t MotionSP_fftInBuild(float *pDst, uint16_t DstSize, float *pSrc, uint16_t SrcSize, uint16_t SrcLastPos)
{
  int16_t initPos;
  uint16_t pos2end;

  if (SrcLastPos < SrcSize)
  {
    // Replace the last index of data to be taken with the first one
    initPos = SrcLastPos - (DstSize - 1);
    if (initPos < 0)
    {
      initPos += SrcSize;
    }

    if (initPos <= (SrcSize - DstSize))
    {
      memcpy((void *)pDst, (void *)(pSrc + initPos), DstSize * sizeof(float));
    }
    else
    {
      pos2end = SrcSize - initPos;
      memcpy((void *)pDst, (void *)(pSrc + initPos), pos2end * sizeof(float));
      memcpy((void *)(pDst + pos2end), (void *)pSrc, (DstSize - pos2end)*sizeof(float));
    }
  }
  else
  {
    return 1;
  }

  return 0;
}

/**
  * @brief  Averaging of all the FFT Arrays Outputs based on acquisitions number
  * @param  pDstArr pointer to temporary and final Average Arrays Output
  * @param  pSrcArr pointer to input array
  * @param  LenArr array size lenght
  * @param  pSumCnt max number of samples to process during the average
  * @param  MaxSumCnt description for MaxSumCnt
  * @retval 1 in case of success
  * @retval 0 in case of failure
  */
uint8_t MotionSP_fftAverageCalcSamples(float *pDstArr, float *pSrcArr, uint16_t LenArr, uint16_t *pSumCnt, uint8_t MaxSumCnt)
{
  uint16_t j;

  if ((*pSumCnt == 0) || (MaxSumCnt <= *pSumCnt))
  {
    // reset the destination array
    memset((void *)pDstArr, 0, LenArr * sizeof(float));

    // reset the number of times
    *pSumCnt = 0;
  }

  // add new source values to destination
  for (j = 0; j < LenArr; j++)
  {
    *(pDstArr + j) += *(pSrcArr + j);
  }

  // increase the number of times
  *pSumCnt += 1;

  // check for reaching of max number of times
  if (*pSumCnt == MaxSumCnt)
  {
    // calculate the average
    for (j = 0; j < LenArr; j++)
    {
      *(pDstArr + j) = *(pDstArr + j) / (*pSumCnt);
    }

    // reset the number of times
    *pSumCnt = 0;

    return 1;
  }

  return 0;
}

/**
  *  @brief  Averaging of all the FFT Arrays Outputs based on timing acquisition
  *  @param  pDstArr pDstArr pointer to Temporary and final Average Arrays Output
  *  @param  pSrcArr pSrcArr pointer to Input Array
  *  @param  LenArr LenArr Array Size Lenght
  *  @param  pSumCnt Description for pSumCnt
  *  @param  FinishAvg Description for FinishAvg
  *  @retval 1 in case of success
  *  @retval 0 in case of failure
  */
uint8_t MotionSP_fftAverageCalcTime(float *pDstArr, float *pSrcArr, uint16_t LenArr, uint16_t *pSumCnt, uint8_t FinishAvg)
{
  if ((*pSumCnt == 0))
  {
    // reset the destination array
    memset((void *)pDstArr, 0, LenArr * sizeof(float));
  }

  // add new source values to destination
  for (uint16_t j = 0; j < LenArr; j++)
  {
    *(pDstArr + j) += *(pSrcArr + j);
  }

  // increase the number of times
  *pSumCnt += 1;

  if (FinishAvg)
  {
    // Process the average
    for (uint16_t j = 0; j < LenArr; j++)
    {
      *(pDstArr + j) = *(pDstArr + j) / (*pSumCnt);
    }

    return 1;
  }
  
  return 0;
}

/**
  * @brief  Frequency Domain Processing
  * @param  None
  * @return None
  */
void MotionSP_FrequencyDomainProcess(void)
{
  static struct {
    uint8_t xAccAvgRdy : 1;
    uint8_t yAccAvgRdy : 1;
    uint8_t zAccAvgRdy : 1;
  } AvgRdy = {0,0,0};
 
  static sAccAxesArray_t AccAxesArray_fftIn;  //!< Array for acc axes input values for FFT
  static float fftIn[FFT_SIZE_MAX];           //!< Array for input values for FFT
  static float fftOut[FFT_SIZE_MAX];          //!< Array for output values for the complex magnitude function

  /* ------------------ Freeze the Accelerometer data to analyze--------------*/
  MotionSP_fftInBuild(AccAxesArray_fftIn.AXIS_X, MotionSP_Parameters.FftSize, AccCircBuffer.Data.AXIS_X, AccCircBuffer.Size, accCircBuffIndexForFft);
  MotionSP_fftInBuild(AccAxesArray_fftIn.AXIS_Y, MotionSP_Parameters.FftSize, AccCircBuffer.Data.AXIS_Y, AccCircBuffer.Size, accCircBuffIndexForFft);
  MotionSP_fftInBuild(AccAxesArray_fftIn.AXIS_Z, MotionSP_Parameters.FftSize, AccCircBuffer.Data.AXIS_Z, AccCircBuffer.Size, accCircBuffIndexForFft);

  /* ------------------ First Axis: Analysis on X-Acceleration--------------*/
  /* Apply the Windowing before to perform FFT */
  motionSP_fftUseWindow(fftIn, AccAxesArray_fftIn.AXIS_X, MotionSP_Parameters.FftSize, Filter_Params);
  MotionSP_fftCalc(&fftS, fftIn, fftOut);
  if (MotionSP_fftAverageCalcTime(AccAxesAvgMagBuff.AXIS_X, fftOut, magSize, &(AccSumCnt.AXIS_X), FinishAvgFlag))
  {
    // Save the Max FFT Number evaluated
    AccAxesMagResults.X_FFT_AVG = AccSumCnt.AXIS_X;
    // Reset the FFT AVG Number for axis evaluated
    AccSumCnt.AXIS_X = 0;
    // AVG available
    AvgRdy.xAccAvgRdy = 1;
  }

  /* ------------------ Second Axis: Analysis on Y-Acceleration ----------*/
  /* Apply the Windowing before to perform FFT */
  motionSP_fftUseWindow(fftIn, AccAxesArray_fftIn.AXIS_Y, MotionSP_Parameters.FftSize,
                        Filter_Params); /* Evaluate FFT on Acc Data */
  MotionSP_fftCalc(&fftS, fftIn, fftOut);
  if (MotionSP_fftAverageCalcTime(AccAxesAvgMagBuff.AXIS_Y, fftOut, magSize, &(AccSumCnt.AXIS_Y), FinishAvgFlag))
  {
    // Save the Max FFT Number evaluated
    AccAxesMagResults.Y_FFT_AVG = AccSumCnt.AXIS_Y;    // Reset the FFT AVG Number for axis evaluated
    AccSumCnt.AXIS_Y = 0;
    // AVG available
    AvgRdy.yAccAvgRdy = 1;
  }

  /* ------------------ Third Axis: Analysis on Z-Acceleration-------------*/
  /* Apply the Windowing before to perform FFT */
  motionSP_fftUseWindow(fftIn, AccAxesArray_fftIn.AXIS_Z, MotionSP_Parameters.FftSize, Filter_Params);
  MotionSP_fftCalc(&fftS, fftIn, fftOut);
  if (MotionSP_fftAverageCalcTime(AccAxesAvgMagBuff.AXIS_Z, fftOut, magSize, &(AccSumCnt.AXIS_Z), FinishAvgFlag))
  {
    // Save the Max FFT Number evaluated
    AccAxesMagResults.Z_FFT_AVG = AccSumCnt.AXIS_Z;
    // Reset the FFT AVG Number for axis evaluated
    AccSumCnt.AXIS_Z = 0;
    // AVG available
    AvgRdy.zAccAvgRdy = 1;
  }

  /* ---------------------------- Finish ----------------------------------*/
  if (AvgRdy.xAccAvgRdy & AvgRdy.yAccAvgRdy & AvgRdy.zAccAvgRdy) 
  {
    MotionSP_fftAdapt(&AccAxesAvgMagBuff, magSize);
    MotionSP_fftFindPeak(&AccAxesAvgMagBuff, magSize, &AccAxesMagResults);


#ifdef USE_SUBRANGE	
    MotionSP_evalMaxAmplitudeRange (AccAxesAvgMagBuff.AXIS_X, MotionSP_Parameters.subrange_num, SRAmplitude.AXIS_X, SRBinVal.AXIS_X);
    MotionSP_evalMaxAmplitudeRange (AccAxesAvgMagBuff.AXIS_Y, MotionSP_Parameters.subrange_num, SRAmplitude.AXIS_Y, SRBinVal.AXIS_Y);
    MotionSP_evalMaxAmplitudeRange (AccAxesAvgMagBuff.AXIS_Z, MotionSP_Parameters.subrange_num, SRAmplitude.AXIS_Z, SRBinVal.AXIS_Z);
#endif /* USE_SUBRANGE */

    AvgRdy.xAccAvgRdy = 0;
    AvgRdy.yAccAvgRdy = 0;
    AvgRdy.zAccAvgRdy = 0;
  }
}

/**
  * @brief  Frequency Domain Analysis
  * @param  avg Performing average
  * @return None
  */
void MotionSP_fftExecution(uint8_t avg)
{
  static struct
  {
    uint16_t X;
    uint16_t Y;
    uint16_t Z;
  } FftCnt = {0,0,0};       //!< Sum counter for FFT during averaging

  static struct
  {
    uint8_t X: 1;
    uint8_t Y: 1;
    uint8_t Z: 1;
  } IsFftAvgRdy = {0,0,0};  //!< FFT average has been done
 
  static sAccAxesArray_t AccAxesArray_fftIn;  //!< Array for acc axes input values for FFT
  static float fftIn[FFT_SIZE_MAX];           //!< Array for input values for FFT
  static float fftOut[FFT_SIZE_MAX];          //!< Array for output values for the complex magnitude function

  /* ------------------ Freeze the Accelerometer data to analyze--------------*/
  MotionSP_fftInBuild(AccAxesArray_fftIn.AXIS_X, MotionSP_Parameters.FftSize, MotionSP_Data.AccCircBuff.Array.X, MotionSP_Data.AccCircBuff.Size, accCircBuffIndexForFft);
  MotionSP_fftInBuild(AccAxesArray_fftIn.AXIS_Y, MotionSP_Parameters.FftSize, MotionSP_Data.AccCircBuff.Array.Y, MotionSP_Data.AccCircBuff.Size, accCircBuffIndexForFft);
  MotionSP_fftInBuild(AccAxesArray_fftIn.AXIS_Z, MotionSP_Parameters.FftSize, MotionSP_Data.AccCircBuff.Array.Z, MotionSP_Data.AccCircBuff.Size, accCircBuffIndexForFft);
  
  /* ------------------ First Axis: Analysis on X-Acceleration--------------*/
  /* Apply the Windowing before to perform FFT */
  motionSP_fftUseWindow(fftIn, AccAxesArray_fftIn.AXIS_X, MotionSP_Parameters.FftSize, Filter_Params);
  MotionSP_fftCalc(&fftS, fftIn, fftOut);
  if (MotionSP_fftAverageCalcTime(AccMagResults.Array.X, fftOut, AccMagResults.MagSizeTBU, &(FftCnt.X), avg))
  {
    // Save the max evaluated FFT Number
    AccMagResults.FFT_Items.X = FftCnt.X;
    // Set flag about available average
    IsFftAvgRdy.X = 1;
  }

  /* ------------------ Second Axis: Analysis on Y-Acceleration ----------*/
  /* Apply the Windowing before to perform FFT */
  motionSP_fftUseWindow(fftIn, AccAxesArray_fftIn.AXIS_Y, MotionSP_Parameters.FftSize, Filter_Params);
  MotionSP_fftCalc(&fftS, fftIn, fftOut);
  if (MotionSP_fftAverageCalcTime(AccMagResults.Array.Y, fftOut, AccMagResults.MagSizeTBU, &(FftCnt.Y), avg))
  {
    // Save the max evaluated FFT Number
    AccMagResults.FFT_Items.Y = FftCnt.Y;
    // Set flag about available average
    IsFftAvgRdy.Y = 1;
  }

  /* ------------------ Third Axis: Analysis on Z-Acceleration-------------*/
  /* Apply the Windowing before to perform FFT */
  motionSP_fftUseWindow(fftIn, AccAxesArray_fftIn.AXIS_Z, MotionSP_Parameters.FftSize, Filter_Params);
  MotionSP_fftCalc(&fftS, fftIn, fftOut);
  if (MotionSP_fftAverageCalcTime(AccMagResults.Array.Z, fftOut, AccMagResults.MagSizeTBU, &(FftCnt.Z), avg))
  {
    // Save the max evaluated FFT Number
    AccMagResults.FFT_Items.Z = FftCnt.Z;
    // Set flag about available average
    IsFftAvgRdy.Z = 1;
  }

  /* ---------------------------- Finish ----------------------------------*/
  if (IsFftAvgRdy.X & IsFftAvgRdy.Y & IsFftAvgRdy.Z) 
  {
    MotionSP_fftAdapting(&AccMagResults, Window_Scale_Factor);
    MotionSP_fftPeakFinding(&AccMagResults);

#ifdef USE_SUBRANGE	
    MotionSP_evalMaxAmplitudeRange (AccMagResults.Array.X, MotionSP_Parameters.subrange_num, SRAmplitude.AXIS_X, SRBinVal.AXIS_X);
    MotionSP_evalMaxAmplitudeRange (AccMagResults.Array.Y, MotionSP_Parameters.subrange_num, SRAmplitude.AXIS_Y, SRBinVal.AXIS_Y);
    MotionSP_evalMaxAmplitudeRange (AccMagResults.Array.Z, MotionSP_Parameters.subrange_num, SRAmplitude.AXIS_Z, SRBinVal.AXIS_Z);
#endif /* USE_SUBRANGE */

    // Reset FFT sum counter
    FftCnt.X = 0;
    FftCnt.Y = 0;
    FftCnt.Z = 0;

    // Reset FFT average status
    IsFftAvgRdy.X = 0;
    IsFftAvgRdy.Y = 0;
    IsFftAvgRdy.Z = 0;
  }
}

/**
  * @brief FFT subrange analysis to detect the Max Amplitude and related Bin frequency for each subranges
  * @param pfftCmplxMagAxis: FFT Input Arrays to analyze for each subrange
  * @param subrange:  Subranges number
  * @param SR_Amplitude: Max Amplitude for each Subrange analyzed
  * @param SR_Bin_Value: Bin Frequency related to Max Amplitude detected inside subrange analyzed
  * @return None
  */
void MotionSP_evalMaxAmplitudeRange(float *pfftCmplxMagAxis, uint16_t subrange, float *SR_Amplitude, float *SR_Bin_Value)
{
  uint8_t winsamples;
  float MaxValue;
  uint32_t MaxIndex;
  uint16_t FFTindex;
  winsamples = AccMagResults.MagSizeTBU / subrange;
  for (int i = 0; i < subrange; i++)
  {
    FFTindex = winsamples * i;
    arm_max_f32(&pfftCmplxMagAxis[FFTindex], winsamples, &MaxValue, &MaxIndex);
    SR_Amplitude[i] = MaxValue;
    SR_Bin_Value[i] = MaxIndex + FFTindex;
  }
}

/**
  * @brief Get real accelerometer ODR
  * @return sAcceleroODR_t Pointer to the real accelerometer ODR
  */
sAcceleroODR_t *MotionSP_GetRealAcceleroOdr(void)
{
  return &AcceleroODR;
}

/**
  * @brief Get MotionSP Parameters
  * @return sMotionSP_Parameter_t Pointer to the MotionSP Parameters
  */
sMotionSP_Parameter_t *MotionSP_GetParameters(void)
{
  return &MotionSP_Parameters;
}

/**
  * @brief Get accelero magnitude results
  * @return sAccMagResults_t Pointer to the accelero magnitude results
  */
sAccMagResults_t *MotionSP_GetAccMagResults(void)
{
  return &AccMagResults;
}

/**
  * @brief Get time domain data
  * @return sTimeDomainData_t Pointer to the time domain data
  */
sTimeDomainData_t *MotionSP_GetTimeDomainData(void)
{
  return &TimeDomainData;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
