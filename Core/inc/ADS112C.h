#pragma once
#include "ti_msp_dl_config.h"

/*
 * ADS112C04 驱动使用的 I2C 外设实例。
 * 如果在 SysConfig 中修改了 I2C 实例名（例如改为 I2C_1_INST），
 * 只需修改这一行宏即可。
 */
#define I2C_ADS112C                     I2C_0_INST

/*
 * ============================================================================
 * ADS112C04 - 16 位、4 通道、2kSPS、带 I2C 接口的 Δ-Σ 模数转换器
 * 数据手册: TI SBAS894A (ZHCSI06A)，寄存器映射见第 8.6 节
 *
 * 寄存器映射:
 *   00h  CFG0 : MUX[3:0]  GAIN[2:0]  PGA_BYPASS
 *   01h  CFG1 : DR[2:0]   MODE  CM  VREF[1:0]  TS
 *   02h  CFG2 : DRDY  DCNT  CRC[1:0]  BCS  IDAC[2:0]
 *   03h  CFG3 : I1MUX[2:0]  I2MUX[2:0]  0  0
 *
 * 所有寄存器复位值为 0x00。
 * ============================================================================
 */
#define ADS112C_I2C_ADDR  (0x40u)  /* A1/A0 接 DGND，默认地址 */

/* ---------------------------------------------------------------------------
 * 寄存器地址
 * ------------------------------------------------------------------------- */
#define ADS112C_REG_CFG0                (0x00u)  /* 配置寄存器 0 */
#define ADS112C_REG_CFG1                (0x01u)  /* 配置寄存器 1 */
#define ADS112C_REG_CFG2                (0x02u)  /* 配置寄存器 2 */
#define ADS112C_REG_CFG3                (0x03u)  /* 配置寄存器 3 */

/* ---------------------------------------------------------------------------
 * 命令 (表 16) - rr = 寄存器地址，x = 任意值
 * ------------------------------------------------------------------------- */
#define ADS112C_CMD_RESET               (0x06u)  /* 0000 011x 复位器件 */
#define ADS112C_CMD_START_SYNC          (0x08u)  /* 0000 100x 启动/重新启动转换 */
#define ADS112C_CMD_POWERDOWN           (0x02u)  /* 0000 001x 进入掉电模式 */
#define ADS112C_CMD_RDATA               (0x10u)  /* 0001 xxxx 按命令读取数据 */
#define ADS112C_CMD_RREG(reg)           (0x20u | ((reg) << 2))  /* 0010 rrxx 读取地址 rr 处的寄存器 */
#define ADS112C_CMD_WREG(reg)           (0x40u | ((reg) << 2))  /* 0100 rrxx 写入地址 rr 处的寄存器 */

/* ---------------------------------------------------------------------------
 * 配置寄存器 0 (00h) - 复位值 00h
 * ------------------------------------------------------------------------- */

/* 位掩码 */
#define ADS112C_CFG0_MUX_MASK           (0xF0u)  /* 位 7:4 输入多路复用器 */
#define ADS112C_CFG0_GAIN_MASK          (0x0Eu)  /* 位 3:1 增益 */
#define ADS112C_CFG0_PGA_BYPASS_MASK    (0x01u)  /* 位 0  PGA 旁路 */

