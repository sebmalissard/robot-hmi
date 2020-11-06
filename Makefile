# https://github.com/sudar/Arduino-Makefile
#
# sudo apt install python3-serial
#
# git clone https://github.com/sudar/Arduino-Makefile.git $HOME/Workspace/arduino-makefile/Arduino-Makefile
#
# Add in ~/.bashrc:
#     export ARDMK_DIR=$HOME/Workspace/arduino-makefile/Arduino-Makefile
#     export ALTERNATE_CORE_PATH=$HOME/.arduino15/packages/SparkFun/hardware/avr/1.1.12/

PROJECT_DIR       = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
USER_LIB_PATH    := $(PROJECT_DIR)/lib

BOARD_TAG = promicro
BOARD_SUB = 16MHzatmega32U4

ARDUINO_LIBS = SPI Wire AX12A Max7219

include $(ARDMK_DIR)/Arduino.mk
