# https://github.com/sudar/Arduino-Makefile
#
# sudo apt install python3-serial
#
# sudo mkdir /opt/Arduino-Makefile
# sudo chown seb:seb /opt/Arduino-Makefile
# git clone https://github.com/sudar/Arduino-Makefile.git /opt/Arduino-Makefile
#
# Add in ~/.bashrc:
#     export ARDMK_DIR=/opt/Arduino-Makefile
#     export ARDUINO_DIR=/opt/arduino-1.8.7/
#     export ALTERNATE_CORE_PATH=$HOME/.arduino15/packages/SparkFun/hardware/avr/1.1.12/

PROJECT_DIR       = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
USER_LIB_PATH    := $(PROJECT_DIR)/lib

BOARD_TAG = promicro
BOARD_SUB = 16MHzatmega32U4

ARDUINO_LIBS = SPI Wire AX12A Max7219

ifeq ($(AC_CONFIG),)
$(error "You need to define the Arduino Controller config to build with AC_CONFIG")
endif

CFLAGS    += -D$(AC_CONFIG)
CPPFLAGS  += -D$(AC_CONFIG)

include $(ARDMK_DIR)/Arduino.mk