/* MUX[3:0] - 输入多路复用器配置 (表 19) */
#define ADS112C_CFG0_MUX_AIN0_AIN1      (0x00u)  /* AINP = AIN0, AINN = AIN1 (默认) */
#define ADS112C_CFG0_MUX_AIN0_AIN2      (0x10u)  /* AINP = AIN0, AINN = AIN2 */
#define ADS112C_CFG0_MUX_AIN0_AIN3      (0x20u)  /* AINP = AIN0, AINN = AIN3 */
#define ADS112C_CFG0_MUX_AIN1_AIN0      (0x30u)  /* AINP = AIN1, AINN = AIN0 */
#define ADS112C_CFG0_MUX_AIN1_AIN2      (0x40u)  /* AINP = AIN1, AINN = AIN2 */
#define ADS112C_CFG0_MUX_AIN1_AIN3      (0x50u)  /* AINP = AIN1, AINN = AIN3 */
#define ADS112C_CFG0_MUX_AIN2_AIN3      (0x60u)  /* AINP = AIN2, AINN = AIN3 */
#define ADS112C_CFG0_MUX_AIN3_AIN2      (0x70u)  /* AINP = AIN3, AINN = AIN2 */
#define ADS112C_CFG0_MUX_AIN0_AVSS      (0x80u)  /* AINP = AIN0, AINN = AVSS (单端) */
#define ADS112C_CFG0_MUX_AIN1_AVSS      (0x90u)  /* AINP = AIN1, AINN = AVSS (单端) */
#define ADS112C_CFG0_MUX_AIN2_AVSS      (0xA0u)  /* AINP = AIN2, AINN = AVSS (单端) */
#define ADS112C_CFG0_MUX_AIN3_AVSS      (0xB0u)  /* AINP = AIN3, AINN = AVSS (单端) */
#define ADS112C_CFG0_MUX_REF_MON        (0xC0u)  /* (V(REFP) - V(REFN)) / 4 监测 (PGA 旁路) */
#define ADS112C_CFG0_MUX_AVDD_MON       (0xD0u)  /* (AVDD - AVSS) / 4 监测 (PGA 旁路) */
#define ADS112C_CFG0_MUX_MID_SUPPLY     (0xE0u)  /* AINP 与 AINN 短接至 (AVDD + AVSS) / 2 */
                                                 /* 0xF0 = 保留 */

/* GAIN[2:0] - 增益配置 (表 19) */
#define ADS112C_CFG0_GAIN_1             (0x00u)  /* 增益 = 1 (默认) */
#define ADS112C_CFG0_GAIN_2             (0x02u)  /* 增益 = 2 */
#define ADS112C_CFG0_GAIN_4             (0x04u)  /* 增益 = 4 */
#define ADS112C_CFG0_GAIN_8             (0x06u)  /* 增益 = 8 */
#define ADS112C_CFG0_GAIN_16            (0x08u)  /* 增益 = 16 */
#define ADS112C_CFG0_GAIN_32            (0x0Au)  /* 增益 = 32 */
#define ADS112C_CFG0_GAIN_64            (0x0Cu)  /* 增益 = 64 */
#define ADS112C_CFG0_GAIN_128           (0x0Eu)  /* 增益 = 128 */

/* PGA_BYPASS - 位 0 */
#define ADS112C_CFG0_PGA_ENABLED        (0x00u)  /* PGA 使能 (默认) */
#define ADS112C_CFG0_PGA_BYPASS         (0x01u)  /* PGA 禁用并旁路 */

/* ---------------------------------------------------------------------------
 * 配置寄存器 1 (01h) - 复位值 00h
 * ------------------------------------------------------------------------- */

/* 位掩码 */
#define ADS112C_CFG1_DR_MASK            (0xE0u)  /* 位 7:5 数据速率 */
#define ADS112C_CFG1_MODE_MASK          (0x10u)  /* 位 4  工作模式 */
#define ADS112C_CFG1_CM_MASK            (0x08u)  /* 位 3  转换模式 */
#define ADS112C_CFG1_VREF_MASK          (0x06u)  /* 位 2:1 电压基准 */
#define ADS112C_CFG1_TS_MASK            (0x01u)  /* 位 0  温度传感器 */

/* DR[2:0] - 数据速率 (表 21) */
#define ADS112C_CFG1_DR_20SPS           (0x00u)  /* 20 SPS  (normal) / 40 SPS  (turbo) (默认) */
#define ADS112C_CFG1_DR_45SPS           (0x20u)  /* 45 SPS  (normal) / 90 SPS  (turbo) */
#define ADS112C_CFG1_DR_90SPS           (0x40u)  /* 90 SPS  (normal) / 180 SPS (turbo) */
#define ADS112C_CFG1_DR_175SPS          (0x60u)  /* 175 SPS (normal) / 350 SPS (turbo) */
#define ADS112C_CFG1_DR_330SPS          (0x80u)  /* 330 SPS (normal) / 660 SPS (turbo) */
#define ADS112C_CFG1_DR_600SPS          (0xA0u)  /* 600 SPS (normal) / 1200 SPS (turbo) */
#define ADS112C_CFG1_DR_1000SPS         (0xC0u)  /* 1000 SPS (normal) / 2000 SPS (turbo) */
                                                 /* 0xE0 = 保留 */

