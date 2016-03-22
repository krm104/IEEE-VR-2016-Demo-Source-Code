#ifndef __RENDERFUNCTIONS_H__
#define __RENDERFUNCTIONS_H__
/////////////////////////////////////////////////////////////////////

#include "Globals.h"
#include "ARTFunctions.h"
#include "StateFunctions.h"

void MainRenderFunction();
void RenderHands();
void RenderOcludedHands();
void RenderARTMarker();
void RenderState(EYE_ENUM eye);
void RenderMenuState(EYE_ENUM eye);
void RenderCalibrationState(EYE_ENUM eye);
void RenderDemo1State(EYE_ENUM eye);

void RenderReticle(int eye);

/////////////////////////////////////////////////////////////////////
#endif //__RENDERFUNCTIONS_H__