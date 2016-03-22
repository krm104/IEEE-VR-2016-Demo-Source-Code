#include "CalibrationFunctions.h"

bool LoadHMDCalibration()
{
	//initialize both left and right projection matrices//
	memset(primary_hmd_calibration_left, 0, sizeof(double) * 16);
	memset(primary_hmd_calibration_right, 0, sizeof(double) * 16);

	memset(secondary_hmd_calibration_left, 0, sizeof(double) * 16);
	memset(secondary_hmd_calibration_right, 0, sizeof(double) * 16);

	bool read_flag = false;
	//Attempt to Read in the Calibration matrix files//
	//Left//
	ifstream HMDLeftCalibrationFile("HMDLeft.calib");
	if (HMDLeftCalibrationFile.good())
	{
		read_flag = true;
		int index = 0;
		while (!HMDLeftCalibrationFile.eof())
		{
			HMDLeftCalibrationFile >> primary_hmd_calibration_left[index++];
		}
	}

	//Right//
	ifstream HMDRightCalibrationFile("HMDRight.calib");
	if (HMDRightCalibrationFile.good())
	{
		int index = 0;
		while (!HMDRightCalibrationFile.eof())
		{
			HMDRightCalibrationFile >> primary_hmd_calibration_right[index++];
		}
	}
	else
		read_flag = false;

	HMDLeftCalibrationFile.close();
	HMDLeftCalibrationFile.clear();

	HMDRightCalibrationFile.close();
	HMDRightCalibrationFile.clear();

	/////////////////////////////
	memcpy(secondary_hmd_calibration_left, primary_hmd_calibration_left, sizeof(primary_hmd_calibration_left));
	memcpy(secondary_hmd_calibration_right, primary_hmd_calibration_right, sizeof(primary_hmd_calibration_right));

	return read_flag;
}

bool LoadLeapCalibration()
{
	//Initialize the calibratrion matrix//
	memset(leap_art_calibration, 0, sizeof(double) * 16);
	float tm[16];
	
	//Open and Read in the ART to Leap Calibration File//
	bool read_flag = false;
	ifstream LeapARTCalibrationFile("LeapART.calib");
	if (LeapARTCalibrationFile.good())
	{
		read_flag = true;
		int index = 0;
		while (!LeapARTCalibrationFile.eof())
		{
			LeapARTCalibrationFile >> leap_art_calibration[index++];
			tm[index - 1] = leap_art_calibration[index - 1];
		}
	}

	LeapARTCalibrationFile.close();
	LeapARTCalibrationFile.clear();

	memcpy(&ARTLeapTransform[0][0], tm, sizeof(float)*16);

	//Open and Read in the Leap to ART Calibration File//
	ifstream ARTLeapCalibrationFile("ARTLeap.calib");
	if (ARTLeapCalibrationFile.good())
	{
		read_flag = true;
		int index = 0;
		while (!ARTLeapCalibrationFile.eof())
		{
			ARTLeapCalibrationFile >> art_leap_calibration[index++];
			tm[index - 1] = art_leap_calibration[index - 1];
		}
	}

	ARTLeapCalibrationFile.close();
	ARTLeapCalibrationFile.clear();

	memcpy(&LeapARTTransform[0][0], tm, sizeof(float) * 16);
	memcpy(&LeapARTOrient[0][0], tm, sizeof(float) * 16);

	//remove translation from orient//
	LeapARTOrient[3][0] = 0.0; LeapARTOrient[3][1] = 0.0; LeapARTOrient[3][2] = 0.0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << LeapARTOrient[i][j] << " ";
		cout << endl;
	}
	return read_flag;
}

bool IntersectPointPlane(float point[], float y, float xn, float xp, float zn, float zp)
{
	//Check the point against the bounds of the plane//
	if (point[0] < xp && point[0] > xn && point[2] < zp && point[2] > zn && point[1] > y)
		return true;
	else
		return false;
}

////////////////////////////////////////////////////////////////////

