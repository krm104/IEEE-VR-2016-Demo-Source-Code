#include "Globals.h"

//////////Window Globals//////////
char WindowName[] = "VR 2016 Demo";
int WindowHandle = 0;
int window_x = -2560;
int window_y = 0;
int window_w = 2560;
int window_h = 1024;

//////////Leap Globals//////////
Controller LMcontrol;
SampleListener listener;
Leap::Hand right_hand;
bool tracking_right_hand = false;
Leap::Hand left_hand;
bool tracking_left_hand = false;
Leap::Arm right_arm;
Leap::Arm left_arm;

vector<tuple<float, float, float>> hand_velocity;
int hand_velocity_records = 20;
int hand_velocity_iterator = 0;

double alignment_pos[3] = { 0.0, 0.0, 0.0 };
double recorded_pos[3] = { 0.0, 0.0, 0.0 };
double tool_pos[3] = { 0.0, 0.0, 0.0 };
int tool_pos_pass = 0;
double tool_now_pos[3];

//////////ART Globals//////////
DTrackSDK* dt = NULL;
Pose head_pose;
Pose art_head_pose;
Pose calib_body;

//////////Calibration Globals//////////
SPAAM_SVD<double, double> spaam_result_left;
SPAAM_SVD<double, double> spaam_result_right;

int alignment_num = 0;
int render_reticle = 0;
int alignment_left_num = 0;
int alignment_right_num = 0;
int CNEAR = 1.0;
int CFAR = 2000.0;

double primary_hmd_calibration_left[16];
double primary_hmd_calibration_right[16];

double secondary_hmd_calibration_left[16];
double secondary_hmd_calibration_right[16];

vector<Correspondence_Pair<double, double>> alignment_pairsL;
vector<Correspondence_Pair<double, double>> alignment_pairsR;
vector<bool> saved_pair;
vector<Pose> recording_points;

int RET_WDTH = 65;
int RET_HGT = 65;
int RET_THICK = 5;

bool tracking = false;
bool recording = false;

float tracked_color[3] = { 0.0, 1.0, 0.0 };
float untracked_color[3] = { 1.0, 0.0, 0.0 };
float recording_color[3] = { 1.0, 1.0, 0.0 };

float* reticle_tracked;
float* reticle_untracked;
float* reticle_recording;

vector<string> calibrationScriptNames;

string alignment_resultsL;
string alignment_resultsR;

string recording_result;
vector<string> recording_results;

double leap_art_calibration[16];
double art_leap_calibration[16];

//////////GLUI Globals//////////
GLUI* glui = NULL; //Glui control pane//

GLUI_Panel* window_panel = NULL; //Panel with window state buttons//
GLUI_Panel* state_panel = NULL; //Panel with demo state buttons//
GLUI_Panel* calibration_panel = NULL; //Panel with calibration controls//

GLUI_Panel* state_checkbox_panel = NULL;
GLUI_Checkbox* menu_state_checkbox = NULL;
int menu_state_checkbox_value = 1;
GLUI_Checkbox* demo1_state_checkbox = NULL;
int demo1_state_checkbox_value = 0;
GLUI_Checkbox* calibration_state_checkbox = NULL;
int calibration_state_checkbox_value = 0;

GLUI_Spinner* window_x_spinner = NULL; //Control to change window x screen location
GLUI_Spinner* window_y_spinner = NULL; //Control to change window y screen location
GLUI_Spinner* window_w_spinner = NULL; //Control to change window width
GLUI_Spinner* window_h_spinner = NULL; //Control to change window height

GLUI_Spinner* alignment_num_spinner = NULL; //Control to change the active alignment crosshair
GLUI_Spinner* alignment_left_num_spinner = NULL; //Control to change the active alignment crosshair
GLUI_Spinner* alignment_right_num_spinner = NULL; //Control to change the active alignment crosshair
GLUI_Checkbox* render_reticle_checkbox = NULL; //Control to change if the reticle is rendered to the screen

GLUI_Button* load_hmd_calibration_data_button = NULL; //Control to load the HMD calibration data 
GLUI_Button* load_leap_calibration_data_button = NULL; //Control to load the LEAP to ART calibration data

//////////State Globals//////////
STATE cur_state = STATE_MENU;
STATE_MENU_ITEMS state_menu_items;
STATE_DEMO1_ITEMS state_demo1_items;
STATE_CALIBRATION_ITEMS state_calibration_items;

