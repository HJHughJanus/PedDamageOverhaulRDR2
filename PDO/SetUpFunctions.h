#pragma once
#include "script.h"
#include "MiscFunctions.h"

#include <vector>
#include <unordered_map>
#include <array>

bool SetKey(int* key, int valToKey)
{
	switch (valToKey)
	{
	case 1:
		*key = VK_F1;
		return true;
	case 2:
		*key = VK_F2;
		return true;
	case 3:
		*key = VK_F3;
		return true;
	case 4:
		*key = VK_F4;
		return true;
	case 5:
		*key = VK_F5;
		return true;
	case 6:
		*key = VK_F6;
		return true;
	case 7:
		*key = VK_F7;
		return true;
	case 8:
		*key = VK_F8;
		return true;
	case 9:
		*key = VK_F9;
		return true;
	case 10:
		*key = VK_F10;
		return true;
	case 11:
		*key = VK_F11;
		return true;
	case 12:
		*key = VK_F12;
		return true;
	default:
		return false;
	}
}

std::array<char*, 56> SetUpAllWeaponMap()
{
	std::array<char*, 56> allweaponmap;
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
	allweaponmap[55] = "WEAPON_REVOLVER_NAVY";
	return allweaponmap;
}

std::array<Hash, 14> SetUpSpecialWeaponMap()
{
	std::array<Hash, 14> specialweaponmap;
	specialweaponmap[0] = GAMEPLAY::GET_HASH_KEY("WEAPON_THROWN_DYNAMITE");
	specialweaponmap[1] = GAMEPLAY::GET_HASH_KEY("WEAPON_THROWN_TOMAHAWK_ANCIENT");
	specialweaponmap[2] = GAMEPLAY::GET_HASH_KEY("WEAPON_THROWN_TOMAHAWK");
	specialweaponmap[3] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_DOUBLEBARREL");
	specialweaponmap[4] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_DOUBLEBARREL_EXOTIC");
	specialweaponmap[5] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_PUMP");
	specialweaponmap[6] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_REPEATING");
	specialweaponmap[7] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_SAWEDOFF");
	specialweaponmap[8] = GAMEPLAY::GET_HASH_KEY("WEAPON_SHOTGUN_SEMIAUTO");
	specialweaponmap[9] = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_CARCANO");
	specialweaponmap[10] = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_ROLLINGBLOCK");
	specialweaponmap[11] = GAMEPLAY::GET_HASH_KEY("WEAPON_SNIPERRIFLE_ROLLINGBLOCK_EXOTIC");
	specialweaponmap[12] = GAMEPLAY::GET_HASH_KEY("WEAPON_RIFLE_BOLTACTION");
	specialweaponmap[13] = GAMEPLAY::GET_HASH_KEY("WEAPON_RIFLE_SPRINGFIELD");
	return specialweaponmap;
}

std::map<Hash, Hash> SetUpSpecialAmmoMap()
{
	std::map<Hash, Hash> specialammomap;
	specialammomap[GAMEPLAY::GET_HASH_KEY("AMMO_ARROW_DYNAMITE")] = GAMEPLAY::GET_HASH_KEY("WEAPON_BOW");
	specialammomap[GAMEPLAY::GET_HASH_KEY("AMMO_ARROW_IMPROVED")] = GAMEPLAY::GET_HASH_KEY("WEAPON_BOW");
	return specialammomap;
}

std::array<char*, 7> SetUpKnivesMap()
{
	std::array<char*, 7> knivesmap;
	knivesmap[0] = "WEAPON_MELEE_KNIFE_JOHN";
	knivesmap[1] = "WEAPON_MELEE_KNIFE";
	knivesmap[2] = "WEAPON_MELEE_KNIFE_MINER";
	knivesmap[3] = "WEAPON_MELEE_KNIFE_CIVIL_WAR";
	knivesmap[4] = "WEAPON_MELEE_KNIFE_BEAR";
	knivesmap[5] = "WEAPON_MELEE_KNIFE_VAMPIRE";
	knivesmap[6] = "WEAPON_MELEE_MACHETE";
	return knivesmap;
}

std::array<int, 6> SetUpLegBonesMap()
{
	std::array<int, 6> legBonesMap;
	legBonesMap[0] = 6884; //SKEL_R_Thigh
	legBonesMap[1] = 65478; //SKEL_L_Thigh
	legBonesMap[2] = 55120; //SKEL_L_CALF
	legBonesMap[3] = 33646; //unknown leg bone
	legBonesMap[4] = 43312; //SKEL_R_Calf
	legBonesMap[5] = 45454; //SKEL_L_Foot
	return legBonesMap;
}

