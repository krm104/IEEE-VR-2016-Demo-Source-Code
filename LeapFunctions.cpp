#include "LeapFunctions.h"

bool initLM()
{
	//Add Listener//
	LMcontrol.addListener(listener);
	return true;
}

bool unInitLM()
{
	// Remove the sample listener when done
	LMcontrol.removeListener(listener);
	return true;
}

bool LeapTracking()
{
	tracking_left_hand = listener.GetHand(0, left_hand);
	tracking_right_hand = listener.GetHand(1, right_hand);

	return true;
}