//////////Remaining and Misc Items//////////
int selected_tile = 0;

gl_Shader hand_shader;

glm::mat4 ARTLeapTransform;
glm::mat4 LeapARTTransform;
glm::mat4 LeapARTOrient;

float CalcDistance(float d1[], float d2[])
{
	return std::sqrt((d1[0] - d2[0])*(d1[0] - d2[0]) + (d1[1] - d2[1])*(d1[1] - d2[1]) + (d1[2] - d2[2])*(d1[2] - d2[2]));
}

//////////Class Functions//////////

double* calcQuat(double* mat)
{
	double* quatmat = new double[4];

	//Calculate the trace of the matrix T from the equation:
	//           2     2     2
	// T = 4 - 4x  - 4y  - 4z
	//            2    2    2
	//   = 4( 1 -x  - y  - z )
	double T = mat[0] + mat[4] + mat[8] + 1;

	//If the trace of the matrix is greater than zero, then
	//perform an "instant" calculation.
	if (T > 0)
	{
		double S = 0.5 / std::sqrt(T);
		double W = 0.25 / S;
		double X = (mat[5] - mat[7]) * S;
		double Y = (mat[6] - mat[2]) * S;
		double Z = (mat[1] - mat[3]) * S;
		quatmat[0] = X;
		quatmat[1] = Y;
		quatmat[2] = Z;
		quatmat[3] = W;
	}
	else
	{
		//If the trace of the matrix is less than or equal to zero
		//then identify which major diagonal element has the greatest
		//value. Depending on this value, calculate the following:
		double S, Qx, Qy, Qz, Qw;

		//Column 0:
		S = std::sqrt(1.0 + mat[0] - mat[4] - mat[8]) * 2;
		Qx = 0.5 / S;
		Qy = (mat[3] + mat[1]) / S;
		Qz = (mat[6] + mat[2]) / S;
		Qw = (mat[7] + mat[5]) / S;

		//Column 1:
		S = std::sqrt(1.0 + mat[4] - mat[0] - mat[8]) * 2;
		Qx = (mat[3] + mat[1]) / S;
		Qy = 0.5 / S;
		Qz = (mat[7] + mat[5]) / S;
		Qw = (mat[6] + mat[2]) / S;

		//Column 2:
		S = std::sqrt(1.0 + mat[8] - mat[0] - mat[4]) * 2;
		Qx = (mat[6] + mat[2]) / S;
		Qy = (mat[7] + mat[5]) / S;
		Qz = 0.5 / S;
		Qw = (mat[3] + mat[1]) / S;

		//The quaternion is then defined as:
		//Q = | Qx Qy Qz Qw |
		quatmat[0] = Qx;
		quatmat[1] = Qy;
		quatmat[2] = Qz;
		quatmat[3] = Qw;
	}
	return quatmat;
}

Pose PoseMultiplication(Pose P1, Pose P2)
{
	Pose result_pose;

	//m_rotation * Q.rotation(),
	result_pose.rotations = P2.rotations*P1.rotations;

	//( m_rotation * Q.translation() ) + m_translation
	double xy = P2.rotations.x * P2.rotations.y;
	double xz = P2.rotations.x * P2.rotations.z;
	double yz = P2.rotations.y * P2.rotations.z;
	double ww = P2.rotations.w * P2.rotations.w;
	double wx = P2.rotations.w * P2.rotations.x;
	double wy = P2.rotations.w * P2.rotations.y;
	double wz = P2.rotations.w * P2.rotations.z;

	result_pose.x = P1.x * (2 * (P2.rotations.x*P2.rotations.x + ww) - 1) + P1.y * 2 * (xy - wz) + P1.z * 2 * (wy + xz);
	result_pose.y = P1.x * 2 * (xy + wz) + P1.y * (2 * (P2.rotations.y*P2.rotations.y + ww) - 1) + P1.z * 2 * (yz - wx);
	result_pose.z = P1.x * 2 * (xz - wy) + P1.y * 2 * (wx + yz) + P1.z * (2 * (P2.rotations.z*P2.rotations.z + ww) - 1);

	result_pose.x += P2.x;
	result_pose.y += P2.y;
	result_pose.z += P2.z;

	return result_pose;
}

