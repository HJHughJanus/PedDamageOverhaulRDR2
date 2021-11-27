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

//method for drawing text on the screen
void DrawText(float x, float y, char* str)
{
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

//method for checking if file exists
bool does_file_exist(const char* fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void PedPanic(Ped ped)
{
	if (PED::IS_PED_MALE(ped))
	{
		STREAMING::REQUEST_ANIM_DICT("face_human@gen_male@base");
		while (!STREAMING::HAS_ANIM_DICT_LOADED("face_human@gen_male@base"))
		{
		}
		PED::x_0x8B3B71C80A29A4BB(ped, "pain_1", "face_human@gen_male@base");
	}
	else
	{
		STREAMING::REQUEST_ANIM_DICT("face_human@gen_female@base");
		while (!STREAMING::HAS_ANIM_DICT_LOADED("face_human@gen_female@base"))
		{
		}
		PED::x_0x8B3B71C80A29A4BB(ped, "pain_1", "face_human@gen_female@base");
	}
	AUDIO::PLAY_PAIN(ped, 1, 1.0f, false, false);
}

void PedPain(Ped ped)
{
	//container for pain face animations
	std::map<char*, char*> faceanimmale;
	faceanimmale["pain_01"] = "face_human@gen_male@base";
	//faceanimmale["pain_2"] = "face_human@gen_male@base";
	//faceanimmale["pain_3"] = "face_human@gen_male@base";
	//faceanimmale["pain_4"] = "face_human@gen_male@base";
	//faceanimmale["pain_5"] = "face_human@gen_male@base";
	//faceanimmale["burning_1"] = "face_human@gen_male@base";
	//faceanimmale["coughing_1"] = "face_human@gen_male@base";
	//faceanimmale["die_1"] = "face_human@gen_male@base";
	//faceanimmale["die_2"] = "face_human@gen_male@base";
	//faceanimmale["electrocuted_1"] = "face_human@gen_male@base";
	//faceanimmale["mood_injured_1"] = "face_human@gen_male@base";
	//faceanimmale["pose_injured_1"] = "face_human@gen_male@base";

	//container for pain face animations
	std::map<char*, char*> faceanimfemale;
	faceanimfemale["pain_01"] = "face_human@gen_female@base";
	//faceanimfemale["pain_2"] = "face_human@gen_female@base";
	//faceanimfemale["pain_3"] = "face_human@gen_female@base";
	//faceanimfemale["pain_4"] = "face_human@gen_female@base";
	//faceanimfemale["pain_5"] = "face_human@gen_female@base";
	//faceanimfemale["burning_1"] = "face_human@gen_female@base";
	//faceanimfemale["coughing_1"] = "face_human@gen_female@base";
	//faceanimfemale["die_1"] = "face_human@gen_female@base";
	//faceanimfemale["die_2"] = "face_human@gen_female@base";
	//faceanimfemale["electrocuted_1"] = "face_human@gen_female@base";
	//faceanimfemale["mood_injured_1"] = "face_human@gen_female@base";
	//faceanimfemale["pose_injured_1"] = "face_human@gen_female@base";

	int pain = 2 + (std::rand() % (12 - 2 + 1));;
	if (PED::IS_PED_MALE(ped))
	{
		bool animplaying = false;
		std::map<char*, char*>::iterator anim_it = faceanimmale.begin();

		/*
		while (anim_it != faceanimmale.end())
		{
			char* anim = anim_it->first;
			char* dict = anim_it->second;
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(ped, dict, anim, 3))
			{
				animplaying = true;
			}
			anim_it++;
		}
		*/

		if (!animplaying)
		{
			anim_it = faceanimmale.begin();
			int facerand = 1 + (std::rand() % (faceanimmale.size() - 1 + 1));
			int counter = 1;
			while (anim_it != faceanimmale.end())
			{
				if (facerand == counter)
				{
					char* anim = anim_it->first;
					char* dict = anim_it->second;
					STREAMING::REQUEST_ANIM_DICT(dict);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict))
					{
						PED::RESET_PED_RAGDOLL_TIMER(ped);
					}
					PED::x_0x8B3B71C80A29A4BB(ped, anim, dict);
					AUDIO::PLAY_PAIN(ped, pain, 1.0f, false, false);
				}
				counter++;
				anim_it++;
			}
		}
	}
	else
	{
		bool animplaying = false;
		std::map<char*, char*>::iterator anim_it = faceanimfemale.begin();

		/*
		while (anim_it != faceanimfemale.end())
		{
			char* anim = anim_it->first;
			char* dict = anim_it->second;
			if (ENTITY::IS_ENTITY_PLAYING_ANIM(peds[i], dict, anim, 3))
			{
				animplaying = true;
			}
			anim_it++;
		}
		*/

		if (!animplaying)
		{
			anim_it = faceanimfemale.begin();
			int facerand = 1 + (std::rand() % (faceanimfemale.size() - 1 + 1));
			int counter = 1;
			while (anim_it != faceanimfemale.end())
			{
				if (facerand == counter)
				{
					char* anim = anim_it->first;
					char* dict = anim_it->second;
					STREAMING::REQUEST_ANIM_DICT(dict);
					while (!STREAMING::HAS_ANIM_DICT_LOADED(dict))
					{
						PED::RESET_PED_RAGDOLL_TIMER(ped);
					}
					PED::x_0x8B3B71C80A29A4BB(ped, anim, dict);
					AUDIO::PLAY_PAIN(ped, pain, 1.0f, false, false);
				}
				counter++;
				anim_it++;
			}
		}
	}
}

