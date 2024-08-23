#pragma once
#include "script.h"
#include "Structs.h"

#include <vector>
#include <map>
#include <fstream>
#include <unordered_map>
#include <string>

void DrawText(float x, float y, char* str)
{
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

void DrawStatsOnScreen(int loopcounter, std::unordered_map<std::string, float>* iniValues, std::unordered_map<char, std::array<char, 60>*>* charsets, Player* player, std::unordered_map<std::string, Ped*>* pedInfo, std::unordered_map<std::string, std::string*>* textInfo, std::tr1::unordered_map<Ped, PedAttributes>* pedmap, std::unordered_map<std::string, int*>* miscInfo)
{
	//show ped health
	if ((int)((*iniValues)["ini_shownpchealth"]) == 1)
	{
		std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
		std::strcpy((char*)((*charsets)['d']), (*(*textInfo)["torso"]).c_str());
		std::strcpy((char*)((*charsets)['e']), (*(*textInfo)["weapondam"]).c_str());
		std::strcpy((char*)((*charsets)['f']), (*(*textInfo)["legshit"]).c_str());
		std::strcpy((char*)((*charsets)['g']), (*(*textInfo)["dyingbool"]).c_str());
		std::strcpy((char*)((*charsets)['h']), (*(*textInfo)["dshistory"]).c_str());
		std::strcpy((char*)((*charsets)['j']), (*(*textInfo)["lastdambone"]).c_str());
		DrawText(0.55, 0.05, (char*)((*charsets)['c']));
		DrawText(0.55, 0.18, (char*)((*charsets)['j']));
		DrawText(0.55, 0.31, (char*)((*charsets)['e']));
		DrawText(0.55, 0.44, (char*)((*charsets)['d']));
		DrawText(0.55, 0.57, (char*)((*charsets)['f']));
		DrawText(0.55, 0.70, (char*)((*charsets)['g']));
		DrawText(0.55, 0.83, (char*)((*charsets)['h']));

		if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT((*(*pedInfo)["playerPed"])))
		{
			Entity playerTargetTemp = *(*pedInfo)["playerTargetyolo"];
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(*player, (*pedInfo)["playerTargetyolo"]))
			{
				*(*pedInfo)["playerTargetOld"] = playerTargetTemp;
			}
		}
		if (*(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerPed"] && *(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerTargetyolo"])
		{
			*(*textInfo)["text"] = "NPC Health: ";
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
		}
		if (*(*pedInfo)["playerTargetyolo"] != *(*pedInfo)["playerPed"])
		{
			PED::GET_PED_LAST_DAMAGE_BONE(*(*pedInfo)["playerTargetyolo"], (*miscInfo)["lastbonedamagedtemp"]);
			if (*(*miscInfo)["lastbonedamagedtemp"] != 0) *(*miscInfo)["lastbonedamaged"] = *(*miscInfo)["lastbonedamagedtemp"];

			*(*textInfo)["text"] = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(*(*pedInfo)["playerTargetyolo"]));
			*(*textInfo)["torso"] = "Max HP/Use Max HP: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).maxhealth) + " / " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).usemaxhealth);
			*(*textInfo)["weapondam"] = "Damaged by weapon: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).hasbeendamagedbyweapon);
			*(*textInfo)["legshit"] = "Legs hit/Torso Hit: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).legsdownsurvivaltimer) + " / " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).torsohit);
			*(*textInfo)["dyingbool"] = "DS enabled: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstatebool);
			*(*textInfo)["dshistory"] = "DS visited (1, 2, 3): " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstate1entered) + ", " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstate2entered) + ", " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstate3entered);
			*(*textInfo)["lastdambone"] = "Last dmg bone/StoryNPC: " + std::to_string(*(*miscInfo)["lastbonedamaged"]) + " / " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).isstorychar);
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
			std::strcpy((char*)((*charsets)['d']), (*(*textInfo)["torso"]).c_str());
			std::strcpy((char*)((*charsets)['e']), (*(*textInfo)["weapondam"]).c_str());
			std::strcpy((char*)((*charsets)['f']), (*(*textInfo)["legshit"]).c_str());
			std::strcpy((char*)((*charsets)['g']), (*(*textInfo)["dyingbool"]).c_str());
			std::strcpy((char*)((*charsets)['h']), (*(*textInfo)["dshistory"]).c_str());
			std::strcpy((char*)((*charsets)['j']), (*(*textInfo)["lastdambone"]).c_str());
			DrawText(0.55, 0.05, (char*)((*charsets)['c']));
			DrawText(0.55, 0.18, (char*)((*charsets)['j']));
			DrawText(0.55, 0.31, (char*)((*charsets)['e']));
			DrawText(0.55, 0.44, (char*)((*charsets)['d']));
			DrawText(0.55, 0.57, (char*)((*charsets)['f']));
			DrawText(0.55, 0.70, (char*)((*charsets)['g']));
			DrawText(0.55, 0.83, (char*)((*charsets)['h']));
		}
	}

	if ((int)((*iniValues)["ini_shownpchealth"]) == 2)
	{
		std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
		std::strcpy((char*)((*charsets)['d']), (*(*textInfo)["torso"]).c_str());
		std::strcpy((char*)((*charsets)['e']), (*(*textInfo)["textstat"]).c_str());
		std::strcpy((char*)((*charsets)['f']), (*(*textInfo)["legshit"]).c_str());
		std::strcpy((char*)((*charsets)['g']), (*(*textInfo)["dyingbool"]).c_str());
		std::strcpy((char*)((*charsets)['h']), (*(*textInfo)["dshistory"]).c_str());
		std::strcpy((char*)((*charsets)['j']), (*(*textInfo)["textpm"]).c_str());
		DrawText(0.55, 0.05, (char*)((*charsets)['c']));
		DrawText(0.55, 0.15, (char*)((*charsets)['j']));
		DrawText(0.55, 0.25, (char*)((*charsets)['e']));
		DrawText(0.55, 0.35, (char*)((*charsets)['d']));
		DrawText(0.55, 0.45, (char*)((*charsets)['f']));
		DrawText(0.55, 0.55, (char*)((*charsets)['g']));
		DrawText(0.55, 0.65, (char*)((*charsets)['h']));

		if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT((*(*pedInfo)["playerPed"])))
		{
			Entity playerTargetTemp = *(*pedInfo)["playerTargetyolo"];
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(*player, (*pedInfo)["playerTargetyolo"]))
			{
				*(*pedInfo)["playerTargetOld"] = playerTargetTemp;
			}
		}
		if (*(*pedInfo)["playerTargetOld"] != (*(*pedInfo)["playerPed"]) && *(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerTargetyolo"])
		{
			*(*textInfo)["text"] = "NPC Health: ";
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
		}
		if (*(*pedInfo)["playerTargetyolo"] != (*(*pedInfo)["playerPed"]))
		{
			PED::GET_PED_LAST_DAMAGE_BONE(*(*pedInfo)["playerTargetyolo"], (*miscInfo)["lastbonedamagedtemp"]);
			if (*(*miscInfo)["lastbonedamagedtemp"] != 0) *(*miscInfo)["lastbonedamaged"] = *(*miscInfo)["lastbonedamagedtemp"];

			*(*textInfo)["text"] = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(*(*pedInfo)["playerTargetyolo"]));
			*(*textInfo)["torso"] = "Torso hit: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).torsohit);
			*(*textInfo)["legshit"] = "Legs hit: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).legsdownsurvivaltimer);
			*(*textInfo)["dyingbool"] = "DS enabled: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstatebool);
			*(*textInfo)["dshistory"] = "DS visited (1, 2, 3): " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstate1entered) + ", " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstate2entered) + ", " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstate3entered);
			*(*textInfo)["textpm"] = "Hostile NPC Count: " + std::to_string(*(*miscInfo)["combatcounter"]);
			*(*textInfo)["textstat"] = "NPC Status: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).status);
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
			std::strcpy((char*)((*charsets)['d']), (*(*textInfo)["torso"]).c_str());
			std::strcpy((char*)((*charsets)['e']), (*(*textInfo)["textstat"]).c_str());
			std::strcpy((char*)((*charsets)['f']), (*(*textInfo)["legshit"]).c_str());
			std::strcpy((char*)((*charsets)['g']), (*(*textInfo)["dyingbool"]).c_str());
			std::strcpy((char*)((*charsets)['h']), (*(*textInfo)["dshistory"]).c_str());
			std::strcpy((char*)((*charsets)['j']), (*(*textInfo)["textpm"]).c_str());
			DrawText(0.55, 0.05, (char*)((*charsets)['c']));
			DrawText(0.55, 0.15, (char*)((*charsets)['j']));
			DrawText(0.55, 0.25, (char*)((*charsets)['e']));
			DrawText(0.55, 0.35, (char*)((*charsets)['d']));
			DrawText(0.55, 0.45, (char*)((*charsets)['f']));
			DrawText(0.55, 0.55, (char*)((*charsets)['g']));
			DrawText(0.55, 0.65, (char*)((*charsets)['h']));
		}
	}

	if ((int)((*iniValues)["ini_shownpchealth"]) == 3)
	{
		std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
		std::strcpy((char*)((*charsets)['d']), (*(*textInfo)["torso"]).c_str());
		std::strcpy((char*)((*charsets)['e']), (*(*textInfo)["textstat"]).c_str());
		std::strcpy((char*)((*charsets)['f']), (*(*textInfo)["legshit"]).c_str());
		std::strcpy((char*)((*charsets)['g']), (*(*textInfo)["dyingbool"]).c_str());
		std::strcpy((char*)((*charsets)['j']), (*(*textInfo)["textpm"]).c_str());
		DrawText(0.55, 0.05, (char*)((*charsets)['c']));
		DrawText(0.55, 0.15, (char*)((*charsets)['j']));
		DrawText(0.55, 0.25, (char*)((*charsets)['e']));
		DrawText(0.55, 0.35, (char*)((*charsets)['d']));
		DrawText(0.55, 0.45, (char*)((*charsets)['f']));
		DrawText(0.55, 0.55, (char*)((*charsets)['g']));

		if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT((*(*pedInfo)["playerPed"])))
		{
			Entity playerTargetTemp = *(*pedInfo)["playerTargetyolo"];
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(*player, (*pedInfo)["playerTargetyolo"]))
			{
				*(*pedInfo)["playerTargetOld"] = playerTargetTemp;
			}
		}
		if (*(*pedInfo)["playerTargetOld"] != (*(*pedInfo)["playerPed"]) && *(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerTargetyolo"])
		{
			*(*textInfo)["text"] = "NPC Health: ";
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
		}
		if (*(*pedInfo)["playerTargetyolo"] != (*(*pedInfo)["playerPed"]))
		{
			PED::GET_PED_LAST_DAMAGE_BONE(*(*pedInfo)["playerTargetyolo"], (*miscInfo)["lastbonedamagedtemp"]);
			if (*(*miscInfo)["lastbonedamagedtemp"] != 0) *(*miscInfo)["lastbonedamaged"] = *(*miscInfo)["lastbonedamagedtemp"];

			Vector3 vechead = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(*(*pedInfo)["playerTargetyolo"], PED::GET_PED_BONE_INDEX(*(*pedInfo)["playerTargetyolo"], 21030));
			Vector3 vechip = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(*(*pedInfo)["playerTargetyolo"], PED::GET_PED_BONE_INDEX(*(*pedInfo)["playerTargetyolo"], 14415));
			Vector3 vecleg1 = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(*(*pedInfo)["playerTargetyolo"], PED::GET_PED_BONE_INDEX(*(*pedInfo)["playerTargetyolo"], 43312));
			Vector3 vecleg2 = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(*(*pedInfo)["playerTargetyolo"], PED::GET_PED_BONE_INDEX(*(*pedInfo)["playerTargetyolo"], 55120));
			Vector3 vecweaponhand = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(*(*pedInfo)["playerTargetyolo"], PED::GET_PED_BONE_INDEX(*(*pedInfo)["playerTargetyolo"], 11300));
			float groundzcoordhead, groundzcoordhip, groundzcoordleg1, groundzcoordleg2;
			GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vechead.x, vechead.y, vechead.z, &groundzcoordhead, true);
			GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vechip.x, vechip.y, vechip.z, &groundzcoordhip, true);
			GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecleg1.x, vecleg1.y, vecleg1.z, &groundzcoordleg1, true);
			GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecleg2.x, vecleg2.y, vecleg2.z, &groundzcoordleg2, true);

			//set standing bool
			bool isstanding;
			if (vechead.z > groundzcoordhead + 0.5f && vechead.z < groundzcoordhead + 3000) isstanding = true;
			else isstanding = false;


			*(*textInfo)["text"] = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(*(*pedInfo)["playerTargetyolo"]));
			*(*textInfo)["torso"] = "Torso hit: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).torsohit);
			*(*textInfo)["legshit"] = "pedmapSize, loopCounter: " + std::to_string(std::size((*pedmap))) + ", " + std::to_string(loopcounter);
			*(*textInfo)["dyingbool"] = "DS enabled: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).dyingstatebool);
			*(*textInfo)["textpm"] = "IsStanding: " + std::to_string(isstanding) + ", h,g: " + std::to_string(vechead.z) + "," + std::to_string(groundzcoordhead);
			*(*textInfo)["textstat"] = "HitCounter LL/RL: " + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).leftleghitcounter) + "/" + std::to_string(((*pedmap)[*(*pedInfo)["playerTargetyolo"]]).rightleghitcounter);
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["text"]).c_str());
			std::strcpy((char*)((*charsets)['d']), (*(*textInfo)["torso"]).c_str());
			std::strcpy((char*)((*charsets)['e']), (*(*textInfo)["textstat"]).c_str());
			std::strcpy((char*)((*charsets)['f']), (*(*textInfo)["legshit"]).c_str());
			std::strcpy((char*)((*charsets)['g']), (*(*textInfo)["dyingbool"]).c_str());
			std::strcpy((char*)((*charsets)['j']), (*(*textInfo)["textpm"]).c_str());
			DrawText(0.55, 0.05, (char*)((*charsets)['c']));
			DrawText(0.55, 0.25, (char*)((*charsets)['e']));
			DrawText(0.55, 0.35, (char*)((*charsets)['d']));
			DrawText(0.55, 0.45, (char*)((*charsets)['f']));
			DrawText(0.55, 0.55, (char*)((*charsets)['g']));
			DrawText(0.55, 0.65, (char*)((*charsets)['j']));
		}
	}

	//show ped model
	if ((int)((*iniValues)["ini_showpedmodel"]) == 1)
	{
		DrawText(0.5, 0.15, (char*)((*charsets)['c']));

		if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT((*(*pedInfo)["playerPed"])))
		{
			Entity playerTargetTemp = *(*pedInfo)["playerTargetyolo"];
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(*player, (*pedInfo)["playerTargetyolo"]))
			{
				*(*pedInfo)["playerTargetOld"] = playerTargetTemp;
			}
		}
		if (*(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerPed"] && *(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerTargetyolo"])
		{
			*(*textInfo)["mtext"] = "NPC Model: ";
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["mtext"]).c_str());
		}
		if (*(*pedInfo)["playerTargetyolo"] != *(*pedInfo)["playerPed"])
		{
			*(*textInfo)["mtext"] = "NPC Model: " + std::to_string(ENTITY::GET_ENTITY_MODEL(*(*pedInfo)["playerTargetyolo"]));
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["mtext"]).c_str());
			DrawText(0.5, 0.15, (char*)((*charsets)['c']));
		}
	}

	//show task ids
	if ((int)((*iniValues)["ini_showpedtasks"]) == 1)
	{
		std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["ttext"]).c_str());
		DrawText(0.5, 0.15, (char*)((*charsets)['c']));

		if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT((*(*pedInfo)["playerPed"])))
		{
			Entity playerTargetTemp = *(*pedInfo)["playerTargetyolo"];
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(*player, (*pedInfo)["playerTargetyolo"]))
			{
				*(*pedInfo)["playerTargetOld"] = playerTargetTemp;
			}
		}
		if (*(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerPed"] && *(*pedInfo)["playerTargetOld"] != *(*pedInfo)["playerTargetyolo"])
		{
			*(*textInfo)["ttext"] = "NPC Tasks: ";
			std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["ttext"]).c_str());
		}
		if (*(*pedInfo)["playerTargetyolo"] != *(*pedInfo)["playerPed"])
		{
			for (int g = 0; g < 999; g++)
			{
				if (g != 582 && g != 594 && AI::GET_IS_TASK_ACTIVE(*(*pedInfo)["playerTargetyolo"], g))
				{
					if ((*(*textInfo)["ttext"]).find("," + std::to_string(g) + ",") == std::string::npos)
					{
						if (*(*textInfo)["ttext"] == "NPC Tasks: ") *(*textInfo)["ttext"] += std::to_string(g);
						else *(*textInfo)["ttext"] += "," + std::to_string(g);
						std::strcpy((char*)((*charsets)['c']), (*(*textInfo)["ttext"]).c_str());
						DrawText(0.5, 0.15, (char*)((*charsets)['c']));
					}
				}
			}
		}
	}
}