Pose InversePose(Pose P1)
{
	Pose result_pose;
	//~(m_rotation) );
	P1.rotations = glm::inverse(P1.rotations);

	//-( rinv * m_translation ) );
	double xy = P1.rotations.x * P1.rotations.y;
	double xz = P1.rotations.x * P1.rotations.z;
	double yz = P1.rotations.y * P1.rotations.z;
	double ww = P1.rotations.w * P1.rotations.w;
	double wx = P1.rotations.w * P1.rotations.x;
	double wy = P1.rotations.w * P1.rotations.y;
	double wz = P1.rotations.w * P1.rotations.z;

	result_pose.x = -(P1.x * (2 * (P1.rotations.x*P1.rotations.x + ww) - 1) + P1.y * 2 * (xy - wz) + P1.z * 2 * (wy + xz));
	result_pose.y = -(P1.x * 2 * (xy + wz) + P1.y * (2 * (P1.rotations.y*P1.rotations.y + ww) - 1) + P1.z * 2 * (yz - wx));
	result_pose.z = -(P1.x * 2 * (xz - wy) + P1.y * 2 * (wx + yz) + P1.z * (2 * (P1.rotations.z*P1.rotations.z + ww) - 1));
	result_pose.rotations = P1.rotations;

	return result_pose;
}

//////////      //////////
VOBJECT::VOBJECT()
{
	object_model = new gl_Model();
	visible = false;

	prev_time = 0.0;

	speed = 0.0;
	memset(direction, 0, sizeof(double) * 3);
	gravity_enabled = false;

	memset(location, 0, sizeof(float) * 3);
	memset(reset_location, 0, sizeof(float) * 3);

	alpha = 1.0;
	is_fade = 0;

	elapsed = 0.0;

	scale = 1.0;
};

VOBJECT::VOBJECT(GLchar* model_name, float start_loc[], double start_dir[], double start_spd, float i_scale)
{
	object_model = new gl_Model();
	if (!object_model->Load_OBJ(model_name))
		printf("Error Loading Object %s\n", model_name);
	else
		printf("Object %s Loaded \n", model_name);

	visible = false;

	prev_time = 0.0;

	speed = start_spd;
	memcpy(direction, start_dir, sizeof(double) * 3);
	gravity_enabled = false;

	memcpy(location, start_loc, sizeof(float) * 3);
	memcpy(reset_location, start_loc, sizeof(float) * 3);

	alpha = 1.0;
	is_fade = 0;

	elapsed = 0.0;

	scale = i_scale;
}

VOBJECT::~VOBJECT(){};

void VOBJECT::CalcMovement()
{
	object_pose.rotations.x = 0.0; object_pose.rotations.y = 0.0; object_pose.rotations.z = 0.0; object_pose.rotations.w = 1.0;
	//ART Coordinate Frame: +X Right, +Y Forward, +Z Up//
	location[0] = location[0] + direction[0] * speed * elapsed;
	location[1] = location[1] + direction[1] * speed * elapsed;
	location[2] = location[2] + direction[2] * speed * elapsed;

	if (gravity_enabled)
	{
		location[2] = location[2] - elapsed;
	}

	object_pose.x = location[0];
	object_pose.y = location[1];
	object_pose.z = location[2];
	object_pose = PoseMultiplication(object_pose, art_head_pose);
}

void VOBJECT::Render(int rot)
{
	//Calculate Time Since Last Render//
	double cur_time = glutGet(GLUT_ELAPSED_TIME);
	double s_elapsed = (float)(cur_time - prev_time) * 0.001f;

	//Update Location//
	if (s_elapsed > 0.01f)
	{
		prev_time = cur_time; // Don't update more often than 100 Hz.
		elapsed = s_elapsed;

		if (is_fade == 2)//out//
		{
			alpha = std::max(alpha - .5*s_elapsed, 0.0);
			is_fade = alpha > 0.0 ? is_fade : 0;
		}
		else if (is_fade == 1)//in//
		{
			alpha = std::min(alpha + .5*s_elapsed, 1.0);
			is_fade = alpha < 1.0 ? is_fade : 0;
		}
	}

	glPushMatrix();
	glm::mat4 artOrient = glm::mat4_cast((object_pose).rotations);
	glTranslatef(object_pose.x, object_pose.y, object_pose.z);
	if ( rot == 1)
		glRotatef(90.0, 1.0, 0.0, 0.0);
	glMultMatrixf(glm::value_ptr(artOrient));
	
	glScalef(scale, scale, scale);
	object_model->Load_Uniform(1, string("object_shader"), string("Alpha"), alpha);
	object_model->Draw();
	glPopMatrix();
}