/* MODE - 位 4 */
#define ADS112C_CFG1_MODE_NORMAL        (0x00u)  /* 正常模式，256-kHz 调制器时钟 (默认) */
#define ADS112C_CFG1_MODE_TURBO         (0x10u)  /* Turbo 模式，512-kHz 调制器时钟 */

/* CM - 位 3 */
#define ADS112C_CFG1_CM_SINGLE_SHOT     (0x00u)  /* 单次转换模式 (默认) */
#define ADS112C_CFG1_CM_CONTINUOUS      (0x08u)  /* 连续转换模式 */

/* VREF[1:0] - 位 2:1，电压基准选择 (表 20) */
#define ADS112C_CFG1_VREF_INTERNAL      (0x00u)  /* 内部 2.048-V 基准 (默认) */
#define ADS112C_CFG1_VREF_EXTERNAL      (0x02u)  /* 外部基准 (REFP 至 REFN) */
#define ADS112C_CFG1_VREF_AVDD          (0x04u)  /* 模拟电源 (AVDD - AVSS) 作基准 */
#define ADS112C_CFG1_VREF_AVDD2         (0x06u)  /* 模拟电源 (AVDD - AVSS) 作基准 */

/* TS - 位 0，温度传感器模式 */
#define ADS112C_CFG1_TS_DISABLE         (0x00u)  /* 温度传感器模式禁用 (默认) */
#define ADS112C_CFG1_TS_ENABLE          (0x01u)  /* 温度传感器模式使能 */

/* ---------------------------------------------------------------------------
 * 配置寄存器 2 (02h) - 复位值 00h
 * ------------------------------------------------------------------------- */

/* 位掩码 */
#define ADS112C_CFG2_DRDY_MASK          (0x80u)  /* 位 7  转换结果就绪标志 (只读) */
#define ADS112C_CFG2_DCNT_MASK          (0x40u)  /* 位 6  数据计数器使能 */
#define ADS112C_CFG2_CRC_MASK           (0x30u)  /* 位 5:4 数据完整性校验 */
#define ADS112C_CFG2_BCS_MASK           (0x08u)  /* 位 3  烧毁检测电流源 */
#define ADS112C_CFG2_IDAC_MASK          (0x07u)  /* 位 2:0 IDAC 电流设置 */

/* DRDY - 位 7 (只读) */
#define ADS112C_CFG2_DRDY_NO_DATA       (0x00u)  /* 无新的转换结果 */
#define ADS112C_CFG2_DRDY_NEW_DATA      (0x80u)  /* 新的转换结果就绪 */

/* DCNT - 位 6，转换数据计数器使能 */
#define ADS112C_CFG2_DCNT_DISABLE       (0x00u)  /* 转换计数器禁用 (默认) */
#define ADS112C_CFG2_DCNT_ENABLE        (0x40u)  /* 转换计数器使能 */

/* CRC[1:0] - 位 5:4，数据完整性校验 */
#define ADS112C_CFG2_CRC_DISABLE        (0x00u)  /* 禁用 (默认) */
#define ADS112C_CFG2_CRC_INVERT         (0x10u)  /* 数据反相输出使能 */
#define ADS112C_CFG2_CRC_CRC16          (0x20u)  /* CRC16 使能 */
                                                 /* 0x30 = 保留 */

/* BCS - 位 3，烧毁检测电流源 (10 uA) */
#define ADS112C_CFG2_BCS_OFF            (0x00u)  /* 电流源关闭 (默认) */
#define ADS112C_CFG2_BCS_ON             (0x08u)  /* 电流源打开 */

