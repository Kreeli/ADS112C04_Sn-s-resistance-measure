#include "ti_msp_dl_config.h"
#include "arm_math.h"
#include "board.h"
#include "stdio.h"
#include "ADS112C.h"
#define sig_len 128

int main(void)
{
    SYSCFG_DL_init();

    /* ---- ADS112C04 quick init: continuous mode, gain=1, 660SPS(turbo),
     * ---- AIN0 single-ended, AVDD reference ---- */
    ADS112C_sendCommand(ADS112C_CMD_RESET);
    delay_cycles(1000);
    ADS112C_writeRegister(ADS112C_REG_CFG0,
        ADS112C_CFG0_MUX_AIN2_AIN3 |
        ADS112C_CFG0_GAIN_8 |
        ADS112C_CFG0_PGA_ENABLED);
    ADS112C_writeRegister(ADS112C_REG_CFG1,
        ADS112C_CFG1_DR_20SPS |
        ADS112C_CFG1_MODE_TURBO |
        ADS112C_CFG1_CM_CONTINUOUS |
        ADS112C_CFG1_VREF_INTERNAL |
        ADS112C_CFG1_TS_DISABLE);
    ADS112C_writeRegister(ADS112C_REG_CFG2,
        ADS112C_CFG2_DRDY_NO_DATA |
        ADS112C_CFG2_DCNT_DISABLE |
        ADS112C_CFG2_CRC_DISABLE |
        ADS112C_CFG2_BCS_OFF |
        ADS112C_CFG2_IDAC_1500UA);
    ADS112C_writeRegister(ADS112C_REG_CFG3,
        ADS112C_CFG3_I1MUX_AIN0 |
        ADS112C_CFG3_I2MUX_AIN0);//两个电流源并联弄一个3mA的电流源
    printf("Hello\n");
    int16_t data16 = 0;
    while (1) {
        for(uint16_t gain = 1;gain<=128;gain*=2){
            switch (gain)
            {
            case 1:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_1);
                break;
            case 2:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_2);
                break;
            case 4:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_4);
                break;
            case 8:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_8);
                break;
            case 16:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_16);
                break;
            case 32:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_32);
                break;
            case 64:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_64);
                break;
            case 128:
                ADS112C_configPGA(true, ADS112C_CFG0_GAIN_128);
                break;
            default:
                break;
            }
            delay_cycles(16000000);
            data16 = ADS112C_readData();
            double mV = (double)data16 * 2.048 / 32768.0 * 1000.0 / gain;
            printf("gain = %d,", gain);
            printf("mV = %.3fmV,", mV);
            printf("R = %.5fOhm\n", mV/3.0);
        }
        
    }
}