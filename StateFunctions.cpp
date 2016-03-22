#include "StateFunctions.h"

void BuildStates()
{
	BuildMenuState();

	BuildCalibrationState();

	BuildDemo1State();

	BuildMiscItems();
}

void BuildMenuState()
{
	///Build Calibration Tile
	float s_loc[] = { -150, 0.0, 450.0 };
	double s_dir[] = { 0.0, 0.0, 0.0 };
	
	state_menu_items.menu_tiles.push_back(VOBJECT("Menu_Calib_Tile.obj", s_loc, s_dir, 0.0, 15.0)); 
	if (state_menu_items.menu_tiles.back().object_model->is_Valid())
	{
		state_menu_items.menu_tiles.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
	
	s_loc[0] = 150;
	state_menu_items.menu_tiles.push_back(VOBJECT("Menu_Game_Tile.obj", s_loc, s_dir, 0.0, 15.0));
	if (state_menu_items.menu_tiles.back().object_model->is_Valid())
	{
		state_menu_items.menu_tiles.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
}

///////////////////////////////////////////////
void BuildCalibrationState()
{
	SetupReticles(RET_WDTH, RET_HGT, RET_THICK);
	LoadAlignments("Sets_Mounted\\CalibrationScript.txt");

	///Build Calibration Menu Tiles
	float s_loc[] = { -150, 0.0, 450.0 };
	double s_dir[] = { 0.0, 0.0, 0.0 };
	state_calibration_items.menu_tiles.push_back(VOBJECT("Calib_Save_Tile.obj", s_loc, s_dir, 0.0, 15.0));
	if (state_calibration_items.menu_tiles.back().object_model->is_Valid())
	{
		state_calibration_items.menu_tiles.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}

	s_loc[0] = 150;
	state_calibration_items.menu_tiles.push_back(VOBJECT("Calib_Reset_Tile.obj", s_loc, s_dir, 0.0, 15.0));
	if (state_calibration_items.menu_tiles.back().object_model->is_Valid())
	{
		state_calibration_items.menu_tiles.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
}

void BuildDemo1State()
{
	float ball_color[3] = { 1.0, 0.0, 0.0 };
	float ball_loc[3] = { 0.0, 0.0, 450 };
	state_demo1_items.ball = BALL(35, ball_color, ball_loc);
	state_demo1_items.ball.ball_shader.create_shaders("ball.vert", "ball.frag");
	//////////////

	float s_loc[] = { -250, 1000.0, 450.0 };
	double s_dir[] = { 0.0, 0.0, 0.0 };
	state_demo1_items.target_objects.push_back(VOBJECT("Target.obj", s_loc, s_dir, 0.0, 15.0));
	if (state_demo1_items.target_objects.back().object_model->is_Valid())
	{
		state_demo1_items.target_objects.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
	////////////

	s_loc[0] = 250; s_loc[1] = 500.0; s_loc[2] = 600.0;
	s_dir[0] = 0.0; s_dir[1] = 0.0; s_dir[2] = 0.0;
	state_demo1_items.target_objects.push_back(VOBJECT("Target.obj", s_loc, s_dir, 0.0, 8.0));
	if (state_demo1_items.target_objects.back().object_model->is_Valid())
	{
		state_demo1_items.target_objects.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
	////////////

	s_loc[0] = -250; s_loc[1] = 800.0; s_loc[2] = 250.0;
	s_dir[0] = 1.0; s_dir[1] = 0.0; s_dir[2] = 0.0;
	state_demo1_items.target_objects.push_back(VOBJECT("Target.obj", s_loc, s_dir, 10.0, 8.0));
	if (state_demo1_items.target_objects.back().object_model->is_Valid())
	{
		state_demo1_items.target_objects.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
	////////////

	s_loc[0] = 0; s_loc[1] = 300; s_loc[2] = 200.0;
	s_dir[0] = 0.0; s_dir[1] = 0.0; s_dir[2] = 1.0;
	state_demo1_items.target_objects.push_back(VOBJECT("Target.obj", s_loc, s_dir, 20.0, 8.0));
	if (state_demo1_items.target_objects.back().object_model->is_Valid())
	{
		state_demo1_items.target_objects.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}

	////////////
	s_loc[0] = 150; s_loc[1] = 0; s_loc[2] = 500;
	s_dir[0] = 0.0; s_dir[1] = 1.0; s_dir[2] = 0.0;
	state_demo1_items.target_objects.push_back(VOBJECT("Target.obj", s_loc, s_dir, 80.0, 8.0));
	if (state_demo1_items.target_objects.back().object_model->is_Valid())
	{
		state_demo1_items.target_objects.back().object_model->Load_Uniform(0, "object_shader", "Texture", 0);
	}
}

void BuildMiscItems()
{
	hand_shader.create_shaders("Hands.vert", "Hands.frag");
}

void UpdateState()
{
	switch (cur_state)
	{
		case STATE_MENU:
			menu_state_checkbox_value = 1;
			demo1_state_checkbox_value = 0;
			calibration_state_checkbox_value = 0;
			UpdateMenuState();
			break;

		case STATE_CALIBRATION:
			menu_state_checkbox_value = 0;
			demo1_state_checkbox_value = 0;
			calibration_state_checkbox_value = 1;
			UpdateCalibrationState();
			break;

		case STATE_DEMO1:
			menu_state_checkbox_value = 0;
			demo1_state_checkbox_value = 1;
			calibration_state_checkbox_value = 0;
			UpdateDemo1State();
			break;
	}
}

void UpdateMenuState( int reset )
{
	//Move items and calc position//
	for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
	{
		state_menu_items.menu_tiles[i].CalcMovement();
	}
	///////////////////////////////
	
	//Check if Menu needs to be redisplayed//
	if (reset == 1)
	{
		cout << "Menu State" << endl;
		for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
		{
			state_menu_items.menu_tiles[i].is_fade = 1;
		}
		selected_tile = 0;
		return;
	}
	else
	{
		int zerocount = 0;
		for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
		{
			if (state_menu_items.menu_tiles[i].alpha == 0)
				zerocount += 1;
		}

		if (zerocount == state_menu_items.menu_tiles.size())
		{
			switch (selected_tile)
			{
				case 1:
					cur_state = STATE_CALIBRATION;
					UpdateCalibrationState(1);
					break;
				case 2:
					cur_state = STATE_DEMO1;
					UpdateDemo1State(1);
					break;
			}
			return;
		}
	}
	////////////////////////////////////

	//Check if already in transition//
	for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
	{
		if (state_menu_items.menu_tiles[i].is_fade != 0)
			return;
	}
	///////////////////////////////////

	//Check for Selection of Menu Items//
	if ( tracking_left_hand)
	{
		float handloc[3]; handloc[0] = left_hand.palmPosition()[0]; handloc[1] = left_hand.palmPosition()[1]; handloc[2] = left_hand.palmPosition()[2];
		for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
		{
			//Transform Tile into Leap Coordinate Frame//
			glm::vec4 myVector = glm::vec4(state_menu_items.menu_tiles[i].object_pose.x, state_menu_items.menu_tiles[i].object_pose.y,
				state_menu_items.menu_tiles[i].object_pose.z, 1.0);
			glm::vec4 transformedVector = ARTLeapTransform * myVector;

			//Get Tile Bounds//
			float y = transformedVector[1];
			float xn = transformedVector[0] - 5.0*state_menu_items.menu_tiles[i].scale;
			float xp = transformedVector[0] + 5.0*state_menu_items.menu_tiles[i].scale;
			float zn = transformedVector[2] - 5.0*state_menu_items.menu_tiles[i].scale;
			float zp = transformedVector[2] + 5.0*state_menu_items.menu_tiles[i].scale;

			if (IntersectPointPlane(handloc, y, xn, xp, zn, zp))
			{
				for (int j = 0; j < state_menu_items.menu_tiles.size(); j++)
				{
					state_menu_items.menu_tiles[j].is_fade = 2;
				}
				selected_tile = i + 1;
				return;
			}
		}
	}
	//////////////////////////////////////////

	if (tracking_right_hand)
	{
		float handloc[3]; handloc[0] = right_hand.palmPosition()[0]; handloc[1] = right_hand.palmPosition()[1]; handloc[2] = right_hand.palmPosition()[2];
		for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
		{
			//Transform Tile into Leap Coordinate Frame//
			glm::vec4 myVector = glm::vec4(state_menu_items.menu_tiles[i].object_pose.x, state_menu_items.menu_tiles[i].object_pose.y,
				state_menu_items.menu_tiles[i].object_pose.z, 1.0);
			glm::vec4 transformedVector = ARTLeapTransform * myVector;

			//Get Tile Bounds//
			float y = transformedVector[1];
			float xn = transformedVector[0] - 5.0*state_menu_items.menu_tiles[i].scale;
			float xp = transformedVector[0] + 5.0*state_menu_items.menu_tiles[i].scale;
			float zn = transformedVector[2] - 5.0*state_menu_items.menu_tiles[i].scale;
			float zp = transformedVector[2] + 5.0*state_menu_items.menu_tiles[i].scale;

			if (IntersectPointPlane(handloc, y, xn, xp, zn, zp))
			{
				for (int j = 0; j < state_menu_items.menu_tiles.size(); j++)
				{
					state_menu_items.menu_tiles[j].is_fade = 2;
				}
				selected_tile = i + 1;
				return;
			}
		}
	}
	
}

//////////////////////////////////////////////
void UpdateCalibrationState(int reset)
{
	//Move items and calc position//
	for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
	{
		state_calibration_items.menu_tiles[i].CalcMovement();
	}
	///////////////////////////////

	if (reset == 1)
	{
		cout << "Calibration State" << endl;
		for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
		{
			state_calibration_items.menu_tiles[i].is_fade = 0;
			state_calibration_items.menu_tiles[i].alpha = 1.0;
		}
		selected_tile = 0;

		alignment_num = 0;
		saved_pair.clear();
		for (int i = 0; i < alignment_pairsL.size() || i < alignment_pairsR.size(); i++)
		{
			saved_pair.push_back(false);
		}
		spaam_result_left.clearMatrix();
		spaam_result_left.corr_points.clear();
		spaam_result_right.clearMatrix();
		spaam_result_right.corr_points.clear();

		memcpy(secondary_hmd_calibration_left, primary_hmd_calibration_left, sizeof(primary_hmd_calibration_left));
		memcpy(secondary_hmd_calibration_right, primary_hmd_calibration_right, sizeof(primary_hmd_calibration_right));

		glutTimerFunc(100, TrackToolTimer, 0);
	}
	else
	{
		int zerocount = 0;
		for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
		{
			if (state_calibration_items.menu_tiles[i].alpha == 0)
				zerocount += 1;
		}

		if (zerocount == state_calibration_items.menu_tiles.size())
		{
			//memcpy(secondary_hmd_calibration_left, spaam_result_left.projMat3x4, sizeof(secondary_hmd_calibration_left));
			//memcpy(secondary_hmd_calibration_right, spaam_result_right.projMat3x4, sizeof(secondary_hmd_calibration_right));

			cur_state = STATE_MENU;
			selected_tile = 0;
			UpdateMenuState(1);
			return;
		}
		else if ( selected_tile == 2 )
		{
			//Reset Calibration//
			alignment_num = 0;
			saved_pair.clear();
			for (int i = 0; i < alignment_pairsL.size() || i < alignment_pairsR.size(); i++)
			{
				saved_pair.push_back(false);
			}
			spaam_result_left.clearMatrix();
			spaam_result_left.corr_points.clear();
			spaam_result_right.clearMatrix();
			spaam_result_right.corr_points.clear();
			
			memcpy(secondary_hmd_calibration_left, primary_hmd_calibration_left, sizeof(primary_hmd_calibration_left));
			memcpy(secondary_hmd_calibration_right, primary_hmd_calibration_right, sizeof(primary_hmd_calibration_right));
			selected_tile = 0;
			return;
		}
	}

	////////////////////////////////////

	//Check if already in transition//
	for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
	{
		if (state_calibration_items.menu_tiles[i].is_fade != 0)
			return;
	}
	///////////////////////////////////

	tracking = listener.isTrackingTool();
	if (!tracking)
	{
		memset(tool_pos, 0, sizeof(tool_pos));
		tool_pos_pass = 0;
	}

	if (alignment_num >= alignment_pairsL.size() || alignment_num >= alignment_pairsR.size())
	{
		alignment_num = 0;
	}

	///////////////////////////////////
	//Check for Selection of Menu Items//
	if (tracking_left_hand)
	{
		float handloc[3]; handloc[0] = left_hand.palmPosition()[0]; handloc[1] = left_hand.palmPosition()[1]; handloc[2] = left_hand.palmPosition()[2];
		for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
		{
			//Transform Tile into Leap Coordinate Frame//
			glm::vec4 myVector = glm::vec4(state_calibration_items.menu_tiles[i].object_pose.x, state_calibration_items.menu_tiles[i].object_pose.y,
				state_calibration_items.menu_tiles[i].object_pose.z, 1.0);
			glm::vec4 transformedVector = ARTLeapTransform * myVector;

			//Get Tile Bounds//
			float y = transformedVector[1];
			float xn = transformedVector[0] - 5.0*state_calibration_items.menu_tiles[i].scale;
			float xp = transformedVector[0] + 5.0*state_calibration_items.menu_tiles[i].scale;
			float zn = transformedVector[2] - 5.0*state_calibration_items.menu_tiles[i].scale;
			float zp = transformedVector[2] + 5.0*state_calibration_items.menu_tiles[i].scale;

			if (IntersectPointPlane(handloc, y, xn, xp, zn, zp))
			{
				if (i == 0)
					for (int j = 0; j < state_calibration_items.menu_tiles.size(); j++)
					{
						state_calibration_items.menu_tiles[j].is_fade = 2;
					}
				selected_tile = i + 1;
				return;
			}
		}
	}
	//////////////////////////////////////////

	if (tracking_right_hand)
	{
		float handloc[3]; handloc[0] = right_hand.palmPosition()[0]; handloc[1] = right_hand.palmPosition()[1]; handloc[2] = right_hand.palmPosition()[2];
		for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
		{
			//Transform Tile into Leap Coordinate Frame//
			glm::vec4 myVector = glm::vec4(state_calibration_items.menu_tiles[i].object_pose.x, state_calibration_items.menu_tiles[i].object_pose.y,
				state_calibration_items.menu_tiles[i].object_pose.z, 1.0);
			glm::vec4 transformedVector = ARTLeapTransform * myVector;

			//Get Tile Bounds//
			float y = transformedVector[1];
			float xn = transformedVector[0] - 5.0*state_calibration_items.menu_tiles[i].scale;
			float xp = transformedVector[0] + 5.0*state_calibration_items.menu_tiles[i].scale;
			float zn = transformedVector[2] - 5.0*state_calibration_items.menu_tiles[i].scale;
			float zp = transformedVector[2] + 5.0*state_calibration_items.menu_tiles[i].scale;

			if (IntersectPointPlane(handloc, y, xn, xp, zn, zp))
			{
				if ( i == 0 )
					for (int j = 0; j < state_calibration_items.menu_tiles.size(); j++)
					{
						state_calibration_items.menu_tiles[j].is_fade = 2;
					}
				selected_tile = i + 1;
				return;
			}
		}
	}
}

void UpdateDemo1State(int reset)
{
	//check for reset//
	if (reset == 1)
	{
		cout << "Demo 1 state" << endl;
		state_demo1_items.ball.Reset();
		state_demo1_items.ball.alpha = .0125;
		state_demo1_items.ball.is_fade = 1;
		for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
		{
			state_demo1_items.target_objects[i].alpha = 0.125;
			state_demo1_items.target_objects[i].is_fade = 1;
		}
		hand_velocity.clear();
		hand_velocity_iterator = 0;
		return;
	}

	///////////////////////////////////////////////
	//Check for Menu Reset//
	if (tracking_left_hand && tracking_right_hand)
	{
		state_demo1_items.ball.is_fade = 2;
		for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
		{
			state_demo1_items.target_objects[i].is_fade = 2;
		}
	
		int zerocount = 0;
		if (state_demo1_items.ball.alpha == 0)
			zerocount += 1;
		for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
		{
			if (state_demo1_items.target_objects[i].alpha == 0)
				zerocount += 1;
		}

		if (zerocount == state_demo1_items.target_objects.size() + 1)
		{
			cur_state = STATE_MENU;
			UpdateMenuState(1);
		}
		return;
	}

	/////////////////////////////////////////////////
	//Check for item reset//
	if (state_demo1_items.ball.alpha == 0.0)
	{
		state_demo1_items.ball.Reset();
		state_demo1_items.ball.is_fade = 1;
		hand_velocity_iterator = 0;
		hand_velocity.clear();
	}
	for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
	{
		if (state_demo1_items.target_objects[i].alpha == 0.0)
		{
			state_demo1_items.target_objects[i].ResetLocation();
			state_demo1_items.target_objects[i].is_fade = 1;
		}
	}

	////////////////////////////////////////////////
	//Move items and calc position//
	state_demo1_items.ball.CalcMovement();
	for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
	{
		state_demo1_items.target_objects[i].CalcMovement();
	}
	
	///////////////////////////////////////////////
	//Perform Ball Interctions//
	if (state_demo1_items.ball.is_fade == 0)
	{
		//cout << left_hand.grabStrength() << endl;
		//////////////////////////////////////////////////
		//Check for Hand Grasp//
		if (state_demo1_items.ball.grabbed)
		{
			glm::vec4 transformedVector;
			switch (state_demo1_items.ball.grabbed_hand)
			{
				//left hand//
				case 1:
					if (tracking_left_hand && left_hand.grabStrength() > 0.8)
					{
						glm::vec4 HandVector = glm::vec4(left_hand.palmPosition()[0], left_hand.palmPosition()[1], left_hand.palmPosition()[2], 1.0);
						//calculate hand location in art//
						transformedVector = LeapARTTransform * HandVector;
						Pose hand_pose; hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
						hand_pose = PoseMultiplication(hand_pose, InversePose(art_head_pose));
						float newloc[] = { hand_pose.x, hand_pose.y, hand_pose.z };
						state_demo1_items.ball.SetLocation(newloc);
						
						glm::mat4 artOrient = glm::mat4_cast(InversePose(art_head_pose).rotations)*LeapARTOrient;
						glm::vec4 VelocityVector = glm::vec4(left_hand.palmVelocity()[0], left_hand.palmVelocity()[1], left_hand.palmVelocity()[2], 1.0);
						transformedVector = artOrient* VelocityVector;//LeapARTOrient* VelocityVector;
						//transformedVector = artOrient*transformedVector;
						hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
						float newvel[] = { hand_pose.x, hand_pose.y, hand_pose.z };

						//add velocity vector//
						if (hand_velocity.size() < hand_velocity_records)
							hand_velocity.push_back(make_tuple(newvel[0], newvel[1], newvel[2]));
						else
						{
							hand_velocity[hand_velocity_iterator] = make_tuple(newvel[0], newvel[1], newvel[2]);
							hand_velocity_iterator = hand_velocity_iterator == hand_velocity_records - 1 ? 0 : hand_velocity_iterator + 1;
						}
						return;
					}
					else //ball released//
					{
						state_demo1_items.ball.grabbed = false;
						state_demo1_items.ball.grabbed_hand = 0;
						state_demo1_items.ball.gravity_enabled = true;

						//calc and set velocity//
						tuple<float, float, float> sum_tuple = make_tuple(0.0f, 0.0f, 0.0f);
						for (int i = 0; i < hand_velocity.size(); i++)
						{
							sum_tuple = make_tuple(std::get<0>(sum_tuple) + std::get<0>(hand_velocity[i]),
													std::get<1>(sum_tuple) + std::get<1>(hand_velocity[i]),
													std::get<2>(sum_tuple) + std::get<2>(hand_velocity[i]));
						}
						double newvel[] = { std::get<0>(sum_tuple) / hand_velocity.size(), std::get<1>(sum_tuple) / hand_velocity.size(), std::get<2>(sum_tuple) / hand_velocity.size() };
						state_demo1_items.ball.SetVelocity(newvel);

						//reset velocity//
						hand_velocity_iterator = 0;
						hand_velocity.clear();
						return;
					}
				break;

				//right hand//
				case 2:
					if (tracking_right_hand && right_hand.grabStrength() > 0.8)
					{
						glm::vec4 HandVector = glm::vec4(right_hand.palmPosition()[0], right_hand.palmPosition()[1], right_hand.palmPosition()[2], 1.0);
						//calculate hand location in art//
						transformedVector = LeapARTTransform * HandVector;
						Pose hand_pose; hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
						hand_pose = PoseMultiplication(hand_pose, InversePose(art_head_pose));
						float newloc[] = { hand_pose.x, hand_pose.y, hand_pose.z };
						state_demo1_items.ball.SetLocation(newloc);

						glm::mat4 artOrient = glm::mat4_cast(InversePose(art_head_pose).rotations)*LeapARTOrient;
						glm::vec4 VelocityVector = glm::vec4(right_hand.palmVelocity()[0], right_hand.palmVelocity()[1], right_hand.palmVelocity()[2], 1.0);
						transformedVector = artOrient* VelocityVector;//LeapARTOrient* VelocityVector;
						hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
						float newvel[] = { hand_pose.x, hand_pose.y, hand_pose.z };

						//add velocity vector//
						if (hand_velocity.size() < hand_velocity_records)
							hand_velocity.push_back(make_tuple(newvel[0], newvel[1], newvel[2]));
						else
						{
							hand_velocity[hand_velocity_iterator] = make_tuple(newvel[0], newvel[1], newvel[2]);
							hand_velocity_iterator = hand_velocity_iterator == hand_velocity_records - 1 ? 0 : hand_velocity_iterator + 1;
						}
						return;
					}
					else //ball released//
					{
						state_demo1_items.ball.grabbed = false;
						state_demo1_items.ball.grabbed_hand = 0;
						state_demo1_items.ball.gravity_enabled = true;

						//calc and set velocity//
						tuple<float, float, float> sum_tuple = make_tuple(0.0f, 0.0f, 0.0f);
						for (int i = 0; i < hand_velocity.size(); i++)
						{
							sum_tuple = make_tuple(std::get<0>(sum_tuple) + std::get<0>(hand_velocity[i]),
								std::get<1>(sum_tuple) + std::get<1>(hand_velocity[i]),
								std::get<2>(sum_tuple) + std::get<2>(hand_velocity[i]));
						}
						double newvel[] = { std::get<0>(sum_tuple) / hand_velocity.size(), std::get<1>(sum_tuple) / hand_velocity.size(), std::get<2>(sum_tuple) / hand_velocity.size() };
						state_demo1_items.ball.SetVelocity(newvel);
						//reset velocity//
						hand_velocity_iterator = 0;
						hand_velocity.clear();
						return;
					}
				break;

				//hand not specified, reset ball//
			default:
				state_demo1_items.ball.Reset();
				hand_velocity_iterator = 0;
				hand_velocity.clear();
				break;
			}
		}
		///////////////////////////////////////////////////////
		else //Check for Grab on ball//
		{
			glm::vec4 transformedVector;
			float dist1[3] = { state_demo1_items.ball.location[0], state_demo1_items.ball.location[1], state_demo1_items.ball.location[2] };
			//check left hand//
			if (tracking_left_hand && left_hand.grabStrength() > 0.8)
			{
				//calculate hand location in art//
				glm::vec4 HandVector = glm::vec4(left_hand.palmPosition()[0], left_hand.palmPosition()[1], left_hand.palmPosition()[2], 1.0);
				transformedVector = LeapARTTransform * HandVector;
				Pose hand_pose; hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
				hand_pose = PoseMultiplication(hand_pose, InversePose(art_head_pose));
				float dist2[3] = { hand_pose.x, hand_pose.y, hand_pose.z };
				if (CalcDistance(dist1, dist2) <= state_demo1_items.ball.radius + 20)
				{
					state_demo1_items.ball.grabbed = true;
					state_demo1_items.ball.grabbed_hand = 1;
					state_demo1_items.ball.SetLocation(dist2);
					
					glm::mat4 artOrient = glm::mat4_cast(art_head_pose.rotations)*LeapARTOrient;
					glm::vec4 VelocityVector = glm::vec4(left_hand.palmVelocity()[0], left_hand.palmVelocity()[1], left_hand.palmVelocity()[2], 1.0);
					transformedVector = artOrient* VelocityVector;//LeapARTOrient* VelocityVector;
					//transformedVector = artOrient*transformedVector;
					hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
					double newvel[] = { hand_pose.x, hand_pose.y, hand_pose.z };
					//state_demo1_items.ball.SetVelocity(newvel);
					return;
				}
			}

			if (tracking_right_hand && right_hand.grabStrength() > 0.8)
			{
				//calculate hand location in art//
				glm::vec4 HandVector = glm::vec4(right_hand.palmPosition()[0], right_hand.palmPosition()[1], right_hand.palmPosition()[2], 1.0);
				transformedVector = LeapARTTransform * HandVector;
				Pose hand_pose; hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
				hand_pose = PoseMultiplication(hand_pose, InversePose(art_head_pose));
				float dist2[3] = { hand_pose.x, hand_pose.y, hand_pose.z };
				if (CalcDistance(dist1, dist2) <= state_demo1_items.ball.radius + 20)
				{
					state_demo1_items.ball.grabbed = true;
					state_demo1_items.ball.grabbed_hand = 2;
					state_demo1_items.ball.SetLocation(dist2);

					glm::mat4 artOrient = glm::mat4_cast(art_head_pose.rotations)*LeapARTOrient;
					glm::vec4 VelocityVector = glm::vec4(right_hand.palmVelocity()[0], right_hand.palmVelocity()[1], right_hand.palmVelocity()[2], 1.0);
					transformedVector = artOrient* VelocityVector;//LeapARTOrient* VelocityVector;
					//transformedVector = artOrient*transformedVector;
					hand_pose.x = transformedVector[0]; hand_pose.y = transformedVector[1]; hand_pose.z = transformedVector[2];
					double newvel[] = { hand_pose.x, hand_pose.y, hand_pose.z };
					//state_demo1_items.ball.SetVelocity(newvel);
					return;
				}
			}
		}

		/////////////////////////////////////////////////////
		//Check for Ball reset//
		if (state_demo1_items.ball.location[2] < -1.0 ||
			!(state_demo1_items.ball.location[0] < 1000.0 && state_demo1_items.ball.location[0] > -1000.0) ||
			!(state_demo1_items.ball.location[1] < 1500.0 && state_demo1_items.ball.location[1] > -1000.0) )
		{
			state_demo1_items.ball.is_fade = 2;
			return;
		}

		//Check for target reset//
		for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
		{
			if (!(state_demo1_items.target_objects[i].location[2] > -1.0 && state_demo1_items.target_objects[i].location[2] < 1000.0) ||
				!(state_demo1_items.target_objects[i].location[0] < 500.0 && state_demo1_items.target_objects[i].location[0] > -5000.0) ||
				!(state_demo1_items.target_objects[i].location[1] < 1500.0 && state_demo1_items.target_objects[i].location[1] > -1000.0))
			{
				state_demo1_items.target_objects[i].is_fade = 2;
				return;
			}
		}
		/////////////////////////////////////////////////////
		//Check for Ball Collision//
		for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
		{
			float ballloc[3] = { state_demo1_items.ball.location[0], state_demo1_items.ball.location[1], state_demo1_items.ball.location[2] };
			float targetloc[3] = { state_demo1_items.target_objects[i].location[0], state_demo1_items.target_objects[i].location[1], 
				state_demo1_items.target_objects[i].location[2] };

			if (CalcDistance(ballloc, targetloc) <= state_demo1_items.target_objects[i].scale * 5.0 + 20.0)
			{
				state_demo1_items.target_objects[i].is_fade = 2;
				state_demo1_items.ball.is_fade = 2;
			}
		}
	}
}
