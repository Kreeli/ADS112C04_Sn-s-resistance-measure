#include "ADS112C.h"
#include "ti_msp_dl_config.h"

/*
 * ADS112C04 驱动 - MSPM0 I2C 控制器 (I2C_ADS112C, 400 kHz)
 *
 * 每个函数的事务流程：
 *   1. 将命令字节填入控制器 TX FIFO。
 *   2. 启动传输，START + STOP 由硬件自动产生。
 *   3. 轮询控制器状态直到传输完成 (IDLE)。
 *
 * WREG 时序 (图 59)：  [WREG 命令][数据]
 * RREG 时序 (图 58)：  [RREG 命令]  -> 重复起始 -> 读取 1 字节
 * 独立命令 (图：仅命令字节)
 */

/* 判定总线卡死前的最大重试次数 */
#define ADS112C_BUS_TIMEOUT_LOOPS   (10000u)

/*
 * 当前转换模式（本地跟踪）：
 *   true  = 连续转换模式
 *   false = 单次转换模式（器件复位默认值）
 *
 * 由 ADS112C_quickInit() / ADS112C_configSampleRate() 设置。
 * 如果直接通过 ADS112C_writeRegister() 修改 CFG1 的 CM 位，
 * 请保持此变量同步。
 */
static bool gConversionContinuous = false;

/* 等待控制器传输完成（空闲） */
static void ADS112C_waitControllerIdle(void)
{
    uint32_t timeout = ADS112C_BUS_TIMEOUT_LOOPS;

    while ((DL_I2C_getControllerStatus(I2C_ADS112C) & DL_I2C_CONTROLLER_STATUS_IDLE) == 0u)
    {
        if (--timeout == 0u)
        {
            /* 总线卡死：复位控制器的传输状态 */
            DL_I2C_resetControllerTransfer(I2C_ADS112C);
            break;
        }
    }
}

/* 向配置寄存器写入一个值（WREG 命令，图 59） */
void ADS112C_writeRegister(uint8_t reg, uint8_t value)
{
    uint8_t txBuffer[2];

    txBuffer[0] = ADS112C_CMD_WREG(reg);  /* 0100 rrxx：写地址 rr 处的寄存器 */
    txBuffer[1] = value;

    DL_I2C_fillControllerTXFIFO(I2C_ADS112C, txBuffer, 2u);
    DL_I2C_startControllerTransfer(I2C_ADS112C, ADS112C_I2C_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_TX, 2u);
    ADS112C_waitControllerIdle();
}

/* 读取配置寄存器（RREG 命令，图 58） */
uint8_t ADS112C_readRegister(uint8_t reg)
{
    uint8_t txBuffer;
    uint8_t value = 0u;

    /* 第一帧：发送 RREG 命令 */
    txBuffer = ADS112C_CMD_RREG(reg);  /* 0010 rrxx：读地址 rr 处的寄存器 */
    DL_I2C_fillControllerTXFIFO(I2C_ADS112C, &txBuffer, 1u);
    DL_I2C_startControllerTransfer(I2C_ADS112C, ADS112C_I2C_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_TX, 1u);
    ADS112C_waitControllerIdle();

    /* 第二帧：读取寄存器内容 */
    DL_I2C_startControllerTransfer(I2C_ADS112C, ADS112C_I2C_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_RX, 1u);
    ADS112C_waitControllerIdle();
    value = DL_I2C_receiveControllerData(I2C_ADS112C);

    return value;
}

/* 发送一个独立命令字节（RESET / START_SYNC / POWERDOWN / RDATA） */
void ADS112C_sendCommand(uint8_t cmd)
{
    uint8_t txBuffer;

    txBuffer = cmd;
    DL_I2C_fillControllerTXFIFO(I2C_ADS112C, &txBuffer, 1u);
    DL_I2C_startControllerTransfer(I2C_ADS112C, ADS112C_I2C_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_TX, 1u);
    ADS112C_waitControllerIdle();
}

/* 读取最新的 16 位转换结果（RDATA 命令，图 57） */
int16_t ADS112C_readData(void)
{
    uint16_t data16 = 0u;

    /* 单次模式下，每次读取前必须先发送 START/SYNC 命令触发一次转换
     * (第 8.4.2.1 节)。连续模式下转换已在后台持续进行。 */
    if (!gConversionContinuous)
    {
        ADS112C_sendCommand(ADS112C_CMD_START_SYNC);
    }

    /* 第一帧：发送 RDATA 命令 */
    ADS112C_sendCommand(ADS112C_CMD_RDATA);
    DL_I2C_startControllerTransfer(I2C_ADS112C, ADS112C_I2C_ADDR,
        DL_I2C_CONTROLLER_DIRECTION_RX, 2u);
    ADS112C_waitControllerIdle();
    *((uint8_t*)&data16 + 1) = DL_I2C_receiveControllerData(I2C_ADS112C);
    ADS112C_waitControllerIdle();
    *((uint8_t*)&data16) = DL_I2C_receiveControllerData(I2C_ADS112C);
    return (int16_t)data16;
}

