# Optional: print out extra messages to see what is going on. Comment it to have less verbose messages
set(CMAKE_VERBOSE_MAKEFILE ON)
set (BOARD_TYPE msp432p401r)
# Path to SDK Folder
set(SDK_Folder /home/elia/TexasInstruments/SimpleLinkSDK2/simplelink_msp432p4_sdk_3_40_01_02)
set(SDK_Src_Folder ${SDK_Folder}/source)
set(SDK_Kernel_Folder ${SDK_Folder}/kernel)

# Use this snippet *after* PROJECT(xxx): Force install path
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

#Set linker file
set(LINKER_FILE ${SDK_Src_Folder}/ti/devices/msp432p4xx/linker_files/gcc/${BOARD_TYPE}.lds)
