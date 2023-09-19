# IITH-FWC-2022-23

A collection of all the work done during the internship at Indian Institute of Technology, Hyderabad (IITH) as part of the [Certificate Program in Future Wireless Communications](https://fwc.iith.ac.in) (FWC).

# Overview

The internship consisted of doing projects under the supervision of [Dr. GVV Sharma](https://iith.ac.in/ee/gadepall). The projects that I completed were
- [**Arduino library for ESP32 to communicate with Quicklogic EOS-S3 SoC**](#project-wireless-car)
- [**Generation of FM signal using ESP32**](#project-fm-using-esp32)
- [**Real-time NavIC L5/S band software receiver**](#project-navic-receiver)

The main subjects learnt during the program were

- **Hardware programming** : Writing code for microcontrollers and FPGAs
- **Mathematics** : Matrix analysis and convex optimization
- **Basics of Digital communication**

All the subjects were self-learnt using **open source e-books designed by Dr. GVV Sharma**. A unique feature of these books is that they contain minimal theory. The topics are mainly introduced through solved examples after which the learner has to solve many problems. They were very helpful to gain a good understanding of the subjects. A list of the e-books are given [here](#e-book-list).

# Table of contents

- [Hardware programming](#hardware-programming)
  + [Arduino UNO](#arduino-uno)
  + [Vaman](#vaman)
  + [Project: Wireless car](#project-wireless-car)
- [Mathematics](#mathematics)
  + [Matrix analysis](#matrix-analysis)
  + [Convex optimization](#convex-optimization)
- [Digital communication](#digital-communication)
  + [Textbook problems](#textbook-problems)
  + [Project: FM using ESP32](#project-fm-using-esp32)
- [Project: NavIC receiver](#project-navic-receiver)
  + [Simulation](#simulation)
  + [Real-Time receiver](#real-time-receiver)
- [Miscellaneous](#miscellaneous)
  + [Book list](#book-list)

# Hardware programming

The book for this section can be found [here](https://github.com/gadepall/digital-design/blob/main/main.pdf)

## Arduino UNO

Learnt to write code for the atmega328p present on the Arduino UNO board in different methods. The relevant folders are

    .\Hardware
        .\arduino_ide     Problems using Arduino framework and Platformio  
        .\arduino_asm     Problems using assembly language
        .\avr_gcc         Problems using pure C, and sometimes a combination of C and assembly

## Vaman

The Vaman board consists of a Quicklogic EOS-S3 Soc and a ESP32. The EOS-S3 SoC consists of an ARM-M4 microcontroller and an embedded FPGA. More information on the Vaman board can be found [here](https://www.optimuslogic.in/product_pygmy.html#pygmystamp). A unique feature of this board is that the FPGA is supported by an open-source toolchain called [Symbiflow](https://www.hackster.io/news/gcc-for-fpga-symbiflow-open-source-toolchain-254cf1ab15ff).

The relevant folders are
    
    .\Hardware
        .\arm             Problems using the ARM-M4 microcontroller  
        .\fpga            Problems using the embedded FPGA
        .\esp32           Problems using ESP32

## Project: Wireless car

The goal of the project was to use all three components of the Vaman board (ARM-M4, FPGA and ESP32) to control a toy car wirelessly. The ESP32 was used to connect via Bluetooth to an Android phone which was running the [Dabble](https://thestempedia.com/product/dabble) app's joystick widget. An Arduino library was developed for the ESP32 in order to communicate with the ARM-M4 present on the EOS-S3. This enabled the ESP32 to pass the raw joystick values it receives from the user to the ARM-M4. The ARM-M4 in turn converts the joystick values to PWM and direction values that are passed to the FPGA by writing to a common memory region. The FPGA uses the wishbone interface to read the values and control the speed and direction of the wheels.

The relevant folders are

    .\wireless_car
          .\esp32_pwmctrl             Code for ESP32 containing libraries to communicate with EOS-S3 SoC and Dabble app
          .\fpga_pwmctrl              Code for FPGA implementing GPIO controller, PWM controller and Wishbone interface
          .\m4_pwmctrl                Code for ARM-M4 
          .\build_instruction.txt

# Mathematics

## Matrix analysis

 The main goal was to get familiar with thinking about mathematical problems in terms of matrices. This enables the learner to use the power of computers to solve the problems. The book mainly uses concepts from coordinate geometry to teach the learner about matrix analysis. The exercises are to be solved through Python scripts utilizing Numpy library. The book for this section can be found [here](https://github.com/gadepall/matrix-analysis/blob/main/main.pdf).

The relevant folders are
    
    .\CoordGeo                    A library containing functions to generate coordinates for 2D shapes     
    .\mathematics
        .\matrices
            .\line_assignment     Problem based on straight line
            .\circle_assignment   Problem based on circle
            .\conic_assignment    Problem based on a conic section

## Convex optimization

The book teaches the learners how to classify optimization problems and frame the problems in a format (using matrices) which can then be solved by computers. The exercises are to be solved using python scripts utilizing the cvxpy library. The book for this section can be found [here](https://github.com/gadepall/optimization/blob/master/main.pdf).

The relevant folders are

    .\mathematics
        .\optimization
            .\basic_assignment      Problem based on linear programming
            .\advanced_assignment   Problem based on geometric programming

# Digital communication

## Textbook problems

 The main topics learnt were probability basics, random variables, probability distributions, and digital modulation schemes like BPSK, FSK, and QPSK. A solution document was prepared along with supporting python scripts as solutions for certain problems from the book. The book for this section can be found [here](https://github.com/gadepall/digital-communication/blob/main/main.pdf).

The relevant folders are

    .\communication
        .\main.pdf        Solution document
        .\codes           Python scripts

## Project: FM using ESP32

The main goal of the project was to generate a Frequency Modulation (FM) signal using a ESP32. With $x_{m}(t)$ as the message signal and $f_{c}$ as the carrier frequency, the FM signal $y(t)$ is given by

```math
\begin{aligned}y(t)&=A_{c}\cos \left(2\pi \int _{0}^{t}f(\tau )d\tau \right)\\&=A_{c}\cos \left(2\pi \int _{0}^{t}\left[f_{c}+f_{\Delta }x_{m}(\tau )\right]d\tau \right)\\&=A_{c}\cos \left(2\pi f_{c}t+2\pi f_{\Delta }\int _{0}^{t}x_{m}(\tau )d\tau \right)\\\end{aligned}
```

To compute the above expression quickly in a discrete time system, the following changes were made 

1. The integral can be approximated by summing up rectangles
1. The cosine can be computed using look up tables (LUTs)

The above ideas were validated by writing python scripts. The final discrete time algorithm was then written in C for execution on the ESP32. The DAC on the ESP32 was used to output the analog FM signal. The I2S peripheral was connected to the DAC to attain higher sample rates.

The relevant folders are

    .\fm_trans
      .\python_codes
        .\lut_vs_sin.py       Comparing the FM signal when using LUTs over inbuilt trigonometry functions
        .\rect_vs_trap.py     Comparing the FM signal when summing up rectangles to compute integral
      .\fm_esp32              C code for ESP32

# Project: NavIC receiver

The end goal of the project was to design a real-time software receiver for the Navigation with Indian Constellation (NavIC) L5 and S band that performs satellite search, acquisition, tracking, data decoding and position computation.

## Simulation

Firstly, a complete end-to-end simulation was done in python that mainly consisted of the trasmitter, channel model and receiver. The complete document explaining the simulation along with the python scripts can be found [here](https://github.com/gadepall/navic)

## Real-Time receiver

After verifying the receiver algorithms through the simulation, a real-time receiver software receiver was to be built. The hardware consists of an antenna supporting the L5/S band and a Software Defined Radio (SDR) (in this case, a bladeRF micro 2.0). The code for the NavIC receiver was designed as an addition to an existing GNSS software receiver called [GNSS-SDRLIB](https://github.com/taroz/GNSS-SDRLIB) by Taroz Suzuki. The repo for the NavIC receiver can be found [here](https://github.com/Muhammed-Hamdan/GNSS-SDRLIB)

# Miscellaneous

## Book list

- [Digital Design](https://github.com/gadepall/digital-design/blob/main/main.pdf)
- [Matrix Analysis](https://github.com/gadepall/matrix-analysis/blob/main/main.pdf)
- [Optimization](https://github.com/gadepall/optimization/blob/master/main.pdf)
- [Digital Communication](https://github.com/gadepall/digital-communication/blob/main/main.pdf)