#pragma once
#include "script.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <fstream>

using namespace std;

struct ScriptedSpeechParams
{
	const char* speechName;
	const Any* voiceName;
	//const char* voiceName;
	alignas(8) int v3;
	alignas(8) Hash speechParamHash;
	alignas(8) Entity entity;
	alignas(8) BOOL v6;
	alignas(8) int v7;
	alignas(8) int v8;
};

bool DisarmPed(Ped ped, Vector3 weaponhandpos)
{
	Hash pedweaphash;
	bool ispedarmed = WEAPON::GET_CURRENT_PED_WEAPON(ped, &pedweaphash, false, 0, false);
	WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(ped, pedweaphash, weaponhandpos.x, weaponhandpos.y, weaponhandpos.z, 30);
	return ispedarmed;
}

bool isPedStoryChar(Ped ped)
{
	//container for ped model names
	std::map<int, char*> pedmodels;
	pedmodels[1] = "CS_johnmarston";
	pedmodels[2] = "CS_MicahBell";
	pedmodels[3] = "CS_dutch";
	pedmodels[4] = "CS_lenny";
	pedmodels[5] = "CS_hoseamatthews";
	pedmodels[6] = "CS_leostrauss";
	pedmodels[7] = "CS_rainsfall";
	pedmodels[8] = "CS_javierescuella";
	pedmodels[9] = "CS_billwilliamson";
	pedmodels[10] = "CS_uncle";
	pedmodels[11] = "CS_sean";
	pedmodels[12] = "CS_charlessmith";
	pedmodels[13] = "CS_EagleFlies";
	pedmodels[14] = "CS_PAYTAH";
	pedmodels[15] = "CS_hercule";
	pedmodels[16] = "CS_leon";
	pedmodels[17] = "CS_mrsadler";
	pedmodels[18] = "CS_kieran";
	pedmodels[19] = "CS_mrpearson";
	pedmodels[20] = "CS_JAMIE";
	pedmodels[21] = "CS_Jules";
	pedmodels[22] = "CS_bronte";
	pedmodels[23] = "CS_revswanson";
	pedmodels[24] = "CS_MARSHALL_THURWELL";
	pedmodels[25] = "CS_LARAMIE";
	pedmodels[26] = "CS_ArchieDown";
	pedmodels[27] = "CS_thomasdown";
	pedmodels[28] = "CS_EdithDown";
	pedmodels[29] = "U_M_M_BHT_BENEDICTALLBRIGHT";
	pedmodels[30] = "CS_creolecaptain";
	pedmodels[31] = "CS_josiahtrelawny";

	std::map<int, char*>::iterator pedmodels_it = pedmodels.begin();
	
	//Spines of America NPCs
	if (PED::IS_PED_MODEL(ped, 1045059103) || PED::IS_PED_MODEL(ped, 1765636093) || PED::IS_PED_MODEL(ped, 847448713)) return true;

	//NPCs listed above
	while (pedmodels_it != pedmodels.end())
	{
		char* pedmodel = pedmodels_it->second;
		if (PED::IS_PED_MODEL(ped, GAMEPLAY::GET_HASH_KEY(pedmodel))) return true;
		pedmodels_it++;
	}

	return false;
}

int PedFear(Ped ped, int mode, int lastfearaudio, int ds2chance) //returns last used speech line (key)
{	
	//container for speech lines
	std::map<int, char*> speech;
		
	//setting up the speech options
	if (mode == 1) //dying stage 1
	{
		speech[1] = "PANIC_HELP";
		speech[2] = "GENERIC_SHOCKED_HIGH";
		speech[3] = "SCARED_HELP";
		speech[4] = "GENERIC_CURSE_HIGH";
		//- speech[5] = "INTIMIDATED_GEN";
		speech[5] = "GENERIC_FRIGHTENED_HIGH";
		//-speech[7] = "GENERIC_AFRAID_REACTION";
		speech[6] = "SCREAM_SHOCKED";
		speech[7] = "SCREAM_TERROR";
		//speech[10] = "PLEAD";
		//speech[6] = "GUN_RUN";
		//speech[8] = "INTIMIDATED_AGAIN_GEN";
		//speech[9] = "GUN";
	}
	else if (mode == 2) //dying stage 2
	{
		speech[1] = "PANIC_HELP";
		speech[2] = "SCARED_HELP";
		speech[3] = "GENERIC_FRIGHTENED_HIGH";
		speech[4] = "SCREAM_SHOCKED";
		speech[5] = "SCREAM_TERROR";
		//speech[3] = "PLEAD";
	}
	else if (mode == 3)//only pleading, not used
	{
		speech[1] = "PLEAD";
	}
	else if (mode == 4) //screaming
	{
		speech[1] = "SCREAM_SHOCKED";
		speech[2] = "SCREAM_TERROR";
		speech[3] = "GENERIC_FRIGHTENED_HIGH";
		speech[4] = "GENERIC_SHOCKED_HIGH";
	}
	else if (mode == 5) //fire screaming
	{
		speech[1] = "SCREAM_SHOCKED";
		speech[2] = "SCREAM_TERROR";
	}

	std::map<int, char*>::iterator speechl_it = speech.begin();

	int linerand = 1 + (std::rand() % (speech.size() - 1 + 1));
	int linecounter = 1;

	while (linerand == lastfearaudio) linerand = 1 + (std::rand() % (speech.size() - 1 + 1));

	while (speechl_it != speech.end())
	{
		if (linerand == linecounter)
		{
			string speechline = speechl_it -> second;
			ScriptedSpeechParams params{ speechline.c_str(), 0, 1, 0x67F3AB43, 0, true, 1, 1 };
			if(!AUDIO::IS_ANY_SPEECH_PLAYING(ped)) 
			{
				int yrand = 0 + (std::rand() % (100 - 0 + 1));
				if (yrand > ds2chance && mode == 2) return lastfearaudio;
			
				if (AUDIO::x_PLAY_AMBIENT_SPEECH1(ped, (Any*)&params)) return linerand;
				else return lastfearaudio;			
			}
			else return lastfearaudio;
		}
		linecounter++;
		speechl_it++;
	}
}

void DrawText(float x, float y, char* str)
{
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

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