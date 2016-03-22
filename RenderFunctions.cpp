
#include "RenderFunctions.h"

void MainRenderFunction()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	///////////////////////////////////////
	/************Render Left Eye*******************/
	//////clear buffers and load projection matrix//////
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixd(primary_hmd_calibration_left);
	glViewport(0, 0, window_w / 2, window_h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//////Render the Current State Items/////
	RenderState(LEFT);

	///////////////////////////////////////
	/************Render Right Eye*******************/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixd(primary_hmd_calibration_right);
	glViewport(window_w / 2, 0, window_w / 2, window_h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//////Render the Current State Items/////
	RenderState(RIGHT);

	//////////////////////////////////////////////
}

void RenderHands()
{
	if (tracking_left_hand)
	{
		glPushMatrix();
		hand_shader.Activate();
		hand_shader.set_uniform_3f("Color", .8, .2, .4);
		LeapUtilGL::drawSkeletonHand(left_hand);
		hand_shader.DeActivate();
		glPopMatrix();
	}

	if (tracking_right_hand)
	{
		glPushMatrix();
		hand_shader.Activate();
		hand_shader.set_uniform_3f("Color", .4, .2, .8);
		LeapUtilGL::drawSkeletonHand(right_hand);
		hand_shader.DeActivate();
		glPopMatrix();
	}
}

void RenderOcludedHands()
{
	glColorMask(false, false, false, true);
	if (tracking_left_hand)
	{
		glPushMatrix();
		hand_shader.Activate();
		hand_shader.set_uniform_3f("Color", .8, .2, .4);
		LeapUtilGL::OcludedrawSkeletonHand(left_hand);
		hand_shader.DeActivate();
		glPopMatrix();
	}

	if (tracking_right_hand)
	{
		glPushMatrix();
		hand_shader.Activate();
		hand_shader.set_uniform_3f("Color", .4, .2, .8);
		LeapUtilGL::OcludedrawSkeletonHand(right_hand);
		hand_shader.DeActivate();
		glPopMatrix();
	}
	glColorMask(true, true, true, true);
}

void RenderARTMarker()
{	
	Pose origin_pose;
	origin_pose.x = origin_pose.y = origin_pose.z = 0.0;
	Pose calib_pose = PoseMultiplication(origin_pose, art_head_pose);

	glPushMatrix();
	glLoadMatrixd(leap_art_calibration);

	glTranslatef(calib_pose.x, calib_pose.y, calib_pose.z);

	glutSolidSphere(100.0, 25, 25);

	glPopMatrix();
}

void RenderState(EYE_ENUM eye)
{
	switch (cur_state)
	{
		case STATE_MENU:
			RenderMenuState(eye);
			break;

		case STATE_CALIBRATION:
			RenderCalibrationState(eye);
			break;

		case STATE_DEMO1:
			RenderDemo1State(eye);
			break;
	}
}

void RenderMenuState(EYE_ENUM eye)
{
	//////Render the Menu Tiles and Verification Hand//////
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if (eye == LEFT)
	{
		glMultMatrixd(secondary_hmd_calibration_left);
	}
	else
	{
		glMultMatrixd(secondary_hmd_calibration_right);
	}
	glMatrixMode(GL_MODELVIEW);
	
	RenderHands();
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderOcludedHands();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	///////////////////////////////////////////////////
	//////Render Menu Tiles//////
	for (int i = 0; i < state_menu_items.menu_tiles.size(); i++)
	{
		glPushMatrix();
		glLoadMatrixd(leap_art_calibration);
		
		if (state_menu_items.menu_tiles[i].object_model->is_Valid())
		{
			state_menu_items.menu_tiles[i].Render();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void RenderCalibrationState(EYE_ENUM eye)
{
	glPushMatrix();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if (eye == LEFT)
	{
		glMultMatrixd(secondary_hmd_calibration_left);
	}
	else
	{
		glMultMatrixd(secondary_hmd_calibration_right);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//////Render the Calibration reticles and verification hand//////
	glPushMatrix();

	RenderReticle(eye);
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderHands();
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderOcludedHands();
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	///////////////////////////////////////////////////
	//////Render Menu Tiles//////
	if (tracking_left_hand || tracking_right_hand)
	{
		for (int i = 0; i < state_calibration_items.menu_tiles.size(); i++)
		{
			glPushMatrix();
			glLoadMatrixd(leap_art_calibration);

			if (state_calibration_items.menu_tiles[i].object_model->is_Valid())
			{
				state_calibration_items.menu_tiles[i].Render();
			}
			glPopMatrix();
		}
	}

	glPopMatrix();
}

void RenderDemo1State(EYE_ENUM eye)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if (eye == LEFT)
	{
		glMultMatrixd(secondary_hmd_calibration_left);
	}
	else
	{
		glMultMatrixd(secondary_hmd_calibration_right);
	}
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	RenderHands();
	glClear(GL_DEPTH_BUFFER_BIT);
	if ( !state_demo1_items.ball.grabbed )
		RenderOcludedHands();


	//////Render the Ball and Targets//////
	glPushMatrix();
	glLoadMatrixd(leap_art_calibration);

	//Ball//
	state_demo1_items.ball.Render();
	
	for (int i = 0; i < state_demo1_items.target_objects.size(); i++)
	{
		glPushMatrix();
		glLoadMatrixd(leap_art_calibration);

		if (state_demo1_items.target_objects[i].object_model->is_Valid())
		{
			state_demo1_items.target_objects[i].Render(1);
		}
		glPopMatrix();
	}
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


//////////////////////////////////////////////////
void RenderReticle(int eye)
{

	//////////////////////////////////////////////////////////////////////////////
	/////////////////////Cross Reticle//////////////////////
	//Render Left Eye Reticle//
	if (eye == LEFT)
	{
		if ((alignment_num >= 0 && alignment_num < alignment_pairsL.size()))
		{
			glWindowPos2i(alignment_pairsL[alignment_num].screenPoint(0) - ceil(RET_WDTH / 2.0),
				alignment_pairsL[alignment_num].screenPoint(1) - ceil(RET_HGT / 2.0));

			if (tracking && !recording)
				glDrawPixels(RET_WDTH, RET_HGT, GL_RGB, GL_FLOAT, reticle_tracked);
			else if (recording)
				glDrawPixels(RET_WDTH, RET_HGT, GL_RGB, GL_FLOAT, reticle_recording);
			else
				glDrawPixels(RET_WDTH, RET_HGT, GL_RGB, GL_FLOAT, reticle_untracked);
		}
	}

	///////////////////////////////////////////////////////
	//Render Right Eye Reticle//
	if (eye == RIGHT)
	{
		if ((alignment_num >= 0 && alignment_num < alignment_pairsR.size()))
		{
			glWindowPos2i(window_w / 2 + alignment_pairsR[alignment_num].screenPoint(0) - ceil(RET_WDTH / 2.0),
				alignment_pairsR[alignment_num].screenPoint(1) - ceil(RET_HGT / 2.0));
			if (tracking && !recording)
				glDrawPixels(RET_WDTH, RET_HGT, GL_RGB, GL_FLOAT, reticle_tracked);
			else if (recording)
				glDrawPixels(RET_WDTH, RET_HGT, GL_RGB, GL_FLOAT, reticle_recording);
			else
				glDrawPixels(RET_WDTH, RET_HGT, GL_RGB, GL_FLOAT, reticle_untracked);
		}
	}
}