void VOBJECT::SetLocation(float new_loc[])
{
	memcpy(location, new_loc, sizeof(float) * 3);
}

void VOBJECT::SetResetLocation(float new_loc[])
{
	memcpy(reset_location, new_loc, sizeof(float) * 3);
}

void VOBJECT::ResetLocation()
{
	memcpy(location, reset_location, sizeof(float) * 3);
}

void VOBJECT::SetDirection(double new_dir[])
{
	memcpy(direction, new_dir, sizeof(double) * 3);
}

void VOBJECT::SetSpeed(double new_spd)
{
	speed = new_spd;
}

//////////        //////////
BALL::BALL()
{
	prev_time = 0.0;
	elapsed = 0.0;

	memset(velocity, 0, sizeof(double) * 3);
	memset(location, 0, sizeof(float) * 3);
	memset(reset_location, 0, sizeof(float) * 3);

	gravity_enabled = false;

	alpha = 1.0;
	is_fade = 0;

	radius = 1.0;
	color[0] = .8; color[1] = .3; color[2] = .3;

	grabbed = false;
	grabbed_hand = 0;
}

BALL::BALL(float new_radius, float new_color[], float start_loc[])
{
	prev_time = 0.0;
	elapsed = 0.0;

	memset(velocity, 0, sizeof(double) * 3);
	memcpy(location, start_loc, sizeof(float) * 3);
	memcpy(reset_location, start_loc, sizeof(float) * 3);

	gravity_enabled = false;

	alpha = 1.0;
	is_fade = 0;

	radius = new_radius;
	memcpy(color, new_color, sizeof(float) * 3);

	grabbed = false;
	grabbed_hand = 0;
}

BALL::~BALL()
{

}

void BALL::CalcMovement()
{
	//ART Coordinate Frame: +X Right, +Y Forward, +Z Up//
	if (!grabbed)
	{
		location[0] = location[0] + 2*velocity[0] * elapsed;
		location[1] = location[1] + 2*velocity[1] * elapsed;
		location[2] = location[2] + 2*velocity[2] * elapsed;

		if (gravity_enabled)
		{
			location[2] = location[2] - 100*elapsed;
		}
	}

	object_pose.x = location[0];
	object_pose.y = location[1];
	object_pose.z = location[2];
	object_pose = PoseMultiplication(object_pose, art_head_pose);
}

void BALL::Render()
{
	//Calculate Time Since Last Render//
	double cur_time = glutGet(GLUT_ELAPSED_TIME);
	double s_elapsed = (float)(cur_time - prev_time) * 0.001f;

	//Update Location//
	if (s_elapsed > 0.01f)
	{
		prev_time = cur_time; // Don't update more often than 100 Hz.
		elapsed = s_elapsed;

		if (is_fade == 2)//out//
		{
			alpha = std::max(alpha - .5*s_elapsed, 0.0);
			is_fade = alpha > 0.0 ? is_fade : 0;
		}
		else if (is_fade == 1)//in//
		{
			alpha = std::min(alpha + .5*s_elapsed, 1.0);
			is_fade = alpha < 1.0 ? is_fade : 0;
		}
	}

	glPushMatrix();
	glTranslatef(object_pose.x, object_pose.y, object_pose.z);
	ball_shader.Activate();
	if ( alpha < 1.0 )
		ball_shader.set_uniform_3f("Color", color[0], color[1], color[2]);
	else if ( grabbed )
		ball_shader.set_uniform_3f("Color", 0.4, 0.4, 0.8);
	else
		ball_shader.set_uniform_3f("Color", 0.4, 0.8, 0.4);
	ball_shader.set_uniform_1f("Alpha", alpha);
	glutSolidSphere(radius, 25, 25);
	ball_shader.DeActivate();
	glPopMatrix();
}

void BALL::SetLocation(float new_loc[])
{
	memcpy(location, new_loc, sizeof(float) * 3);
}

void BALL::Reset()
{
	memset(velocity, 0, sizeof(double) * 3);
	memcpy(location, reset_location, sizeof(float) * 3);

	gravity_enabled = false;

	grabbed = false;
	grabbed_hand = 0;
}

void BALL::SetVelocity(double new_vel[])
{
	memcpy(velocity, new_vel, sizeof(double) * 3);
}
