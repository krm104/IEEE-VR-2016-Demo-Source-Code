
#ifndef __LP__H__
#define __LP__H__

#include <Leap.h>

using namespace Leap;

class SampleListener : public Listener {
    public:
		virtual void onConnect(const Controller&);
		virtual void onFrame(const Controller&);

		bool ReadPos( int type,  double* r_pos );
		bool GetHand( int handnum, Leap::Hand& _hand );
		float GetToolLength();
		bool isTrackingFinger();
		bool isTrackingTool();

	private:
		bool tracking_finger;
		bool tracking_tool;
		bool tracking_left;
		bool tracking_right;
		bool Lock;
		double pos_F[3];
		double pos_T[3];
		float length_T;
		Leap::Hand right_hand;
		Leap::Hand left_hand;
};

#endif