void LoadAlignments(char* file_name)
{
	calibrationScriptNames.clear();
	//////////Read in the Script File//////////
	ifstream script_file(file_name);
	if (script_file.good())
	{
		cout << "Reading Script File " << file_name << endl;
		string token;
		while (!script_file.eof())
		{
			script_file >> token;
			calibrationScriptNames.push_back(token);
			cout << "Read File " << token << endl;
		}
	}
	else
	{
		cout << "Unable to Read Script File " << file_name << endl;
	}
	script_file.close();
	script_file.clear();

	LoadFile(1);
}

//////////////////////////////////////////////////
void LoadFile(int file_num)
{
	if (calibrationScriptNames.size() <= file_num - 1)
	{
		cout << "No Such File in Script File" << endl;
		return;
	}

	//////////Read in the indicated file//////////
	ifstream alignment_file(calibrationScriptNames[file_num - 1]);
	if (alignment_file.good())
	{
		alignment_pairsL.clear();
		alignment_pairsR.clear();
		cout << endl << "Reading in file " << calibrationScriptNames[file_num - 1] << endl;
		string token;
		while (!alignment_file.eof())
		{
			alignment_file >> token;
			if (token == "crossL")
			{
				Correspondence_Pair<double, double> new_pair;
				alignment_file >> new_pair.screenPoint(0);
				alignment_file >> new_pair.screenPoint(1);
				alignment_pairsL.push_back(new_pair);
			}
			else if (token == "crossR")
			{
				Correspondence_Pair<double, double> new_pair;
				alignment_file >> new_pair.screenPoint(0);
				alignment_file >> new_pair.screenPoint(1);
				alignment_pairsR.push_back(new_pair);
			}
		}

		cout << alignment_pairsL.size() << " Left alignments read" << endl;

		cout << alignment_pairsR.size() << " Right alignments read" << endl;

		saved_pair.clear();
		for (int i = 0; i < alignment_pairsL.size() || i < alignment_pairsR.size(); i++)
		{
			saved_pair.push_back(false);
		}

		alignment_num_spinner->set_int_limits(0, std::max(alignment_pairsL.size(), alignment_pairsR.size()));
		alignment_left_num_spinner->set_int_val(alignment_pairsL.size());
		alignment_right_num_spinner->set_int_val(alignment_pairsR.size());

		//string tfn; tfn.append(calibrationScriptNames[file_num - 1]);

		/*
		//Setup Recording Results (position) Files//
		recording_results.clear();
		for (int i = 0; (i < alignment_pairsL.size() || i < alignment_pairsR.size()); i++)
		{
		recording_result.clear();
		recording_result.append(tfn.begin(), tfn.end() - 4);
		recording_result.append(".recording.");
		stringstream anum; anum.clear(); anum << i; anum << ".txt";
		recording_result.append(anum.str());
		recording_results.push_back(recording_result);
		}
		*/

		////////////////////////////////////////////////////////////////////////
		spaam_result_left.clearMatrix();
		spaam_result_right.clearMatrix();
	}
	else
	{
		cout << "Could not read file " << calibrationScriptNames[file_num - 1] << endl;
	}

	alignment_file.close();
	alignment_file.clear();
}

