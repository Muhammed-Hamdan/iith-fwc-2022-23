#include <driver/dac.h>
#include <soc/rtc.h>
#include <soc/sens_reg.h>

extern portMUX_TYPE rtc_spinlock;
int cleanRegDAC1;
int cleanRegDAC2;

void inline DACPrepare(const bool useMutex)
{
  if(useMutex)
    portENTER_CRITICAL(&rtc_spinlock);
  CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL1_REG, SENS_SW_TONE_EN);
  CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN1_M);
  CLEAR_PERI_REG_MASK(SENS_SAR_DAC_CTRL2_REG, SENS_DAC_CW_EN2_M);
  SET_PERI_REG_BITS(RTC_IO_PAD_DAC1_REG, RTC_IO_PDAC1_DAC, 255, RTC_IO_PDAC1_DAC_S);
  cleanRegDAC1 = (READ_PERI_REG(RTC_IO_PAD_DAC1_REG)&(~((RTC_IO_PDAC1_DAC)<<(RTC_IO_PDAC1_DAC_S))));
  cleanRegDAC2 = (READ_PERI_REG(RTC_IO_PAD_DAC2_REG)&(~((RTC_IO_PDAC2_DAC)<<(RTC_IO_PDAC2_DAC_S))));
}

void inline DAC1Write(const int value)
{
  WRITE_PERI_REG(RTC_IO_PAD_DAC1_REG, cleanRegDAC1 | ((value & RTC_IO_PDAC1_DAC) << RTC_IO_PDAC1_DAC_S));
}

void inline DAC2Write(const int value)
{
  WRITE_PERI_REG(RTC_IO_PAD_DAC2_REG, cleanRegDAC2 | ((value & RTC_IO_PDAC2_DAC) << RTC_IO_PDAC2_DAC_S));  
}

void inline DACUnprepare(const bool useMutex)
{
  if(useMutex)
    portEXIT_CRITICAL(&rtc_spinlock);
}