bool DisarmPed(Ped ped, Vector3 weaponhandpos)
{
	Hash pedweaphash;
	bool ispedarmed = WEAPON::GET_CURRENT_PED_WEAPON(ped, &pedweaphash, false, 0, false);
	WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(ped, pedweaphash, weaponhandpos.x, weaponhandpos.y, weaponhandpos.z, 30);
	return ispedarmed;
}

//returns 3 if its neither a story- nor a friendly-NPC, returns 1 if its a friendly-NPC, returns 2 if its a story-NPC
int isPedStoryChar(Ped* ped, const std::vector<Hash>* friendlystorypedlist, const std::vector<Hash>* otherstorypedlist)
{
	//friendly NPCs
	for (int i = 0; i < (*friendlystorypedlist).size(); i++)
	{
		if (PED::IS_PED_MODEL(*ped, (*friendlystorypedlist)[i])) return 1;
	}

	//other NPCs
	for (int i = 0; i < (*otherstorypedlist).size(); i++)
	{
		if (PED::IS_PED_MODEL(*ped, (*otherstorypedlist)[i])) return 2;
	}

	//normal NPCs
	return 3;
}

std::vector<Hash> getFriendlyStoryPedList()
{
	std::vector<Hash> pedlist;
	pedlist.reserve(31);
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_josiahtrelawny"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_johnmarston"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_MicahBell"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_dutch"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_lenny"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_hoseamatthews"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_leostrauss"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_rainsfall"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_javierescuella"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_billwilliamson"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_uncle"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_sean"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_charlessmith"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_EagleFlies"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_PAYTAH"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_hercule"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_leon"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_mrsadler"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_kieran"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_mrpearson"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_JAMIE"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_Jules"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_revswanson"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_MARSHALL_THURWELL"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_ArchieDown"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_thomasdown"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_EdithDown"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_creolecaptain"));
	pedlist.emplace_back(1407031519); //Epilogue: Charles
	pedlist.emplace_back(2828949318); //Epilogue: Mr. Devon
	pedlist.emplace_back(2695284288); //Epilogue: Mr. Wayne
	return pedlist;
}

