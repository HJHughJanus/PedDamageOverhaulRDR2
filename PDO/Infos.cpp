//AI::SET_ANIM_RATE(peds[i], 0.1, 0, true);
							//PED::SET_PED_MOVE_RATE_OVERRIDE(peds[i], 0.1);
							//if (PED::IS_PED_SHOOTING(peds[i]))
							//{
							//	if (painrand < 20) PED::SET_PED_TO_RAGDOLL(peds[i], 1, 2, 4, false, false, false);
							//	PED::SET_PED_TO_RAGDOLL(peds[i], 1, 10, 4, false, false, false);
							//	WEAPON::REMOVE_ALL_PED_WEAPONS(peds[i], false, false);
							//}

//if (AI::IS_PED_IN_WRITHE(peds[i])) ENTITY::SET_ENTITY_INVINCIBLE(peds[i], true);ENTITY::GET_ENTITY_HEALTH(peds[i]) < painthreshold

							//AI::SET_ANIM_RATE(peds[i], 0.1, 0, false);
							//PED::SET_PED_MOVE_RATE_OVERRIDE(peds[i], 0.1);

//PED::SET_PED_TO_RAGDOLL(peds[i], 1, 1, 3, false, false, false);
							//PED::SET_PED_TO_RAGDOLL_WITH_FALL(peds[i], 1, 1, 3, 10.0, 10.0, 10.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
							//PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);

//if (PED::IS_PED_HUMAN(peds[i]) && (ENTITY::GET_ENTITY_HEALTH(peds[i]) < painthreshold)) 
					//{
					//	int rand = 0 + (std::rand() % (12 - 0 + 1));
					//	AUDIO::PLAY_PAIN(peds[i], rand, 50.0, false, false);
					//	if (PED::IS_PED_INJURED(peds[i]))
					//	{
					//		if (AI::IS_PED_RUNNING(peds[i]) || PED:IS_PED_FLEEING(peds[i])) PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
					//		AI::SET_PED_PATH_CAN_USE_CLIMBOVERS(peds[i], false);
					//		AI::SET_PED_PATH_CAN_USE_LADDERS(peds[i], false);
					//		AI::SET_PED_PATH_CAN_DROP_FROM_HEIGHT(peds[i], false);
					//	}
					//	if (AI::IS_PED_IN_WRITHE(peds[i])) ENTITY::SET_ENTITY_HEALTH(peds[i], writhehealth, 0);
					//}

//Vector3 vec = ENTITY::GET_ENTITY_FORWARD_VECTOR(peds[i]);
								//float x = vec.x;
								//float y = vec.y;
								//float z = vec.z;
constexpr int toggleKey2{ VK_F9 };
bool modState{ false };
int painn = 1;
char c[40];
std::string text = "Painaudio = ";
if (IsKeyJustUp(toggleKey2))
{
	painn += 1;
	if (painn > 12) painn = 1;
	text = "Painaudio = " + std::to_string(painn);
	strcpy(c, text.c_str());
	modState = !modState;
	msgTime = GetTickCount() + 3000;
}
//if (modState) DrawText(0.4, 0.4, c);
//else DrawText(0.4, 0.4, c);

______________________playerhealthset
#include "script.h"
#include "scriptmenu.h"
#include "keyboard.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <fstream>

using namespace std;