std::array<int, 6> SetUpTorsoBonesMap()
{
	std::array<int, 6> torsoBonesMap;
	torsoBonesMap[0] = 14410; //unknown torso bone
	torsoBonesMap[1] = 14411; //SKEL_Spine1
	torsoBonesMap[2] = 14412; //SKEL_Spine2
	torsoBonesMap[3] = 14413; //SKEL_Spine3
	torsoBonesMap[4] = 14414; //SKEL_Spine4
	torsoBonesMap[5] = 14415; //SKEL_Spine5
	return torsoBonesMap;
}

std::array<int, 6> SetUpNeckBonesMap()
{
	std::array<int, 6> neckBonesMap;
	neckBonesMap[0] = 14283; //SKEL_Neck0
	neckBonesMap[1] = 14284; //SKEL_Neck1
	neckBonesMap[2] = 14285; //SKEL_Neck2
	neckBonesMap[3] = 14286; //SKEL_Neck3
	neckBonesMap[4] = 14287; //SKEL_Neck4
	neckBonesMap[5] = 14288; //SKEL_Neck5
	return neckBonesMap;
}

std::array<int, 17> SetUpArmBonesMap()
{
	std::array<int, 17> armBonesMap;
	armBonesMap[0] = 34606; //SKEL_L_Hand
	armBonesMap[1] = 22798; //SKEL_R_Hand
	armBonesMap[2] = 54187; //SKEL_R_Forearm
	armBonesMap[3] = 53675; //SKEL_L_Forearm
	armBonesMap[4] = 41357; //SKEL_L_Finger32
	armBonesMap[5] = 41341; //SKEL_L_Finger32
	armBonesMap[6] = 41405; //SKEL_L_Finger12
	armBonesMap[7] = 41326; //SKEL_L_Finger12
	armBonesMap[8] = 41325; //SKEL_L_Finger12
	armBonesMap[9] = 35876; //SKEL_L_Finger41
	armBonesMap[10] = 16829; //SKEL_L_Finger32
	armBonesMap[11] = 16781; //SKEL_R_Finger32
	armBonesMap[12] = 16765; //SKEL_L_Finger32
	armBonesMap[13] = 16749; //SKEL_R_Finger12
	armBonesMap[14] = 11300; //SKEL_R_Finger41
	armBonesMap[15] = 37873; //Left upper arm (name unknown)
	armBonesMap[16] = 46065; //right upper arm (name unknown)
	return armBonesMap;
}

std::array<int, 1> SetUpHeadBonesMap()
{
	std::array<int, 1> headBonesMap;
	headBonesMap[0] = 21030; //SKEL_Head
	return headBonesMap;
}

std::array<int, 14> SetUpTorsoBonesBleedingMap()
{
	std::array<int, 14> TorsoBonesBleedingMap;
	TorsoBonesBleedingMap[0] = 14410; //unknown torso bone
	TorsoBonesBleedingMap[1] = 14411; //SKEL_Spine1
	TorsoBonesBleedingMap[2] = 14412; //SKEL_Spine2
	TorsoBonesBleedingMap[3] = 14413; //SKEL_Spine3
	TorsoBonesBleedingMap[4] = 14414; //SKEL_Spine4
	TorsoBonesBleedingMap[5] = 14415; //SKEL_Spine5
	TorsoBonesBleedingMap[6] = 30226; //left shoulder
	TorsoBonesBleedingMap[7] = 54802; //right shoulder
	TorsoBonesBleedingMap[8] = 14283; //SKEL_Neck0
	TorsoBonesBleedingMap[9] = 14284; //SKEL_Neck1
	TorsoBonesBleedingMap[10] = 14285; //SKEL_Neck2
	TorsoBonesBleedingMap[11] = 14286; //SKEL_Neck3
	TorsoBonesBleedingMap[12] = 14287; //SKEL_Neck4
	TorsoBonesBleedingMap[13] = 14288; //SKEL_Neck5
	return TorsoBonesBleedingMap;
}

