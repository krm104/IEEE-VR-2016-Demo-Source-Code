#include "TimerFunctions.h"

void TrackToolTimer(int flag)
{
	if (cur_state == STATE_CALIBRATION)
	{
		if (tracking && !recording)
		{
			listener.ReadPos(1, tool_now_pos);
			if (tool_pos_pass == 0)
				memcpy(tool_pos, tool_now_pos, sizeof(tool_pos));

			if (CalcPosDifference(tool_now_pos, tool_pos) <= 20)
			{
				tool_pos_pass++;
			}
			else
			{
				tool_pos_pass = 0;
			}

			//memcpy(tool_pos, tool_now_pos, sizeof(tool_pos));
		}

		if (tool_pos_pass > 24)
		{
			recording = true;
			glutTimerFunc(100, RecordingTimer, 1);
			tool_pos_pass = 0;
		}

		glutTimerFunc(100, TrackToolTimer, 0);
	}
}

void RecordingTimer(int n)
{
	if (n == 1)
		recording_points.clear();

	//////////process LeapMotion//////////
	//if ( LeapTracking() )
	{
		Pose recorded_pose;
		listener.ReadPos(1, recorded_pos);

		recorded_pose.x = recorded_pos[0];
		recorded_pose.y = recorded_pos[1];
		recorded_pose.z = recorded_pos[2];

		recording_points.push_back(recorded_pose);
	}

	if (n < 10)
		glutTimerFunc(100, RecordingFunc, n + 1);
	else
	{
		//////////Save the Average Position///////////////
		Pose averageloc; averageloc.x = 0.0; averageloc.y = 0.0; averageloc.z = 0.0;
		for (int i = 0; i < recording_points.size(); i++)
		{
			averageloc.x += recording_points[i].x;
			averageloc.y += recording_points[i].y;
			averageloc.z += recording_points[i].z;
		}

		//Save the Left Correspondence Points//
		if (alignment_num < alignment_pairsL.size())
		{
			alignment_pairsL[alignment_num].worldPoint(0) = averageloc.x / recording_points.size();
			alignment_pairsL[alignment_num].worldPoint(1) = averageloc.y / recording_points.size();
			alignment_pairsL[alignment_num].worldPoint(2) = averageloc.z / recording_points.size();
		}

		//Save the Right Correspondence Points//
		if (alignment_num < alignment_pairsR.size())
		{
			alignment_pairsR[alignment_num].worldPoint(0) = averageloc.x / recording_points.size();
			alignment_pairsR[alignment_num].worldPoint(1) = averageloc.y / recording_points.size();
			alignment_pairsR[alignment_num].worldPoint(2) = averageloc.z / recording_points.size();
		}

		saved_pair[alignment_num] = true;

		///////////Update the Projection Matrix Results//////////
		CalcProjection();

		//////////Increment the Alignment num and Stop Recording//////////
		alignment_num++;
		recording = false;
	}
}