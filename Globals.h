#ifndef __GLOBALS_H__
#define __GLOBALS_H__
//////////////////////////////////////////////////////////////////////////

#include "Header.h"

//////////Global Constructs//////////
struct Pose
{
	float x;
	float y;
	float z;
	quat rotations;
};
double* calcQuat(double* mat);
Pose PoseMultiplication(Pose P1, Pose P2);
Pose InversePose(Pose P1);

class VOBJECT
{
	public:
		VOBJECT();
		VOBJECT(GLchar* model_name, float start_loc[], double start_dir[], double start_spd, float i_scale);

		~VOBJECT();

		void CalcMovement();

		void Render(int rot = 0);

		void SetLocation(float new_loc[]);

		void SetResetLocation(float new_loc[]);

		void ResetLocation();

		void SetDirection(double new_dir[]);

		void SetSpeed(double new_spd);

	public:

		gl_Model* object_model;
		bool visible;
		double prev_time;
		double speed;
		double direction[3];
		bool gravity_enabled;

		float location[3];
		float reset_location[3];

		double alpha;
		int is_fade;

		float elapsed;

		float scale;
		Pose object_pose;
};

class BALL
{
public:
	BALL();
	BALL(float new_radius, float new_color[], float start_loc[]);

	~BALL();

	void CalcMovement();

	void Render();

	void SetLocation(float new_loc[]);

	void Reset();

	void SetVelocity(double new_vel[]);

public:
	double prev_time;
	double velocity[3];
	bool gravity_enabled;

	float location[3];
	float reset_location[3];

	double alpha;
	int is_fade;

	float elapsed;

	float radius;
	float color[3];

	bool grabbed;
	int grabbed_hand;
	Pose object_pose;

	gl_Shader ball_shader;
};

//////////Window Globals///////////
extern char WindowName[];
extern int WindowHandle;
extern int window_x;
extern int window_y;
extern int window_w;
extern int window_h;

//////////Leap Globals//////////
extern Controller LMcontrol;
extern SampleListener listener;
extern Leap::Hand right_hand;
extern bool tracking_right_hand;
extern Leap::Hand left_hand;
extern bool tracking_left_hand;
extern Leap::Arm right_arm;
extern Leap::Arm left_arm;

extern vector<tuple<float, float, float>> hand_velocity;
extern int hand_velocity_records;
extern int hand_velocity_iterator;

extern double alignment_pos[3];
extern double recorded_pos[3];
extern double tool_pos[3];
extern int tool_pos_pass;
extern double tool_now_pos[3];

//////////ART Globals//////////
extern DTrackSDK* dt;
extern Pose head_pose;
extern Pose art_head_pose;
extern Pose calib_body;

//////////Calibration Globals//////////
extern SPAAM_SVD<double, double> spaam_result_left;
extern SPAAM_SVD<double, double> spaam_result_right;

extern int alignment_num;
extern int render_reticle;
extern int alignment_left_num;
extern int alignment_right_num;
extern int CNEAR;
extern int CFAR;

extern double primary_hmd_calibration_left[16];
extern double primary_hmd_calibration_right[16];

extern double secondary_hmd_calibration_left[16];
extern double secondary_hmd_calibration_right[16];

extern vector<Correspondence_Pair<double, double>> alignment_pairsL;
extern vector<Correspondence_Pair<double, double>> alignment_pairsR;
extern vector<bool> saved_pair;
extern vector<Pose> recording_points;

extern int RET_WDTH;
extern int RET_HGT;
extern int RET_THICK;

enum EYE_ENUM
{
	LEFT,
	RIGHT
};

extern bool tracking;
extern bool recording;

extern float tracked_color[3];
extern float untracked_color[3];
extern float recording_color[3];

extern float* reticle_tracked;
extern float* reticle_untracked;
extern float* reticle_recording;

extern vector<string> calibrationScriptNames;

extern string alignment_resultsL;
extern string alignment_resultsR;

extern string recording_result;
extern vector<string> recording_results;

extern double leap_art_calibration[16];
extern double art_leap_calibration[16];

//////////GLUI Globals//////////
extern GLUI* glui; //Glui control pane//

extern GLUI_Panel* window_panel; //Panel with window state buttons//
extern GLUI_Panel* state_panel; //Panel with demo state buttons//
extern GLUI_Panel* calibration_panel; //Panel with calibration controls//

extern GLUI_Panel* state_checkbox_panel;
extern GLUI_Checkbox* menu_state_checkbox;
extern int menu_state_checkbox_value;
extern GLUI_Checkbox* demo1_state_checkbox;
extern int demo1_state_checkbox_value;
extern GLUI_Checkbox* calibration_state_checkbox;
extern int calibration_state_checkbox_value;

extern GLUI_Spinner* window_x_spinner; //Control to change window x screen location
extern GLUI_Spinner* window_y_spinner; //Control to change window y screen location
extern GLUI_Spinner* window_w_spinner; //Control to change window width
extern GLUI_Spinner* window_h_spinner; //Control to change window height

extern GLUI_Spinner* alignment_num_spinner; //Control to change the active alignment crosshair
extern GLUI_Spinner* alignment_left_num_spinner; //Control to change the active alignment crosshair
extern GLUI_Spinner* alignment_right_num_spinner; //Control to change the active alignment crosshair
extern GLUI_Checkbox* render_reticle_checkbox; //Control to change if the reticle is rendered to the screen

extern GLUI_Button* load_hmd_calibration_data_button; //Control to load the HMD calibration data 
extern GLUI_Button* load_leap_calibration_data_button; //Control to load the LEAP to ART calibration data

enum GLUI_CONTROL{
	WINDOW_X_SPINNER,
	WINDOW_Y_SPINNER,
	WINDOW_W_SPINNER,
	WINDOW_H_SPINNER,
	MENU_STATE_CHECKBOX,
	DEMO1_STATE_CHECKBOX,
	CALIBRATION_STATE_CHECKBOX,
	ALIGNMENT_NUM_SPINNER,
	RENDER_RETICLE_CHECKBOX,
	LOAD_HMD_CALIBRATION_DATA_BUTTON,
	LOAD_LEAP_CALIBRATION_DATA_BUTTON,
};

//////////State Globals//////////
enum STATE{
	STATE_MENU,
	STATE_DEMO1,
	STATE_CALIBRATION,
};

extern STATE cur_state;

struct STATE_MENU_ITEMS
{
	vector<VOBJECT> menu_tiles;
};

extern STATE_MENU_ITEMS state_menu_items;

struct STATE_DEMO1_ITEMS
{
	vector<VOBJECT> target_objects;
	BALL ball;
};

extern STATE_DEMO1_ITEMS state_demo1_items;

struct STATE_CALIBRATION_ITEMS{
	vector<VOBJECT> menu_tiles;
};

extern STATE_CALIBRATION_ITEMS state_calibration_items;

//////////Remaining and Misc Items//////////
extern int selected_tile;

extern gl_Shader hand_shader;

extern glm::mat4 ARTLeapTransform;
extern glm::mat4 LeapARTTransform;
extern glm::mat4 LeapARTOrient;

float CalcDistance(float d1[], float d2[]);

//////////////////////////////////////////////////////////////////////////
#endif //__GLOBALS_H__//