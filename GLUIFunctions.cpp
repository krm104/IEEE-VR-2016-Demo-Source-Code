#include "GLUIFunctions.h"

void BuildGLUI()
{
	printf("GLUI version: %3.2f\n", GLUI_Master.get_version());
	glui = GLUI_Master.create_glui("VR 2016 Demo Controls", 0, 600, 300);
	new GLUI_StaticText(glui, "Demo Controls");

	/////////////////////////////////////
	//Add Window Control Panel//
	window_panel = new GLUI_Panel(glui, "Window Panel");
	window_x_spinner = new GLUI_Spinner(window_panel, "Window X", &window_x, WINDOW_X_SPINNER, control_cb);
	window_x_spinner->set_int_limits(-3872, 3872);
	window_x_spinner->set_int_val(window_x);
	window_w_spinner = new GLUI_Spinner(window_panel, "Window W", &window_w, WINDOW_W_SPINNER, control_cb);
	window_w_spinner->set_int_limits(0, 3872);
	window_w_spinner->set_int_val(window_w);

	new GLUI_Column(window_panel, false);
	window_y_spinner = new GLUI_Spinner(window_panel, "Window Y", &window_y, WINDOW_Y_SPINNER, control_cb);
	window_y_spinner->set_int_limits(-2592, 2592);
	window_y_spinner->set_int_val(window_y);
	window_h_spinner = new GLUI_Spinner(window_panel, "Window H", &window_h, WINDOW_H_SPINNER, control_cb);
	window_h_spinner->set_int_limits(0, 2592);
	window_h_spinner->set_int_val(window_h);

	//////////////////////////////////////
	//Add Alignment Control Panel//
	calibration_panel = new GLUI_Panel(glui, "State Panel");

	state_checkbox_panel = new GLUI_Panel(calibration_panel, "State");
	menu_state_checkbox = new GLUI_Checkbox(state_checkbox_panel, "Menu", &menu_state_checkbox_value, MENU_STATE_CHECKBOX, control_cb);
	new GLUI_Column(state_checkbox_panel);
	demo1_state_checkbox = new GLUI_Checkbox(state_checkbox_panel, "Target", &demo1_state_checkbox_value, DEMO1_STATE_CHECKBOX, control_cb);
	new GLUI_Column(state_checkbox_panel);
	calibration_state_checkbox = new GLUI_Checkbox(state_checkbox_panel, "Calibration", &calibration_state_checkbox_value, CALIBRATION_STATE_CHECKBOX, control_cb);

	alignment_num_spinner = new GLUI_Spinner(calibration_panel, "Alignment Number", &alignment_num, ALIGNMENT_NUM_SPINNER, control_cb);
	alignment_num_spinner->set_int_limits(0, 0);

	alignment_left_num_spinner = new GLUI_Spinner(calibration_panel, "Number Left Alignments");//, &alignment_left_num);
	alignment_left_num_spinner->set_int_val(0);
	alignment_left_num_spinner->disable();

	alignment_right_num_spinner = new GLUI_Spinner(calibration_panel, "Number Right Alignments");//, &alignment_right_num);
	alignment_right_num_spinner->set_int_val(0);
	alignment_right_num_spinner->disable();

	render_reticle_checkbox = new GLUI_Checkbox(calibration_panel, "Render Reticle", &render_reticle);
	
	load_hmd_calibration_data_button = new GLUI_Button(calibration_panel, "Load HMD Calibration Data", LOAD_HMD_CALIBRATION_DATA_BUTTON, control_cb);
	load_leap_calibration_data_button = new GLUI_Button(calibration_panel, "Load LEAP Calibration Data", LOAD_LEAP_CALIBRATION_DATA_BUTTON, control_cb);

	glui->set_main_gfx_window(WindowHandle);
	
	control_cb(WINDOW_X_SPINNER);
	control_cb(WINDOW_W_SPINNER);
}

void control_cb(int control_id)
{
	switch (control_id)
	{
		case WINDOW_X_SPINNER:
			glutPositionWindow(window_x, window_y);
			break;
		case WINDOW_Y_SPINNER:
			glutPositionWindow(window_x, window_y);
			break;
		case WINDOW_W_SPINNER:
			glutReshapeWindow(window_w, window_h);
			break;
		case WINDOW_H_SPINNER:
			glutReshapeWindow(window_w, window_h);
			break;
		case MENU_STATE_CHECKBOX:
			if (demo1_state_checkbox->get_int_val() || calibration_state_checkbox->get_int_val())
			{
				demo1_state_checkbox->set_int_val(0);
				calibration_state_checkbox->set_int_val(0);
				cur_state = STATE_MENU;
				UpdateMenuState(1);
			}
			else
			{
				menu_state_checkbox->set_int_val(1);
				cur_state = STATE_MENU;
			}
			break;
		case DEMO1_STATE_CHECKBOX:
			if (menu_state_checkbox->get_int_val() || calibration_state_checkbox->get_int_val())
			{
				menu_state_checkbox->set_int_val(0);
				calibration_state_checkbox->set_int_val(0);
				cur_state = STATE_DEMO1;
				UpdateDemo1State(1);
			}
			else
			{
				demo1_state_checkbox->set_int_val(1);
				cur_state = STATE_DEMO1;
			}
			break;
		case CALIBRATION_STATE_CHECKBOX:
			if (demo1_state_checkbox->get_int_val() || menu_state_checkbox->get_int_val())
			{
				demo1_state_checkbox->set_int_val(0);
				menu_state_checkbox->set_int_val(0);
				cur_state = STATE_CALIBRATION;
			}
			else
			{
				calibration_state_checkbox->set_int_val(1);
				cur_state = STATE_CALIBRATION;
			}
			break;
		case LOAD_HMD_CALIBRATION_DATA_BUTTON:
			if (!LoadHMDCalibration())
				printf("Error Loading HMD Calibration Files");
			break;
		case LOAD_LEAP_CALIBRATION_DATA_BUTTON:
			if ( !LoadLeapCalibration())
				printf("Error Loading Leap Calibration Files");
			break;
	}
}
