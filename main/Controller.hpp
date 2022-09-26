//
// Created by jadjer on 29.08.22.
//

#pragma once

#include "Bluetooth.hpp"
#include "Indicator.hpp"
#include <esp_err.h>

class Controller {
 public:
  Controller();
  ~Controller();

 public:
  [[noreturn]] void spin();

 private:
  Indicator m_indicator;
  Bluetooth m_bluetooth;

 private:
  void spinOnce();
};