std::vector<Hash> getOtherStoryPedList()
{
	std::vector<Hash> pedlist;
	pedlist.reserve(12);
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("U_M_M_BHT_BENEDICTALLBRIGHT"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_bronte"));
	pedlist.emplace_back(GAMEPLAY::GET_HASH_KEY("CS_LARAMIE"));
	pedlist.emplace_back(1744529953); //Police Chief Dunbar
	pedlist.emplace_back(2410820464); //Sheriff Malloy
	pedlist.emplace_back(1347320453); //Gunslingers Mission (Flaco Hernandez)
	pedlist.emplace_back(3736835937); //Gunslingers Mission (Billy Midnight)
	pedlist.emplace_back(4196879928); //Gunslingers Mission (Emmet Granger)
	pedlist.emplace_back(3273604429); //Danbury from "American Fathers II" mission
	pedlist.emplace_back(847448713); //NPC from "Spines of America" mission
	pedlist.emplace_back(1765636093); //NPC from "Spines of America" mission
	pedlist.emplace_back(1045059103); //NPC from "Spines of America" mission
	return pedlist;
}

int PedFear(Ped ped, int mode, int lastfearaudio, int ds2chance) //returns last used speech line (key)
{	
	//container for speech lines
	std::map<int, char*> speech;
		
	//setting up the speech options
	if (mode == 1) //dying stage 1
	{
		/*
		PANIC_HELP
		SCREAM_SHOCKED
		SCREAM_TERROR
		PAIN_EXERT_AGGRO
		PAIN_EXERT_TIMID
		PAIN_LOW
		PAIN_STRUGGLE
		GENERIC_AFRAID_REACTION
		GENERIC_SHOCKED_HIGH
		GENERIC_CURSE_HIGH
		NEED_HELP "come on, no"
		NEED_SOME_HELP - nothing?
		LEAVE_ME_PLEAD - nothing?
		PLEAD_HOGTIED
		GUN_BEG
		ALLY_COMBAT_ENGAGE_INJURED - nothing?
		ALLY_COMBAT_STATUS_INJURED
		SERIOUSLY_WOUNDED_MOAN - nothing?
		SCARED_COMMUNICATE - nothing?
		SCARED_HELP
		INTIMIDATED_GEN
		INTIMIDATED_AGAIN_GEN
		PLEAD
		*/
		
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
			std::string speechline = speechl_it -> second;
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