#ifndef __STATEFUNCTIONS_H__
#define __STATEFUNCTIONS_H__
/////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "CalibrationFunctions.h"
#include "TimerFunctions.h"

void BuildStates();
void BuildMenuState();
void BuildCalibrationState();
void BuildDemo1State();
void BuildMiscItems();

void UpdateState();
void UpdateMenuState(int reset = 0);
void UpdateCalibrationState(int reset = 0);
void UpdateDemo1State(int reset = 0);

/////////////////////////////////////////////////////////////////////
#endif //__STATEFUNCTIONS_H__//