// Copyright(c) 2019 Ken Okabe
// This software is released under the MIT License, see LICENSE.
#include "platform.h"

#include "exception_event.h"
#include "microcontroller.h"
#include "section.h"

void Platform_Initialize(void) {
  Section_Initialize();
  ExceptionEvent_Initialize();
  Microcontroller_Initialize();
}

void Platform_Abort(void) {
  // do something
}
