#ifndef __CALIBRATIONFUNCTIONS_H__
#define __CALIBRATIONFUNCTIONS_H__
/////////////////////////////////////////////////////////////////////
#include "Globals.h"

bool LoadHMDCalibration();
bool LoadLeapCalibration();
bool IntersectPointPlane(float point[], float y, float xn, float xp, float zn, float zp);

void LoadAlignments(char* file_name);

void LoadFile(int file_num);

bool SetupReticles(int width, int height, int thickness);

void CalcProjection();

void RecordingFunc(int n);

void StartRecording();

int CalcPosDifference(double* pos1, double* pos2);

/////////////////////////////////////////////////////////////////////
#endif //__CALIBRATIONFUNCTIONS_H__