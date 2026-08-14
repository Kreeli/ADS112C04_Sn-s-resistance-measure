# ADS112C04 Sn's resistance measurement

基于 TI MSPM0G3507 与 ADS112C04（16-bit 4通道 Δ-Σ ADC, I2C 接口）的锡丝电阻测量工程。

- 通过两个 IDAC 电流源并联输出 3mA 激励，四线法测量小电阻
- 集成 PGA（增益 1~128）、内部/外部/AVDD 电压基准、温度传感器
- 完整实现 I2C 读写驱动与高级配置 API（见 [Core/inc/ADS112C.h](Core/inc/ADS112C.h)）
- 详细使用记录见 [Record.md](Record.md)（博客文章）

## 工程信息

- **芯片**: TI ADS112C04 (16-bit, 4通道, 2kSPS, Δ-Σ ADC, I2C 接口)
- **主控**: TI MSPM0G3507 (Cortex-M0+)
- **工具链**: Keil MDK (uVision) + ARM Compiler V6 (armclang)
- **配置工具**: TI SysConfig（工程根目录的 `01_template_project.syscfg`，可重新生成 `ti_msp_dl_config.c/h`）
- **数据手册**: 仓库内附 TI-ADS112C04.pdf（中文版 ZHCSI06A）

## 目录结构

```
├── Core/     # 用户代码（main.c、ADS112C.c/h 驱动、board.c/h）
├── Driver/   # TI MSPM0 DriverLib 库（不要手动改）
├── keil/     # Keil 工程文件（.uvprojx），编译产物在 Objects/ 和 Listings/
├── pic/      # 文档配图
└── *.syscfg  # SysConfig 工程配置
```

## 编译说明

1. 用 TI SysConfig 打开 `01_template_project.syscfg` 生成外设初始化代码（I2C_0 400kHz、UART_PC）
2. 用 Keil uVision 打开 `keil/01_template_project.uvprojx`，选择 ARM Compiler V6
3. 编译下载即可，默认配置为 ADS112C04 连续采样（AIN0 单端、增益1、660SPS）

## 许可证

MIT License，详见 [LICENSE](LICENSE)。
