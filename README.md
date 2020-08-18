## Introduction
I created a tool to measure and label audio PCM data for signal processing (Fourier transform etc.) & machine learning.  
I started with Arduino AVR devices but realized their limitations of computing power and frequency.  
Human audible frequency is considered to be around `20 Hz` to `20 kHz`, and initial method with an arduino I could only acheive `300 Hz`. This was greatly imporved after several experiments.
## Experiments
### Read One, Send Each Time
 (Without any floating point calculation, Arduino's 2-byte integer arithmetic is very powerful. With a single floating point variable per loop, 11.18 kHz goes down to 1 kHz.)
  BaudRate | Arduino | STM32  
  --- | --- | ---
  9600 | 321.79 Hz | 319.90 Hz  
  115200 | 3.92 kHz | 3.84 kHz  
  230400 | 11.18 kHz | 7.69 kHz
### Read All, Send Once (Baudrate 230400)
  DataPoints (AVR) | Samp.Rate (AVR) | DataPoints (STM32) | Samp.Rate (STM32)  
  --- | --- | --- | ---  
  900 | 7.5 ~ 7.91 kHz | 8750  | 45 ~ 46.8 kHz
---
These results are not accurate but an approximation. The results were consistant during the experiment, but it may vary on devices, and small change in code may alter the result dramatically.

---
### Data
The data is labeled as 
- t : tap on mic
- c : clap
- w : whistle
- s : snap
---

In this project, I learned
- how to use an Arduino as an USB-SerialTTL converter and how MCU programming works.
- how sampling rates work and how to measure it in MCU.
- comparing the cost-benefit of using MCU's memory or communication capability. The two different methods of record the audio PCM has their pros and cons.
- utilizing MCU's memory capacity to its max to increase sampling rate.