void SetUpPed(Ped* ped, PedAttributes* p, std::unordered_map<std::string, float>* iniValues, const std::vector<Hash>* friendlyStoryNPCs, const std::vector<Hash>* otherStoryNPCs)
{
	//check if ped is police
	if (PED::GET_PED_RELATIONSHIP_GROUP_HASH(*ped) == GAMEPLAY::GET_HASH_KEY("REL_COP"))
	{
		(*p).ispolice = true;
		int dismrand = (int)((*iniValues)["ini_policedismountdistancemin"]) + (std::rand() % ((int)((*iniValues)["ini_policedismountdistancemax"]) - (int)((*iniValues)["ini_policedismountdistancemin"]) + 1));
		(*p).policedismountdistance = dismrand;
	}
	else (*p).ispolice = false;

	/*status:
					0 = default
					1 = friendly story character
					2 = other story character
					3 = none of the above
					*/
	int charstat = isPedStoryChar(ped, friendlyStoryNPCs, otherStoryNPCs);
	(*p).isstorychar = charstat;
	int maxhealth;
	int minhealth;
	//friendly NPCs
	if (charstat == 1)
	{
		if ((int)((*iniValues)["ini_excludefriendlystorynpcs"]) == 1) (*p).usemaxhealth = 0;
		minhealth = (int)((*iniValues)["ini_friendlystorypedhealthmin"]);
		maxhealth = (int)((*iniValues)["ini_friendlystorypedhealthmax"]);
	}
	//other NPCs
	else if (charstat == 2)
	{
		if ((int)((*iniValues)["ini_excludeotherstorynpcs"]) == 1) (*p).usemaxhealth = 0;
		minhealth = (int)((*iniValues)["ini_otherstorypedhealthmin"]);
		maxhealth = (int)((*iniValues)["ini_otherstorypedhealthmax"]);
	}
	//normal NPCs
	else
	{
		if ((*p).ispolice)
		{
			minhealth = (int)((*iniValues)["ini_lawpedhealthmin"]);
			maxhealth = (int)((*iniValues)["ini_lawpedhealthmax"]);
		}
		else
		{
			minhealth = (int)((*iniValues)["ini_pedhealthmin"]);
			maxhealth = (int)((*iniValues)["ini_pedhealthmax"]);
		}
		(*p).usemaxhealth = 1;
	}

	if (minhealth > 0 && maxhealth > 0) {
		(*p).maxhealth = minhealth + (std::rand() % (maxhealth - minhealth + 1));
	}
	else {
		(*p).maxhealth = ENTITY::GET_ENTITY_MAX_HEALTH(*ped, false);
	}
	/*
	//set health of story characters, if set in the ini
	//set if story characters are affected by the mod
	if ((*p).isstorychar == 1)
	{
		if ((int)((*iniValues)["ini_excludefriendlystorynpcs"]) != 1) (*p).isstorychar = 3;

		if ((*p).usemaxhealth == 0)
		{
			if ((int)((*iniValues)["ini_friendlystorypedhealthmin"]) > 0 && (int)((*iniValues)["ini_friendlystorypedhealthmax"]) > 0)
			{
				(*p).usemaxhealth = 2;
				if (PED::GET_PED_MAX_HEALTH(*ped) != (*p).maxhealth)
				{
					PED::SET_PED_MAX_HEALTH(*ped, (*p).maxhealth);
					ENTITY::SET_ENTITY_HEALTH(*ped, (*p).maxhealth, 0);
				}
			}
		}
	}
	else if ((*p).isstorychar == 2)
	{
		if ((int)((*iniValues)["ini_excludeotherstorynpcs"]) != 1) (*p).isstorychar = 3;

		if ((*p).usemaxhealth == 0)
		{
			if ((int)((*iniValues)["ini_otherstorypedhealthmin"]) > 0 && (int)((*iniValues)["ini_otherstorypedhealthmax"]) > 0)
			{
				(*p).usemaxhealth = 2;
				if (PED::GET_PED_MAX_HEALTH(*ped) != (*p).maxhealth)
				{
					PED::SET_PED_MAX_HEALTH(*ped, (*p).maxhealth);
					ENTITY::SET_ENTITY_HEALTH(*ped, (*p).maxhealth, 0);
				}
			}
		}
	}
	else if ((*p).ispolice)
	{
		if ((*p).usemaxhealth == 0)
		{
			if ((int)((*iniValues)["ini_lawpedhealthmin"]) > 0 && (int)((*iniValues)["ini_lawpedhealthmax"]) > 0)
			{
				(*p).usemaxhealth = 2;
				if (PED::GET_PED_MAX_HEALTH(*ped) != (*p).maxhealth)
				{
					PED::SET_PED_MAX_HEALTH(*ped, (*p).maxhealth);
					ENTITY::SET_ENTITY_HEALTH(*ped, (*p).maxhealth, 0);
				}
			}
		}
	}
	else (*p).usemaxhealth = 1;
	*/

	/*status:
	0 = default
	1 = both legs crippled
	2 = both legs crippled & will survive burning
	3 = default & will survive burning
	4 = is/was burning and should survive
	5 = is burning and should not survive
	6 = left leg crippled
	7 = left leg crippled & will survive burning
	8 = right leg crippled
	9 = right leg crippled & will survive burning
	10 = paralyzed
	11 = paralyzed and will survive burning
	99 = temporary status in which a prolonging of the ragdoll behavior is not wanted
	*/
	(*p).status = 0;

	//status: 0 = default value
	(*p).health = 0;

	//status: 0 = default value, >0 = was already in dying state 3 at least once
	(*p).dyingstate3entered = 0;

	//status: 0 = default value, >0 = was already in dying state 2 at least once
	(*p).dyingstate2entered = 0;

	//status: 0 = default value, >0 = was already in dying state 1 at least once
	(*p).dyingstate1entered = 0;

	//status: -1 = default value, will later be filled with a game timer value to make transition into writhing animation occur after a while (looks smoother than if it is done immediately)
	(*p).animtrans = -1;

	//status: 0 = default value
	(*p).healthds = 0;

	//status: 0 = disabled, 1 = enabled
	int randu = 0 + (std::rand() % (99 - 0 + 1));
	if (randu < (int)((*iniValues)["ini_dyingstatechance"])) (*p).dyingstatebool = 1;
	else (*p).dyingstatebool = 0;

	//gets filled with current NPC health values, so later the delta (damage done) can be computed
	(*p).limbhealth = ENTITY::GET_ENTITY_HEALTH(*ped);

	//status: 0 = has not been knocked out, 1 = has been knocked out and effects pending, 2 = has been knocked out and effects have been triggered
	(*p).isknockedout = 0;

	//status: 0 = do not use animation, 1 = use animation
	int animrand = 0 + (std::rand() % (99 - 0 + 1));
	if (animrand < (int)((*iniValues)["ini_useanimchance"])) (*p).dsanimationused = 1;
	else (*p).dsanimationused = 0;

	//status: 0 = euphoria stumbing disabled, 1 = euphoria stumbling enabled
	int randi = 0 + (std::rand() % (99 - 0 + 1));
	if (randi < (int)((*iniValues)["ini_euphoriastumblechance"])) (*p).euphstumblingenabled = 1;
	else (*p).euphstumblingenabled = 0;

	//status: 0 = default value
	(*p).isfalling = 0;

	//status: 0 = default value
	(*p).falltime = 0;

	//status: 0 = default value, 1 = to be killed
	(*p).tobekilled = 0;

	//status: 0 = default value, 1 = to be downed
	(*p).tobedowned = 0;

	//status: 0 = was not hit in the torso, 1 = was hit in the torso
	(*p).torsohit = 0;

	//status: 0 = default value
	(*p).accuracy = 0;

	//status: 0 = not down, 1 = down
	(*p).isdown = 0;

	//status: 0 = not dead, 1 = dead
	(*p).isdead = 0;

	//status: 0 = has not reacted to friends dying, 1 = has reacted to friends dying (hands up or cowering), 2 = has reacted to friends dying (fleeing), 9 = fleeing after status 1
	(*p).hasreacted = 0;

	//status: false = bleeding will apply, true = the ped won't bleed
	(*p).nobleeding = false;

	//status: 0 = has been on a mount
	(*p).lasttimeonmount = 0;

	(*p).handsup = 0;

	(*p).firsttimehandsup = 0;

	//status: 0 = no stumbling time calculated yet
	(*p).stumblingtime = 0;

	(*p).headvecdelta.x = 0;
	(*p).headvecdelta.y = 0;
	(*p).headvecdelta.z = 0;

	(*p).headvectime = 0;

	(*p).lastfearaudio = 0;

	(*p).rightleghitcounter = 0;

	(*p).leftleghitcounter = 0;

	(*p).legsdamageddowntime = 0;

	(*p).legsdownsurvivaltimer = 0;

	(*p).fireaudiotime = 0;

	(*p).fireaudiosample = 0;

	(*p).arteryshot = false;

	(*p).wasaimedat = false;

	(*p).hasbeendamagedbyweapon = false;

	(*p).ishanging = false;

	(*p).hangingtime = 0;

	(*p).shotbyplayer = 0;

	(*p).wasnotaloneincombat = false;

	(*p).isincombatwithplayer = false;

	(*p).isarmed = false;

	(*p).wasdisarmed = false;

	(*p).movementstyle = "";

	(*p).blipset = 0;

	(*p).dismount = 0;

	(*p).disarmingstop = false;

	(*p).isinvehicle = 0;

	if ((int)((*iniValues)["ini_excludebodyguardmodnpcs"]) == 1 && DECORATOR::DECOR_EXIST_ON(*ped, "BodyguardSpawner")) (*p).isFromBodyguardSpawner = true;
	else (*p).isFromBodyguardSpawner = false;

	(*p).wasSetUpProperly = true;
}
