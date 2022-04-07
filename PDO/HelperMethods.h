#pragma once
#include "script.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <fstream>

using namespace std;

struct PedAttributes
{
	//container for flag if ped is a story character (0 is default, 1 is friendly story char (like a gang member), 2 is other story char (like the gunslingers), 3 is not any story char
	int pedmapisstorychar;
	//container for peds and their status (is burning, is shot in the leg, is shot in both legs, etc.)
	int pedmapstat;
	//container for peds and a flag for doing something (is used during the dying states to be able to stop the NPC from moving (when the value is 0))
	int pedmapdosomething;
	//container for peds and time passed since they have been doing something (is used during the dying states to be able to make NPCs move for a certain amount of time)
	int pedmaptime;
	//container for peds and their current health (for checking if damage was dealt - mostly used for reactions like "hands up" or cowering (if enough damage is done, NPCs will then flee))
	int pedmaphealth;
	//container for peds and a value to decide whether dying states kick in or vanilla behavior is triggered
	bool pedmapdyingstatebool;
	//container for peds and a value to calculate the limb specific damage
	int pedmaplimbhealth;
	//container for peds and a value for dying movement stage 1 (for remembering if it has been entered before)
	int pedmapdyingstate1entered;
	//container for peds and a value for dying movement stage 2 (for remembering if it has been entered before)
	int pedmapdyingstate2entered;
	//container for peds and a value for dying movement stage 3 (for remembering if it has been entered before)
	int pedmapdyingstate3entered;
	//container for peds and a value for animation transitioning in dying state 1
	int pedmapanimtrans;
	//container for ped health (for NPC pain reactions during dying states)
	int pedmaphealthds;
	//container for ped stumbling (euphoria enabled or not)
	int pedmapeuph;
	//container for ped animation use in dying states
	int pedmapanimuse;
	//container for remembering if ped was k.o.ed
	int pedmapko;
	//container for flag if ped is falling
	int pedmapisfalling;
	//container for remembering how long ped has been falling
	int pedmapfalltime;
	//container for flag if ped is to be killed
	int pedmapkill;
	//container for flag if ped is to be downed
	int pedmapdown;
	//container for flag if ped was hit in the torso
	int pedmaptorsohit;
	//container for the ped's accuracy
	int pedmapaccuracy;
	//container for flag if ped is down
	int pedmapisdown;
	//container for flag if ped is dead
	int pedmapisdead;
	//container for flag if ped is has reacted to other peds dying in combat
	int pedmaphasreacted;
	//container for bool if ped should bleed
	bool pedmapnobleeding;
	//container for time when ped was last on a mount
	int pedmaplasttimeonmount;
	//container for delaying the dying state effects, so euphoria motions can be fully executed
	int pedmapdsdelaytimer;
	//container for stumbling time
	int pedmapstumbling;
	//container for making sure peds don't stand still
	Vector3 pedmapheadvecdelta;
	int pedmapheadvectime;
	//container for last fear audio (to prevent repeating audio)
	int pedmaplastfearaudio;
	//container for how many times the right leg of an NPC was hit
	int pedmaprightleghitcounter;
	//container for how many times the left leg of an NPC was hit
	int pedmapleftleghitcounter;
	//container for damaged legs triggering falling down into dying states
	int pedmaplegsdamageddowntime;
	//container for timer of peds being saved from bleeing out (to not die from leg shots)
	int pedmaplegsdown;
	//container for timer (fire audio interval)
	int pedmapfireaudiotime;
	//container for last sound used (to prevent looping)
	int pedmapfireaudiosample;
	//container for bool if ped is bleeding from an artery shot
	bool pedmaparteryshot;
	//container for bool if ped has been aimed at when down
	bool pedmapwasaimedat;
	//container for bool if ped has been aimed at when down
	bool pedmaphasbeendamagedbyweapon;
	//container for bool if ped is hanging (TieYourLasso mod)
	bool pedmapishanging;
	//container for hanging time (TieYourLasso mod)
	int pedmaphangingtime;
	//container for the last time the ped was shot by the player
	int pedmapshotbyplayer;
	//container for bool if ped was fighting against the player alone
	bool pedmapwasnotaloneincombat;
	//container for bool if ped is in combat with player
	bool pedmapisincombatwithplayer;
	//container for when ped should be in hands-up task
	int pedmaphandsup;
	//container for the first time a ped was forced into "hands up"
	int pedmapfirsttimehandsup;
	//container for the information if a ped is or was armed and has been disarmed
	bool pedmapisarmed;
	bool pedmapwasdisarmed;
	//container for remembering which movement style ped is supposed to have
	char* pedmapmovement;
	//container for remembering if ped blip is set
	Blip pedmapblipset;
	//container for remembering when ped did a forced dismount (for surrendering)
	int pedmapdismount;
	//container for remembering if ped should be immune to disarms (only used for duels, so ingame disarms occur naturally)
	bool pedmapdisarmingstop;
};

struct ScriptedSpeechParams
{
	const char* speechName;
	const Any* voiceName;
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

//returns 3 if its neither a story- nor a friendly-NPC, returns 1 if its a friendly-NPC, returns 2 if its a story-NPC
int isPedStoryChar(Ped ped)
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
	pedmodels[22] = "CS_revswanson";
	pedmodels[23] = "CS_MARSHALL_THURWELL";
	pedmodels[24] = "CS_LARAMIE";
	pedmodels[25] = "CS_ArchieDown";
	pedmodels[26] = "CS_thomasdown";
	pedmodels[27] = "CS_EdithDown";
	pedmodels[28] = "CS_creolecaptain";
	pedmodels[29] = "CS_josiahtrelawny";

	std::map<int, char*>::iterator pedmodels_it = pedmodels.begin();
	
	//NPCs listed above
	while (pedmodels_it != pedmodels.end())
	{
		char* pedmodel = pedmodels_it->second;
		if (PED::IS_PED_MODEL(ped, GAMEPLAY::GET_HASH_KEY(pedmodel))) return 1;
		pedmodels_it++;
	}
	
	if (PED::IS_PED_MODEL(ped, 1045059103) || PED::IS_PED_MODEL(ped, 1765636093) || PED::IS_PED_MODEL(ped, 847448713) //NPCs from "Spines of America" mission
		|| PED::IS_PED_MODEL(ped, 3273604429) //Danbury from "American Fathers II" mission
		|| PED::IS_PED_MODEL(ped, 4196879928) || PED::IS_PED_MODEL(ped, 3736835937) || PED::IS_PED_MODEL(ped, 1347320453) //Gunslingers (Emmet Granger, Billy Midnight, Flaco Hernandez)
		|| PED::IS_PED_MODEL(ped, 2410820464) //Sheriff Malloy
		|| PED::IS_PED_MODEL(ped, GAMEPLAY::GET_HASH_KEY("CS_bronte")) || PED::IS_PED_MODEL(ped, GAMEPLAY::GET_HASH_KEY("U_M_M_BHT_BENEDICTALLBRIGHT")))
		return 2;

	return 3;
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