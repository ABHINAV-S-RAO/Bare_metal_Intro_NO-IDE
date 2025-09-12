# Bare-Metal Development Board Projects

## Overview

This repository contains three bare-metal projects implemented on an ARM-based development board. All projects are developed **without using any operating system, middleware, or high-level libraries**. The code is written entirely in **low-level C and assembly**, with **direct register access**. No auto-generated code from IDEs has been used, and all configurations—including linker scripts, startup code, and peripheral register setups—have been implemented manually.

The three tasks included in this repository are:

1. **Blink an On-Board LED**
2. **Button Interrupt**
3. **[BONUS] UART-Based Tracing Function**

---

## Task 1: Blink an On-Board LED

### Description
- Configure a GPIO pin as an output connected to the on-board LED.  
- Implement a simple delay loop to toggle the LED state, producing a visible blink.  

### Bare-Metal Approach
- All GPIO registers are manually configured.  
- A delay function is implemented using a busy-wait loop.  
- Startup code and linker script are manually configured for this project.  

### Build and Flash Instructions
- Follow the **General Instructions** section below to build and load the program onto your board.  
- Once flashed, the LED will toggle continuously at a fixed interval.


https://github.com/user-attachments/assets/870d5006-630c-47bd-9e90-19bf2fa91be0


---

## Task 2: Button Interrupt

### Description
- Configure a GPIO pin as an input connected to a push button.  
- Implement an **Interrupt Service Routine (ISR)** that toggles the LED state when the button is pressed.

### Bare-Metal Approach
- GPIO input registers and interrupt configuration registers are manually set.  
- The ISR is written in C/assembly and registered in the vector table.  
- No external libraries or auto-generated code are used.

### Build and Flash Instructions
- Follow the **General Instructions** section below to build and load the program.  
- Pressing the configured button will toggle the LED state.


https://github.com/user-attachments/assets/1c925cf0-6c0e-406d-825e-3d577008408f


---

## Task 3: UART-Based Tracing Function [BONUS]

### Description
- Implement a custom tracing function similar to `printf()`.  
- Sends debug messages via the board’s UART peripheral to a serial terminal.

### Bare-Metal Approach
- UART registers are manually configured for baud rate, data bits, stop bits, and parity.  
- The tracing function sends each character directly to the UART transmit register.  
- Useful for debugging without relying on high-level drivers.

### Build and Flash Instructions
- Follow the **General Instructions** section below to build and load the program.  
- Connect a serial terminal (e.g., PuTTY, minicom) to observe debug messages.
- OPEN PuTTY and Set the COM port by checking with which port is the target connected to. GO to Windows->Device Manager->Ports 
<img width="359" height="141" alt="image" src="https://github.com/user-attachments/assets/743a00d2-0097-4718-9bd7-bb8ad7ed5ddc" />

- Make sure the connection type is Serial and the Speed is set to 115200 and click on Open.
  
<img width="440" height="434" alt="image" src="https://github.com/user-attachments/assets/8b2b533a-cf84-4088-a1cb-ab3a30ddcc7d" />



https://github.com/user-attachments/assets/a48bf57f-b5bd-47e3-9f4e-5d623ef75317


---

## General Instructions for All Projects

### Prerequisites
- ARM development board (e.g., STM32)  
- `arm-none-eabi-gcc` toolchain  
- `GNU Make`  
- `OpenOCD` or compatible debugger  
- Git Bash

### Steps

1. **Clone the repository**

```bash
git clone https://github.com/ABHINAV-S-RAO/WEC_TASK.git
cd WEC_TASK
```

2. **Connect to the Target and Initialize**

Follow these steps to manually connect to the board and initialize it:

i. Open a new Git Bash terminal.

ii. Navigate to the project directory:
   The make command reads the instructions present in the Makefile written.
```bash
cd WEC_TASK/<task_name>
make load
```
3. **Open another Gitbash Terminal and navigate to the project directory and run the following command**
```bash
cd WEC_TASK/<task_name>
arm-none-eabi-gdb.exe
```
4. **It starts the GNU Debugger** 
In (gdb) type the following commands:
```bash
target remote localhost:3333
monitor reset init
monitor flash write_image erase final.elf
```
5. **You can now reset and halt, and resume when ready!**
```bash
monitor reset halt
monitor resume
```
**SO YOU SHOULD BE ABLE TO SEE THE PROGRAM WORKING ON THE MICROCONTROLLER.**
 To stop the execution:

```bash
quit
y
```

