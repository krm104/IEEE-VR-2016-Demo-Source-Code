#include "LP.h"

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "LeapMotion Connected" << std::endl;
	controller.enableGesture(Gesture::TYPE_SWIPE);
	//In C++
	controller.setPolicyFlags(static_cast<Leap::Controller::PolicyFlag>(Leap::Controller::PolicyFlag::POLICY_OPTIMIZE_HMD | Leap::Controller::PolicyFlag::POLICY_BACKGROUND_FRAMES));
	//Leap::Controller::PolicyFlag::POLICY_IMAGES |

	tracking_finger = false;
	tracking_tool = false;
	tracking_left = false;
	tracking_right = false;
	Lock = false;
	pos_T[0] = pos_T[1] = pos_T[2] = pos_F[0] = pos_F[1] = pos_F[2] = 0.0;
	length_T = 0.0f;
}

void SampleListener::onFrame(const Controller& controller) {
	//std::cout << "\r Frame available";// << std::endl;
	const Frame frame = controller.frame();
	/*std::cout << "\r Frame id: " << frame.id()
		//<< ", timestamp: " << frame.timestamp()
		<< ", hands: " << frame.hands().count()
		<< ", fingers: " << frame.fingers().count()
		<< ", tools: " << frame.tools().count();
	//<< ", gestures: " << frame.gestures().count();// << std::endl;
	*/
	int flag_tracking_finger = 0;
	int flag_tracking_tool = 0;
	int flag_tracking_right = 0;
	int flag_tracking_left = 0;
	for (int i = 0; i < frame.hands().count(); i++)
	{
		if (frame.hands()[i].isRight())
		{
			flag_tracking_right = 1;
			right_hand = frame.hands()[i];
			Leap::FingerList fingers = frame.hands()[i].fingers();
			if (fingers[1].isValid())
			{
				flag_tracking_finger = 1;
				Leap::Vector vec = fingers[1].stabilizedTipPosition();
				if (!Lock)
				{
					Lock = true;
					pos_F[0] = vec[0]; pos_F[1] = vec[1]; pos_F[2] = vec[2];
					Lock = false;
				}
			}
		}
		else if (frame.hands()[i].isLeft())
		{
			flag_tracking_left = 1;
			left_hand = frame.hands()[i];
		}
	}

	Leap::ToolList tools = frame.tools();
	for (int i = 0; i < tools.count(); i++)
	{
		if (tools[i].isValid())
		{
			flag_tracking_tool = 1;
			Lock = true;
			Leap::Vector vec = tools[i].stabilizedTipPosition();//tipPosition();
			pos_T[0] = vec[0]; pos_T[1] = vec[1]; pos_T[2] = vec[2];
			length_T = tools[i].length();
			Lock = false;
		}
	}

	if (flag_tracking_finger == 1)
		tracking_finger = true;
	else
		tracking_finger = false;

	if (flag_tracking_tool == 1)
		tracking_tool = true;
	else
		tracking_tool = false;

	if (flag_tracking_right == 1)
		tracking_right = true;
	else
		tracking_right = false;

	if (flag_tracking_left == 1)
		tracking_left = true;
	else
		tracking_left = false;
}

bool SampleListener::ReadPos(int type, double* r_pos)
{
	if (Lock)
		return false;
	else
	{
		//finger//
		if (type == 0)
		{
			Lock = true;
			r_pos[0] = pos_F[0]; r_pos[1] = pos_F[1]; r_pos[2] = pos_F[2];
			Lock = false;
			return true;
		}
		//tool//
		else if (type == 1)
		{
			Lock = true;
			r_pos[0] = pos_T[0]; r_pos[1] = pos_T[1]; r_pos[2] = pos_T[2];
			Lock = false;
			return true;
		}
	}
	return false;
}

bool SampleListener::GetHand(int handnum, Leap::Hand& _hand)
{
	if (handnum == 0 && tracking_left)
	{
		_hand = left_hand;
		return true;
	}

	if (handnum == 1 && tracking_right)
	{
		_hand = right_hand;
		return true;
	}

	return false;
}

float SampleListener::GetToolLength()
{
	if (tracking_tool)
		return length_T;
	else
		return 0.0;
}

bool SampleListener::isTrackingFinger()
{
	return tracking_finger;
}

bool SampleListener::isTrackingTool()
{
	return tracking_tool;
}

