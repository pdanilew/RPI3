#include <libcec/cec.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>

#include "bcm_host.h"

#include <iostream>
#include <algorithm>  // for std::min
#include <signal.h>

using std::cout;
using std::endl;

#include <libcec/cecloader.h>

struct libevdev *dev;
struct libevdev_uinput *uidev;

void logMessage(void * , const CEC::cec_log_message* message)
{
  cout << message->time << ": " << message->message << std::endl;
}

void onKeypress(void* , const CEC::cec_keypress* key)
{
  unsigned int code;
  int          value = 0;

  switch( key->keycode )
  {
    case CEC::CEC_USER_CONTROL_CODE_SELECT:
    {
      code = BTN_SELECT;
      break;
    }
    case CEC::CEC_USER_CONTROL_CODE_UP:
    {
      code = BTN_NORTH;
      break;
    }
    case CEC::CEC_USER_CONTROL_CODE_DOWN:
    {
      code = BTN_SOUTH;
      break;
    }
    case CEC::CEC_USER_CONTROL_CODE_LEFT:
    {
      code = BTN_EAST;
      break;
    }
    case CEC::CEC_USER_CONTROL_CODE_RIGHT:
    {
      code = BTN_WEST;
      break;
    }
    case CEC::CEC_USER_CONTROL_CODE_EXIT:
    {
      code = BTN_Z;
      break;
    }
    case CEC::CEC_USER_CONTROL_CODE_SUB_PICTURE:
    {
      code = BTN_MODE;
      break;
    }
    default:
      break;
  };

  if( key->duration == 0)
    value = 1;

  if (libevdev_uinput_write_event(uidev, EV_KEY, code, value) == 0)
  {
    if (libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0) != 0)
      std::cerr << "ERROR: Cannot write SYN_REPORT to uinput." << std::endl;
  }
  else
    std::cerr << "ERROR: Cannot write EV_KEY:" << code << " to uinput." << std::endl;
}

int main(int argc, char* argv[])
{
    // Set up the CEC config and specify the keypress callback function
    CEC::ICECCallbacks        cec_callbacks;
    CEC::libcec_configuration cec_config;
    cec_config.Clear();
    cec_callbacks.Clear();

    const std::string devicename("CEC joystick");
    devicename.copy(cec_config.strDeviceName, std::min(devicename.size(),13u) );

    cec_config.clientVersion       = CEC::LIBCEC_VERSION_CURRENT;
    cec_config.bActivateSource     = 0;
    cec_config.deviceTypes.Add(CEC::CEC_DEVICE_TYPE_RECORDING_DEVICE);

    cec_callbacks.keyPress    = &onKeypress;
//    cec_callbacks.logMessage  = &logMessage;

    // Get a cec adapter by initialising the cec library
    CEC::ICECAdapter* cec_adapter = CECInitialise(&cec_config);
    if( !cec_adapter )
    {
        std::cerr << "ERROR: Failed loading libcec.so" << std::endl;
        return 1;
    }

    // Initialise the video api
    cec_adapter->InitVideoStandalone();
    // Try to automatically determine the CEC devices
    CEC::cec_adapter_descriptor devices[10];
    uint8_t devices_found = cec_adapter->DetectAdapters(devices, 10, NULL, true);
    if( devices_found <= 0)
    {
        std::cerr << "ERROR: Could not automatically determine the cec adapter devices" << std::endl;
        CECDestroy(cec_adapter);
        return 1;
    }
    std::cout << "Found device. Path: " << devices[0].strComPath << " port: " << devices[0].strComName << std::endl;

    // Open a connection to the zeroth CEC device
    if( !cec_adapter->Open(devices[0].strComName) )
    {
        std::cerr << "ERROR: Failed to open the CEC device on port " << devices[0].strComName << std::endl;
        CECDestroy(cec_adapter);
        return 1;
    }

    sigset_t signal_set;
    int      sig;

  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGABRT);
  int ret = 0;

  dev = libevdev_new();
  libevdev_set_name(dev, "CEC joystick");
  libevdev_enable_event_type(dev, EV_KEY);
  libevdev_enable_event_code(dev, EV_KEY, BTN_SOUTH, NULL);
  libevdev_enable_event_code(dev, EV_KEY, BTN_EAST, NULL);
  libevdev_enable_event_code(dev, EV_KEY, BTN_NORTH, NULL);
  libevdev_enable_event_code(dev, EV_KEY, BTN_WEST, NULL);
  libevdev_enable_event_code(dev, EV_KEY, BTN_SELECT, NULL);
  libevdev_enable_event_code(dev, EV_KEY, BTN_Z, NULL);
  libevdev_enable_event_code(dev, EV_KEY, BTN_MODE, NULL);
  libevdev_set_id_vendor (dev, 7);
  libevdev_set_id_product (dev, 7);

  if( libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev) == 0 )
  {
    cec_adapter->EnableCallbacks(nullptr, &cec_callbacks);
    std::cout << "Fully started." << std::endl;
    sigwait(&signal_set, &sig);
    std::cout << "Signal " << sig << " received, closing." << std::endl;
    libevdev_uinput_destroy(uidev);
  }
  else
  {
    std::cerr << "ERROR: Cannot create uinput from device. You should be running as root (or sudo)." << std::endl;
    ret = 1;
  }

  libevdev_free(dev);
  // Close down and cleanup
  cec_adapter->Close();
  CECDestroy(cec_adapter);

  return ret;
}