//////////////////////////////////////////////////
bool SetupReticles(int width, int height, int thickness)
{
	if (width <= 0 || height <= 0 || thickness <= 0)
	{
		return false;
	}

	reticle_tracked = new float[3 * width*height];
	reticle_untracked = new float[3 * width*height];
	reticle_recording = new float[3 * width*height];

	int startx = (width / 2 - thickness / 2);
	int starty = (height / 2 - thickness / 2);

	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
		{
			if (i >= startx && i < startx + thickness)
			{
				reticle_tracked[(j*width + i) * 3] = tracked_color[0];
				reticle_tracked[(j*width + i) * 3 + 1] = tracked_color[1];
				reticle_tracked[(j*width + i) * 3 + 2] = tracked_color[2];
				reticle_untracked[(j*width + i) * 3] = untracked_color[0];
				reticle_untracked[(j*width + i) * 3 + 1] = untracked_color[1];
				reticle_untracked[(j*width + i) * 3 + 2] = untracked_color[2];
				reticle_recording[(j*width + i) * 3] = recording_color[0];
				reticle_recording[(j*width + i) * 3 + 1] = recording_color[1];
				reticle_recording[(j*width + i) * 3 + 2] = recording_color[2];
			}
			else if (j >= starty && j < starty + thickness)
			{
				reticle_tracked[(j*width + i) * 3] = tracked_color[0];
				reticle_tracked[(j*width + i) * 3 + 1] = tracked_color[1];
				reticle_tracked[(j*width + i) * 3 + 2] = tracked_color[2];
				reticle_untracked[(j*width + i) * 3] = untracked_color[0];
				reticle_untracked[(j*width + i) * 3 + 1] = untracked_color[1];
				reticle_untracked[(j*width + i) * 3 + 2] = untracked_color[2];
				reticle_recording[(j*width + i) * 3] = recording_color[0];
				reticle_recording[(j*width + i) * 3 + 1] = recording_color[1];
				reticle_recording[(j*width + i) * 3 + 2] = recording_color[2];
			}
			else
			{
				reticle_tracked[(j*width + i) * 3] = 0.0; reticle_tracked[(j*width + i) * 3 + 1] = 0.0; reticle_tracked[(j*width + i) * 3 + 2] = 0.0;
				reticle_untracked[(j*width + i) * 3] = 0.0; reticle_untracked[(j*width + i) * 3 + 1] = 0.0; reticle_untracked[(j*width + i) * 3 + 2] = 0.0;
				reticle_recording[(j*width + i) * 3] = 0.0; reticle_recording[(j*width + i) * 3 + 1] = 0.0; reticle_recording[(j*width + i) * 3 + 2] = 0.0;
			}
		}

	return true;
}

//////////////////////////////////////////////////
void CalcProjection()
{
	////////////////////////////////////////////////////////////////////
	//Update Left Projection//
	spaam_result_left.corr_points.clear();
	for (unsigned int i = 0; (i < saved_pair.size() && i < alignment_pairsL.size()); i++)
	{
		if (saved_pair[i])
		{
			spaam_result_left.corr_points.push_back(alignment_pairsL[i]);
		}
	}

	spaam_result_left.projectionDLTImpl();
	spaam_result_left.BuildGLMatrix3x4(CNEAR, CFAR, window_w / 2, 0, window_h, 0);

	///////////////////////////////////////////////////////////////////
	//Update Right Projection//
	spaam_result_right.corr_points.clear();
	for (unsigned int i = 0; (i < saved_pair.size() && i < alignment_pairsR.size()); i++)
	{
		if (saved_pair[i])
		{
			spaam_result_right.corr_points.push_back(alignment_pairsR[i]);
		}
	}

	spaam_result_right.projectionDLTImpl();
	spaam_result_right.BuildGLMatrix3x4(CNEAR, CFAR, window_w / 2, 0, window_h, 0);

	memcpy(secondary_hmd_calibration_left, spaam_result_left.projMat3x4, sizeof(secondary_hmd_calibration_left));
	memcpy(secondary_hmd_calibration_right, spaam_result_right.projMat3x4, sizeof(secondary_hmd_calibration_right));

	ofstream calibFile("claibFile.txt");
	if (calibFile.good())
	{
	calibFile << "LEFT ";
	for (int i = 0; i < 16; i++)
	{
	calibFile << spaam_result_left.projMat3x4[i] << " ";
	}
	calibFile << endl;

	calibFile << "RIGHT ";
	for (int i = 0; i < 16; i++)
	{
	calibFile << spaam_result_right.projMat3x4[i] << " ";
	}
	calibFile << endl;
	}
}

/////////////////////////////////////////////////
void RecordingFunc(int n)
{
	if (n == 1)
		recording_points.clear();

	//////////process LeapMotion//////////
	listener.ReadPos(1, recorded_pos);

	Pose recorded_pose;
	recorded_pose.x = recorded_pos[0];
	recorded_pose.y = recorded_pos[1];
	recorded_pose.z = recorded_pos[2];

	recording_points.push_back(recorded_pose);

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

/////////////////////////////////////////////////
void StartRecording()
{
	recording = true;

	RecordingFunc(1);
}

/////////////////////////////////////////////////
int CalcPosDifference(double *pos1, double* pos2)
{
	if (pos1 != NULL && pos2 != NULL)
	{
		return std::sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2) + pow(pos1[2] - pos2[2], 2));
	}

	return INT_MAX;
}