/* IDAC[2:0] - 位 2:0，IDAC1/IDAC2 激励电流设置 (表 22) */
#define ADS112C_CFG2_IDAC_OFF           (0x00u)  /* 关闭 (默认) */
#define ADS112C_CFG2_IDAC_10UA          (0x01u)  /* 10 uA */
#define ADS112C_CFG2_IDAC_50UA          (0x02u)  /* 50 uA */
#define ADS112C_CFG2_IDAC_100UA         (0x03u)  /* 100 uA */
#define ADS112C_CFG2_IDAC_250UA         (0x04u)  /* 250 uA */
#define ADS112C_CFG2_IDAC_500UA         (0x05u)  /* 500 uA */
#define ADS112C_CFG2_IDAC_1000UA        (0x06u)  /* 1000 uA */
#define ADS112C_CFG2_IDAC_1500UA        (0x07u)  /* 1500 uA */

/* ---------------------------------------------------------------------------
 * 配置寄存器 3 (03h) - 复位值 00h
 * ------------------------------------------------------------------------- */

/* 位掩码 */
#define ADS112C_CFG3_I1MUX_MASK         (0xE0u)  /* 位 7:5 IDAC1 路由 */
#define ADS112C_CFG3_I2MUX_MASK         (0x1Cu)  /* 位 4:2 IDAC2 路由 */
                                                 /* 位 1:0 保留，始终写 0 */

/* I1MUX[2:0] - 位 7:5，IDAC1 路由配置 (表 23) */
#define ADS112C_CFG3_I1MUX_DISABLE      (0x00u)  /* IDAC1 禁用 (默认) */
#define ADS112C_CFG3_I1MUX_AIN0         (0x20u)  /* IDAC1 连接到 AIN0 */
#define ADS112C_CFG3_I1MUX_AIN1         (0x40u)  /* IDAC1 连接到 AIN1 */
#define ADS112C_CFG3_I1MUX_AIN2         (0x60u)  /* IDAC1 连接到 AIN2 */
#define ADS112C_CFG3_I1MUX_AIN3         (0x80u)  /* IDAC1 连接到 AIN3 */
#define ADS112C_CFG3_I1MUX_REFP         (0xA0u)  /* IDAC1 连接到 REFP */
#define ADS112C_CFG3_I1MUX_REFN         (0xC0u)  /* IDAC1 连接到 REFN */
                                                 /* 0xE0 = 保留 */

/* I2MUX[2:0] - 位 4:2，IDAC2 路由配置 (表 23) */
#define ADS112C_CFG3_I2MUX_DISABLE      (0x00u)  /* IDAC2 禁用 (默认) */
#define ADS112C_CFG3_I2MUX_AIN0         (0x04u)  /* IDAC2 连接到 AIN0 */
#define ADS112C_CFG3_I2MUX_AIN1         (0x08u)  /* IDAC2 连接到 AIN1 */
#define ADS112C_CFG3_I2MUX_AIN2         (0x0Cu)  /* IDAC2 连接到 AIN2 */
#define ADS112C_CFG3_I2MUX_AIN3         (0x10u)  /* IDAC2 连接到 AIN3 */
#define ADS112C_CFG3_I2MUX_REFP         (0x14u)  /* IDAC2 连接到 REFP */
#define ADS112C_CFG3_I2MUX_REFN         (0x18u)  /* IDAC2 连接到 REFN */
                                                 /* 0x1C = 保留 */

/* ---------------------------------------------------------------------------
 * I2C 从机地址 (表 14) - A1/A0 接 DGND、DVDD、SDA 或 SCL
 * ------------------------------------------------------------------------- */