void DrawText(float x, float y, char* str)
{
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

bool does_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void main()
{
	bool modScriptState{ false };
	constexpr int toggleKey{ VK_F7 };
	int msgTime{ 0 };

	bool iniexists = false;
	iniexists = does_file_exist(".\\SetPlayerHealth.ini");

	int pedhealth = GetPrivateProfileInt("PlayerHealthConfig", "PedHealth", 100, ".\\SetPlayerHealth.ini");

	while (true)
	{
		if (IsKeyJustUp(toggleKey))
		{
			modScriptState = !modScriptState;
			msgTime = GetTickCount() + 3000;
		}


		if (modScriptState)
		{
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();

			if (PED::GET_PED_MAX_HEALTH(playerPed) != pedhealth)
			{
				PED::SET_PED_MAX_HEALTH(playerPed, pedhealth);
				ENTITY::SET_ENTITY_HEALTH(playerPed, PED::GET_PED_MAX_HEALTH(playerPed), 0);
			}
		}

		if (GetTickCount() < msgTime)
		{
			if (modScriptState)
			{
				DrawText(0.4, 0.4, "SetPlayerHealth has been ENABLED.");
				if (iniexists) DrawText(0.6, 0.6, "ini file found.");
				else DrawText(0.6, 0.6, "ini file not found.");
			}
			else DrawText(0.4, 0.4, "SetPlayerHealth has been DISABLED.");
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
_________________________________________________________________________


________________lasso disarm (into modscriptstate bool)________________
Hash lassoHash = GAMEPLAY::GET_HASH_KEY("WEAPON_LASSO");
___________________lasso disarm (into i-loop)_______________________________
if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, lassoHash, 0, 0))
{
	Hash lassoHash2;
	if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &lassoHash2, false, 0, false))
	{
		if (lassoHash == lassoHash2)
		{
			if (lastTarget != playerPed)
			{
				WEAPON::REMOVE_ALL_PED_WEAPONS(lastTarget, true, true);
			}
			if (PED::IS_PED_SHOOTING(playerPed))
			{
				Entity playerTarget;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
				{
					lastTarget = playerTarget;
				}
			}
		}
	}
}
______________________________________________________________________________