/* ---------------------------------------------------------------------------
 * 高级配置 API
 * ------------------------------------------------------------------------- */

/* 1: 配置 PGA（增益 + 旁路） */
void ADS112C_configPGA(bool pgaEnable, uint8_t gain)
{
    uint8_t reg = ADS112C_readRegister(ADS112C_REG_CFG0);

    /* 增益为 8..128 时器件总是使能 PGA，与 PGA_BYPASS 位无关 */
    if (gain > ADS112C_CFG0_GAIN_4)
    {
        pgaEnable = true;
    }

    reg &= ~(ADS112C_CFG0_GAIN_MASK | ADS112C_CFG0_PGA_BYPASS_MASK);
    reg |= (gain & ADS112C_CFG0_GAIN_MASK);
    if (!pgaEnable)
    {
        reg |= ADS112C_CFG0_PGA_BYPASS;
    }
    ADS112C_writeRegister(ADS112C_REG_CFG0, reg);
}

/* 2: 配置转换模式、工作模式和数据速率 */
void ADS112C_configSampleRate(bool continuous, uint8_t mode, uint8_t dr)
{
    uint8_t reg = ADS112C_readRegister(ADS112C_REG_CFG1);

    reg &= ~(ADS112C_CFG1_DR_MASK | ADS112C_CFG1_MODE_MASK | ADS112C_CFG1_CM_MASK);
    reg |= (mode & ADS112C_CFG1_MODE_MASK) | (dr & ADS112C_CFG1_DR_MASK);
    if (continuous)
    {
        reg |= ADS112C_CFG1_CM_CONTINUOUS;
    }
    ADS112C_writeRegister(ADS112C_REG_CFG1, reg);

    /* 使本地跟踪的转换模式与 CM 位保持同步 */
    gConversionContinuous = continuous;
}

/* 3: 配置电压基准源 */
void ADS112C_configReference(uint8_t vref)
{
    uint8_t reg = ADS112C_readRegister(ADS112C_REG_CFG1);

    reg &= ~ADS112C_CFG1_VREF_MASK;
    reg |= (vref & ADS112C_CFG1_VREF_MASK);
    ADS112C_writeRegister(ADS112C_REG_CFG1, reg);
}

/* 4: 配置输入多路复用器（测量通道） */
void ADS112C_configChannel(uint8_t mux)
{
    uint8_t reg = ADS112C_readRegister(ADS112C_REG_CFG0);

    reg &= ~ADS112C_CFG0_MUX_MASK;
    reg |= (mux & ADS112C_CFG0_MUX_MASK);
    ADS112C_writeRegister(ADS112C_REG_CFG0, reg);
}

/* 5: 快速初始化（复位 + 连续模式 + 增益1 + turbo 660SPS + AIN0 单端 + AVDD 基准） */
void ADS112C_quickInit(void)
{
    /* 复位器件到默认状态（所有寄存器 = 00h） */
    ADS112C_sendCommand(ADS112C_CMD_RESET);

    /* CFG0：AINP = AIN0, AINN = AVSS（单端），增益 = 1，PGA 旁路 */
    ADS112C_writeRegister(ADS112C_REG_CFG0,
        ADS112C_CFG0_MUX_AIN0_AVSS |
        ADS112C_CFG0_GAIN_1 |
        ADS112C_CFG0_PGA_BYPASS);

    /* CFG1：turbo 模式 (660 SPS)、连续转换、AVDD 基准 */
    ADS112C_writeRegister(ADS112C_REG_CFG1,
        ADS112C_CFG1_DR_330SPS |
        ADS112C_CFG1_MODE_TURBO |
        ADS112C_CFG1_CM_CONTINUOUS |
        ADS112C_CFG1_VREF_AVDD |
        ADS112C_CFG1_TS_DISABLE);

    /* CFG2、CFG3：复位后均为默认值 0（DRDY 标志、IDAC 关闭等） */

    /* 跟踪转换模式，供 ADS112C_readData() 使用 */
    gConversionContinuous = true;

    /* 启动连续转换（设置 CM = continuous 后必须发送一次 START/SYNC） */
    ADS112C_sendCommand(ADS112C_CMD_START_SYNC);
}