#define ADS112C_ADDR_DGND_DGND          (0x40u)  /* 100 0000 */
#define ADS112C_ADDR_DGND_DVDD          (0x41u)  /* 100 0001 */
#define ADS112C_ADDR_DGND_SDA           (0x42u)  /* 100 0010 */
#define ADS112C_ADDR_DGND_SCL           (0x43u)  /* 100 0011 */
#define ADS112C_ADDR_DVDD_DGND          (0x44u)  /* 100 0100 */
#define ADS112C_ADDR_DVDD_DVDD          (0x45u)  /* 100 0101 */
#define ADS112C_ADDR_DVDD_SDA           (0x46u)  /* 100 0110 */
#define ADS112C_ADDR_DVDD_SCL           (0x47u)  /* 100 0111 */
#define ADS112C_ADDR_SDA_DGND           (0x48u)  /* 100 1000 */
#define ADS112C_ADDR_SDA_DVDD           (0x49u)  /* 100 1001 */
#define ADS112C_ADDR_SDA_SDA            (0x4Au)  /* 100 1010 */
#define ADS112C_ADDR_SDA_SCL            (0x4Bu)  /* 100 1011 */
#define ADS112C_ADDR_SCL_DGND           (0x4Cu)  /* 100 1100 */
#define ADS112C_ADDR_SCL_DVDD           (0x4Du)  /* 100 1101 */
#define ADS112C_ADDR_SCL_SDA            (0x4Eu)  /* 100 1110 */
#define ADS112C_ADDR_SCL_SCL            (0x4Fu)  /* 100 1111 */

/* ---------------------------------------------------------------------------
 * 转换数据
 * ------------------------------------------------------------------------- */
#define ADS112C_DATA_FORMAT_TWOS        /* 16 位二进制补码输出 */

/* ---------------------------------------------------------------------------
 * 驱动 API
 * ------------------------------------------------------------------------- */

/**
 * @brief 向配置寄存器写入一个值 (WREG 命令，表 16)。
 * @param reg   寄存器地址：
 *                ADS112C_REG_CFG0 / CFG1 / CFG2 / CFG3
 * @param value 要写入的 8 位值，由 CFG0/1/2/3 的宏按位或组合而成。
 */
void ADS112C_writeRegister(uint8_t reg, uint8_t value);

/**
 * @brief 读取配置寄存器 (RREG 命令，表 16)。
 * @param reg 寄存器地址：
 *              ADS112C_REG_CFG0 / CFG1 / CFG2 / CFG3
 * @return 8 位寄存器内容。
 */
uint8_t ADS112C_readRegister(uint8_t reg);

/**
 * @brief 发送一个独立命令字节 (表 16)。
 * @param cmd 命令，取以下值之一：
 *              ADS112C_CMD_RESET        (复位器件)
 *              ADS112C_CMD_START_SYNC   (启动/重新启动转换)
 *              ADS112C_CMD_POWERDOWN    (进入掉电模式)
 *              ADS112C_CMD_RDATA        (按命令读取数据)
 */
void ADS112C_sendCommand(uint8_t cmd);

/**
 * @brief 读取最新的 16 位转换结果 (RDATA，图 57)。
 * @return 有符号 16 位二进制补码 ADC 码值。
 * @note  单次模式下读取前会自动发送 START/SYNC 触发一次转换；
 *        连续模式下直接读取后台持续转换的最新结果。
 */
int16_t ADS112C_readData(void);

/* ---------------------------------------------------------------------------
 * 高级配置 API
 * ------------------------------------------------------------------------- */

/**
 * @brief 1: 配置 PGA (增益 + 旁路)。
 * @param pgaEnable  PGA 使能/禁用：
 *                     true  = PGA 使能
 *                     false = PGA 禁用并旁路
 *                     (禁用时增益仅限 1/2/4)
 * @param gain       增益设置，取以下值之一：
 *                     ADS112C_CFG0_GAIN_1   (增益 = 1)
 *                     ADS112C_CFG0_GAIN_2   (增益 = 2)
 *                     ADS112C_CFG0_GAIN_4   (增益 = 4)
 *                     ADS112C_CFG0_GAIN_8   (增益 = 8)
 *                     ADS112C_CFG0_GAIN_16  (增益 = 16)
 *                     ADS112C_CFG0_GAIN_32  (增益 = 32)
 *                     ADS112C_CFG0_GAIN_64  (增益 = 64)
 *                     ADS112C_CFG0_GAIN_128 (增益 = 128)
 * @note  增益为 8..128 时器件总是使能 PGA，与 pgaEnable 无关。
 *        当 AINN = AVSS（单端）通道时 PGA 总是被旁路，只能使用增益 1/2/4。
 */
