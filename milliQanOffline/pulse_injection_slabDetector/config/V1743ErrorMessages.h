#ifndef V1743_ERROR_MESSAGES_H
#define V1743_ERROR_MESSAGES_H

#include <map>
#include <string>

#include "CAENDigitizer.h"

namespace mdaq {
  class V1743ErrorMessages {
   public:
     V1743ErrorMessages() :
      map_({
         {CAEN_DGTZ_Success,                  "Operation completed successfully."},
         {CAEN_DGTZ_CommError,                "Communication error."},
         {CAEN_DGTZ_GenericError,             "Unspecified error."},
         {CAEN_DGTZ_InvalidParam,             "Invalid parameter."},
         {CAEN_DGTZ_InvalidLinkType,          "Invalid link type."},
         {CAEN_DGTZ_InvalidHandle,            "Invalid device handle."},
         {CAEN_DGTZ_MaxDevicesError,          "Maximum number of devices exceeded."},
         {CAEN_DGTZ_BadBoardType,             "The operation is not allowed on this type of board."},
         {CAEN_DGTZ_BadInterruptLev,          "The interrupt level is not allowed."},
         {CAEN_DGTZ_BadEventNumber,           "The event number is bad."},
         {CAEN_DGTZ_ReadDeviceRegisterFail,   "Unable to read the registry."},
         {CAEN_DGTZ_WriteDeviceRegisterFail,  "Unable to write into the registry."},
         {CAEN_DGTZ_InvalidChannelNumber,     "The channel number is invalid."},
         {CAEN_DGTZ_ChannelBusy,              "The channel is busy."},
         {CAEN_DGTZ_FPIOModeInvalid,          "Invalid FPIO mode."},
         {CAEN_DGTZ_WrongAcqMode,             "Wrong acquisition mode."},
         {CAEN_DGTZ_FunctionNotAllowed,       "This function is not allowed for this module."},
         {CAEN_DGTZ_Timeout,                  "Communication timeout."},
         {CAEN_DGTZ_InvalidBuffer,            "The buffer is invalid."},
         {CAEN_DGTZ_EventNotFound,            "The event is not found."},
         {CAEN_DGTZ_InvalidEvent,             "The event is invalid."},
         {CAEN_DGTZ_OutOfMemory,              "Out of memory."},
         {CAEN_DGTZ_CalibrationError,         "Unable to calibrate the board."},
         {CAEN_DGTZ_DigitizerNotFound,        "Unable to open the digitizer."},
         {CAEN_DGTZ_DigitizerAlreadyOpen,     "The digitizer is already open."},
         {CAEN_DGTZ_DigitizerNotReady,        "The digitizer is not ready to operate."},
         {CAEN_DGTZ_InterruptNotConfigured,   "The digitizer is not configured for IRQ."},
         {CAEN_DGTZ_DigitizerMemoryCorrupted, "The digitizer flash memory is corrupted."},
         {CAEN_DGTZ_DPPFirmwareNotSupported,  "The digitizer DPP firmware is not supported in this lib version."},
         {CAEN_DGTZ_InvalidLicense,           "Invalid firmware license."},
         {CAEN_DGTZ_InvalidDigitizerStatus,   "The digitizer is found in a corrupted status."},
         {CAEN_DGTZ_UnsupportedTrace,         "The given trace is not supported by the digitizer."},
         {CAEN_DGTZ_InvalidProbe,             "The given probe is not supported for the given digitizer's trace."},
         {CAEN_DGTZ_NotYetImplemented,        "The function is not yet implemented."}
       }) {};

     ~V1743ErrorMessages() { map_.clear(); };

     std::string message(const CAEN_DGTZ_ErrorCode code) const {
       return map_.find(code)->second;
     };

   private:
     std::map<CAEN_DGTZ_ErrorCode, std::string> map_;
  };

}

#endif