RANDOM DYING MOVEMENT
______________________________________________________________________________
if ((rando >= ini_dyingmovementchanceleg01 && rando <= ini_dyingmovementchanceleg02) ||
(rando >= ini_dyingmovementchancelegarm01 && rando <= ini_dyingmovementchancelegarm02) ||
(rando >= ini_dyingmovementchancelegspine01 && rando <= ini_dyingmovementchancelegspine02) ||
(rando >= ini_dyingmovementchanceall01 && rando <= ini_dyingmovementchanceall02)) //legs
{
	if (rando3 < 33) //right calf
	{
		if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrellegx, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegx, 0, 0);
		else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyleg, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrellegy, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegy, 0, 0);
		else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrellegz, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, ini_dyingmovementpushyleg, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
	}
	//else if (rando3 < 50) //right thigh
	//{
	//	if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 6884), ini_dyingmovementdirrellegx, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegx, 0, 0);
	//	else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyleg, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 6884), ini_dyingmovementdirrellegy, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegy, 0, 0);
	//	else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 6884), ini_dyingmovementdirrellegz, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegz, 0, 0);
	//	else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, ini_dyingmovementpushyleg, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 6884), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
	//}
	else if (rando3 < 66) //left calf
	{
		if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrellegx, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegx, 0, 0);
		else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyleg, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrellegy, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegy, 0, 0);
		else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrellegz, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, ini_dyingmovementpushyleg, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
	}
	else //left and right calf
	{
		if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrellegx, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegx, 0, 0);
		else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyleg, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrellegy, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegy, 0, 0);
		else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrellegz, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, ini_dyingmovementpushyleg, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 55120), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);

		if (rando4 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrellegx, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegx, 0, 0);
		else if (rando4 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyleg, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrellegy, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegy, 0, 0);
		else if (rando4 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrellegz, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, ini_dyingmovementpushyleg, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 43312), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
	}
	//else //left thigh
	//{
	//	if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 65478), ini_dyingmovementdirrellegx, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegx, 0, 0);
	//	else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyleg, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 65478), ini_dyingmovementdirrellegy, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegy, 0, 0);
	//	else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 65478), ini_dyingmovementdirrellegz, ini_dyingmovementignoreupvec, ini_dyingmovementforrellegz, 0, 0);
	//	else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg, ini_dyingmovementpushyleg, ini_dyingmovementpushzleg, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 65478), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
	//}
}
if ((rando >= ini_dyingmovementchancearm01 && rando <= ini_dyingmovementchancearm02) ||
(rando >= ini_dyingmovementchancelegarm01 && rando <= ini_dyingmovementchancelegarm02) ||
(rando >= ini_dyingmovementchancearmspine01 && rando <= ini_dyingmovementchancearmspine02) ||
(rando >= ini_dyingmovementchanceall01 && rando <= ini_dyingmovementchanceall02)) //arms
{
	if (rando3 < 33) //left forearm
	{
		if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarmx, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmx, 0, 0);
		else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyarm, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarmy, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmy, 0, 0);
		else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarmz, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, ini_dyingmovementpushyarm, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
	}
	else if (rando3 < 66)//right forearm
	{
		if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarmx, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmx, 0, 0);
		else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyarm, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarmy, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmy, 0, 0);
		else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarmz, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, ini_dyingmovementpushyarm, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
	}
	else //left and right forearm
	{
		if (rando2 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarmx, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmx, 0, 0);
		else if (rando2 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyarm, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarmy, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmy, 0, 0);
		else if (rando2 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarmz, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, ini_dyingmovementpushyarm, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 54187), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);

		if (rando4 < 25) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarmx, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmx, 0, 0);
		else if (rando4 < 50) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyarm, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarmy, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmy, 0, 0);
		else if (rando4 < 75) ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarmz, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarmz, 0, 0);
		else ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm, ini_dyingmovementpushyarm, ini_dyingmovementpushzarm, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 53675), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
	}
}
if ((rando >= ini_dyingmovementchancespine01 && rando <= ini_dyingmovementchancespine02) ||
(rando >= ini_dyingmovementchancearmspine01 && rando <= ini_dyingmovementchancearmspine02) ||
(rando >= ini_dyingmovementchancelegspine01 && rando <= ini_dyingmovementchancelegspine02) ||
(rando >= ini_dyingmovementchanceall01 && rando <= ini_dyingmovementchanceall02)) //spine
{
	if (rando3 < 20) //spine01 and neck01
	{
		if (rando2 < 25)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14411), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14284), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
		}
		else if (rando2 < 50)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14411), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14284), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
		}
		else if (rando2 < 75)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14411), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14284), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
		}
		else
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14411), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14284), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
		}
	}
	else if (rando3 < 40) //spine02 and neck02
	{
		if (rando2 < 25)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14412), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14285), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
		}
		else if (rando2 < 50)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14412), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14285), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
		}
		else if (rando2 < 75)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14412), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14285), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
		}
		else
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14412), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14285), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
		}
	}
	else if (rando3 < 60) //spine03 and neck03
	{
		if (rando2 < 25)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14413), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14286), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
		}
		else if (rando2 < 50)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14413), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14286), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
		}
		else if (rando2 < 75)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14413), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14286), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
		}
		else
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14413), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14286), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
		}
	}
	else if (rando3 < 80) //spine04 and neck04
	{
		if (rando2 < 25)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14414), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14287), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
		}
		else if (rando2 < 50)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14414), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14287), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
		}
		else if (rando2 < 75)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14414), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14287), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
		}
		else
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14414), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14287), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
		}
	}
	else //spine05 and neck05
	{
		if (rando2 < 25)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14415), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, 0, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14288), ini_dyingmovementdirrelspinex, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinex, 0, 0);
		}
		else if (rando2 < 50)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14415), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, ini_dyingmovementpushyspine, 0, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14288), ini_dyingmovementdirrelspiney, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspiney, 0, 0);
		}
		else if (rando2 < 75)
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14415), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, 0, 0, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14288), ini_dyingmovementdirrelspinez, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspinez, 0, 0);
		}
		else
		{
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14415), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
			ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine, ini_dyingmovementpushyspine, ini_dyingmovementpushzspine, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 14288), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
		}
	}
}