void ADS112C_configPGA(bool pgaEnable, uint8_t gain);

/**
 * @brief 2: 配置转换模式、工作模式和数据速率。
 * @param continuous  转换模式：
 *                      true  = 连续转换（后台持续转换）
 *                      false = 单次转换（每次 START/SYNC 触发一次）
 * @param mode        工作模式，取以下值之一：
 *                      ADS112C_CFG1_MODE_NORMAL (256-kHz 调制器时钟)
 *                      ADS112C_CFG1_MODE_TURBO  (512-kHz 调制器时钟)
 * @param dr          数据速率编码，取 ADS112C_CFG1_DR_xxx 之一。实际 SPS
 *                    取决于 `mode` (表 21)：
 *                      normal: 20 / 45 / 90 / 175 / 330 / 600 / 1000 SPS
 *                      turbo : 40 / 90 / 180 / 350 / 660 / 1200 / 2000 SPS
 *                    示例：660 SPS = MODE_TURBO + ADS112C_CFG1_DR_330SPS
 * @note  在单次/连续之间切换时，驱动内部会跟踪当前模式，使
 *        ADS112C_readData() 仅在单次模式下触发转换。此函数不会发送
 *        START/SYNC 命令，如需启动请随后调用
 *        ADS112C_sendCommand(ADS112C_CMD_START_SYNC)。
 */
void ADS112C_configSampleRate(bool continuous, uint8_t mode, uint8_t dr);

/**
 * @brief 3: 配置电压基准源。
 * @param vref 基准选择，取以下值之一：
 *               ADS112C_CFG1_VREF_INTERNAL (内部 2.048-V 基准)
 *               ADS112C_CFG1_VREF_EXTERNAL (外部基准，REFP/REFN 引脚)
 *               ADS112C_CFG1_VREF_AVDD    (以 AVDD - AVSS 为基准)
 */
void ADS112C_configReference(uint8_t vref);

/**
 * @brief 4: 配置输入多路复用器（测量通道）。
 * @param mux 通道选择，取 ADS112C_CFG0_MUX_xxx 之一：
 *              ADS112C_CFG0_MUX_AIN0_AIN1  (差分 AIN0/AIN1)
 *              ADS112C_CFG0_MUX_AIN0_AIN2  (差分 AIN0/AIN2)
 *              ADS112C_CFG0_MUX_AIN0_AIN3  (差分 AIN0/AIN3)
 *              ADS112C_CFG0_MUX_AIN1_AIN0  (差分 AIN1/AIN0，反接)
 *              ADS112C_CFG0_MUX_AIN1_AIN2  (差分 AIN1/AIN2)
 *              ADS112C_CFG0_MUX_AIN1_AIN3  (差分 AIN1/AIN3)
 *              ADS112C_CFG0_MUX_AIN2_AIN3  (差分 AIN2/AIN3)
 *              ADS112C_CFG0_MUX_AIN3_AIN2  (差分 AIN3/AIN2，反接)
 *              ADS112C_CFG0_MUX_AIN0_AVSS  (单端 AIN0)
 *              ADS112C_CFG0_MUX_AIN1_AVSS  (单端 AIN1)
 *              ADS112C_CFG0_MUX_AIN2_AVSS  (单端 AIN2)
 *              ADS112C_CFG0_MUX_AIN3_AVSS  (单端 AIN3)
 * @note  单端通道 (AINN = AVSS) 会强制 PGA 旁路，此时只能使用增益 1/2/4。
 */
void ADS112C_configChannel(uint8_t mux);

/**
 * @brief 5: 快速初始化。
 * 复位器件并配置：
 *   - 连续转换模式
 *   - 增益 = 1
 *   - turbo 模式，660 SPS (DR 编码 330)
 *   - 通道 AIN0 单端 (AINN = AVSS)
 *   - AVDD 基准
 * 并启动连续转换。
 */
void ADS112C_quickInit(void);
