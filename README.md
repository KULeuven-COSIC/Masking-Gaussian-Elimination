# Masking Gaussian Elimination at Arbitrary Order
This repository contains ARM Cortex-M4 (C) code for the arbitrary-order masked implementation of Gaussian Elimination with Back Substitution.\
NOTE: This implementation does not include any mitigation of micro-architectural leakages and has not undergone practical leakage evaluation. Our implementation is a proof-of-concept for the (masking) techniques described in our paper 'Masking Gaussian Elimination at Arbitrary Order' [[ePrint]](http://eprint.iacr.org/2024/1777), to appear at CT-RSA 2025. Its primary goal is to evaluate the performance overhead for different parameters sets. 

## Requirements
The code in this repository targets the [STM32F4 Discovery board](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), containing an Arm Cortex-M4 microcontroller.

Clone this repository and move to its root directory:
```shell
git clone https://github.com/KULeuven-COSIC/Masking-Gaussian-Elimination 
```
```shell
cd Masking-Gaussian-Elimination/
```

### [libopencm3](https://github.com/libopencm3/libopencm3)
After cloning this repository, initialize libopencm3:
```shell
git submodule update --init --recursive
```
### [Arm toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
Install the `arm-none-eabi-gcc` (or `gcc-arm-none-eabi`) toolchain/package.

### [stlink](https://github.com/texane/stlink)
Depending on your operating system, stlink may be available in your package manager -- if not, please refer to the stlink Github page for instructions on how to compile it from source (in that case, be careful to use libusb-1.0.0-dev, not libusb-0.1).

### [pyserial](https://github.com/pyserial/pyserial)
(Requires Python >= 3.8). Your package repository might offer `python3-serial` (Debian, Ubuntu) or `python-pyserial` (Arch) or `python3-pyserial` (Fedora, openSUSE) or `pyserial` (Slack, CentOS, Gentoo) or `py3-pyserial` (Alpine) directly. Alternatively, this can be easily installed from PyPA by calling `pip3 install -r requirements.txt`. If you do not have pip3 installed yet, you can typically find it as `python3-pip` (Debian, Ubuntu) or `python-pip` (Arch) using your package manager.

Make sure the board is connected to the host machine using the mini-USB port (USB to TLL adapter). Connect the `TX`/`TXD` pin of the USB connector to the `PA3` pin on the board and connect `RX`/`RXD` pin to `PA2`. Also connect the `GND` pins.

## Running, Testing and Benchmarking
Select a pre-defined parameter set (`UOV-I`, `UOV-III`, `UOV-V`) or define a custom one in [params.h](src/params.h).

Choose `RUN_BENCHMARK` for detailed benchmark numbers and/or `RUN_VERIFY` for verifying correctness in [settings.h](src/settings.h).

Compile by executing:
```shell
make all
```

For reading the output over USART (`/dev/ttyUSB0`), open the listener:
```shell
python3 host.py
```

Flash `mGE_m4.bin` to the target board by executing:
```shell
./deploy.sh 
```

When done, to remove generated binaries:
```shell
make clean
```

## Bibliography
If you use or build upon the code in this repository, please cite our paper using our [citation key](CITATION).