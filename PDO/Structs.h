#pragma once
#include "script.h"

struct PedAttributes
{
	//container for flag if ped is a story character (0 is default, 1 is friendly story char (like a gang member), 2 is other story char (like the gunslingers), 3 is not any story char)
	int isstorychar;
	//container for peds and their status (is burning, is shot in the leg, is shot in both legs, etc.)
	int status;
	//container for peds and their current health (for checking if damage was dealt - mostly used for reactions like "hands up" or cowering (if enough damage is done, NPCs will then flee))
	int health;
	//container for peds and a value to decide whether dying states kick in or vanilla behavior is triggered
	bool dyingstatebool;
	//container for peds and a value to calculate the limb specific damage
	int limbhealth;
	//container for peds and a value for dying movement stage 1 (for remembering if it has been entered before)
	int dyingstate1entered;
	//container for peds and a value for dying movement stage 2 (for remembering if it has been entered before)
	int dyingstate2entered;
	//container for peds and a value for dying movement stage 3 (for remembering if it has been entered before)
	int dyingstate3entered;
	//container for peds and a value for animation transitioning in dying state 1
	int animtrans;
	//container for ped health (for NPC pain reactions during dying states)
	int healthds;
	//container for ped stumbling (euphoria enabled or not)
	int euphstumblingenabled;
	//container for ped animation use in dying states
	int dsanimationused;
	//container for remembering if ped was k.o.ed
	int isknockedout;
	//container for flag if ped is falling
	int isfalling;
	//container for remembering how long ped has been falling
	int falltime;
	//container for flag if ped is to be killed
	int tobekilled;
	//container for flag if ped is to be downed
	int tobedowned;
	//container for flag if ped was hit in the torso
	int torsohit;
	//container for the ped's accuracy
	int accuracy;
	//container for flag if ped is down
	int isdown;
	//container for flag if ped is dead
	int isdead;
	//container for flag if ped is has reacted to other peds dying in combat
	int hasreacted;
	//container for bool if ped should bleed
	bool nobleeding;
	//container for time when ped was last on a mount
	int lasttimeonmount;
	//container for stumbling time
	int stumblingtime;
	//container for making sure peds don't stand still
	Vector3 headvecdelta;
	int headvectime;
	//container for last fear audio (to prevent repeating audio)
	int lastfearaudio;
	//container for how many times the right leg of an NPC was hit
	int rightleghitcounter;
	//container for how many times the left leg of an NPC was hit
	int leftleghitcounter;
	//container for damaged legs triggering falling down into dying states
	int legsdamageddowntime;
	//container for timer of peds being saved from bleeing out (to not die from leg shots)
	int legsdownsurvivaltimer;
	//container for timer (fire audio interval)
	int fireaudiotime;
	//container for last sound used (to prevent looping)
	int fireaudiosample;
	//container for bool if ped is bleeding from an artery shot
	bool arteryshot;
	//container for bool if ped has been aimed at when down
	bool wasaimedat;
	//container for bool if ped has been aimed at when down
	bool hasbeendamagedbyweapon;
	//container for bool if ped is hanging (TieYourLasso mod)
	bool ishanging;
	//container for hanging time (TieYourLasso mod)
	int hangingtime;
	//container for the last time the ped was shot by the player
	int shotbyplayer;
	//container for bool if ped was fighting against the player alone
	bool wasnotaloneincombat;
	//container for bool if ped is in combat with player
	bool isincombatwithplayer;
	//container for when ped should be in hands-up task
	int handsup;
	//container for the first time a ped was forced into "hands up"
	int firsttimehandsup;
	//container for the information if a ped is or was armed and has been disarmed
	bool isarmed;
	bool wasdisarmed;
	//container for remembering which movement style ped is supposed to have
	char* movementstyle;
	//container for remembering if ped blip is set
	Blip blipset;
	//container for remembering when ped did a forced dismount (for surrendering)
	int dismount;
	//container for remembering if ped should be immune to disarms (only used for duels, so ingame disarms occur naturally)
	bool disarmingstop;
	//container for remembering if ped is police or not and in which distance police might dismount in a fight (to go for cover)
	bool ispolice;
	float policedismountdistance;
	//container for the ped's maximum health and if it should be applied (usemaxhealth: 0 = default, 1 = use the health value, 2 = don't use the health value)
	int maxhealth;
	int usemaxhealth;
	//container for remembering if ped was in a vehicle (for setting the health value)
	bool isinvehicle;
	//container for remembering if ped is from the mod "Bodyguard Spawner"
	bool isFromBodyguardSpawner;
	//container for remembering if the instance of this struct has been properly initialized
	bool wasSetUpProperly;
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