# 3-Phase Digital Power Inverter Controller (RP2040 PIO & Pico SDK)

A bare-metal embedded power electronics project implementing a multi-phase digital control block. This repository utilizes the unique hardware architecture of the RP2040 to generate synchronous, interleaved switching logic required to drive a **3-Phase Inverter Bridge** (e.g., for BLDC motor control, variable frequency drives, or solar power conversion).

## 🛠️ System Architecture & Specs
*   **Microcontroller:** Raspberry Pi Pico (RP2040 ARM Cortex-M0+)
*   **Firmware Layer:** Bare-metal **Pico SDK (C/C++)**
*   **Switching Method:** Multi-state **Programmable I/O (PIO) state machines** (`ph0.pio`, `ph1.pio`, `ph2.pio`)
*   **Phase Distribution:** Independent, deterministic, hardware-timed phase offset tracking (U, V, W / Ph0, Ph1, Ph2)

## ⚡ Key Engineering & Hardware Architecture Implementation

### 1. Hardware-Offloaded Phase Generation (`ph0.pio`, `ph1.pio`, `ph2.pio`)
Instead of relying on the CPU to constantly toggle GPIO pins—which risks phase drift, interrupt latency, and processor starvation—this system offloads the raw timing waveforms directly into **three independent PIO state machines**. 
* Each phase handles its own clock-cycle precise timing loops.
* Guarantees perfectly deterministic phase angle separation (e.g., 120-degree offsets) at microsecond resolutions.

### 2. Dedicated UI & Signal Handling Isolation (`ui.pio`, `sigint.pio`)
* **`sigint.pio`**: Manages real-time signal inputs or fault protection checking at hardware speeds.
* **`ui.pio`**: Offloads interface logic so that adjusting user configurations or display updates never delays or interrupts the critical power switching loop.

## 📂 Codebase Breakdown
* `/src` : Main program loops, interrupt service routines, and parameter math.
* `phX.pio` : The assembly blocks responsible for the independent phase timing patterns.
* `CMakeLists.txt` : Native toolchain settings for the arm-none-eabi compilation pipeline.

## 🚀 Compilation Blueprint
```bash
cmake -S . -B build
cmake --build build
```