//main logic
void main()
{
	//checking if there is an ini file (used for drawing according information on screen)
	bool iniexists = false;
	iniexists = does_file_exist(".\\PedDamageOverhaul.ini");

	//getting values from the ini or setting default values, if ini not there (default represent mod version "standard" of v1.41)
	float ini_sweaponmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpecialWeaponModifier", 190, ".\\PedDamageOverhaul.ini") / 100;
	float ini_weaponmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "WeaponModifier", 60, ".\\PedDamageOverhaul.ini") / 100;
	float ini_knifemodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "KnifeModifier", 100, ".\\PedDamageOverhaul.ini") / 100;
	float ini_meleemodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MeleeModifier", 150, ".\\PedDamageOverhaul.ini") / 100;
	float ini_npcweaponmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCWeaponModifier", 100, ".\\PedDamageOverhaul.ini") / 100;
	float ini_npcmeleemodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCMeleeModifier", 100, ".\\PedDamageOverhaul.ini") / 100;
	int ini_storypedhealth = GetPrivateProfileInt("PedDamageConfiguration_Basic", "StoryNPCHealth", 0, ".\\PedDamageOverhaul.ini");
	int ini_togglekey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "ToggleKey", 9, ".\\PedDamageOverhaul.ini");
	int ini_enabled = GetPrivateProfileInt("PedDamageConfiguration_Basic", "EnableScriptByDefault", 1, ".\\PedDamageOverhaul.ini");
	int ini_playerinvincibility = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PlayerInvincibility", 0, ".\\PedDamageOverhaul.ini");
	int ini_survivfirchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FireSurvivalChance", 23, ".\\PedDamageOverhaul.ini");
	int ini_firehealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FireHealth", 300, ".\\PedDamageOverhaul.ini");
	int ini_pedhealth = GetPrivateProfileInt("PedDamageConfiguration_Basic", "NPCHealth", 240, ".\\PedDamageOverhaul.ini");
	int ini_painthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "KnockbackThreshold", 230, ".\\PedDamageOverhaul.ini");
	int ini_knockbackchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "KnockbackChance", 11, ".\\PedDamageOverhaul.ini");
	int ini_dyingthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingThreshold", 60, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingMovementThreshold", 215, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementthreshold2 = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingMovementThreshold2", 130, ".\\PedDamageOverhaul.ini");
	int ini_stumblechancebothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumbleChanceBothLegs", 100, ".\\PedDamageOverhaul.ini");
	int ini_stumblechanceoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumbleChanceOneLeg", 15, ".\\PedDamageOverhaul.ini");
	int ini_bleedwhendying = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedWhenDying", 1, ".\\PedDamageOverhaul.ini");
	int ini_bleedwhenshot = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedWhenShot", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingvalue = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingValue", 2, ".\\PedDamageOverhaul.ini");
	int ini_runningthresholdbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RunningThresholdBothLegs", 30, ".\\PedDamageOverhaul.ini");
	int ini_runningthresholdoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RunningThresholdOneLeg", 50, ".\\PedDamageOverhaul.ini");
	float ini_femalepushmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FemalePushModifier", 3, ".\\PedDamageOverhaul.ini");
	float ini_forcepushx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushX", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushy = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushY", 320, ".\\PedDamageOverhaul.ini");
	float ini_forcepushz = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushxboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushX", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushyboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushY", 375, ".\\PedDamageOverhaul.ini");
	float ini_forcepushzboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_dforcepushx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushX", 35, ".\\PedDamageOverhaul.ini");
	float ini_dforcepushy = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushY", 0, ".\\PedDamageOverhaul.ini");
	float ini_dforcepushz = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_dforcepushheadx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushHeadX", 100, ".\\PedDamageOverhaul.ini");
	float ini_dforceoffx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementForceOffsetX", 1, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dforceoffy = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementForceOffsetY", 1, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dforceoffz = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementForceOffsetZ", 1, ".\\PedDamageOverhaul.ini") / 10;
	int ini_dyingmovementchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementChance", 1000, ".\\PedDamageOverhaul.ini");
	float ini_dyingmovementpushtime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushTime", 500, ".\\PedDamageOverhaul.ini");
	float ini_dyingmovementwaittime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementWaitTime", 200, ".\\PedDamageOverhaul.ini");
	int ini_bdyingmovementchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementChance", 250, ".\\PedDamageOverhaul.ini");
	float ini_bdyingmovementpushtime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementPushTime", 900, ".\\PedDamageOverhaul.ini");
	float ini_bdyingmovementwaittime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementWaitTime", 3500, ".\\PedDamageOverhaul.ini");
	float ini_dyingmovementbuilduptime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementBuildUpTime", 3000, ".\\PedDamageOverhaul.ini");
	float ini_bdyingmovementbuilduptime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementBuildUpTime", 3000, ".\\PedDamageOverhaul.ini");
	float ini_dyingmovementbuildupcount = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementBuildUpCount", 100, ".\\PedDamageOverhaul.ini");
	float ini_bdyingmovementbuildupcount = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementBuildUpCount", 100, ".\\PedDamageOverhaul.ini");
	float ini_dyingmovementpushxleg01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Legs01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyleg01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Legs01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzleg01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Legs01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxleg02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Legs02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyleg02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Legs02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzleg02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Legs02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxleg03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Legs03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyleg03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Legs03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzleg03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Legs03", 20, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxleg04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Legs04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyleg04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Legs04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzleg04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Legs04", 30, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxarm01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Arms01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyarm01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Arms01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzarm01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Arms01", 95, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxarm02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Arms02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyarm02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Arms02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzarm02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Arms02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxspine01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Spine01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyspine01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Spine01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzspine01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Spine01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxspine02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Spine02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyspine02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Spine02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzspine02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Spine02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxspine03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Spine03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyspine03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Spine03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzspine03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Spine03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxspine04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Spine04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyspine04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Spine04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzspine04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Spine04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxspine05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Spine05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyspine05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Spine05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzspine05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Spine05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxneck01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Neck01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyneck01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Neck01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzneck01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Neck01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxneck02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Neck02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyneck02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Neck02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzneck02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Neck02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxneck03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Neck03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyneck03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Neck03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzneck03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Neck03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxneck04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Neck04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyneck04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Neck04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzneck04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Neck04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushxneck05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushX_Neck05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushyneck05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushY_Neck05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementpushzneck05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementPushZ_Neck05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxleg01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXLeg01", 12, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxleg02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXLeg02", 24, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxleg03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXLeg03", 12, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxleg04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXLeg04", 14, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxarm01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXArm01", 40, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxarm02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXArm02", 40, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxspine01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXSpine01", 20, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxspine02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXSpine02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxspine03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXSpine03", 30, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxspine04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXSpine04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxspine05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXSpine05", 20, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxneck01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXNeck01", 23, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxneck02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXNeck02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxneck03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXNeck03", 30, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxneck04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXNeck04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dmforcepushxneck05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DMPushXNeck05", 23, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxleg01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXLeg01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxleg02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXLeg02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxleg03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXLeg03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxleg04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXLeg04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxarm01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXArm01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxarm02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXArm02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxspine01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXSpine01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxspine02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXSpine02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxspine03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXSpine03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxspine04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXSpine04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxspine05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXSpine05", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxneck01 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXNeck01", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxneck02 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXNeck02", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxneck03 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXNeck03", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxneck04 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXNeck04", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_bdmforcepushxneck05 = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDMPushXNeck05", 0, ".\\PedDamageOverhaul.ini") / 10;
	int ini_dyingmovementlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementLegsEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementarms = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementArmsEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementspine = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementSpineEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementneck = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementNeckEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_bdyingmovementlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementLegsEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_bdyingmovementarms = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementArmsEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_bdyingmovementspine = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementSpineEnable", 1, ".\\PedDamageOverhaul.ini");
	int ini_bdyingmovementneck = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "bDyingMovementNeckEnable", 1, ".\\PedDamageOverhaul.ini");
	float ini_dyingmovementenablespeed = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementEnableWhenSpeedLowerThan", 10, ".\\PedDamageOverhaul.ini") / 10;
	int ini_dyingmovementignoreupvec = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IgnoreDyingMovementUpVector", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementdirrelleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementDirectionRelative_Legs", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementdirrelarm = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementDirectionRelative_Arms", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementdirrelspine = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementDirectionRelative_Spine", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementdirrelneck = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementDirectionRelative_Neck", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementforrelleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementForceRelative_Legs", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementforrelarm = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementForceRelative_Arms", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementforrelspine = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementForceRelative_Spine", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementforrelneck = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingMovementForceRelative_Neck", 0, ".\\PedDamageOverhaul.ini");
	float ini_zvaluehead = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHeadBone", 500, ".\\PedDamageOverhaul.ini") / 1000;
	float ini_zvaluehip = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHipBone", 250, ".\\PedDamageOverhaul.ini") / 1000;
	int ini_ignoreupvec = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IgnoreUpVector", 1, ".\\PedDamageOverhaul.ini");
	int ini_dforceisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingForceRelative", 0, ".\\PedDamageOverhaul.ini");
	int ini_ddirectionisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingDirectionRelative", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingpushchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingPushChance", 1000, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_shot = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceShot", 8, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_dying = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying", 8, ".\\PedDamageOverhaul.ini");
	int ini_dirreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeOneLeg", 1, ".\\PedDamageOverhaul.ini");
	int ini_forreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeOneLeg", 0, ".\\PedDamageOverhaul.ini");
	int ini_dirrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeBothLegs", 1, ".\\PedDamageOverhaul.ini");
	int ini_forrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeBothLegs", 0, ".\\PedDamageOverhaul.ini");
	int ini_lassodisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "LassoDisarms", 0, ".\\PedDamageOverhaul.ini");
	int ini_hogtiedisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "HogtyingDisarms", 1, ".\\PedDamageOverhaul.ini");
	int ini_disarmingchance = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisarmingChance", 1, ".\\PedDamageOverhaul.ini");
	float ini_underwatertime = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "UnderWaterTime", 100, ".\\PedDamageOverhaul.ini");
	int ini_xrollchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementXRollChance", 40, ".\\PedDamageOverhaul.ini");
	int ini_dmaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudioChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_painaudio01 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio01", 4, ".\\PedDamageOverhaul.ini");
	int ini_painaudio02 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio02", 5, ".\\PedDamageOverhaul.ini");
	int ini_painaudio03 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio03", 9, ".\\PedDamageOverhaul.ini");
	int ini_ragdollfalltime01 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RagdollFallTime01", 10000, ".\\PedDamageOverhaul.ini");
	int ini_ragdollfalltime02 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RagdollFallTime02", 10000, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamounthealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountHealth", 5, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamountstamina = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountStamina", 5, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamountdeadeye = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountDeadeye", 5, ".\\PedDamageOverhaul.ini");
	int ini_coredepletiontime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionTime", 300000, ".\\PedDamageOverhaul.ini");
	int ini_coredepletion = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletion", 0, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionafterdeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAfterDeath", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingstatesmelee = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStatesForMelee", 0, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracy = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracy", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingstatechance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_panicchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PanicChance", 2, ".\\PedDamageOverhaul.ini");
	int ini_panicchancerag = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PanicChanceHelpless", 3, ".\\PedDamageOverhaul.ini");
	int ini_excludestorynpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ExcludeStoryNPCs", 1, ".\\PedDamageOverhaul.ini");
	float ini_legdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LegDamageModifier", 100, ".\\PedDamageOverhaul.ini") / 100);
	float ini_armdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ArmDamageModifier", 100, ".\\PedDamageOverhaul.ini") / 100);
	float ini_torsodamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TorsoDamageModifier", 100, ".\\PedDamageOverhaul.ini") / 100);
	float ini_headdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadDamageModifier", 100, ".\\PedDamageOverhaul.ini") / 100);
	float ini_neckdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NeckDamageModifier", 100, ".\\PedDamageOverhaul.ini") / 100);
	int ini_meleedyingstateknockout = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MeleeDyingStateKnockout", 0, ".\\PedDamageOverhaul.ini");
	int ini_useanimchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "UseAnimChance", 50, ".\\PedDamageOverhaul.ini");
	int ini_transdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TransitioningDelta", 3500, ".\\PedDamageOverhaul.ini");
	bool ini_specialhogtie = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateHogtie", 1, ".\\PedDamageOverhaul.ini");
	int ini_euphoriastumblechance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSEuphoriaStumbleChance", 0, ".\\PedDamageOverhaul.ini");
	int ini_disarmfleeingchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmFleeingChance", 31, ".\\PedDamageOverhaul.ini");
	int ini_disarmattackchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmAttackChance", 55, ".\\PedDamageOverhaul.ini");
	int ini_disarmhandsupchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmHandsUpChance", 7, ".\\PedDamageOverhaul.ini");
	int ini_disarmcowerchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmCowerChance", 7, ".\\PedDamageOverhaul.ini");
	int ini_modeffectrange = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ModEffectRange", 1024, ".\\PedDamageOverhaul.ini");
	int ini_falltimedown = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FallTimeDown", 1500, ".\\PedDamageOverhaul.ini");
	int ini_falltimedeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FallTimeDeath", 2000, ".\\PedDamageOverhaul.ini");
	int ini_onlybleedshottorso = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OnlyBleedOnTorsoShot", 1, ".\\PedDamageOverhaul.ini");
	int toggleKey = VK_F9;
	switch (ini_togglekey)
	{
	case 1:
		toggleKey = VK_F1;
		break;
	case 2:
		toggleKey = VK_F2;
		break;
	case 3:
		toggleKey = VK_F3;
		break;
	case 4:
		toggleKey = VK_F4;
		break;
	case 5:
		toggleKey = VK_F5;
		break;
	case 6:
		toggleKey = VK_F6;
		break;
	case 7:
		toggleKey = VK_F7;
		break;
	case 8:
		toggleKey = VK_F8;
		break;
	case 9:
		toggleKey = VK_F9;
		break;
	case 10:
		toggleKey = VK_F10;
		break;
	case 11:
		toggleKey = VK_F11;
		break;
	case 12:
		toggleKey = VK_F12;
		break;
	default:
		toggleKey = VK_F9;
	}
	float runningthresholdbothlegs = ini_runningthresholdbothlegs / 10;
	float runningthresholdoneleg = ini_runningthresholdoneleg / 10;

	//making sure disarm reaction chances are set to valid values
	if (ini_disarmattackchance + ini_disarmcowerchance + ini_disarmfleeingchance + ini_disarmhandsupchance != 100)
	{
		ini_disarmattackchance = 55;
		ini_disarmfleeingchance = 31;
		ini_disarmhandsupchance = 7;
		ini_disarmcowerchance = 7;
	}

	//container for peds and their status (is burning, is shot in the leg, is shot in both legs, etc.)
	std::map<Ped, int> pedmapstat;
	//container for peds and a flag for doing something (is used during the dying states to be able to stop the NPC from moving (when the value is 0))
	std::map<Ped, int> pedmapdosomething;
	//container for peds and time passed since they have been doing something (is used during the dying states to be able to make NPCs move for a certain amount of time)
	std::map<Ped, int> pedmaptime;
	//container for peds and a randomizer, in case it is applicable for more than one script loop (is used during the dying states to be able to remember the force with which an NPC is supposed to be moved)
	std::map<Ped, list<int>> pedmaprand;
	//container for peds and their current health (for checking if damage was dealt - mostly used for reactions like "hands up" or cowering (if enough damage is done, NPCs will then flee))
	std::map<Ped, int> pedmaphealth;
	//container for peds and a value to remember for dying movement (remembers the direction in which an NPC is supposed to be moving)
	std::map<Ped, int> pedmaprememb;
	//container for peds and a value to remember for dying movement (remembers the direction in which an NPC is supposed to be moving)
	std::map<Ped, int> pedmaprememb2;
	//container for peds and a value to remember for dying movement (remembers if either one or the other moving method is supposed to be used)
	std::map<Ped, int> pedmaprememb3;
	//container for peds and a value to remember for dying movement (build up push time - an method of increasing the pushing force over time, so it does not seem abrupt)
	std::map<Ped, float> pedmapbupushtime;
	//container for peds and a value to remember for dying movement (pushing values)
	std::map<Ped, std::map<string, float>> pedmappushes;
	//container for peds and a value to decide whether dying states kick in or vanilla behavior is triggered
	std::map<Ped, bool> pedmapdyingstatebool;
	//container for peds and a value to calculate the limb specific damage
	std::map<Ped, int> pedmaplimbhealth;
	//container for peds and a value for dying movement stage 1 (for remembering if it has been entered before)
	std::map<Ped, bool> pedmapdyingstate1entered;
	//container for peds and a value for dying movement stage 2 (for remembering if it has been entered before)
	std::map<Ped, bool> pedmapdyingstate2entered;
	//container for peds and a value for dying movement stage 3 (for remembering if it has been entered before)
	std::map<Ped, bool> pedmapdyingstate3entered;
	//container for peds and a value for animation transitioning in dying state 1
	std::map<Ped, int> pedmapanimtrans;
	//container for ped health (for NPC pain reactions during dying states)
	std::map<Ped, int> pedmaphealthds;
	//container for ped stumbling (euphoria enabled or not)
	std::map<Ped, int> pedmapeuph;
	//container for ped animation use in dying states
	std::map<Ped, int> pedmapanimuse;
	//container for remembering if ped was k.o.ed
	std::map<Ped, int> pedmapko;
	//container for flag if ped is falling
	std::map<Ped, int> pedmapisfalling;
	//container for remembering how long ped has been falling
	std::map<Ped, int> pedmapfalltime;
	//container for flag if ped is to be killed
	std::map<Ped, int> pedmapkill;
	//container for flag if ped is to be down
	std::map<Ped, int> pedmapdown;
	//container for flag if ped was hit in the torso
	std::map<Ped, int> pedmaptorsohit;

	//container for knives
	vector<char*> knives;

	knives.push_back("WEAPON_MELEE_KNIFE_JOHN");
	knives.push_back("WEAPON_MELEE_KNIFE");
	knives.push_back("WEAPON_MELEE_KNIFE_MINER");
	knives.push_back("WEAPON_MELEE_KNIFE_CIVIL_WAR");
	knives.push_back("WEAPON_MELEE_KNIFE_BEAR");
	knives.push_back("WEAPON_MELEE_KNIFE_VAMPIRE");
	knives.push_back("WEAPON_MELEE_MACHETE");

	//container for bones
	vector<int> legbones, armbones, torsobones, headbones, neckbones;

	legbones.push_back(6884); //SKEL_R_Thigh
	legbones.push_back(65478); //SKEL_L_Thigh
	legbones.push_back(55120); //SKEL_L_CALF
	legbones.push_back(33646); //unknown leg bone
	legbones.push_back(43312); //SKEL_R_Calf

	armbones.push_back(54187); //SKEL_R_Forearm
	armbones.push_back(53675); //SKEL_L_Forearm
	armbones.push_back(41357); //SKEL_L_Finger32
	armbones.push_back(41341); //SKEL_L_Finger32
	armbones.push_back(41405); //SKEL_L_Finger12
	armbones.push_back(41326); //SKEL_L_Finger12
	armbones.push_back(41325); //SKEL_L_Finger12
	armbones.push_back(35876); //SKEL_L_Finger41
	armbones.push_back(16829); //SKEL_L_Finger32
	armbones.push_back(16781); //SKEL_R_Finger32
	armbones.push_back(16765); //SKEL_L_Finger32
	armbones.push_back(16749); //SKEL_R_Finger12
	armbones.push_back(11300); //SKEL_R_Finger41
	armbones.push_back(37873); //Left upper arm (name unknown)
	armbones.push_back(46065); //right upper arm (name unknown)

	torsobones.push_back(14410); //unknown torso bone
	torsobones.push_back(14411); //SKEL_Spine1
	torsobones.push_back(14412); //SKEL_Spine2
	torsobones.push_back(14413); //SKEL_Spine3
	torsobones.push_back(14414); //SKEL_Spine4
	torsobones.push_back(14415); //SKEL_Spine5

	headbones.push_back(21030); //SKEL_Head

	neckbones.push_back(14284); //SKEL_Neck1
	neckbones.push_back(14285); //SKEL_Neck2
	neckbones.push_back(14286); //SKEL_Neck3
	neckbones.push_back(14287); //SKEL_Neck4
	neckbones.push_back(14288); //SKEL_Neck5

	//timer for core depletion
	int coredepletiontimer = GAMEPLAY::GET_GAME_TIMER() + ini_coredepletiontime;

	//bool for core depletion after death
	bool playerdead = false;
	int deathtime;

	//is script activated by default or not
	bool modScriptState = false;
	if (ini_enabled == 1) modScriptState = true;
	int msgTime{ 0 };

	//main script
	while (true)
	{
		//check if script is being enabled or disabled
		if (IsKeyJustUp(toggleKey))
		{
			modScriptState = !modScriptState;
			msgTime = GetTickCount() + 3000;
		}

		//if script enabled, start doing your thing
		if (modScriptState)
		{
			//get player and player ped as well as the identifying hash for the lasso weapon and a variable for NPCs getting caught by it and another one for checking if player damages hogtied NPCs
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Hash lassoHash = GAMEPLAY::GET_HASH_KEY("WEAPON_LASSO");
			Ped lastTarget = playerPed; //used for lasso disarming
			Ped lastTarget2 = playerPed; //used for hogtied panicking
			Ped lastTargetx = playerPed; //used for special hogtie fix for dying movement writhing animation

			//set invincibility, if set in ini
			if (ini_playerinvincibility == 1) PLAYER::SET_PLAYER_INVINCIBLE(player, true);

			//core depletion
			if (ini_coredepletion == 1)
			{
				int corehealth = ATTRIBUTE::_0x36731AC041289BB1(playerPed, 0);
				int corestamina = ATTRIBUTE::_0x36731AC041289BB1(playerPed, 1);
				int coredeadeye = ATTRIBUTE::_0x36731AC041289BB1(playerPed, 2);
				if (GAMEPLAY::GET_GAME_TIMER() > coredepletiontimer)
				{
					if ((corehealth - ini_coredepletionamounthealth) < 0) ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 0, 0);
					else ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 0, corehealth - ini_coredepletionamounthealth);
					if ((corestamina - ini_coredepletionamountstamina) < 0) ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 1, 0);
					else ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 1, corestamina - ini_coredepletionamountstamina);
					if ((coredeadeye - ini_coredepletionamountdeadeye) < 0) ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 2, 0);
					else ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 2, coredeadeye - ini_coredepletionamountdeadeye);
					coredepletiontimer = GAMEPLAY::GET_GAME_TIMER() + ini_coredepletiontime;
				}
			}

			//core depletion after death
			if (ini_coredepletionafterdeath == 1)
			{
				if (PLAYER::IS_PLAYER_DEAD(player))
				{
					playerdead = true;
					deathtime = GAMEPLAY::GET_GAME_TIMER() + 15000;
				}
				if (!PLAYER::IS_PLAYER_DEAD(player) && playerdead)
				{
					ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 0, 0);
					ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 1, 0);
					ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 2, 0);
					if (GAMEPLAY::GET_GAME_TIMER() > deathtime) playerdead = false;
				}
			}

			//set NPC damage modifiers
			if (ini_npcmeleemodifier != 0) PED::SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(ini_npcmeleemodifier);
			if (ini_npcweaponmodifier != 0) PED::SET_AI_WEAPON_DAMAGE_MODIFIER(ini_npcweaponmodifier);

			//get all peds near player (those will be affected by the script)
			const int ARR_SIZE = 1024;
			Ped peds[ARR_SIZE];
			//int count = worldGetAllPeds(peds, ARR_SIZE);
			int count = worldGetAllPeds(peds, ini_modeffectrange);

			//container for weapons which cause bleeding
			std::map<int, char*> allweaponmap;
			allweaponmap[0] = "WEAPON_MELEE_HATCHET";
			allweaponmap[1] = "WEAPON_MELEE_CLEAVER";
			allweaponmap[2] = "WEAPON_MELEE_ANCIENT_HATCHET";
			allweaponmap[3] = "WEAPON_MELEE_HATCHET_VIKING";
			allweaponmap[4] = "WEAPON_MELEE_HATCHET_HEWING";
			allweaponmap[5] = "WEAPON_MELEE_HATCHET_DOUBLE_BIT";
			allweaponmap[6] = "WEAPON_MELEE_HATCHET_DOUBLE_BIT_RUSTED";
			allweaponmap[7] = "WEAPON_MELEE_HATCHET_HUNTER";
			allweaponmap[8] = "WEAPON_MELEE_HATCHET_HUNTER_RUSTED";
			allweaponmap[9] = "WEAPON_MELEE_KNIFE_JOHN";
			allweaponmap[10] = "WEAPON_MELEE_KNIFE";
			allweaponmap[11] = "WEAPON_MELEE_KNIFE_JAWBONE";
			allweaponmap[12] = "WEAPON_THROWN_THROWING_KNIVES";
			allweaponmap[13] = "WEAPON_MELEE_KNIFE_MINER";
			allweaponmap[14] = "WEAPON_MELEE_KNIFE_CIVIL_WAR";
			allweaponmap[15] = "WEAPON_MELEE_KNIFE_BEAR";
			allweaponmap[16] = "WEAPON_MELEE_KNIFE_VAMPIRE";
			allweaponmap[17] = "WEAPON_MELEE_MACHETE";
			allweaponmap[18] = "WEAPON_THROWN_TOMAHAWK";
			allweaponmap[19] = "WEAPON_THROWN_TOMAHAWK_ANCIENT";
			allweaponmap[20] = "WEAPON_PISTOL_M1899";
			allweaponmap[21] = "WEAPON_PISTOL_MAUSER";
			allweaponmap[22] = "WEAPON_PISTOL_MAUSER_DRUNK";
			allweaponmap[23] = "WEAPON_PISTOL_SEMIAUTO";
			allweaponmap[24] = "WEAPON_PISTOL_VOLCANIC";
			allweaponmap[25] = "WEAPON_REPEATER_CARBINE";
			allweaponmap[26] = "WEAPON_REPEATER_EVANS";
			allweaponmap[27] = "WEAPON_REPEATER_HENRY";
			allweaponmap[28] = "WEAPON_RIFLE_VARMINT";
			allweaponmap[29] = "WEAPON_REPEATER_WINCHESTER";
			allweaponmap[30] = "WEAPON_REVOLVER_CATTLEMAN";
			allweaponmap[31] = "WEAPON_REVOLVER_CATTLEMAN_JOHN";
			allweaponmap[32] = "WEAPON_REVOLVER_CATTLEMAN_MEXICAN";
			allweaponmap[33] = "WEAPON_REVOLVER_CATTLEMAN_PIG";
			allweaponmap[34] = "WEAPON_REVOLVER_DOUBLEACTION";
			allweaponmap[35] = "WEAPON_REVOLVER_DOUBLEACTION_EXOTIC";
			allweaponmap[36] = "WEAPON_REVOLVER_DOUBLEACTION_GAMBLER";
			allweaponmap[37] = "WEAPON_REVOLVER_DOUBLEACTION_MICAH";
			allweaponmap[38] = "WEAPON_REVOLVER_LEMAT";
			allweaponmap[39] = "WEAPON_REVOLVER_SCHOFIELD";
			allweaponmap[40] = "WEAPON_REVOLVER_SCHOFIELD_GOLDEN";
			allweaponmap[41] = "WEAPON_REVOLVER_SCHOFIELD_CALLOWAY";
			allweaponmap[42] = "WEAPON_RIFLE_BOLTACTION";
			allweaponmap[43] = "WEAPON_SNIPERRIFLE_CARCANO";
			allweaponmap[44] = "WEAPON_SNIPERRIFLE_ROLLINGBLOCK";
			allweaponmap[45] = "WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC";
			allweaponmap[46] = "WEAPON_RIFLE_SPRINGFIELD";
			allweaponmap[47] = "WEAPON_SHOTGUN_DOUBLEBARREL";
			allweaponmap[48] = "WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC";
			allweaponmap[49] = "WEAPON_SHOTGUN_PUMP";
			allweaponmap[50] = "WEAPON_SHOTGUN_REPEATING";
			allweaponmap[51] = "WEAPON_SHOTGUN_SAWEDOFF";
			allweaponmap[52] = "WEAPON_SHOTGUN_SEMIAUTO";
			allweaponmap[53] = "WEAPON_BOW";
			allweaponmap[54] = "WEAPON_THROWN_DYNAMITE";

			//container for special weapons (affected by special weapon damage modifier)
			std::map<int, Hash> weaponmap;
			weaponmap[0] = GAMEPLAY::GET_HASH_KEY("WEAPON_THROWN_DYNAMITE");
			weaponmap[1] = GAMEPLAY::GET_HASH_KEY("WEAPON_THROWN_TOMAHAWK_ANCIENT");
			weaponmap[2] = GAMEPLAY::GET_HASH_KEY("WEAPON_THROWN_TOMAHAWK");
			weaponmap[3] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_DOUBLEBARREL");
			weaponmap[4] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC");
			weaponmap[5] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_PUMP");
			weaponmap[6] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_REPEATING");
			weaponmap[7] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_SAWEDOFF");
			weaponmap[8] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_SEMIAUTO");
			weaponmap[9] = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_CARCANO");
			weaponmap[10] = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_ROLLINGBLOCK");
			weaponmap[11] = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC");

			//container for special ammo (affected by special weapon damage modifier)
			std::map<Hash, Hash> ammomap;
			ammomap[GAMEPLAY::GET_HASH_KEY("AMMO_ARROW_DYNAMITE")] = GAMEPLAY::GET_HASH_KEY("WEAPON_BOW");
			ammomap[GAMEPLAY::GET_HASH_KEY("AMMO_ARROW_IMPROVED")] = GAMEPLAY::GET_HASH_KEY("WEAPON_BOW");

			//filling container for peds and giving them default values
			for (int i = 0; i < count; i++)
			{
				if (pedmapstat.find(peds[i]) == pedmapstat.end())
				{
					//status: 0 = default, 1 = both legs crippled, 2 = both legs crippled & will survive burning, 3 = default & will survive burning, 4 = is/was burning and should survive
					//5 = is burning and should not survive, 6 = left leg crippled, 7 = left leg crippled & will survive burning, 8 = right leg crippled, 9 = right leg crippled & will survive burning
					//99 = temporary status in which a prolonging of the ragdoll behavior is not wanted
					pedmapstat[peds[i]] = 0;
				}
				if (pedmaphealth.find(peds[i]) == pedmaphealth.end())
				{
					//status: 0 = default value
					pedmaphealth[peds[i]] = 0;
				}
				if (pedmapdosomething.find(peds[i]) == pedmapdosomething.end())
				{
					//status: 0 = default value
					pedmapdosomething[peds[i]] = 0;
				}
				if (pedmaptime.find(peds[i]) == pedmaptime.end())
				{
					//status: -1 = default value
					pedmaptime[peds[i]] = -1;
				}
				if (pedmaprand.find(peds[i]) == pedmaprand.end())
				{
					//status: -1 = default value
					pedmaprand[peds[i]].push_back(-1);
				}
				if (pedmaprememb.find(peds[i]) == pedmaprememb.end())
				{
					//status: 0 = default value, positive value (e.g. 1) = positive push, negative value (e.g. -1) = negative push
					pedmaprememb[peds[i]] = 0;
				}
				if (pedmaprememb2.find(peds[i]) == pedmaprememb2.end())
				{
					//status: 0 = default value, positive value (e.g. 1) = left push, negative value (e.g. -1) = right push
					pedmaprememb2[peds[i]] = 0;
				}
				if (pedmaprememb3.find(peds[i]) == pedmaprememb3.end())
				{
					//status: 0 = default value, 1 = x-roll will be used during dying state 1, -1 = z-roll will be used during dying state 1
					pedmaprememb3[peds[i]] = 0;
				}
				if (pedmapdyingstate3entered.find(peds[i]) == pedmapdyingstate3entered.end())
				{
					//status: 0 = default value, 1 = was already in dying state 3 at least once
					pedmapdyingstate3entered[peds[i]] = false;
				}
				if (pedmapdyingstate2entered.find(peds[i]) == pedmapdyingstate2entered.end())
				{
					//status: 0 = default value, 1 = was already in dying state 2 at least once
					pedmapdyingstate2entered[peds[i]] = false;
				}
				if (pedmapdyingstate1entered.find(peds[i]) == pedmapdyingstate1entered.end())
				{
					//status: 0 = default value, 1 = was already in dying state 1 at least once
					pedmapdyingstate1entered[peds[i]] = false;
				}
				if (pedmapanimtrans.find(peds[i]) == pedmapanimtrans.end())
				{
					//status: -1 = default value, will later be filled with a game timer value to make transition into writhing animation occur after a while (looks smoother than if it is done immediately)
					pedmapanimtrans[peds[i]] = -1;
				}
				if (pedmapbupushtime.find(peds[i]) == pedmapbupushtime.end())
				{
					//status: 0 = default value
					pedmapbupushtime[peds[i]] = 0;
				}
				if (pedmaphealthds.find(peds[i]) == pedmaphealthds.end())
				{
					//status: 0 = default value
					pedmaphealthds[peds[i]] = 0;
				}
				if (pedmappushes.find(peds[i]) == pedmappushes.end())
				{
					//status: 0 = default value
					pedmappushes[peds[i]]["dmforcepushxleg01"] = 0;
					pedmappushes[peds[i]]["dmforcepushxleg02"] = 0;
					pedmappushes[peds[i]]["dmforcepushxleg03"] = 0;
					pedmappushes[peds[i]]["dmforcepushxleg04"] = 0;
					pedmappushes[peds[i]]["dmforcepushxarm01"] = 0;
					pedmappushes[peds[i]]["dmforcepushxarm02"] = 0;
					pedmappushes[peds[i]]["dmforcepushxspine01"] = 0;
					pedmappushes[peds[i]]["dmforcepushxspine02"] = 0;
					pedmappushes[peds[i]]["dmforcepushxspine03"] = 0;
					pedmappushes[peds[i]]["dmforcepushxspine04"] = 0;
					pedmappushes[peds[i]]["dmforcepushxspine05"] = 0;
					pedmappushes[peds[i]]["dmforcepushxneck01"] = 0;
					pedmappushes[peds[i]]["dmforcepushxneck02"] = 0;
					pedmappushes[peds[i]]["dmforcepushxneck03"] = 0;
					pedmappushes[peds[i]]["dmforcepushxneck04"] = 0;
					pedmappushes[peds[i]]["dmforcepushxneck05"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxleg01"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxleg02"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxleg03"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxleg04"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxarm01"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxarm02"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxspine01"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxspine02"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxspine03"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxspine04"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxspine05"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxneck01"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxneck02"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxneck03"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxneck04"] = 0;
					pedmappushes[peds[i]]["bdmforcepushxneck05"] = 0;
				}
				if (pedmapdyingstatebool.find(peds[i]) == pedmapdyingstatebool.end())
				{
					//status: 0 = disabled, 1 = enabled
					int rand = 0 + (std::rand() % (99 - 0 + 1));
					if (rand < ini_dyingstatechance) pedmapdyingstatebool[peds[i]] = 1;
					else pedmapdyingstatebool[peds[i]] = 0;
				}
				if (pedmaplimbhealth.find(peds[i]) == pedmaplimbhealth.end())
				{
					//gets filled with current NPC health values, so later the delta (damage done) can be computed
					pedmaplimbhealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
				}
				if (pedmapko.find(peds[i]) == pedmapko.end())
				{
					//status: 0 = has not been knocked out, 1 = has been knocked out and effects pending, 2 = has been knocked out and effects have been triggered
					pedmapko[peds[i]] = 0;
				}
				if (pedmapanimuse.find(peds[i]) == pedmapanimuse.end())
				{
					//status: 0 = do not use animation, 1 = use animation
					int animrand = 0 + (std::rand() % (99 - 0 + 1));
					if (animrand < ini_useanimchance) pedmapanimuse[peds[i]] = 1;
					else pedmapanimuse[peds[i]] = 0;
				}
				if (pedmapeuph.find(peds[i]) == pedmapeuph.end())
				{
					//status: 0 = euphoria stumbing disabled, 1 = euphoria stumbling enabled
					int rand = 0 + (std::rand() % (99 - 0 + 1));
					if (rand < ini_euphoriastumblechance) pedmapeuph[peds[i]] = 1;
					else pedmapeuph[peds[i]] = 0;
				}
				if (pedmapisfalling.find(peds[i]) == pedmapisfalling.end())
				{
					//status: 0 = default value
					pedmapisfalling[peds[i]] = 0;
				}
				if (pedmapfalltime.find(peds[i]) == pedmapfalltime.end())
				{
					//status: 0 = default value
					pedmapfalltime[peds[i]] = 0;
				}
				if (pedmapkill.find(peds[i]) == pedmapkill.end())
				{
					//status: 0 = default value, 1 = to be killed
					pedmapkill[peds[i]] = 0;
				}
				if (pedmapdown.find(peds[i]) == pedmapdown.end())
				{
					//status: 0 = default value, 1 = to be downed
					pedmapdown[peds[i]] = 0;
				}
				if (pedmaptorsohit.find(peds[i]) == pedmaptorsohit.end())
				{
					//status: 0 = was not hit in the torso, 1 = was hit in the torso
					pedmaptorsohit[peds[i]] = 0;
				}
			}

			//set damage modifiers
			if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(player) || PLAYER::IS_PLAYER_FREE_AIMING(player))
			{
				//knife bool for checking if knife is being used
				bool knife = false;
				//special weapon bool for checking if a special weapon is being used
				bool specialweapon = false;
				//special ammo bool for checking if special ammo is being used
				bool specialammo = false;
				std::map<int, Hash>::iterator w_it = weaponmap.begin();
				std::map<Hash, Hash>::iterator a_it = ammomap.begin();

				//check if knife is used
				for (vector<char*>::size_type it = 0; it != knives.size(); it++)
				{
					if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, GAMEPLAY::GET_HASH_KEY(knives[it]), 0, 0))
					{
						Hash weaponhash;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, false, 0, false))
						{
							if (weaponhash == GAMEPLAY::GET_HASH_KEY(knives[it])) knife = true;
						}
					}
				}

				//check if special weapon is used
				while (w_it != weaponmap.end())
				{
					int index = w_it->first;
					Hash weapon = w_it->second;
					if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, weapon, 0, 0))
					{
						Hash weaponhash;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, false, 0, false))
						{
							if (weaponhash == weapon) specialweapon = true;
						}
					}
					w_it++;
				}

				//check if special ammo is used
				while (a_it != ammomap.end())
				{
					Hash ammo = a_it->first;
					Hash weapon = a_it->second;
					if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, weapon, 0, 0))
					{
						Hash weaponhash;
						if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, false, 0, false))
						{
							if (weaponhash == weapon)
							{
								if (WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weapon) == ammo) specialammo = true;
							}
						}
					}
					a_it++;
				}

				//check if player is aiming at non human NPC or special weapon or ammo is used and set the damage modifier accordingly
				Ped playertarget;
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(player, &playertarget) || PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playertarget))
				{
					if (!PED::IS_PED_HUMAN(playertarget)) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 1.0);
				}
				else if (specialweapon || specialammo) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, ini_sweaponmodifier);
				else if (knife) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, ini_knifemodifier);
				else
				{
					PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, ini_weaponmodifier);
					PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, ini_meleemodifier);
				}
			}


			//iterate through NPCs, check them for stuff and apply stuff (main function of this mod)
			for (int i = 0; i < count; i++)
			{
				//bool for checking if current NPC is a story character
				bool isstorychar = false;
				if (ini_excludestorynpcs == 1)
				{
					isstorychar = PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_johnmarston")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_MicahBell"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_dutch")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_lenny"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_hoseamatthews")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_leostrauss"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_rainsfall")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_javierescuella"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_billwilliamson")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_uncle"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_sean")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_charlessmith"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_EagleFlies")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_PAYTAH"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_hercule")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_leon"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_mrsadler")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_kieran"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_mrpearson")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_JAMIE"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_Jules")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_bronte"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_revswanson")) || PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_MARSHALL_THURWELL"))
						|| PED::IS_PED_MODEL(peds[i], GAMEPLAY::GET_HASH_KEY("CS_LARAMIE"));
				}

				//set health of story characters, if set in the ini
				if (isstorychar && ini_excludestorynpcs == 1)
				{
					if (ini_storypedhealth > 0 && PED::GET_PED_MAX_HEALTH(peds[i]) != ini_storypedhealth)
					{
						PED::SET_PED_MAX_HEALTH(peds[i], ini_storypedhealth);
						ENTITY::SET_ENTITY_HEALTH(peds[i], ini_storypedhealth, 0);
					}
				}

				//if NPC is a non-story-char, is not dead and not the player himself, do stuff
				if (peds[i] != playerPed && !isstorychar && PED::IS_PED_HUMAN(peds[i]) && !ENTITY::IS_ENTITY_DEAD(peds[i]))
				{
					/*
					ENTITY::SET_ENTITY_CAN_BE_DAMAGED_BY_RELATIONSHIP_GROUP(peds[i], true, PED::GET_RELATIONSHIP_BETWEEN_PEDS(playerPed, peds[i]));
					ENTITY::SET_ENTITY_CAN_BE_DAMAGED(peds[i], true);
					OBJECT::SET_OBJECT_TARGETTABLE(peds[i], true);
					PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(peds[i], player, true);
					PED::SET_PED_CAN_BE_TARGETTED(peds[i], true);
					*/

					//check if NPC is submerged in water
					bool submerged = false;
					if (ENTITY::GET_ENTITY_SUBMERGED_LEVEL(peds[i]) > 0.37f) submerged = true;

					//PED::x_0x8B3B71C80A29A4BB(peds[i], "pain_1", "face_human@gen_male@base");
					//PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(peds[i], "pain_1", "face_human@gen_male@base");
					//if (PED::IS_PED_MALE(peds[i]))
					//{
					//	PED::x_0x8B3B71C80A29A4BB(peds[i], "pain_1", "face_human@gen_male@pain");
					//	PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(peds[i], "pain_1", "face_human@gen_male@pain");
					//}
					//else
					//{
					//	PED::x_0x8B3B71C80A29A4BB(peds[i], "pain_1", "face_human@gen_female@pain");
					//	PED::SET_FACIAL_IDLE_ANIM_OVERRIDE(peds[i], "pain_1", "face_human@gen_female@pain");
					//}


					//check if NPC has been damaged by a weapon (needed for the bleeding effect)
					bool hasbeendamagedbyweapon = false;
					map<int, map<int, char*>>::iterator mapit;
					for (auto mapit = allweaponmap.begin(); mapit != allweaponmap.end(); mapit++)
					{
						int weapontype = mapit->first;
						char* weaponname = mapit->second;
						if (WEAPON::_0xDCF06D0CDFF68424(peds[i], GAMEPLAY::GET_HASH_KEY(weaponname), 0)) hasbeendamagedbyweapon = true;
					}

					//set accuracy for NPCs
					if (ini_pedaccuracy > 0) PED::SET_PED_ACCURACY(peds[i], ini_pedaccuracy);

					//handle the falling of an NPC (so it wont survive falls from unrealistic heights)
					if (PED::IS_PED_FALLING(peds[i]))
					{
						pedmapisfalling[peds[i]] = 1;
						if (pedmapfalltime[peds[i]] == 0) pedmapfalltime[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
						else if (GAMEPLAY::GET_GAME_TIMER() > pedmapfalltime[peds[i]] + ini_falltimedeath) pedmapkill[peds[i]] = 1;
						else if (GAMEPLAY::GET_GAME_TIMER() > pedmapfalltime[peds[i]] + ini_falltimedown) pedmapdown[peds[i]] = 1;
					}
					else
					{
						pedmapisfalling[peds[i]] = 0;
						pedmapfalltime[peds[i]] = 0;
						if (pedmapkill[peds[i]] == 1)
						{
							if (!submerged) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
							pedmapkill[peds[i]] = 0;
						}
						else if (pedmapdown[peds[i]] == 1)
						{
							if (!submerged)
							{
								ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
								PED::SET_PED_TO_RAGDOLL(peds[i], 2000, 2000, 0, false, false, false);
							}
							pedmapdown[peds[i]] = 0;
						}
					}

					//if the NPC does not have the specified health yet, set the health accordingly
					if ((PED::GET_PED_MAX_HEALTH(peds[i]) != ini_pedhealth) && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 4)
					{
						PED::SET_PED_MAX_HEALTH(peds[i], ini_pedhealth);
						ENTITY::SET_ENTITY_HEALTH(peds[i], ini_pedhealth, 0);
						PED::SET_PED_MAX_TIME_UNDERWATER(peds[i], ini_underwatertime);
					}

					//disarming by catching someone with a lasso
					if (ini_lassodisarm == 1)
					{
						//making sure player has lasso weapon and it is equipped
						if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, lassoHash, 0, 0))
						{
							Hash lassoHash2;
							if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &lassoHash2, false, 0, false))
							{
								if (lassoHash == lassoHash2)
								{
									//check if an NPC exists at which the player is shooting the lasso
									if (lastTarget != playerPed)
									{
										WEAPON::REMOVE_ALL_PED_WEAPONS(lastTarget, true, true);
										AI::_0xFD45175A6DFD7CE9(lastTarget, playerPed, 4, 0, -1.0f, -1, 0); //fleeing

										//behavior randomizer
										int rand = 0 + (std::rand() % (99 - 0 + 1));
										if (rand < 5)
										{
											AI::TASK_COWER(lastTarget, 5, 1, 0);
											pedmaphealth[lastTarget] = ENTITY::GET_ENTITY_HEALTH(lastTarget);
										}
										else if (rand < 10)
										{
											AI::TASK_HANDS_UP(lastTarget, 5, playerPed, 0, false);
											pedmaphealth[lastTarget] = ENTITY::GET_ENTITY_HEALTH(lastTarget);
										}
										else if (rand < 55)
										{
											AI::_0xFD45175A6DFD7CE9(lastTarget, playerPed, 4, 0, -1.0f, -1, 0);
										}
										else pedmaphealth[lastTarget] = ENTITY::GET_ENTITY_HEALTH(lastTarget); //remember health for later use
									}

									//get specific NPC, the player is shooting the lasso at
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
					}

					//disarming by hogtying someone
					if (ini_hogtiedisarm == 1)
					{
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 399))
						{
							WEAPON::REMOVE_ALL_PED_WEAPONS(peds[i], true, true);
							AI::_0xFD45175A6DFD7CE9(peds[i], playerPed, 4, 0, -1.0f, -1, 0); //fleeing

							//behavior randomizer
							int rand = 0 + (std::rand() % (99 - 0 + 1));
							if (rand < 5)
							{
								AI::TASK_COWER(peds[i], 5, 1, 0);
								pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
							}
							else if (rand < 10)
							{
								AI::TASK_HANDS_UP(peds[i], 5, playerPed, 0, false);
								pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
							}
							else if (rand < 55)
							{
								AI::_0xFD45175A6DFD7CE9(peds[i], playerPed, 4, 0, -1.0f, -1, 0); //fleeing
							}
							else pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]); //remember health for later use
						}
					}

					//if the NPC has been shot somewhere special, set the status accordingly and create proper behavior
					int actBone;
					if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &actBone))
					{
						//determining if NPC was hit in the torso
						for (vector<int>::size_type it = 0; it != torsobones.size(); it++)
						{
							if (actBone == torsobones[it]) pedmaptorsohit[peds[i]] = 1;
						}

						//disarming and according behavior
						int randdis = 0 + (std::rand() % (99 - 0 + 1));
						if (randdis < ini_disarmingchance)
						{
							if ((actBone == 11300 || actBone == 16749 || actBone == 16781 || actBone == 54187 || actBone == 22798) && !WEAPON::_0xBDD9C235D8D1052E(peds[i]))
							{
								Hash pedweaphash;
								Vector3 vec = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], actBone));
								WEAPON::GET_CURRENT_PED_WEAPON(peds[i], &pedweaphash, false, 0, false);
								WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(peds[i], pedweaphash, vec.x, vec.y, vec.z, 30);

								//behavior randomizer
								int rand = 0 + (std::rand() % (99 - 0 + 1));
								if (rand < ini_disarmcowerchance)
								{
									AI::TASK_COWER(peds[i], -1, 1, 0);
									pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
								}
								else if (rand < ini_disarmcowerchance + ini_disarmhandsupchance)
								{
									AI::TASK_HANDS_UP(peds[i], 5, playerPed, 0, false);
									pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
								}
								else if (rand < ini_disarmcowerchance + ini_disarmhandsupchance + ini_disarmfleeingchance)
								{
									AI::_0xFD45175A6DFD7CE9(peds[i], playerPed, 4, 0, -1.0f, -1, 0); //fleeing
								}
								else pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]); //remember health for later use
							}
						}

						//right leg
						if (actBone == 43312 || actBone == 6884)
						{
							if (pedmapstat[peds[i]] == 0) pedmapstat[peds[i]] = 8;
							else if (pedmapstat[peds[i]] == 3) pedmapstat[peds[i]] = 9;
							else if (pedmapstat[peds[i]] == 6) pedmapstat[peds[i]] = 1;
							else if (pedmapstat[peds[i]] == 7) pedmapstat[peds[i]] = 2;
						}
						//left leg
						else if (actBone == 65478 || actBone == 55120)
						{
							if (pedmapstat[peds[i]] == 0) pedmapstat[peds[i]] = 6;
							else if (pedmapstat[peds[i]] == 3) pedmapstat[peds[i]] = 7;
							else if (pedmapstat[peds[i]] == 8) pedmapstat[peds[i]] = 1;
							else if (pedmapstat[peds[i]] == 9) pedmapstat[peds[i]] = 2;
						}
					}

					//if NPC has lost health since it has been flagged ("remember health for later use"), create according behavior
					if (ini_disarmingchance > 0)
					{
						if (pedmaphealth[peds[i]] != 0)
						{
							if (pedmaphealth[peds[i]] > (ENTITY::GET_ENTITY_HEALTH(peds[i]) + 15))
							{
								AI::_0xFD45175A6DFD7CE9(peds[i], playerPed, 4, 0, -1.0f, -1, 0); //fleeing
								pedmaphealth[peds[i]] = 0;
							}
							else if (pedmaphealth[peds[i]] > (ENTITY::GET_ENTITY_HEALTH(peds[i]) + 5))
							{
								AI::_0xFD45175A6DFD7CE9(peds[i], playerPed, 3, 0, -1.0f, -1, 0); //fleeing
								pedmaphealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
							}
						}
					}


					//panic when hogtied and attacked with knife (if randomizer decides to)
					if (AI::GET_IS_TASK_ACTIVE(peds[i], 399) && !FIRE::IS_ENTITY_ON_FIRE(peds[i]))
					{
						Hash actweaphash;
						WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &actweaphash, false, 0, false);
						for (vector<char*>::size_type it = 0; it != knives.size(); it++)
						{
							if (actweaphash == GAMEPLAY::GET_HASH_KEY(knives[it]))
							{
								Entity playerTarget;
								if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
								{
									if (playerTarget == peds[i] && pedmaplimbhealth[peds[i]] > ENTITY::GET_ENTITY_HEALTH(peds[i]))
									{
										int painaudiorand = 0 + (std::rand() % (99 - 0 + 1));
										if (painaudiorand < 20)	AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
									}
								}
							}
						}
					}

					//panic when in dying state and attacked (if randomizer decides to)
					if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && ENTITY::GET_ENTITY_HEALTH(peds[i]) > ini_dyingthreshold&& pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5)
					{
						Entity playerTarget;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
						{
							if (playerTarget == peds[i] && pedmaplimbhealth[peds[i]] > (ENTITY::GET_ENTITY_HEALTH(peds[i]) + ini_bleedingvalue))
							{
								if (PED::IS_PED_RAGDOLL(peds[i]))
								{
									int painaudiorand = 0 + (std::rand() % (99999 - 0 + 1));
									Hash actweaphash;
									WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &actweaphash, false, 0, false);
									for (vector<char*>::size_type it = 0; it != knives.size(); it++)
									{
										if (actweaphash == GAMEPLAY::GET_HASH_KEY(knives[it]))
										{
											if (painaudiorand < 30)	AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
										}
										else if (painaudiorand < ini_panicchancerag) AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
									}
								}
								else
								{
									int painaudiorand = 0 + (std::rand() % (99999 - 0 + 1));
									if (painaudiorand < ini_panicchance)
									{
										AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
										AI::_0xFD45175A6DFD7CE9(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
									}
								}
							}
						}
					}

					//if NPC has the specified health and was not burning before, check stuff and do stuff
					if (PED::GET_PED_MAX_HEALTH(peds[i]) == ini_pedhealth && pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5)
					{
						//bool for checking if NPC is in a state in which ragdolling him doesnt break anything
						bool pedisready = PED::IS_PED_RAGDOLL(peds[i]) && !FIRE::IS_ENTITY_ON_FIRE(peds[i]) && !PED::IS_PED_FALLING(peds[i]) && !PED::IS_PED_HANGING_ON_TO_VEHICLE(peds[i]); //&& !AI::IS_PED_GETTING_UP(peds[i]);

						//randomizer for different pain sounds
						int painaudio = 0 + (std::rand() % (12 - 0 + 1));
						if (painaudio < 3) painaudio = 4;
						else if (painaudio < 7) painaudio = 5;
						else painaudio = 10;

						//randomizer for chance of pain sound playing
						int painaudiorand = 0 + (std::rand() % (999 - 0 + 1));

						//checking if NPC has been disarmed
						bool disarmed = false;
						if (ini_disarmingchance > 0)
						{
							bool disarmed = false;
							int actBonex;
							if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &actBonex))
							{
								if (actBonex == 11300 || actBonex == 16749 || actBonex == 16781 || actBonex == 54187 || actBonex == 22798) disarmed = true;
							}
						}

						//position of ground for NPC bone checks - is NPC on ground?
						Vector3 vechead = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 21030));
						Vector3 vechip = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 14415));
						Vector3 vecleg1 = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 43312));
						Vector3 vecleg2 = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 55120));
						float groundzcoord, groundzcoordhip, groundzcoordarm1, groundzcoordarm2, groundzcoordleg1, groundzcoordleg2;
						GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vechead.x, vechead.y, vechead.z, &groundzcoord, true);
						GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vechip.x, vechip.y, vechip.z, &groundzcoordhip, true);
						GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecleg1.x, vecleg1.y, vecleg1.z, &groundzcoordleg1, true);
						GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecleg2.x, vecleg2.y, vecleg2.z, &groundzcoordleg2, true);

						//Dying states
						if (pedmapdyingstatebool[peds[i]])
						{
							//if DyingStates should apply to melee damage as well (-> people going into dying states after fist fights), set bool accordingly
							bool xhasbeendamagedbyweapon;
							if (ini_dyingstatesmelee) xhasbeendamagedbyweapon = true;
							else xhasbeendamagedbyweapon = hasbeendamagedbyweapon;

							//if NPC is weak enough and not dead yet, make him move in DyingMovement stage 1 and play pain sounds randomly
							if (xhasbeendamagedbyweapon && ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && ENTITY::GET_ENTITY_HEALTH(peds[i]) > ini_dyingmovementthreshold2&& pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 99)
							{
								//making sure melee knockout victims stay down
								if (pedmapko[peds[i]] != 0 && (pedmapko[peds[i]] == 1 || pedmapko[peds[i]] == 2))
								{
									pedmapko[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + 20000;
								}
								else if (GAMEPLAY::GET_GAME_TIMER() > pedmapko[peds[i]] && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);

								//if NPC is not underwater, stop it from speaking (feels weird when they are down and still taunt the player like nothing happened)
								if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);

								//remembering health for pain reactions
								if (pedmaphealthds[peds[i]] == 0) pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);

								//transition into ragdoll from fighting, horse, cover, etc.
								if ((PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && ini_meleedyingstateknockout))
								{
									AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false);
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);
									pedmapko[peds[i]] = 1;
								}
								else if (PED::IS_PED_ON_MOUNT(peds[i]) || PED::IS_PED_SITTING_IN_ANY_VEHICLE(peds[i]) || PED::IS_PED_IN_ANY_VEHICLE(peds[i], true)
									|| PED::IS_PED_IN_ANY_VEHICLE(peds[i], false) || PED::IS_PED_ON_VEHICLE(peds[i], false) || PED::IS_PED_ON_VEHICLE(peds[i], true))
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 3, false, false, false);
								else if (PED::IS_PED_IN_COVER(peds[i], false, false)) PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);

								if (pedisready)
								{
									//keep NPC on the ground
									PED::RESET_PED_RAGDOLL_TIMER(peds[i]);

									//ensure reactiveness of NPC
									//PED::SET_PED_CAN_ARM_IK(peds[i], true);
									//PED::SET_PED_CAN_HEAD_IK(peds[i], true);
									//PED::SET_PED_CAN_LEG_IK(peds[i], true);
									//PED::SET_PED_CAN_TORSO_IK(peds[i], true);

									//pain reaction
									if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < pedmaphealthds[peds[i]])
									{
										PedPain(peds[i]);
										pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}

									//special hogtie fix for writhing animations
									if (ini_specialhogtie)
									{
										if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, lassoHash, 0, 0))
										{
											Hash lassoHash2;
											if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &lassoHash2, false, 0, false))
											{
												if (lassoHash == lassoHash2)
												{
													if (lastTargetx != playerPed)
													{
														PED::SET_ENABLE_HANDCUFFS(peds[i], true, false);
														PED::SET_ENABLE_BOUND_ANKLES(peds[i], true);
														OBJECT::SET_OBJECT_TARGETTABLE(peds[i], true);
													}
													if (PED::IS_PED_SHOOTING(playerPed))
													{
														Entity playerTarget;
														if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
														{
															lastTargetx = playerTarget;
														}
													}
												}
											}
										}
									}

									//disarming
									if (ini_disarmingchance > 0)
									{
										Hash pedweaphash;
										Vector3 vec = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 11300));
										WEAPON::GET_CURRENT_PED_WEAPON(peds[i], &pedweaphash, false, 0, false);
										WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(peds[i], pedweaphash, vec.x, vec.y, vec.z, 30);
									}

									//dying push (pushing NPC so it falls to the ground, if it is not on the ground already)
									int rand = 0 + (std::rand() % (999 - 0 + 1));
									if (rand < ini_dyingpushchance && vechead.z > groundzcoord + ini_zvaluehead)
									{
										if (pedmapeuph[peds[i]] == 1) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, ini_dforcepushy, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
										else PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
									}
									else
									{
										//push when in dying state (simulating movement)
										if (vechead.z < groundzcoord + ini_zvaluehead && vechip.z < groundzcoordhip + ini_zvaluehip && vecleg1.z < groundzcoordleg1 + ini_zvaluehip && vecleg2.z < groundzcoordleg2 + ini_zvaluehip)
										{
											//if this is the first time entering this loop, create writhing behavior
											if (pedmapanimtrans[peds[i]] == -1) pedmapanimtrans[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
											else if (GAMEPLAY::GET_GAME_TIMER() > pedmapanimtrans[peds[i]] + ini_transdelta)
											{
												//transitioning into writhing animations
												if (!pedmapdyingstate1entered[peds[i]] && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
												}
												else if (pedmapdyingstate1entered[peds[i]] && pedmapko[peds[i]] == 1 && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmapko[peds[i]] = 2;
												}
												pedmapdyingstate1entered[peds[i]] = true;
											}

											//set things up for the first "pushing cycle"
											if (pedmaptime[peds[i]] == -1 && rand < ini_dyingmovementchance && ENTITY::GET_ENTITY_SPEED(peds[i]) < ini_dyingmovementenablespeed)
											{
												pedmaptime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_dyingmovementbuilduptime + ini_dyingmovementpushtime;
												pedmapdosomething[peds[i]] = 1;
												int randaud = 0 + (std::rand() % (99 - 0 + 1));
												int randaud2 = 0 + (std::rand() % (99 - 0 + 1));
												int painaudio = 2 + (std::rand() % (12 - 2 + 1));
												if (randaud < 30) painaudio = ini_painaudio01;
												else if (randaud < 60) painaudio = ini_painaudio02;
												else if (randaud < 90) painaudio = ini_painaudio03;
												else painaudio = 5;
												if (randaud2 < ini_dmaudiochance && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
												int randdm = 0 + (std::rand() % (99 - 0 + 1));
												if (randdm < 50) pedmaprememb2[peds[i]] = 1;
												else pedmaprememb2[peds[i]] = -1;
												int randdmx = 0 + (std::rand() % (99 - 0 + 1));
												if (randdmx < 50) pedmaprememb[peds[i]] = 1;
												else pedmaprememb[peds[i]] = -1;
												int randdmxx = 0 + (std::rand() % (99 - 0 + 1));
												if (randdmxx < ini_xrollchance) pedmaprememb3[peds[i]] = 1;
												else pedmaprememb3[peds[i]] = -1;
												pedmapbupushtime[peds[i]] = ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount;
												if (PED::IS_PED_MALE(peds[i]))
												{
													pedmappushes[peds[i]]["dmforcepushxleg01"] = ini_dmforcepushxleg01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg02"] = ini_dmforcepushxleg02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg03"] = ini_dmforcepushxleg03 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg04"] = ini_dmforcepushxleg04 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxarm01"] = ini_dmforcepushxarm01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxarm02"] = ini_dmforcepushxarm02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine01"] = ini_dmforcepushxspine01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine02"] = ini_dmforcepushxspine02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine03"] = ini_dmforcepushxspine03 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine04"] = ini_dmforcepushxspine04 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine05"] = ini_dmforcepushxspine05 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck01"] = ini_dmforcepushxneck01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck02"] = ini_dmforcepushxneck02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck03"] = ini_dmforcepushxneck03 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck04"] = ini_dmforcepushxneck04 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck05"] = ini_dmforcepushxneck05 / ini_dyingmovementbuildupcount;
												}
												else
												{
													pedmappushes[peds[i]]["dmforcepushxleg01"] = ini_dmforcepushxleg01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg02"] = ini_dmforcepushxleg02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg03"] = ini_dmforcepushxleg03 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg04"] = ini_dmforcepushxleg04 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxarm01"] = ini_dmforcepushxarm01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxarm02"] = ini_dmforcepushxarm02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine01"] = ini_dmforcepushxspine01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine02"] = ini_dmforcepushxspine02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine03"] = ini_dmforcepushxspine03 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine04"] = ini_dmforcepushxspine04 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine05"] = ini_dmforcepushxspine05 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck01"] = ini_dmforcepushxneck01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck02"] = ini_dmforcepushxneck02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck03"] = ini_dmforcepushxneck03 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck04"] = ini_dmforcepushxneck04 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck05"] = ini_dmforcepushxneck05 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;
												}
											}

											//force pushes
											if (pedmapdosomething[peds[i]] == 1)
											{
												if (GAMEPLAY::GET_GAME_TIMER() < (pedmaptime[peds[i]] - ini_dyingmovementbuilduptime - ini_dyingmovementpushtime + pedmapbupushtime[peds[i]]))
												{
													//build up push
													if (pedmapbupushtime[peds[i]] < (ini_dyingmovementbuilduptime - (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)))
													{
														if (pedmaprememb3[peds[i]] == 1)
														{ //x roll
															//left calf
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxleg03"] * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_CALF"), true, true, 0, 0, 0);
															//left thigh
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxleg04"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Thigh"), true, true, 0, 0, 0);
															//right calf
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxleg01"] * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Calf"), true, true, 0, 0, 0);
															//right thigh
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxleg02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Thigh"), true, true, 0, 0, 0);
															//left forearm
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxarm01"] * pedmaprememb[peds[i]], -1.5, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Forearm"), true, true, 0, 0, 0);
															//right forearm
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxarm02"] * pedmaprememb[peds[i]], -1.5, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Forearm"), true, true, 0, 0, 0);
															//left finger
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxarm01"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Finger32"), true, true, 0, 0, 0);
															//right finger
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxarm02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Finger32"), true, true, 0, 0, 0);
															//left upper arm
															//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxarm01"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 37873), true, true, 0, 0, 0);
															//right upper arm
															//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxarm02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 46065), true, true, 0, 0, 0);
															//spine01
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxspine01"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine1"), true, true, 0, 0, 0);
															//spine02
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxspine02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine2"), true, true, 0, 0, 0);
															//spine03
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxspine03"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine3"), true, true, 0, 0, 0);
															//spine04
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxspine04"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine4"), true, true, 0, 0, 0);
															//spine05
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxspine05"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine5"), true, true, 0, 0, 0);
															//neck01
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxneck01"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck1"), true, true, 0, 0, 0);
															//neck02
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxneck02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck2"), true, true, 0, 0, 0);
															//neck03
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxneck03"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck3"), true, true, 0, 0, 0);
															//neck04
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxneck04"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck4"), true, true, 0, 0, 0);
															//neck05
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["dmforcepushxneck05"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck5"), true, 0, 0, 0, 0);
														}
														else
														{ //z roll
															if (ini_dyingmovementlegs == 1)
															{
																if (pedmaprememb2[peds[i]] == 1)
																{
																	//left calf
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg03 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg03 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg03 * (-1), ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_CALF"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
																	//left thigh
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg04 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg04 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg04, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Thigh"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
																}
																else if (pedmaprememb2[peds[i]] == -1)
																{
																	//right calf
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg01 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg01 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg01 * (-1), ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Calf"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
																	//right thigh
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg02 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg02 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Thigh"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
																}
															}
															if (ini_dyingmovementarms == 1)
															{
																if (pedmaprememb2[peds[i]] == 1)
																{
																	//left forearm
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm01, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Forearm"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																	//left finger
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm01, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Finger32"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																	//left upper arm
																	//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm01* pedmaprememb[peds[i]], ini_dyingmovementpushyarm01* pedmaprememb[peds[i]], ini_dyingmovementpushzarm01, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 37873), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																}
																else if (pedmaprememb2[peds[i]] == -1)
																{
																	//right forearm
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Forearm"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																	//right finger
																	ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Finger32"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																	//right upper arm
																	//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm02* pedmaprememb[peds[i]], ini_dyingmovementpushyarm02* pedmaprememb[peds[i]], ini_dyingmovementpushzarm02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 46065), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																}
															}
															if (ini_dyingmovementspine == 1)
															{
																//spine01
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine01 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine01 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine01 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine1"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
																//spine02
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine02 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine02 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine02 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine2"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
																//spine03
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine03 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine03 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine03 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine3"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
																//spine04
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine04 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine04 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine04 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine4"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
																//spine05
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine05 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine05 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine05 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine5"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
															}
															if (ini_dyingmovementneck == 1)
															{
																//neck01
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck01 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck01 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck01 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck1"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
																//neck02
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck02 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck02 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck02 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck2"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
																//neck03
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck03 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck03 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck03 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck3"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
																//neck04
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck04 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck04 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck04 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck4"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
																//neck05
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck05 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck05 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck05 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck5"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
															}
														}
													}
												}
												else if ((GAMEPLAY::GET_GAME_TIMER() < (pedmaptime[peds[i]] - ini_dyingmovementpushtime)))
												{
													if (PED::IS_PED_MALE(peds[i]))
													{
														pedmappushes[peds[i]]["dmforcepushxleg01"] += ini_dmforcepushxleg01 / ini_dyingmovementbuildupcount;// (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxleg02"] += ini_dmforcepushxleg02 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxleg02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxleg03"] += ini_dmforcepushxleg03 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxleg03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxleg04"] += ini_dmforcepushxleg04 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxleg04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxarm01"] += ini_dmforcepushxarm01 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxarm01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxarm02"] += ini_dmforcepushxarm02 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxarm02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck01"] += ini_dmforcepushxneck01 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck02"] += ini_dmforcepushxneck02 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck03"] += ini_dmforcepushxneck03 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck04"] += ini_dmforcepushxneck04 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck05"] += ini_dmforcepushxneck05 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine01"] += ini_dmforcepushxspine01 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine02"] += ini_dmforcepushxspine02 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine03"] += ini_dmforcepushxspine03 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine04"] += ini_dmforcepushxspine04 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine05"] += ini_dmforcepushxspine05 / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
													}
													else
													{
														pedmappushes[peds[i]]["dmforcepushxleg01"] += ini_dmforcepushxleg01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;// (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxleg02"] += ini_dmforcepushxleg02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxleg02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxleg03"] += ini_dmforcepushxleg03 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxleg03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxleg04"] += ini_dmforcepushxleg04 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxleg04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxarm01"] += ini_dmforcepushxarm01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxarm01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxarm02"] += ini_dmforcepushxarm02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxarm02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck01"] += ini_dmforcepushxneck01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck02"] += ini_dmforcepushxneck02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck03"] += ini_dmforcepushxneck03 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck04"] += ini_dmforcepushxneck04 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxneck05"] += ini_dmforcepushxneck05 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxneck05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine01"] += ini_dmforcepushxspine01 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine02"] += ini_dmforcepushxspine02 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine03"] += ini_dmforcepushxspine03 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine04"] += ini_dmforcepushxspine04 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["dmforcepushxspine05"] += ini_dmforcepushxspine05 / ini_femalepushmodifier / ini_dyingmovementbuildupcount;//(ini_dmforcepushxspine05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
													}
													pedmapbupushtime[peds[i]] = pedmapbupushtime[peds[i]] + (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount);
													if (pedmapbupushtime[peds[i]] > ini_dyingmovementbuilduptime) pedmapbupushtime[peds[i]] = ini_dyingmovementbuilduptime;
												}
												else if ((GAMEPLAY::GET_GAME_TIMER() >= (pedmaptime[peds[i]] - ini_dyingmovementpushtime)) && GAMEPLAY::GET_GAME_TIMER() < pedmaptime[peds[i]])
												{
													//non build up push (full push)
													if (pedmaprememb3[peds[i]] == 1)
													{ //x roll
														//left calf
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxleg03 * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_CALF"), true, true, 0, 0, 0);
														//left thigh
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxleg04 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Thigh"), true, true, 0, 0, 0);
														//right calf
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxleg01 * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Calf"), true, true, 0, 0, 0);
														//right thigh
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxleg02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Thigh"), true, true, 0, 0, 0);
														//left forearm
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxarm01 * pedmaprememb[peds[i]], -1.5, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Forearm"), true, true, 0, 0, 0);
														//right forearm
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxarm02 * pedmaprememb[peds[i]], -1.5, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Forearm"), true, true, 0, 0, 0);
														//left finger
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxarm01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Finger32"), true, true, 0, 0, 0);
														//right finger
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxarm02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Finger32"), true, true, 0, 0, 0);
														//left upper arm
														//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxarm01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 37873), true, true, 0, 0, 0);
														//right upper arm
														//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxarm02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 46065), true, true, 0, 0, 0);
														//spine01
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxspine01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine1"), true, true, 0, 0, 0);
														//spine02
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxspine02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine2"), true, true, 0, 0, 0);
														//spine03
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxspine03 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine3"), true, true, 0, 0, 0);
														//spine04
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxspine04 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine4"), true, true, 0, 0, 0);
														//spine05
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxspine05 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine5"), true, true, 0, 0, 0);
														//neck01
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxneck01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck1"), true, true, 0, 0, 0);
														//neck02
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxneck02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck2"), true, true, 0, 0, 0);
														//neck03
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxneck03 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck3"), true, true, 0, 0, 0);
														//neck04
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxneck04 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck4"), true, true, 0, 0, 0);
														//neck05
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dmforcepushxneck05 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck5"), true, 0, 0, 0, 0);
													}
													else
													{ //z roll
														if (ini_dyingmovementlegs == 1)
														{
															if (pedmaprememb2[peds[i]] == 1)
															{
																//left calf
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg03 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg03 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg03 * (-1), ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_CALF"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
																//left thigh
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg04 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg04 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg04, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Thigh"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
															}
															else if (pedmaprememb2[peds[i]] == -1)
															{
																//right calf
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg01 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg01 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg01 * (-1), ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Calf"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
																//right thigh
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxleg02 * pedmaprememb[peds[i]], ini_dyingmovementpushyleg02 * pedmaprememb[peds[i]], ini_dyingmovementpushzleg02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Thigh"), ini_dyingmovementdirrelleg, ini_dyingmovementignoreupvec, ini_dyingmovementforrelleg, 0, 0);
															}
														}
														if (ini_dyingmovementarms == 1)
														{
															if (pedmaprememb2[peds[i]] == 1)
															{
																//left forearm
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm01, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Forearm"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																//left finger
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm01, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Finger32"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																//left upper arm
																//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm01 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm01, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 37873), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
															}
															else if (pedmaprememb2[peds[i]] == -1)
															{
																//right forearm
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Forearm"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																//right finger
																ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushyarm02 * pedmaprememb[peds[i]], ini_dyingmovementpushzarm02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Finger32"), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
																//right upper arm
																//ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxarm02* pedmaprememb[peds[i]], ini_dyingmovementpushyarm02* pedmaprememb[peds[i]], ini_dyingmovementpushzarm02, ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, PED::GET_PED_BONE_INDEX(peds[i], 46065), ini_dyingmovementdirrelarm, ini_dyingmovementignoreupvec, ini_dyingmovementforrelarm, 0, 0);
															}
														}
														if (ini_dyingmovementspine == 1)
														{
															//spine01
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine01 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine01 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine01 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine1"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
															//spine02
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine02 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine02 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine02 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine2"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
															//spine03
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine03 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine03 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine03 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine3"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
															//spine04
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine04 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine04 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine04 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine4"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
															//spine05
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxspine05 * pedmaprememb[peds[i]], ini_dyingmovementpushyspine05 * pedmaprememb[peds[i]], ini_dyingmovementpushzspine05 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine5"), ini_dyingmovementdirrelspine, ini_dyingmovementignoreupvec, ini_dyingmovementforrelspine, 0, 0);
														}
														if (ini_dyingmovementneck == 1)
														{
															//neck01
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck01 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck01 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck01 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck1"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
															//neck02
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck02 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck02 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck02 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck2"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
															//neck03
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck03 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck03 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck03 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck3"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
															//neck04
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck04 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck04 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck04 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck4"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
															//neck05
															ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_dyingmovementpushxneck05 * pedmaprememb[peds[i]], ini_dyingmovementpushyneck05 * pedmaprememb[peds[i]], ini_dyingmovementpushzneck05 * pedmaprememb[peds[i]], ini_dforceoffx, ini_dforceoffy, ini_dforceoffz, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck5"), ini_dyingmovementdirrelneck, ini_dyingmovementignoreupvec, ini_dyingmovementforrelneck, 0, 0);
														}
													}
												}
												else
												{
													pedmaptime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_dyingmovementwaittime;
													pedmapdosomething[peds[i]] = 0;
												}
											}
											else if (pedmapdosomething[peds[i]] == 0)
											{
												//adjust things for next "pushing cycle"
												if (GAMEPLAY::GET_GAME_TIMER() > pedmaptime[peds[i]] && rand < ini_dyingmovementchance && ENTITY::GET_ENTITY_SPEED(peds[i]) < ini_dyingmovementenablespeed)
												{
													pedmaptime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_dyingmovementbuilduptime + ini_dyingmovementpushtime;
													pedmapdosomething[peds[i]] = 1;
													int randaud = 0 + (std::rand() % (99 - 0 + 1));
													int randaud2 = 0 + (std::rand() % (99 - 0 + 1));
													int painaudio = 2 + (std::rand() % (12 - 2 + 1));
													if (randaud < 45) painaudio = ini_painaudio01;
													else if (randaud < 90) painaudio = ini_painaudio02;
													else painaudio = ini_painaudio03;
													if (randaud2 < ini_dmaudiochance && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
													int randdm = 0 + (std::rand() % (99 - 0 + 1));
													if (randdm < 50) pedmaprememb2[peds[i]] = 1;
													else pedmaprememb2[peds[i]] = -1;
													int randdmx = 0 + (std::rand() % (99 - 0 + 1));
													if (randdmx < 50) pedmaprememb[peds[i]] = 1;
													else pedmaprememb[peds[i]] = -1;
													int randdmxx = 0 + (std::rand() % (99 - 0 + 1));
													if (randdmxx < ini_xrollchance) pedmaprememb3[peds[i]] = 1;
													else pedmaprememb3[peds[i]] = -1;
													pedmapbupushtime[peds[i]] = ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg01"] = ini_dmforcepushxleg01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg02"] = ini_dmforcepushxleg02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg03"] = ini_dmforcepushxleg03 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxleg04"] = ini_dmforcepushxleg04 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxarm01"] = ini_dmforcepushxarm01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxarm02"] = ini_dmforcepushxarm02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine01"] = ini_dmforcepushxspine01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine02"] = ini_dmforcepushxspine02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine03"] = ini_dmforcepushxspine03 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine04"] = ini_dmforcepushxspine04 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxspine05"] = ini_dmforcepushxspine05 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck01"] = ini_dmforcepushxneck01 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck02"] = ini_dmforcepushxneck02 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck03"] = ini_dmforcepushxneck03 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck04"] = ini_dmforcepushxneck04 / ini_dyingmovementbuildupcount;
													pedmappushes[peds[i]]["dmforcepushxneck05"] = ini_dmforcepushxneck05 / ini_dyingmovementbuildupcount;
												}
											}
										}
									}
								}
								//bleeding
								if (ini_onlybleedshottorso == 1)
								{
									if (pedmaptorsohit[peds[i]] == 1)
									{
										if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
										{
											int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
											if (bleedrand < ini_bleedingchance_dying)
											{
												if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 5) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
												else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
												pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
											}
										}
										else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}
								}
								else if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
								{
									int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
									if (bleedrand < ini_bleedingchance_dying)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 5) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
										pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}
								}
								else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
							}

							//if NPC is weak enough and not dead yet, make him move in DyingMovement stage 2 and play pain sounds randomly
							if (xhasbeendamagedbyweapon && ENTITY::GET_ENTITY_HEALTH(peds[i]) <= ini_dyingmovementthreshold2 && ENTITY::GET_ENTITY_HEALTH(peds[i]) > ini_dyingthreshold&& pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 99)
							{
								//remembering health for pain reactions
								if (pedmaphealthds[peds[i]] == 0) pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);

								//if NPC is not underwater, stop it from speaking (feels weird when they are down and still taunt the player like nothing happened)
								if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);

								//transition into ragdoll from fighting, horse, cover, etc.
								if (PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && ini_meleedyingstateknockout && pedmapko[peds[i]] == 0)
								{
									AI::CLEAR_PED_TASKS(peds[i], false, false);
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 3, false, false, false);
								}
								else if (PED::IS_PED_ON_MOUNT(peds[i]) || PED::IS_PED_SITTING_IN_ANY_VEHICLE(peds[i]) || PED::IS_PED_IN_ANY_VEHICLE(peds[i], true)
									|| PED::IS_PED_IN_ANY_VEHICLE(peds[i], false) || PED::IS_PED_ON_VEHICLE(peds[i], false) || PED::IS_PED_ON_VEHICLE(peds[i], true))
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 3, false, false, false);
								else if (PED::IS_PED_IN_COVER(peds[i], false, false)) PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);

								if (pedisready)
								{
									//keep NPC on the ground
									PED::RESET_PED_RAGDOLL_TIMER(peds[i]);

									//ensure reactiveness of NPC
									//PED::SET_PED_CAN_ARM_IK(peds[i], true);
									//PED::SET_PED_CAN_HEAD_IK(peds[i], true);
									//PED::SET_PED_CAN_LEG_IK(peds[i], true);
									//PED::SET_PED_CAN_TORSO_IK(peds[i], true);

									//pain reaction
									if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < pedmaphealthds[peds[i]])
									{
										PedPain(peds[i]);
										pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}

									//special hogtie fix for writhing animations
									if (ini_specialhogtie)
									{
										if (WEAPON::HAS_PED_GOT_WEAPON(playerPed, lassoHash, 0, 0))
										{
											Hash lassoHash2;
											if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &lassoHash2, false, 0, false))
											{
												if (lassoHash == lassoHash2)
												{
													if (lastTargetx != playerPed)
													{
														PED::SET_ENABLE_HANDCUFFS(peds[i], true, false);
														PED::SET_ENABLE_BOUND_ANKLES(peds[i], true);
														OBJECT::SET_OBJECT_TARGETTABLE(peds[i], true);
													}
													if (PED::IS_PED_SHOOTING(playerPed))
													{
														Entity playerTarget;
														if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
														{
															lastTargetx = playerTarget;
														}
													}
												}
											}
										}
									}

									//disarming
									if (ini_disarmingchance > 0)
									{
										Hash pedweaphash;
										Vector3 vec = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 11300));
										WEAPON::GET_CURRENT_PED_WEAPON(peds[i], &pedweaphash, false, 0, false);
										WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(peds[i], pedweaphash, vec.x, vec.y, vec.z, 30);
									}

									//dying push (pushing NPC so it falls to the ground, if it is not on the ground already)
									int rand = 0 + (std::rand() % (999 - 0 + 1));
									if (rand < ini_dyingpushchance && vechead.z > groundzcoord + ini_zvaluehead)
									{
										if (pedmapeuph[peds[i]] == 1) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, ini_dforcepushy, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
										else PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
									}
									else
									{
										//push when in dying state (simulating movement)
										if (vechead.z < groundzcoord + ini_zvaluehead && vechip.z < groundzcoordhip + ini_zvaluehip && vecleg1.z < groundzcoordleg1 + ini_zvaluehip && vecleg2.z < groundzcoordleg2 + ini_zvaluehip)
										{
											//if this is the first time entering this loop, create writhing behavior
											if (!pedmapdyingstate2entered[peds[i]])
											{
												pedmapdyingstate2entered[peds[i]] = true;
												pedmaptime[peds[i]] = -2;

												//transitioning into writhing animations
												if (!pedmapdyingstate1entered[peds[i]] && !pedmapdyingstate2entered[peds[i]] && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
												}
											}

											//set things up for the first "pushing cycle"
											if (pedmaptime[peds[i]] == -2 && rand < ini_bdyingmovementchance && ENTITY::GET_ENTITY_SPEED(peds[i]) < ini_dyingmovementenablespeed)
											{
												pedmaptime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_bdyingmovementbuilduptime + ini_bdyingmovementpushtime;
												pedmapdosomething[peds[i]] = 1;
												int randaud = 0 + (std::rand() % (99 - 0 + 1));
												int randaud2 = 0 + (std::rand() % (99 - 0 + 1));
												int painaudio = 2 + (std::rand() % (12 - 2 + 1));
												if (randaud < 90) painaudio = ini_painaudio01;
												else painaudio = ini_painaudio03;
												if (randaud2 < ini_dmaudiochance && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
												int randdm = 0 + (std::rand() % (99 - 0 + 1));
												if (randdm < 50) pedmaprememb2[peds[i]] = 1;
												else pedmaprememb2[peds[i]] = -1;
												int randdmx = 0 + (std::rand() % (99 - 0 + 1));
												if (randdmx < 50) pedmaprememb[peds[i]] = 1;
												else pedmaprememb[peds[i]] = -1;
												pedmapbupushtime[peds[i]] = ini_bdyingmovementbuilduptime / ini_bdyingmovementbuildupcount;
												if (PED::IS_PED_MALE(peds[i]))
												{
													pedmappushes[peds[i]]["bdmforcepushxleg01"] = ini_bdmforcepushxleg01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg02"] = ini_bdmforcepushxleg02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg03"] = ini_bdmforcepushxleg03 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg04"] = ini_bdmforcepushxleg04 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxarm01"] = ini_bdmforcepushxarm01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxarm02"] = ini_bdmforcepushxarm02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine01"] = ini_bdmforcepushxspine01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine02"] = ini_bdmforcepushxspine02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine03"] = ini_bdmforcepushxspine03 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine04"] = ini_bdmforcepushxspine04 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine05"] = ini_bdmforcepushxspine05 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck01"] = ini_bdmforcepushxneck01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck02"] = ini_bdmforcepushxneck02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck03"] = ini_bdmforcepushxneck03 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck04"] = ini_bdmforcepushxneck04 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck05"] = ini_bdmforcepushxneck05 / ini_bdyingmovementbuildupcount;
												}
												else
												{
													pedmappushes[peds[i]]["bdmforcepushxleg01"] = ini_bdmforcepushxleg01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg02"] = ini_bdmforcepushxleg02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg03"] = ini_bdmforcepushxleg03 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg04"] = ini_bdmforcepushxleg04 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxarm01"] = ini_bdmforcepushxarm01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxarm02"] = ini_bdmforcepushxarm02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine01"] = ini_bdmforcepushxspine01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine02"] = ini_bdmforcepushxspine02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine03"] = ini_bdmforcepushxspine03 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine04"] = ini_bdmforcepushxspine04 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine05"] = ini_bdmforcepushxspine05 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck01"] = ini_bdmforcepushxneck01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck02"] = ini_bdmforcepushxneck02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck03"] = ini_bdmforcepushxneck03 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck04"] = ini_bdmforcepushxneck04 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck05"] = ini_bdmforcepushxneck05 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;
												}
											}

											//force pushes
											if (pedmapdosomething[peds[i]] == 1)
											{
												if (GAMEPLAY::GET_GAME_TIMER() < (pedmaptime[peds[i]] - ini_bdyingmovementbuilduptime - ini_bdyingmovementpushtime + pedmapbupushtime[peds[i]]))
												{
													//build up push
													if (pedmapbupushtime[peds[i]] < (ini_bdyingmovementbuilduptime - (ini_bdyingmovementbuilduptime / ini_bdyingmovementbuildupcount)))
													{
														//left calf
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxleg03"] * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_CALF"), true, true, 0, 0, 0);
														//left thigh
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxleg04"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Thigh"), true, true, 0, 0, 0);
														//right calf
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxleg01"] * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Calf"), true, true, 0, 0, 0);
														//right thigh
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxleg02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Thigh"), true, true, 0, 0, 0);
														//left forearm
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxarm01"] * pedmaprememb[peds[i]], -1.5, pedmappushes[peds[i]]["bdmforcepushxarm01"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Forearm"), true, true, 0, 0, 0);
														//right forearm
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxarm02"] * pedmaprememb[peds[i]], -1.5, pedmappushes[peds[i]]["bdmforcepushxarm02"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Forearm"), true, true, 0, 0, 0);
														//left finger
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxarm01"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxarm01"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Finger32"), true, true, 0, 0, 0);
														//right finger
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxarm02"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxarm02"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Finger32"), true, true, 0, 0, 0);
														//left upper arm
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxarm01"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxarm01"] * pedmaprememb[peds[i]], 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 37873), true, true, 0, 0, 0);
														//right upper arm
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxarm02"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxarm02"] * pedmaprememb[peds[i]], 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 46065), true, true, 0, 0, 0);
														//spine01
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxspine01"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxspine01"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine1"), true, true, 0, 0, 0);
														//spine02
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxspine02"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxspine02"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine2"), true, true, 0, 0, 0);
														//spine03
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxspine03"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxspine03"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine3"), true, true, 0, 0, 0);
														//spine04
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxspine04"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxspine04"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine4"), true, true, 0, 0, 0);
														//spine05
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxspine05"] * pedmaprememb[peds[i]], 0, pedmappushes[peds[i]]["bdmforcepushxspine05"] * pedmaprememb[peds[i]], 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine5"), true, true, 0, 0, 0);
														//neck01
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxneck01"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck1"), true, true, 0, 0, 0);
														//neck02
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxneck02"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck2"), true, true, 0, 0, 0);
														//neck03
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxneck03"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck3"), true, true, 0, 0, 0);
														//neck04
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxneck04"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck4"), true, true, 0, 0, 0);
														//neck05
														ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, pedmappushes[peds[i]]["bdmforcepushxneck05"] * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck5"), true, 0, 0, 0, 0);
													}
												}
												else if ((GAMEPLAY::GET_GAME_TIMER() < (pedmaptime[peds[i]] - ini_bdyingmovementpushtime)))
												{
													if (PED::IS_PED_MALE(peds[i]))
													{
														pedmappushes[peds[i]]["bdmforcepushxleg01"] += ini_bdmforcepushxleg01 / ini_bdyingmovementbuildupcount;// (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxleg02"] += ini_bdmforcepushxleg02 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxleg02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxleg03"] += ini_bdmforcepushxleg03 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxleg03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxleg04"] += ini_bdmforcepushxleg04 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxleg04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxarm01"] += ini_bdmforcepushxarm01 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxarm01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxarm02"] += ini_bdmforcepushxarm02 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxarm02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck01"] += ini_bdmforcepushxneck01 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck02"] += ini_bdmforcepushxneck02 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck03"] += ini_bdmforcepushxneck03 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck04"] += ini_bdmforcepushxneck04 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck05"] += ini_bdmforcepushxneck05 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine01"] += ini_bdmforcepushxspine01 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine02"] += ini_bdmforcepushxspine02 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine03"] += ini_bdmforcepushxspine03 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine04"] += ini_bdmforcepushxspine04 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine05"] += ini_bdmforcepushxspine05 / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
													}
													else
													{
														pedmappushes[peds[i]]["bdmforcepushxleg01"] += ini_bdmforcepushxleg01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;// (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxleg02"] += ini_bdmforcepushxleg02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxleg02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxleg03"] += ini_bdmforcepushxleg03 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxleg03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxleg04"] += ini_bdmforcepushxleg04 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxleg04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxarm01"] += ini_bdmforcepushxarm01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxarm01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxarm02"] += ini_bdmforcepushxarm02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxarm02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck01"] += ini_bdmforcepushxneck01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck02"] += ini_bdmforcepushxneck02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck03"] += ini_bdmforcepushxneck03 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck04"] += ini_bdmforcepushxneck04 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxneck05"] += ini_bdmforcepushxneck05 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxneck05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine01"] += ini_bdmforcepushxspine01 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine01 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine02"] += ini_bdmforcepushxspine02 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine02 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine03"] += ini_bdmforcepushxspine03 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine03 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine04"] += ini_bdmforcepushxspine04 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine04 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
														pedmappushes[peds[i]]["bdmforcepushxspine05"] += ini_bdmforcepushxspine05 / ini_femalepushmodifier / ini_bdyingmovementbuildupcount;//(ini_dmforcepushxspine05 / (pedmapbupushtime[peds[i]] / (ini_dyingmovementbuilduptime / ini_dyingmovementbuildupcount)));
													}
													pedmapbupushtime[peds[i]] = pedmapbupushtime[peds[i]] + (ini_bdyingmovementbuilduptime / ini_bdyingmovementbuildupcount);
													if (pedmapbupushtime[peds[i]] > ini_bdyingmovementbuilduptime) pedmapbupushtime[peds[i]] = ini_bdyingmovementbuilduptime;
												}
												else if ((GAMEPLAY::GET_GAME_TIMER() >= (pedmaptime[peds[i]] - ini_bdyingmovementpushtime)) && GAMEPLAY::GET_GAME_TIMER() < pedmaptime[peds[i]])
												{
													//non build up push (full push)
													//left calf
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxleg03 * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_CALF"), true, true, 0, 0, 0);
													//left thigh
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxleg04 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Thigh"), true, true, 0, 0, 0);
													//right calf
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxleg01 * pedmaprememb[peds[i]] * (-1), 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Calf"), true, true, 0, 0, 0);
													//right thigh
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxleg02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Thigh"), true, true, 0, 0, 0);
													//left forearm
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxarm01 * pedmaprememb[peds[i]], -1.5, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Forearm"), true, true, 0, 0, 0);
													//right forearm
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxarm02 * pedmaprememb[peds[i]], -1.5, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Forearm"), true, true, 0, 0, 0);
													//left finger
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxarm01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_L_Finger32"), true, true, 0, 0, 0);
													//right finger
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxarm02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_R_Finger32"), true, true, 0, 0, 0);
													//left upper arm
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxarm01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 37873), true, true, 0, 0, 0);
													//right upper arm
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxarm02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, PED::GET_PED_BONE_INDEX(peds[i], 46065), true, true, 0, 0, 0);
													//spine01
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxspine01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine1"), true, true, 0, 0, 0);
													//spine02
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxspine02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine2"), true, true, 0, 0, 0);
													//spine03
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxspine03 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine3"), true, true, 0, 0, 0);
													//spine04
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxspine04 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine4"), true, true, 0, 0, 0);
													//spine05
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxspine05 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Spine5"), true, true, 0, 0, 0);
													//neck01
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxneck01 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck1"), true, true, 0, 0, 0);
													//neck02
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxneck02 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck2"), true, true, 0, 0, 0);
													//neck03
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxneck03 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck3"), true, true, 0, 0, 0);
													//neck04
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxneck04 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck4"), true, true, 0, 0, 0);
													//neck05
													ENTITY::APPLY_FORCE_TO_ENTITY(peds[i], 1, ini_bdmforcepushxneck05 * pedmaprememb[peds[i]], 0, 0, 1, 1, 1, ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(peds[i], "SKEL_Neck5"), true, 0, 0, 0, 0);
												}
												else
												{
													pedmaptime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_bdyingmovementwaittime;
													pedmapdosomething[peds[i]] = 0;
												}
											}
											else if (pedmapdosomething[peds[i]] == 0)
											{
												//adjust things for next "pushing cycle"
												if (GAMEPLAY::GET_GAME_TIMER() > pedmaptime[peds[i]] && rand < ini_bdyingmovementchance && ENTITY::GET_ENTITY_SPEED(peds[i]) < ini_dyingmovementenablespeed)
												{
													pedmaptime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_bdyingmovementbuilduptime + ini_bdyingmovementpushtime;
													pedmapdosomething[peds[i]] = 1;
													int randaud = 0 + (std::rand() % (99 - 0 + 1));
													int randaud2 = 0 + (std::rand() % (99 - 0 + 1));
													int painaudio = 2 + (std::rand() % (12 - 2 + 1));
													if (randaud < 90) painaudio = ini_painaudio01;
													else painaudio = ini_painaudio03;
													if (randaud2 < ini_dmaudiochance && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
													int randdm = 0 + (std::rand() % (99 - 0 + 1));
													if (randdm < 50) pedmaprememb2[peds[i]] = 1;
													else pedmaprememb2[peds[i]] = -1;
													int randdmx = 0 + (std::rand() % (99 - 0 + 1));
													if (randdmx < 50) pedmaprememb[peds[i]] = 1;
													else pedmaprememb[peds[i]] = -1;
													pedmapbupushtime[peds[i]] = ini_bdyingmovementbuilduptime / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg01"] = ini_bdmforcepushxleg01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg02"] = ini_bdmforcepushxleg02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg03"] = ini_bdmforcepushxleg03 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxleg04"] = ini_bdmforcepushxleg04 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxarm01"] = ini_bdmforcepushxarm01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxarm02"] = ini_bdmforcepushxarm02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine01"] = ini_bdmforcepushxspine01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine02"] = ini_bdmforcepushxspine02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine03"] = ini_bdmforcepushxspine03 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine04"] = ini_bdmforcepushxspine04 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxspine05"] = ini_bdmforcepushxspine05 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck01"] = ini_bdmforcepushxneck01 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck02"] = ini_bdmforcepushxneck02 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck03"] = ini_bdmforcepushxneck03 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck04"] = ini_bdmforcepushxneck04 / ini_bdyingmovementbuildupcount;
													pedmappushes[peds[i]]["bdmforcepushxneck05"] = ini_bdmforcepushxneck05 / ini_bdyingmovementbuildupcount;
												}
											}
										}
									}
								}
								//bleeding
								if (ini_onlybleedshottorso == 1)
								{
									if (pedmaptorsohit[peds[i]] == 1)
									{
										if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
										{
											int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
											if (bleedrand < ini_bleedingchance_dying)
											{
												if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 5) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
												else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
												pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
											}
										}
										else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}
								}
								else if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
								{
									int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
									if (bleedrand < ini_bleedingchance_dying)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 5) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
										pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}
								}
								else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
							}

							//if NPC is weak enough and not dead yet (last dying stage), set him to ragdoll indefinitely and play pain sounds randomly
							if (xhasbeendamagedbyweapon && ENTITY::GET_ENTITY_HEALTH(peds[i]) <= ini_dyingthreshold && pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 99)
							{
								if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);

								//if this is the first time entering this loop, create writhing behavior
								if (!pedmapdyingstate3entered[peds[i]])
								{
									pedmapdyingstate3entered[peds[i]] = true;
									AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC
								}

								if (pedisready)
								{
									//keep NPC on the ground
									PED::RESET_PED_RAGDOLL_TIMER(peds[i]);

									//ensure reactiveness of NPC
									PED::SET_PED_CAN_ARM_IK(peds[i], true);
									PED::SET_PED_CAN_HEAD_IK(peds[i], true);
									PED::SET_PED_CAN_LEG_IK(peds[i], true);
									PED::SET_PED_CAN_TORSO_IK(peds[i], true);

									//pain reaction
									if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < pedmaphealthds[peds[i]])
									{
										PedPain(peds[i]);
										pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}

									//disarming
									if (ini_disarmingchance > 0)
									{
										Hash pedweaphash;
										Vector3 vec = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 11300));
										WEAPON::GET_CURRENT_PED_WEAPON(peds[i], &pedweaphash, false, 0, false);
										WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(peds[i], pedweaphash, vec.x, vec.y, vec.z, 30);
									}

									//force pushes and pain sounds
									if (painaudiorand < 3 && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
									int rand = 0 + (std::rand() % (999 - 0 + 1));
									if (rand < ini_dyingpushchance && vechead.z > groundzcoord + ini_zvaluehead)
									{
										int randxyz = 0 + (std::rand() % (99 - 0 + 1));
										if (randxyz < 33) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, 0, 0, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
										else if (randxyz < 66) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, ini_dforcepushy, 0, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
										else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, 0, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									}

								}
								//bleeding
								if (ini_onlybleedshottorso == 1)
								{
									if (pedmaptorsohit[peds[i]] == 1)
									{
										if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
										{
											int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
											if (bleedrand < ini_bleedingchance_dying)
											{
												if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 5) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
												else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
												pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
											}
										}
										else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}
								}
								else if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
								{
									int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
									if (bleedrand < ini_bleedingchance_dying)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 5) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
										pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									}
								}
								else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
							}
						}

						//if NPC can be knocked down, compute if he is being knocked down now (randomly when hit)
						else if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_painthreshold && !(ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold) && pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 99 && !disarmed)
						{
							int painrand = 0 + (std::rand() % (999 - 0 + 1));
							int painaudio = 9;
							if (pedisready)
							{
								if (painrand < ini_knockbackchance)
								{
									PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
									if (painaudiorand < 5 && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
								}
								int rand = 0 + (std::rand() % (999 - 0 + 1));
								if (rand < ini_dyingpushchance && vechead.z > groundzcoord + ini_zvaluehead)
								{
									int randxyz = 0 + (std::rand() % (99 - 0 + 1));
									if (randxyz < 33) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, 0, 0, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									else if (randxyz < 66) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, ini_dforcepushy, 0, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, 0, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
								}
							}
							//bleeding
							if (ini_onlybleedshottorso == 1)
							{
								if (pedmaptorsohit[peds[i]] == 1)
								{
									if (ini_bleedwhenshot == 1 && hasbeendamagedbyweapon)
									{
										int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
										if (bleedrand < ini_bleedingchance_shot)
										{
											ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
											pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
										}
									}
									else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
								}
							}
							else if (ini_bleedwhenshot == 1 && hasbeendamagedbyweapon)
							{
								int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
								if (bleedrand < ini_bleedingchance_shot)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
									pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
								}
							}
							else pedmaphealthds[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
						}
					}

					//check stuff and do stuff (fire- and stumble/fall-related)
					//if NPC was burning and is not anymore, create according behavior
					if ((pedmapstat[peds[i]] == 4 || pedmapstat[peds[i]] == 5) && !FIRE::IS_ENTITY_ON_FIRE(peds[i]))
					{
						if (PED::GET_PED_MAX_HEALTH(peds[i]) != ini_firehealth)
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_firehealth);
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
						}
						if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);
						if (PED::IS_PED_RAGDOLL(peds[i])) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
						else PED::SET_PED_TO_RAGDOLL(peds[i], 1000, 1000, 3, false, false, false);
						int painrand = 0 + (std::rand() % (999 - 0 + 1));
						int painaudio = 9;
						if (painrand < 5 && !submerged) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
						if (ini_bleedwhendying == 1 && hasbeendamagedbyweapon)
						{
							int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
							if (bleedrand < ini_bleedingchance_dying)
							{
								if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < 2) ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
								else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - ini_bleedingvalue, 0);
							}
						}
					}
					//randomize if NPC can survive fire and set the status accordingly
					int survivfir = 0 + (std::rand() % (99 - 0 + 1));
					if (survivfir < ini_survivfirchance)
					{
						if (pedmapstat[peds[i]] == 1) pedmapstat[peds[i]] = 2;
						else if (pedmapstat[peds[i]] == 0) pedmapstat[peds[i]] = 3;
						else if (pedmapstat[peds[i]] == 6) pedmapstat[peds[i]] = 7;
						else if (pedmapstat[peds[i]] == 8) pedmapstat[peds[i]] = 9;
					}
					else
					{
						if (pedmapstat[peds[i]] == 2) pedmapstat[peds[i]] = 1;
						else if (pedmapstat[peds[i]] == 3) pedmapstat[peds[i]] = 0;
						else if (pedmapstat[peds[i]] == 7) pedmapstat[peds[i]] = 6;
						else if (pedmapstat[peds[i]] == 9) pedmapstat[peds[i]] = 8;
					}
					//if NPC should survive fire, make sure it does, otherwise make sure it doesnt
					if (FIRE::IS_ENTITY_ON_FIRE(peds[i]))
					{
						if (pedmapstat[peds[i]] == 2 || pedmapstat[peds[i]] == 3 || pedmapstat[peds[i]] == 4 || pedmapstat[peds[i]] == 7 || pedmapstat[peds[i]] == 9)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], 800, 0);
							//if (ENTITY::GET_ENTITY_HEALTH(peds[i]) > ini_dyingthreshold - 1) ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
							pedmapstat[peds[i]] = 4;
						}
						else
						{
							if (pedmapstat[peds[i]] != 5)
							{
								if (PED::GET_PED_MAX_HEALTH(peds[i]) != ini_firehealth)
								{
									PED::SET_PED_MAX_HEALTH(peds[i], ini_firehealth);
									ENTITY::SET_ENTITY_HEALTH(peds[i], ini_firehealth, 0);
								}
								pedmapstat[peds[i]] = 5;
							}
						}
					}

					//check if NPC should stumble and create according behavior
					//both legs damaged
					if ((pedmapstat[peds[i]] == 1 || pedmapstat[peds[i]] == 2) && !PED::IS_PED_RAGDOLL(peds[i]) && !AI::IS_PED_GETTING_UP(peds[i]) && (AI::IS_PED_WALKING(peds[i]) || AI::IS_PED_RUNNING(peds[i]) || AI::IS_PED_SPRINTING(peds[i]) || PED::IS_PED_GOING_INTO_COVER(peds[i])) && !PED::IS_PED_ON_MOUNT(peds[i]))
					{
						int fallrand = 0 + (std::rand() % (999 - 0 + 1));
						if (fallrand < ini_stumblechancebothlegs && ENTITY::GET_ENTITY_SPEED(peds[i]) > runningthresholdbothlegs)
						{
							PED::SET_PED_TO_RAGDOLL(peds[i], 1000, 1000, 3, false, false, false);
							ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_forcepushxboth, ini_forcepushyboth, ini_forcepushzboth, false, ini_dirrelbothlegs, ini_forrelbothlegs, false);
							PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
						}
					}
					//one leg damaged
					else if ((pedmapstat[peds[i]] == 6 || pedmapstat[peds[i]] == 7 || pedmapstat[peds[i]] == 8 || pedmapstat[peds[i]] == 9) && (AI::IS_PED_RUNNING(peds[i]) || AI::IS_PED_SPRINTING(peds[i]) || PED::IS_PED_GOING_INTO_COVER(peds[i])) && !PED::IS_PED_RAGDOLL(peds[i]) && !AI::IS_PED_GETTING_UP(peds[i]) && !PED::IS_PED_ON_MOUNT(peds[i]))
					{
						int fallrand = 0 + (std::rand() % (999 - 0 + 1));
						if (fallrand < ini_stumblechanceoneleg && ENTITY::GET_ENTITY_SPEED(peds[i]) > runningthresholdoneleg)
						{
							PED::SET_PED_TO_RAGDOLL(peds[i], 1000, 1000, 3, false, false, false);
							ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_forcepushx, ini_forcepushy, ini_forcepushz, false, ini_dirreloneleg, ini_forreloneleg, false);
							PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
						}
					}

					//checking limb damage and giving back health
					if (pedmaplimbhealth[peds[i]] > ENTITY::GET_ENTITY_HEALTH(peds[i]) && pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 99)
					{
						int damagedone = pedmaplimbhealth[peds[i]] - ENTITY::GET_ENTITY_HEALTH(peds[i]);
						int act_Bone;
						if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &act_Bone))
						{
							for (vector<int>::size_type it = 0; it != legbones.size(); it++)
							{
								if (act_Bone == legbones[it])
								{
									int healthback = (int)(damagedone * ini_legdamagemod);
									if (ini_legdamagemod < 1)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && (healthback + ENTITY::GET_ENTITY_HEALTH(peds[i])) > ini_dyingmovementthreshold) ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) + healthback, 0);
									}
									else if (ini_legdamagemod > 1)
									{
										float dam = ini_legdamagemod - 1;
										dam = damagedone * dam;
										ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - dam, 0);
									}
									pedmaplimbhealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									PED::CLEAR_PED_LAST_DAMAGE_BONE(peds[i]);
								}
							}

							for (vector<int>::size_type it = 0; it != armbones.size(); it++)
							{
								if (act_Bone == armbones[it])
								{
									int healthback = (int)(damagedone * ini_armdamagemod);
									if (ini_armdamagemod < 1)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && (healthback + ENTITY::GET_ENTITY_HEALTH(peds[i])) > ini_dyingmovementthreshold) ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) + healthback, 0);
									}
									else if (ini_armdamagemod > 1)
									{
										float dam = ini_armdamagemod - 1;
										dam = damagedone * dam;
										ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - dam, 0);
									}
									pedmaplimbhealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									PED::CLEAR_PED_LAST_DAMAGE_BONE(peds[i]);
								}
							}

							for (vector<int>::size_type it = 0; it != torsobones.size(); it++)
							{
								if (act_Bone == torsobones[it])
								{
									int healthback = (int)(damagedone * ini_torsodamagemod);
									if (ini_torsodamagemod < 1)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && (healthback + ENTITY::GET_ENTITY_HEALTH(peds[i])) > ini_dyingmovementthreshold) ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) + healthback, 0);
									}
									else if (ini_torsodamagemod > 1)
									{
										float dam = ini_torsodamagemod - 1;
										dam = damagedone * dam;
										ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - dam, 0);
									}
									pedmaplimbhealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									PED::CLEAR_PED_LAST_DAMAGE_BONE(peds[i]);
								}
							}

							for (vector<int>::size_type it = 0; it != headbones.size(); it++)
							{
								if (act_Bone == headbones[it])
								{
									int healthback = (int)(damagedone * ini_headdamagemod);
									if (ini_headdamagemod < 1)
									{
										if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && (healthback + ENTITY::GET_ENTITY_HEALTH(peds[i])) > ini_dyingmovementthreshold) ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
										else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) + healthback, 0);
									}
									else if (ini_headdamagemod > 1)
									{
										float dam = ini_headdamagemod - 1;
										dam = damagedone * dam;
										ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) - dam, 0);
									}
									pedmaplimbhealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									PED::CLEAR_PED_LAST_DAMAGE_BONE(peds[i]);
								}
							}

							for (vector<int>::size_type it = 0; it != neckbones.size(); it++)
							{
								if (act_Bone == neckbones[it])
								{
									int healthback = (int)(damagedone * ini_neckdamagemod);
									if (ENTITY::GET_ENTITY_HEALTH(peds[i]) < ini_dyingmovementthreshold && (healthback + ENTITY::GET_ENTITY_HEALTH(peds[i])) > ini_dyingmovementthreshold) ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
									else ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_HEALTH(peds[i]) + healthback, 0);
									pedmaplimbhealth[peds[i]] = ENTITY::GET_ENTITY_HEALTH(peds[i]);
									PED::CLEAR_PED_LAST_DAMAGE_BONE(peds[i]);
								}
							}
						}
					}
				}
			}
		}

		//draw messages on screen
		if (GetTickCount() < msgTime)
		{
			if (modScriptState)
			{
				DrawText(0.4, 0.4, "PedDamageOverhaul has been ENABLED.");
				if (iniexists) DrawText(0.6, 0.6, "ini file found.");
				else DrawText(0.6, 0.6, "ini file not found.");
			}
			else DrawText(0.4, 0.4, "PedDamageOverhaul has been DISABLED.");
		}
		WAIT(1);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}