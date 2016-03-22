#include "ARTFunctions.h"

void startDtrack(int port)
{
	// init library:
	if (dt == NULL)
	{
		dt = new DTrackSDK(port);
		cout << "DTrack Initialized " << endl;
	}

	if (!dt->isLocalDataPortValid()){
		cout << "DTrack init error" << endl;
		dt = NULL;
	}
}

void stopDtrack()
{
	if (dt != NULL)
		delete dt;
}

void UpdateDtrack()
{
	//////////process dtrack//////////
	DTrack_Body_Type_d body;
	if (dt == NULL || !dt->receive())
		return;

	if (dt->getNumBody() == 1){
		body = *dt->getBody(0);
		if (body.quality > 0){
			art_head_pose.x = body.loc[0];
			art_head_pose.y = body.loc[1];
			art_head_pose.z = body.loc[2];
			double rotmat[9];
			for (int i = 0; i < 9; i++)
			{
				rotmat[i] = body.rot[i];
			}
			double* quatmat = calcQuat(rotmat);
			art_head_pose.rotations.x = quatmat[0];
			art_head_pose.rotations.y = quatmat[1];
			art_head_pose.rotations.z = quatmat[2];
			art_head_pose.rotations.w = quatmat[3];

			art_head_pose = InversePose(art_head_pose);
		}
	}
}