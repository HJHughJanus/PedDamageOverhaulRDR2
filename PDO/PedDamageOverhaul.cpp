/*
TABLE OF CONTENTS (use "CTRL" + "F" on the ID of the code section to find the section you are looking for)

ID: PDO001 SETUP (READING IN INI VALUES, SETTING UP GLOBAL VARIABLES, ETC.)
ID: PDO002 ACTUAL MOD FEATURES STARTING
ID: PDO003 PLAYER DAMAGE MODIFIERS
ID: PDO004 AFFECTING NPCS
ID: PDO005 AFFECTING NPCS (ONLY HUMAN ONES)
ID: PDO006 AFFECTING LIVING NPCs
ID: PDO007 SETTING UP VARIABLES AND BOOLEANs
ID: PDO008 --currently disabled-- ARTERIAL BLEEDING BEHAVIOR (NPCs MUST NOT ENTER DYING STATES)
ID: PDO040 POLICE BEHAVIOR
ID: PDO009 SET NPC HEALTH
ID: PDO010 AFFECTING NPCs WITH PROPERLY SET HEALTH AND NOT CURRENTLY BURNING OR BURNING BEFORE
ID: PDO011 SET NPC ACCURACY
ID: PDO012 KILL NPCs WHEN IN DYING STATE TOO LONG
ID: PDO013 STOP BLEEDING FOR KNOCKED OUT NPCs OR THOSE IN MELEE FIGHTS
ID: PDO014 NPC FALLING CHECKS
ID: PDO015 HORSE FALLING DELTA
ID: PDO016 LASSO DISARMING
ID: PDO017 HOGTIE DISARMING
ID: PDO018 NPC FEAR FEATURE (FLEEING DURING COMBAT)
ID: PDO019 SET NPC STATUS ACCORDING TO BODY PARTS BEING DAMAGED
ID: PDO020 MAKE INJURED NPCs MOVE WITH WOUNDED ANIMATIONS
ID: PDO021 SPINE PARALYSIS
ID: PDO022 PUT NPCs DOWN WHEN HIT IN THE LEGS ENOUGH
ID: PDO023 LEGS HURT - STUMBLING
ID: PDO024 REACTION IF DISARMED NPCs ARE HARMED
ID: PDO025 REACTION OF NPCs WHEN HOGTIED (FOR BEING ATTACKED AND BEING AIMED AT)
ID: PDO026 TIE YOUR LASSO FEATURES
ID: PDO027 BODY PART SPECIFIC DAMAGE
ID: PDO028 KNOCKING THE WIND OUT OF NPCs
ID: PDO029 PREVENTING ONE-SHOT-KILLS OF GOING LIMP
ID: PDO030 DYING STATES (GENERAL)
ID: PDO031 DYING STATE PUSH - PUT NPC DOWN (IF STANDING)
ID: PDO032 DYING STATE BEHAVIOR - IF NPC IS NOT STANDING
ID: PDO033 DYING STATE 1
ID: PDO034 DYING STATE 2
ID: PDO035 DYING STATE 3
ID: PDO036 DYING STATE BLEEDING
ID: PDO037 BLEEDING WHEN SHOT
ID: PDO038 SPINE SHOT BLEEDING
ID: PDO039 FIRE / BURNING
*/

#include "script.h"
#include "keyboard.h"
#include "HelperMethods.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include <map>
#include <fstream>

using namespace std;

//main logic
void main()
{		
	
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								SETUP (READING IN INI VALUES, SETTING UP GLOBAL VARIABLES, ETC.), ID: PDO001
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


	//checking if there is an ini file (used for drawing according information on screen)
	bool iniexists = false;
	iniexists = does_file_exist(".\\PedDamageOverhaul.ini");	
	
	//getting values from the ini or setting default values, if ini not there (default represent mod version "standard" of v1.41)
	int ini_shownpchealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ShowNPCHealth", 0, ".\\PedDamageOverhaul.ini");
	int ini_showpedmodel = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ShowNPCModel", 0, ".\\PedDamageOverhaul.ini");
	int ini_showpedtasks = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ShowNPCTasks", 0, ".\\PedDamageOverhaul.ini");
	float ini_sweaponmodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpecialWeaponModifier", 30, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_weaponmodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "WeaponModifier", 30, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_knifemodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "KnifeModifier", 90, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_meleemodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MeleeModifier", 95, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_npcweaponmodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCWeaponModifier", 230, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_npcmeleemodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCMeleeModifier", 105, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_friendlystorypedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Basic", "FriendlyStoryNPCHealthMin", 450, ".\\PedDamageOverhaul.ini");
	int ini_friendlystorypedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Basic", "FriendlyStoryNPCHealthMax", 450, ".\\PedDamageOverhaul.ini");
	int ini_otherstorypedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Basic", "OtherStoryNPCHealthMin", 200, ".\\PedDamageOverhaul.ini");
	int ini_otherstorypedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Basic", "OtherStoryNPCHealthMax", 200, ".\\PedDamageOverhaul.ini");
	int ini_lawpedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Basic", "PoliceNPCHealthMin", 75, ".\\PedDamageOverhaul.ini");
	int ini_lawpedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Basic", "PoliceNPCHealthMax", 75, ".\\PedDamageOverhaul.ini");
	int ini_togglekey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "ToggleKey", 9, ".\\PedDamageOverhaul.ini");
	int ini_disablemodinmissions = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisableModInMissions", 0, ".\\PedDamageOverhaul.ini");
	int ini_friendlyfirekey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "FirendlyFireKey", 2, ".\\PedDamageOverhaul.ini");
	int ini_killwoundedkey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "KillWoundedKey", 8, ".\\PedDamageOverhaul.ini");
	int ini_longerbleedoutkey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "LongerBleedoutKey", 7, ".\\PedDamageOverhaul.ini");
	int ini_alwayskillwounded = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "AlwaysKillWounded", 0, ".\\PedDamageOverhaul.ini");
	int ini_alwaysbleedoutlonger = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "AlwaysBleedoutLonger", 0, ".\\PedDamageOverhaul.ini");
	int ini_enabled = GetPrivateProfileInt("PedDamageConfiguration_Basic", "EnableScriptByDefault", 1, ".\\PedDamageOverhaul.ini");
	int ini_playerinvincibility = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PlayerInvincibility", 0, ".\\PedDamageOverhaul.ini");
	int ini_survivfirchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FireSurvivalChance", 23, ".\\PedDamageOverhaul.ini");
	int ini_firehealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FireHealth", 320, ".\\PedDamageOverhaul.ini");
	int ini_pedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Basic", "NPCHealthMin", 75, ".\\PedDamageOverhaul.ini");
	int ini_pedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Basic", "NPCHealthMax", 75, ".\\PedDamageOverhaul.ini");
	int ini_pedhealthvehicle = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCHealthInVehicles", 70, ".\\PedDamageOverhaul.ini");
	float ini_pedhealthvehiclemodtods = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCHealthVehicleToDyingState", 75, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_pedhealthvehiclemodtofull = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCHealthVehicleToFull", 90, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_painthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "KnockbackThreshold", 70, ".\\PedDamageOverhaul.ini");
	int ini_knockbackchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "KnockbackChance", 2, ".\\PedDamageOverhaul.ini");
	int ini_dyingthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingThreshold", 20, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingMovementThreshold", 50, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementthreshold2 = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingMovementThreshold2", 30, ".\\PedDamageOverhaul.ini");
	int ini_stumblechancebothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumbleChanceBothLegs", 50, ".\\PedDamageOverhaul.ini");
	int ini_stumblechanceoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumbleChanceOneLeg", 10, ".\\PedDamageOverhaul.ini");
	int ini_dsstumblechancebothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSStumbleChanceBothLegs", 100, ".\\PedDamageOverhaul.ini");
	int ini_dsstumblechanceoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSStumbleChanceOneLeg", 20, ".\\PedDamageOverhaul.ini");
	int ini_bleedwhendying = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedWhenDying", 1, ".\\PedDamageOverhaul.ini");
	int ini_bleedwhenshot = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedWhenShot", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingvalue = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingValue", 2, ".\\PedDamageOverhaul.ini");
	int ini_runningthresholdbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RunningThresholdBothLegs", 3, ".\\PedDamageOverhaul.ini");
	int ini_runningthresholdoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RunningThresholdOneLeg", 3, ".\\PedDamageOverhaul.ini");
	int ini_dsrunningthresholdbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSRunningThresholdBothLegs", 3, ".\\PedDamageOverhaul.ini");
	int ini_dsrunningthresholdoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSRunningThresholdOneLeg", 3, ".\\PedDamageOverhaul.ini");
	float ini_femalepushmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FemalePushModifier", 3, ".\\PedDamageOverhaul.ini");
	float ini_forcepushx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushX", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushy = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushY", 320, ".\\PedDamageOverhaul.ini");
	float ini_forcepushz = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushxboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushX", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushyboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushY", 375, ".\\PedDamageOverhaul.ini");
	float ini_forcepushzboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_dforcepushx = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushX", 145, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_dforcepushy = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushY", 5, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_dforcepushz = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushZ", 5, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_dyingmovementenablespeed = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementEnableWhenSpeedLowerThan", 10, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_zvaluehead = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHeadBone", 260, ".\\PedDamageOverhaul.ini")) / 1000;
	float ini_zvaluehip = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHipBone", 500, ".\\PedDamageOverhaul.ini")) / 1000;
	int ini_ignoreupvec = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IgnoreUpVector", 1, ".\\PedDamageOverhaul.ini");
	int ini_dforceisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingForceRelative", 0, ".\\PedDamageOverhaul.ini");
	int ini_ddirectionisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingDirectionRelative", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingpushchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingPushChance", 600, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_shot = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceShot", 21, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_dying1 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying1", 12, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_dying2 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying2", 6, ".\\PedDamageOverhaul.ini");
	int ini_longerbleedingchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LongerBleedingChance", 3, ".\\PedDamageOverhaul.ini");
	int ini_longerbleedingvalue = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LongerBleedingValue", 4, ".\\PedDamageOverhaul.ini");
	int ini_dirreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeOneLeg", 1, ".\\PedDamageOverhaul.ini");
	int ini_forreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeOneLeg", 0, ".\\PedDamageOverhaul.ini");
	int ini_dirrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeBothLegs", 1, ".\\PedDamageOverhaul.ini");
	int ini_forrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeBothLegs", 0, ".\\PedDamageOverhaul.ini");
	int ini_lassodisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "LassoDisarms", 0, ".\\PedDamageOverhaul.ini");
	int ini_hogtiedisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "HogtyingDisarms", 1, ".\\PedDamageOverhaul.ini");
	int ini_disarmingchance = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisarmingChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_dmaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudioChance", 80, ".\\PedDamageOverhaul.ini");
	int ini_painaudio01 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio01", 4, ".\\PedDamageOverhaul.ini");
	int ini_painaudio02 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio02", 5, ".\\PedDamageOverhaul.ini");
	int ini_painaudio03 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio03", 9, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamounthealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountHealth", 6, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamountstamina = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountStamina", 6, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamountdeadeye = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountDeadeye", 6, ".\\PedDamageOverhaul.ini");
	int ini_coredepletiontime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionTime", 360000, ".\\PedDamageOverhaul.ini");
	int ini_coredepletion = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletion", 0, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionafterdeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAfterDeath", 0, ".\\PedDamageOverhaul.ini");
	int ini_stopcoredepletionafterdeathinmissions = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StopCoreDepletionAfterDeathInMissions", 0, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletionamounthealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletionAmountHealth", 5, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletionamountstamina = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletionAmountStamina", 5, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletiontime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletionTime", 360000, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletion = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletion", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingstatesmelee = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStatesForMelee", 0, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyhealthymin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyHealthyMin", 45, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyinjuredmin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyInjuredMin", 15, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyhealthymax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyHealthyMax", 65, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyinjuredmax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyInjuredMax", 35, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracythreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "NPCAccuracyThreshold", 70, ".\\PedDamageOverhaul.ini");
	int ini_dyingstatechance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_panicchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PanicChance", 2, ".\\PedDamageOverhaul.ini");
	int ini_panicchancerag = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PanicChanceHelpless", 5, ".\\PedDamageOverhaul.ini");
	int ini_excludefriendlystorynpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ExcludeFriendlyStoryNPCs", 1, ".\\PedDamageOverhaul.ini");
	int ini_excludeotherstorynpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ExcludeOtherStoryNPCs", 1, ".\\PedDamageOverhaul.ini");
	float ini_legdamagemod =  1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LegDamageModifier", 1, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_armdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ArmDamageModifier", 1, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_torsodamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TorsoDamageModifier", 120, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_headdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadDamageModifier", 150, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_neckdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NeckDamageModifier", 5, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_meleedyingstateknockout = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MeleeDyingStateKnockout", 0, ".\\PedDamageOverhaul.ini");
	int ini_useanimchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "UseAnimChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_transdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TransitioningDelta", 500, ".\\PedDamageOverhaul.ini");
	bool ini_specialhogtie = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateHogtie", 1, ".\\PedDamageOverhaul.ini");
	int ini_euphoriastumblechance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSEuphoriaStumbleChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_disarmfleeingchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmFleeingChance", 0, ".\\PedDamageOverhaul.ini");
	int ini_disarmattackchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmAttackChance", 0, ".\\PedDamageOverhaul.ini");
	int ini_disarmhandsupchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmHandsUpChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_disarmcowerchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisarmCowerChance", 0, ".\\PedDamageOverhaul.ini");
	int ini_modeffectrange = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ModEffectRange", 50, ".\\PedDamageOverhaul.ini");
	int ini_falltimedown = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FallTimeDown", 100000, ".\\PedDamageOverhaul.ini");
	int ini_falltimedeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FallTimeDeath", 100000, ".\\PedDamageOverhaul.ini");
	int ini_onlybleedshottorso = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OnlyBleedOnTorsoShot", 1, ".\\PedDamageOverhaul.ini");
	int ini_friendlyfire = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FriendlyFire", 0, ".\\PedDamageOverhaul.ini");
	int ini_npcdowntimeintervall = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCsDownIntervall", 3500, ".\\PedDamageOverhaul.ini");
	int ini_npcdownfearthreshold = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCsDownThreshold", 5, ".\\PedDamageOverhaul.ini");
	float ini_npcdownfearchance = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCFearChance", 200, ".\\PedDamageOverhaul.ini")) / 10;
	int ini_cowerchancefear = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearCowerChance", 5, ".\\PedDamageOverhaul.ini");
	int ini_handsupchancefear = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearHandsUpChance", 55, ".\\PedDamageOverhaul.ini");
	int ini_fleechancefear = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearFleeingChance", 40, ".\\PedDamageOverhaul.ini");	
	int ini_fearfleeultimately = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearFleeUltimately", 1, ".\\PedDamageOverhaul.ini");
	float ini_playersneaknoisemult = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SneakNoiseMultiplier", 0, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_reactiononaim = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateAimReaction", 75, ".\\PedDamageOverhaul.ini");
	int ini_ds1audio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DS1Audio", 1, ".\\PedDamageOverhaul.ini");
	int ini_ds2audio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DS2Audio", 1, ".\\PedDamageOverhaul.ini");
	int ini_horsehealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseHealth", 0, ".\\PedDamageOverhaul.ini");
	int ini_horsegod = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseInvincibility", 0, ".\\PedDamageOverhaul.ini");
	int ini_hogtiebleedingstop = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HogtieBleedingStop", 1, ".\\PedDamageOverhaul.ini");
	int ini_arteryshothealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ArteryShotHealth", 70, ".\\PedDamageOverhaul.ini");
	int ini_horsefallingdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseFallingDelta", 8000, ".\\PedDamageOverhaul.ini");
	int ini_maxdstime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MaximumDyingStateTime", 35000, ".\\PedDamageOverhaul.ini");
	int ini_disableitemglow = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisableItemGlow", 0, ".\\PedDamageOverhaul.ini");
	int ini_zvalueheadup = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHeadBone2", 3000, ".\\PedDamageOverhaul.ini");
	int ini_stumbletimemin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSEuphoriaStumbleMin", 2, ".\\PedDamageOverhaul.ini");
	int ini_stumbletimemax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSEuphoriaStumbleMax", 3000, ".\\PedDamageOverhaul.ini");
	int ini_torsooverwrite = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TorsoShotBleedingOverwrite", 0, ".\\PedDamageOverhaul.ini");
	int ini_npcbodydamagedelay = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BodyDamageInterval", 800, ".\\PedDamageOverhaul.ini");
	int ini_headvecmaxdeltab = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadVecMaxDeltaB", 20, ".\\PedDamageOverhaul.ini")) / 1000;
	int ini_headvecmaxtimeb = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadVecMaxTimeB", 2500, ".\\PedDamageOverhaul.ini");
	int ini_playerhealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PlayerHealth", 0, ".\\PedDamageOverhaul.ini");
	int ini_nooseaudio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseAudio", 1, ".\\PedDamageOverhaul.ini");
	int ini_nooseaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseAudioChance", 50, ".\\PedDamageOverhaul.ini");
	int ini_nooselivelonger = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NoosedNPCsLiveLonger", 1, ".\\PedDamageOverhaul.ini");
	int ini_noosehealthboost = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseHealthBoost", 4, ".\\PedDamageOverhaul.ini");
	int ini_noosehealthboostinterval = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseHealthBoostInterval", 1500, ".\\PedDamageOverhaul.ini");
	int ini_tylfeaturesenabled = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TYLFeaturesEnabled", 1, ".\\PedDamageOverhaul.ini");
	int ini_mutenoosednpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MuteNoosedNPCs", 0, ".\\PedDamageOverhaul.ini");
	float ini_hangingheight = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseHangingHeight", 20, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_noosefaceturn = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseFaceTurn", 1, ".\\PedDamageOverhaul.ini");
	int ini_moneylossafterdeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MoneyLossAfterDeath", 0, ".\\PedDamageOverhaul.ini");
	int ini_stopmoneylossafterdeathinmissions = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StopMoneyLossAfterDeathInMissions", 0, ".\\PedDamageOverhaul.ini");
	float ini_moneylosspercentage = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MoneyLossPercentage", 25, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_legshitthreshold = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LegsHitThreshold", 2, ".\\PedDamageOverhaul.ini");
	int ini_damagedlegsdowntime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DamagedLegsDownTime", 1500, ".\\PedDamageOverhaul.ini");
	int ini_spineshotsparalyze = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpineShotsParalyze", 0, ".\\PedDamageOverhaul.ini");
	int ini_paralysisaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpineParalysisAudioChance", 10, ".\\PedDamageOverhaul.ini");
	int ini_parachance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpineParalysisChance", 15, ".\\PedDamageOverhaul.ini");
	int ini_bleedingsave = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingSave", 100000, ".\\PedDamageOverhaul.ini");
	int ini_maxragdollspeed = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RagdollingSpeedThreshold", 7, ".\\PedDamageOverhaul.ini");
	int ini_maxragdollspeed2 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSRagdollingSpeedThreshold", 15, ".\\PedDamageOverhaul.ini");
	int ini_diversefireaudio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DiverseFireAudio", 1, ".\\PedDamageOverhaul.ini");
	int ini_ds2audiochancemod = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DS2AudioChanceModifier", 60, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask01 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask01", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask02 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask02", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask03 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask03", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask04 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask04", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask05 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask05", 0, ".\\PedDamageOverhaul.ini");
	int ini_nopushingdown = GetPrivateProfileInt("PedDamageConfiguration_Basic", "EuphoriaModInstalled", 0, ".\\PedDamageOverhaul.ini");
	int ini_handsuptime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpTime", 400000, ".\\PedDamageOverhaul.ini");
	int ini_combathandsuphostilenpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpOnDisarmingOnlyWhenAlone", 1, ".\\PedDamageOverhaul.ini");
	int ini_handsuphostilenpcthreshold = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpOnDisarmingAllyCount", 2, ".\\PedDamageOverhaul.ini");
	int ini_handsupdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpDelta", 500, ".\\PedDamageOverhaul.ini");
	float ini_handsupradius = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpRadius", 2000, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_dsforcedstumbling = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumblingBelowDSThreshold", 1, ".\\PedDamageOverhaul.ini");
	int ini_dismounthandsupdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpDeltaMounted", 500, ".\\PedDamageOverhaul.ini");
	int ini_blipfords = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MapBlipForNPCsInDS", 0, ".\\PedDamageOverhaul.ini");
	int ini_policebehavior = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ActivateAlternativePoliceBehavior", 0, ".\\PedDamageOverhaul.ini");
	int ini_policedismountdistancemin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DismountDistanceMin", 20, ".\\PedDamageOverhaul.ini");
	int ini_policedismountdistancemax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DismountDistanceMax", 50, ".\\PedDamageOverhaul.ini");
	int ini_policebehaviordisabledinmissions = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PoliceBehaviorDisabledInMissions", 1, ".\\PedDamageOverhaul.ini");
	int ini_excludebodyguardmodnpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ExcludeBodyGuardSpawnerBodyguards", 1, ".\\PedDamageOverhaul.ini");
	int toggleKey = VK_F9;
	int killwoundedkey = VK_F8;
	int longerbleedoutkey = VK_F7;
	int friendlyfirekey = VK_F2;

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

	switch (ini_killwoundedkey)
	{
	case 1:
		killwoundedkey = VK_F1;
		break;
	case 2:
		killwoundedkey = VK_F2;
		break;
	case 3:
		killwoundedkey = VK_F3;
		break;
	case 4:
		killwoundedkey = VK_F4;
		break;
	case 5:
		killwoundedkey = VK_F5;
		break;
	case 6:
		killwoundedkey = VK_F6;
		break;
	case 7:
		killwoundedkey = VK_F7;
		break;
	case 8:
		killwoundedkey = VK_F8;
		break;
	case 9:
		killwoundedkey = VK_F9;
		break;
	case 10:
		killwoundedkey = VK_F10;
		break;
	case 11:
		killwoundedkey = VK_F11;
		break;
	case 12:
		killwoundedkey = VK_F12;
		break;
	default:
		killwoundedkey = VK_F8;
	}

	switch (ini_longerbleedoutkey)
	{
	case 1:
		longerbleedoutkey = VK_F1;
		break;
	case 2:
		longerbleedoutkey = VK_F2;
		break;
	case 3:
		longerbleedoutkey = VK_F3;
		break;
	case 4:
		longerbleedoutkey = VK_F4;
		break;
	case 5:
		longerbleedoutkey = VK_F5;
		break;
	case 6:
		longerbleedoutkey = VK_F6;
		break;
	case 7:
		longerbleedoutkey = VK_F7;
		break;
	case 8:
		longerbleedoutkey = VK_F8;
		break;
	case 9:
		longerbleedoutkey = VK_F9;
		break;
	case 10:
		longerbleedoutkey = VK_F10;
		break;
	case 11:
		longerbleedoutkey = VK_F11;
		break;
	case 12:
		longerbleedoutkey = VK_F12;
		break;
	default:
		longerbleedoutkey = VK_F7;
	}

	switch (ini_friendlyfirekey)
	{
	case 1:
		friendlyfirekey = VK_F1;
		break;
	case 2:
		friendlyfirekey = VK_F2;
		break;
	case 3:
		friendlyfirekey = VK_F3;
		break;
	case 4:
		friendlyfirekey = VK_F4;
		break;
	case 5:
		friendlyfirekey = VK_F5;
		break;
	case 6:
		friendlyfirekey = VK_F6;
		break;
	case 7:
		friendlyfirekey = VK_F7;
		break;
	case 8:
		friendlyfirekey = VK_F8;
		break;
	case 9:
		friendlyfirekey = VK_F9;
		break;
	case 10:
		friendlyfirekey = VK_F10;
		break;
	case 11:
		friendlyfirekey = VK_F11;
		break;
	case 12:
		friendlyfirekey = VK_F12;
		break;
	default:
		friendlyfirekey = VK_F2;
	}

	float runningthresholdbothlegs = ini_runningthresholdbothlegs / 10;
	float runningthresholdoneleg = ini_runningthresholdoneleg / 10;
	float dsrunningthresholdbothlegs = ini_dsrunningthresholdbothlegs / 10;
	float dsrunningthresholdoneleg = ini_dsrunningthresholdoneleg / 10;
	float maxragdollspeed = ini_maxragdollspeed / 10;
	float maxragdollspeed2 = ini_maxragdollspeed2 / 10;

	//making sure disarm reaction chances are set to valid values
	if (ini_disarmattackchance + ini_disarmcowerchance + ini_disarmfleeingchance + ini_disarmhandsupchance != 100)
	{
		ini_disarmattackchance = 55;
		ini_disarmfleeingchance = 31;
		ini_disarmhandsupchance = 7;
		ini_disarmcowerchance = 7;
	}

	//making sure combat fear reaction chances are set to valid values
	if (ini_cowerchancefear + ini_handsupchancefear + ini_fleechancefear != 100)
	{
		ini_cowerchancefear = 5;
		ini_handsupchancefear = 55;
		ini_fleechancefear = 40;
	}

	//container for peds and their attributes
	std::map<Ped, PedAttributes> pedmap;
	
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
	allweaponmap[55] = "WEAPON_REVOLVER_NAVY";

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
	weaponmap[12] = GAMEPLAY::GET_HASH_KEY("WEAPON_RIFLE_BOLTACTION");
	weaponmap[13] = GAMEPLAY::GET_HASH_KEY("WEAPON_RIFLE_SPRINGFIELD");

	//container for special ammo (affected by special weapon damage modifier)
	std::map<Hash, Hash> ammomap;
	ammomap[GAMEPLAY::GET_HASH_KEY("AMMO_ARROW_DYNAMITE")] = GAMEPLAY::GET_HASH_KEY("WEAPON_BOW");
	ammomap[GAMEPLAY::GET_HASH_KEY("AMMO_ARROW_IMPROVED")] = GAMEPLAY::GET_HASH_KEY("WEAPON_BOW");

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
	vector<int> legbones, armbones, torsobones, headbones, neckbones, torsobonesbleeding;

	legbones.push_back(6884); //SKEL_R_Thigh
	legbones.push_back(65478); //SKEL_L_Thigh
	legbones.push_back(55120); //SKEL_L_CALF
	legbones.push_back(33646); //unknown leg bone
	legbones.push_back(43312); //SKEL_R_Calf
	legbones.push_back(45454); //SKEL_L_Foot

	armbones.push_back(34606); //SKEL_L_Hand
	armbones.push_back(22798); //SKEL_R_Hand
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

	neckbones.push_back(14283); //SKEL_Neck0
	neckbones.push_back(14284); //SKEL_Neck1
	neckbones.push_back(14285); //SKEL_Neck2
	neckbones.push_back(14286); //SKEL_Neck3
	neckbones.push_back(14287); //SKEL_Neck4
	neckbones.push_back(14288); //SKEL_Neck5

	torsobonesbleeding.push_back(14410); //unknown torso bone
	torsobonesbleeding.push_back(14411); //SKEL_Spine1
	torsobonesbleeding.push_back(14412); //SKEL_Spine2
	torsobonesbleeding.push_back(14413); //SKEL_Spine3
	torsobonesbleeding.push_back(14414); //SKEL_Spine4
	torsobonesbleeding.push_back(14415); //SKEL_Spine5
	torsobonesbleeding.push_back(30226); //left shoulder
	torsobonesbleeding.push_back(54802); //right shoulder
	torsobonesbleeding.push_back(14283); //SKEL_Neck0
	torsobonesbleeding.push_back(14284); //SKEL_Neck1
	torsobonesbleeding.push_back(14285); //SKEL_Neck2
	torsobonesbleeding.push_back(14286); //SKEL_Neck3
	torsobonesbleeding.push_back(14287); //SKEL_Neck4
	torsobonesbleeding.push_back(14288); //SKEL_Neck5
	
	//fire audio interval
	int fireaudiointerval = 4000;

	//timer for core depletion
	int coredepletiontimer = GAMEPLAY::GET_GAME_TIMER() + ini_coredepletiontime;
	int horsecoredepletiontimer = GAMEPLAY::GET_GAME_TIMER() + ini_horsecoredepletiontime;

	//lasso-bools for checking states
	bool ishogtied = false;
	bool islassoed = false;

	//bool for checking if NPC is standing or not
	bool isstanding = true;

	//bool for core depletion and money loss after death
	bool playerdead = false;
	int playermoney = -1;
	int deathtime;

	//counter for NPCs in combat with the player
	int combatcounter = 0;

	//knockdown time (how long NPC is ragdolled)
	int kotime = 20000;

	//variables for downed NPCs (used when deciding if NPCs flee out of fear) 
	int npcdowncounter = 0;
	int npcdowntimeintervallx = 0;

	//values for drowning
	int drowningticker = 0;
	int drowningintervall = 20000;
	int drowningdamagetime = 8;

	//is script activated by default or not
	bool modScriptState = false;
	if (ini_enabled == 1) modScriptState = true;
	int msgTime = 0;

	//killing wounded settings
	bool killwoundedbool = false;
	int msgTimeKillWounded = 0;

	//firendly fire settings
	bool friendlyfirebool = false;
	int msgTimeFriendlyFire = 0;
	bool ffchangeexecuted = false;

	//killing wounded settings
	bool longerbleedoutbool = false;
	int msgTimeLongerBleedout = 0;
	int longerbleedingvalue = ini_longerbleedingvalue;
	int longerbleedingchance = ini_longerbleedingchance;
	int bleedingvaluetemp = ini_bleedingvalue;
	int bleedingchancetemp = ini_bleedingchance_dying2;
	
	//task id
	char c[60], d[60], e[60], f[60], g[60], h[60], j[60];
	std::string text = "NPC Health: ", textpm = "Hostile NPC Count: ", textstat = "Ped Status: ", ttext = "NPC Tasks: ", mtext = "NPC Model: ", torso = "Torso hit: ", weapondam = "Damaged by weapon: ", legshit = "Legs hit: ", dyingbool = "DS enabled: ", dshistory = "DS visited (1, 2, 3): ", lastdambone = "Last damaged bone: ";
	int taskid = 99999;
	int lastbonedamaged = 0;
	int lastbonedamagedtemp = 0;
	Entity playerTargetyolo = PLAYER::PLAYER_PED_ID();
	Entity playerTargetOld = playerTargetyolo;
	
	//main script
	while (true)
	{
		//check if script is being enabled or disabled
		if (IsKeyJustUp(toggleKey))
		{
			modScriptState = !modScriptState;
			msgTime = GetTickCount() + 3000;
		}

		if (IsKeyJustUp(killwoundedkey))
		{
			killwoundedbool = !killwoundedbool;
			msgTimeKillWounded = GetTickCount() + 3000;
		}

		if (IsKeyJustUp(longerbleedoutkey))
		{
			longerbleedoutbool = !longerbleedoutbool;
			msgTimeLongerBleedout = GetTickCount() + 3000;
		}

		if (IsKeyJustUp(friendlyfirekey))
		{
			friendlyfirebool = !friendlyfirebool;
			msgTimeFriendlyFire = GetTickCount() + 3000;
			if (!friendlyfirebool) ffchangeexecuted = false;
		}

		//get if a mission is currently running
		bool missionactive = GAMEPLAY::GET_MISSION_FLAG();

//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								ACTUAL MOD FEATURES STARTING, ID: PDO002
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

		//if script enabled, start doing your thing
		if (modScriptState && !(missionactive && ini_disablemodinmissions == 1))
		{			
			//get player and player ped as well as the identifying hash for the lasso weapon and a variable for NPCs getting caught by it and another one for checking if player damages hogtied NPCs
			Player player = PLAYER::PLAYER_ID();
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			Hash lassoHash = GAMEPLAY::GET_HASH_KEY("WEAPON_LASSO");
			Ped lastTarget = playerPed; //used for lasso disarming
			Ped lastTarget2 = playerPed; //used for hogtied panicking
			Ped lastTargetx = playerPed; //used for special hogtie fix for dying movement writhing animation
			Ped lastTargety = playerPed; //used for NPC screaming on being attacked
			
			//setting the longer bleeding, firendly fire and killing wounded overrides
			if (ini_alwaysbleedoutlonger == 1) longerbleedoutbool = true;
			if (ini_alwayskillwounded == 1) killwoundedbool = true;
			if (ini_friendlyfire == 1) friendlyfirebool = true;

			//set longer bleedout if enabled
			if (longerbleedoutbool)
			{
				ini_bleedingchance_dying2 = longerbleedingchance;
				ini_bleedingvalue = longerbleedingvalue;
			}
			else
			{
				ini_bleedingchance_dying2 = bleedingchancetemp;
				ini_bleedingvalue = bleedingvaluetemp;
			}

			//set invincibility, if set in ini or adjust player health
			if (ini_playerinvincibility == 1) PLAYER::SET_PLAYER_INVINCIBLE(player, true);
			else if (ini_playerhealth != 0 && ENTITY::GET_ENTITY_MAX_HEALTH(playerPed, false) != ini_playerhealth)
			{
				ENTITY::SET_ENTITY_MAX_HEALTH(playerPed, ini_playerhealth);
				ENTITY::SET_ENTITY_HEALTH(playerPed, ini_playerhealth, false);
			}

			//sneak noise multiplier
			if (ini_playersneaknoisemult > 0) PLAYER::SET_PLAYER_SNEAKING_NOISE_MULTIPLIER(player, ini_playersneaknoisemult);

			//handle counter for peds being down (used for peds fleeing in combat)
			if (GAMEPLAY::GET_GAME_TIMER() > npcdowntimeintervallx)
			{
				npcdowncounter = 0;
				npcdowntimeintervallx = GAMEPLAY::GET_GAME_TIMER() + ini_npcdowntimeintervall;
			}

			//setting the drowningticker, so NPCs get health reduced when drowning (every 5 seconds)
			if (drowningticker < GAMEPLAY::GET_GAME_TIMER()) drowningticker = GAMEPLAY::GET_GAME_TIMER() + drowningintervall;

			//core depletion player
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

			//core depletion horse
			if (ini_horsecoredepletion == 1)
			{
				int horsecorehealth = ATTRIBUTE::_0x36731AC041289BB1(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(player), 0);
				int horsecorestamina = ATTRIBUTE::_0x36731AC041289BB1(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(player), 1);
				if (GAMEPLAY::GET_GAME_TIMER() > horsecoredepletiontimer)
				{
					if ((horsecorehealth - ini_horsecoredepletionamounthealth) < 0) ATTRIBUTE::_0xC6258F41D86676E0(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(player), 0, 0);
					else ATTRIBUTE::_0xC6258F41D86676E0(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(player), 0, horsecorehealth - ini_horsecoredepletionamounthealth);
					if ((horsecorestamina - ini_horsecoredepletionamountstamina) < 0) ATTRIBUTE::_0xC6258F41D86676E0(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(player), 1, 0);
					else ATTRIBUTE::_0xC6258F41D86676E0(PLAYER::_GET_SADDLE_HORSE_FOR_PLAYER(player), 1, horsecorestamina - ini_horsecoredepletionamountstamina);
					horsecoredepletiontimer = GAMEPLAY::GET_GAME_TIMER() + ini_horsecoredepletiontime;
				}
			}

			//core depletion after death
			if (ini_coredepletionafterdeath == 1 || ini_moneylossafterdeath == 1)
			{
				if (PLAYER::IS_PLAYER_DEAD(player))
				{
					playerdead = true;
					if (playermoney == -1) playermoney = (int)((float)CASH::GET_PLAYER_CURRENT_CASH() * (1 - ini_moneylosspercentage));
					deathtime = GAMEPLAY::GET_GAME_TIMER() + 15000;
				}
				if (!PLAYER::IS_PLAYER_DEAD(player) && playerdead)
				{
					if (ini_coredepletionafterdeath == 1)
					{
						if (ini_stopcoredepletionafterdeathinmissions == 1)
						{
							if (!missionactive)
							{
								ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 0, 0);
								ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 1, 0);
								ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 2, 0);
							}
						}
						else
						{
							ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 0, 0);
							ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 1, 0);
							ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 2, 0);
						}
					}
					
					if (ini_moneylossafterdeath == 1)
					{
						if (ini_stopmoneylossafterdeathinmissions == 1)
						{
							if (!missionactive)
							{
								if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 10000) CASH::PLAYER_DECREASE_CASH(10000);
								else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 1000) CASH::PLAYER_DECREASE_CASH(1000);
								else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 100) CASH::PLAYER_DECREASE_CASH(100);
								else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 10) CASH::PLAYER_DECREASE_CASH(10);
								else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney) CASH::PLAYER_DECREASE_CASH(1);
							}
						}
						else
						{
							if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 10000) CASH::PLAYER_DECREASE_CASH(10000);
							else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 1000) CASH::PLAYER_DECREASE_CASH(1000);
							else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 100) CASH::PLAYER_DECREASE_CASH(100);
							else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 10) CASH::PLAYER_DECREASE_CASH(10);
							else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney) CASH::PLAYER_DECREASE_CASH(1);
						}
					}
					
					if (GAMEPLAY::GET_GAME_TIMER() > deathtime)
					{
						playerdead = false;
						playermoney = -1;
					}
				}
			}

			//set NPC damage modifiers
			if (ini_npcmeleemodifier != 0) PED::SET_AI_MELEE_WEAPON_DAMAGE_MODIFIER(ini_npcmeleemodifier);
			if (ini_npcweaponmodifier != 0) PED::SET_AI_WEAPON_DAMAGE_MODIFIER(ini_npcweaponmodifier);

			//get all peds near player (those will be affected by the script)
			const int ARR_SIZE = 150;
			Ped peds[ARR_SIZE];
			//int count = worldGetAllPeds(peds, ARR_SIZE);
			int count = worldGetAllPeds(peds, ini_modeffectrange);
			
			if (ini_disableitemglow != 0) //disable item glow
			{
				const int pickupsRange = 1024, objectsRange = 1024;
				Object pickups[pickupsRange], objects[objectsRange];

				int pickupcount = worldGetAllPickups(pickups, pickupsRange);
				int objectcount = worldGetAllObjects(objects, objectsRange);

				for (int i = 0; i < pickupcount; i++)
				{
					GRAPHICS::x_0x50C14328119E1DD1(pickups[i], true);
				}

				for (int i = 0; i < objectcount; i++)
				{
					GRAPHICS::x_0x50C14328119E1DD1(objects[i], true);
				}
			}

			//filling container for peds and giving them default values
			for (int i = 0; i < count; i++)
			{
				if (pedmap.find(peds[i]) == pedmap.end())
				{
					PedAttributes p;
					
					/*status:
					0 = default
					1 = friendly story character
					2 = other story character
					3 = none of the above
					*/
					p.isstorychar = 0;
					
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
					p.status = 0;

					//status: 0 = default value
					p.health = 0;

					//status: 0 = default value, >0 = was already in dying state 3 at least once
					p.dyingstate3entered = 0;

					//status: 0 = default value, >0 = was already in dying state 2 at least once
					p.dyingstate2entered = 0;

					//status: 0 = default value, >0 = was already in dying state 1 at least once
					p.dyingstate1entered = 0;

					//status: -1 = default value, will later be filled with a game timer value to make transition into writhing animation occur after a while (looks smoother than if it is done immediately)
					p.animtrans = -1;

					//status: 0 = default value
					p.healthds = 0;

					//status: 0 = disabled, 1 = enabled
					int randu = 0 + (std::rand() % (99 - 0 + 1));
					if (randu < ini_dyingstatechance) p.dyingstatebool = 1;
					else p.dyingstatebool = 0;

					//gets filled with current NPC health values, so later the delta (damage done) can be computed
					p.limbhealth = ENTITY::GET_ENTITY_HEALTH(peds[i]);

					//status: 0 = has not been knocked out, 1 = has been knocked out and effects pending, 2 = has been knocked out and effects have been triggered
					p.isknockedout = 0;

					//status: 0 = do not use animation, 1 = use animation
					int animrand = 0 + (std::rand() % (99 - 0 + 1));
					if (animrand < ini_useanimchance) p.dsanimationused = 1;
					else p.dsanimationused = 0;

					//status: 0 = euphoria stumbing disabled, 1 = euphoria stumbling enabled
					int randi = 0 + (std::rand() % (99 - 0 + 1));
					if (randi < ini_euphoriastumblechance) p.euphstumblingenabled = 1;
					else p.euphstumblingenabled = 0;

					//status: 0 = default value
					p.isfalling = 0;

					//status: 0 = default value
					p.falltime = 0;

					//status: 0 = default value, 1 = to be killed
					p.tobekilled = 0;

					//status: 0 = default value, 1 = to be downed
					p.tobedowned = 0;

					//status: 0 = was not hit in the torso, 1 = was hit in the torso
					p.torsohit = 0;

					//status: 0 = default value
					p.accuracy = 0;

					//status: 0 = not down, 1 = down
					p.isdown = 0;

					//status: 0 = not dead, 1 = dead
					p.isdead = 0;

					//status: 0 = has not reacted to friends dying, 1 = has reacted to friends dying (hands up or cowering), 2 = has reacted to friends dying (fleeing), 9 = fleeing after status 1
					p.hasreacted = 0;

					//status: false = bleeding will apply, true = the ped won't bleed
					p.nobleeding = false;

					//status: 0 = has been on a mount
					p.lasttimeonmount = 0;
					
					p.handsup = 0;

					p.firsttimehandsup = 0;

					//status: 0 = no stumbling time calculated yet
					p.stumblingtime = 0;

					p.headvecdelta.x = 0;
					p.headvecdelta.y = 0;
					p.headvecdelta.z = 0;

					p.headvectime = 0;

					p.lastfearaudio = 0;

					p.rightleghitcounter = 0;

					p.leftleghitcounter = 0;

					p.legsdamageddowntime = 0;

					p.legsdownsurvivaltimer = 0;

					p.fireaudiotime = 0;

					p.fireaudiosample = 0;

					p.arteryshot = false;

					p.wasaimedat = false;

					p.hasbeendamagedbyweapon = false;

					p.ishanging = false;

					p.hangingtime = 0;

					p.shotbyplayer = 0;

					p.wasnotaloneincombat = false;

					p.isincombatwithplayer = false;

					p.isarmed = false;

					p.wasdisarmed = false;

					p.movementstyle = "";

					p.blipset = 0;

					p.dismount = 0;

					p.disarmingstop = false;

					if (PED::GET_PED_RELATIONSHIP_GROUP_HASH(peds[i]) == GAMEPLAY::GET_HASH_KEY("REL_COP"))
					{
						p.ispolice = true;
						int dismrand = ini_policedismountdistancemin + (std::rand() % (ini_policedismountdistancemax - ini_policedismountdistancemin + 1));
						p.policedismountdistance = dismrand;
					}
					else p.ispolice = false;

					p.maxhealth = 0;

					p.usemaxhealth = 0;

					pedmap[peds[i]] = p;
				}
			}


//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								PLAYER DAMAGE MODIFIERS, ID: PDO003
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


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
				Ped playertarget, playertarget2;
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(player, &playertarget) || PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playertarget2))
				{
					if (!PED::IS_PED_HUMAN(playertarget)) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 1.0);
					if (!PED::IS_PED_HUMAN(playertarget2)) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, 1.0);
				}
				else if (specialweapon || specialammo) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, ini_sweaponmodifier);
				else if (knife) PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, ini_knifemodifier);
				else
				{
					PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(player, ini_weaponmodifier);
					PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(player, ini_meleemodifier);
				}
			}


//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								AFFECTING NPCS, ID: PDO004
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


			//iterate through NPCs, check them for stuff and apply stuff (main function of this mod)
			for (int i = 0; i < count; i++)
			{
				//remove blip if it still exists
				if (ini_blipfords == 1)
				{
					if (pedmap[peds[i]].blipset != 0)
					{
						if (ENTITY::IS_ENTITY_DEAD(peds[i]) && RADAR::DOES_BLIP_EXIST(pedmap[peds[i]].blipset)) RADAR::REMOVE_BLIP(&pedmap[peds[i]].blipset);
					}

					if (PLAYER::IS_PLAYER_DEAD(player))
					{
						for (int i = 0; i < count; i++)
						{
							if (pedmap[peds[i]].blipset != 0 && RADAR::DOES_BLIP_EXIST(pedmap[peds[i]].blipset)) RADAR::REMOVE_BLIP(&pedmap[peds[i]].blipset);
						}
					}
				}
				
				//show ped health
				if (ini_shownpchealth == 1)
				{
					std::strcpy(c, text.c_str());
					std::strcpy(d, torso.c_str());
					std::strcpy(e, weapondam.c_str());
					std::strcpy(f, legshit.c_str());
					std::strcpy(g, dyingbool.c_str());
					std::strcpy(h, dshistory.c_str());
					std::strcpy(j, lastdambone.c_str());
					DrawText(0.55, 0.05, c);
					DrawText(0.55, 0.15, j);
					DrawText(0.55, 0.25, e);
					DrawText(0.55, 0.35, d);					
					DrawText(0.55, 0.45, f);
					DrawText(0.55, 0.55, g);
					DrawText(0.55, 0.65, h);
					
					if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
					{
						Entity playerTargetTemp = playerTargetyolo;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetyolo))
						{
							playerTargetOld = playerTargetTemp;
						}
					}
					if (playerTargetOld != playerPed && playerTargetOld != playerTargetyolo)
					{
						text = "NPC Health: ";
						std::strcpy(c, text.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						PED::GET_PED_LAST_DAMAGE_BONE(playerTargetyolo, &lastbonedamagedtemp);
						if (lastbonedamagedtemp != 0) lastbonedamaged = lastbonedamagedtemp;
						
						text = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(playerTargetyolo));
						torso = "Torso hit: " + std::to_string(pedmap[playerTargetyolo].torsohit);
						weapondam = "Damaged by weapon: " + std::to_string(pedmap[playerTargetyolo].hasbeendamagedbyweapon);
						legshit = "Legs hit: " + std::to_string(pedmap[playerTargetyolo].legsdownsurvivaltimer);
						dyingbool = "DS enabled: " + std::to_string(pedmap[playerTargetyolo].dyingstatebool);
						dshistory = "DS visited (1, 2, 3): " + std::to_string(pedmap[playerTargetyolo].dyingstate1entered) + ", " + std::to_string(pedmap[playerTargetyolo].dyingstate2entered) + ", " + std::to_string(pedmap[playerTargetyolo].dyingstate3entered);
						lastdambone = "Last damaged bone: " + std::to_string(lastbonedamaged);
						std::strcpy(c, text.c_str());
						std::strcpy(d, torso.c_str());
						std::strcpy(e, weapondam.c_str());
						std::strcpy(f, legshit.c_str());
						std::strcpy(g, dyingbool.c_str());
						std::strcpy(h, dshistory.c_str());
						std::strcpy(j, lastdambone.c_str());
						DrawText(0.55, 0.05, c);
						DrawText(0.55, 0.15, j);
						DrawText(0.55, 0.25, e);
						DrawText(0.55, 0.35, d);
						DrawText(0.55, 0.45, f);
						DrawText(0.55, 0.55, g);
						DrawText(0.55, 0.65, h);
					}
				}

				if (ini_shownpchealth == 2)
				{
					std::strcpy(c, text.c_str());
					std::strcpy(d, torso.c_str());
					std::strcpy(e, textstat.c_str());
					std::strcpy(f, legshit.c_str());
					std::strcpy(g, dyingbool.c_str());
					std::strcpy(h, dshistory.c_str());
					std::strcpy(j, textpm.c_str());
					DrawText(0.55, 0.05, c);
					DrawText(0.55, 0.15, j);
					DrawText(0.55, 0.25, e);
					DrawText(0.55, 0.35, d);
					DrawText(0.55, 0.45, f);
					DrawText(0.55, 0.55, g);
					DrawText(0.55, 0.65, h);

					if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
					{
						Entity playerTargetTemp = playerTargetyolo;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetyolo))
						{
							playerTargetOld = playerTargetTemp;
						}
					}
					if (playerTargetOld != playerPed && playerTargetOld != playerTargetyolo)
					{
						text = "NPC Health: ";
						std::strcpy(c, text.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						PED::GET_PED_LAST_DAMAGE_BONE(playerTargetyolo, &lastbonedamagedtemp);
						if (lastbonedamagedtemp != 0) lastbonedamaged = lastbonedamagedtemp;

						text = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(playerTargetyolo));
						torso = "Torso hit: " + std::to_string(pedmap[playerTargetyolo].torsohit);
						legshit = "Legs hit: " + std::to_string(pedmap[playerTargetyolo].legsdownsurvivaltimer);
						dyingbool = "DS enabled: " + std::to_string(pedmap[playerTargetyolo].dyingstatebool);
						dshistory = "DS visited (1, 2, 3): " + std::to_string(pedmap[playerTargetyolo].dyingstate1entered) + ", " + std::to_string(pedmap[playerTargetyolo].dyingstate2entered) + ", " + std::to_string(pedmap[playerTargetyolo].dyingstate3entered);
						textpm = "Hostile NPC Count: " + std::to_string(combatcounter);
						textstat = "NPC Status: " + std::to_string(pedmap[playerTargetyolo].status);
						std::strcpy(c, text.c_str());
						std::strcpy(d, torso.c_str());
						std::strcpy(e, textstat.c_str());
						std::strcpy(f, legshit.c_str());
						std::strcpy(g, dyingbool.c_str());
						std::strcpy(h, dshistory.c_str());
						std::strcpy(j, textpm.c_str());
						DrawText(0.55, 0.05, c);
						DrawText(0.55, 0.15, j);
						DrawText(0.55, 0.25, e);
						DrawText(0.55, 0.35, d);
						DrawText(0.55, 0.45, f);
						DrawText(0.55, 0.55, g);
						DrawText(0.55, 0.65, h);
					}
				}

				//show ped model
				if (ini_showpedmodel == 1)
				{
					
					DrawText(0.5, 0.15, c);
					
					if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
					{
						Entity playerTargetTemp = playerTargetyolo;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetyolo))
						{
							playerTargetOld = playerTargetTemp;
						}
					}
					if (playerTargetOld != playerPed && playerTargetOld != playerTargetyolo)
					{
						mtext = "NPC Model: ";
						std::strcpy(c, mtext.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						mtext = "NPC Model: " + std::to_string(ENTITY::GET_ENTITY_MODEL(playerTargetyolo));
						std::strcpy(c, mtext.c_str());
						DrawText(0.5, 0.15, c);
					}
				}

				//show task ids
				if (ini_showpedtasks == 1)
				{
					std::strcpy(c, ttext.c_str());
					DrawText(0.5, 0.15, c);

					if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
					{
						Entity playerTargetTemp = playerTargetyolo;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetyolo))
						{
							playerTargetOld = playerTargetTemp;
						}
					}
					if (playerTargetOld != playerPed && playerTargetOld != playerTargetyolo)
					{
						ttext = "NPC Tasks: ";
						std::strcpy(c, ttext.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						for (int g = 0; g < 999; g++)
						{
							if (g != 582 && g != 594 && AI::GET_IS_TASK_ACTIVE(playerTargetyolo, g))
							{
								if (ttext.find("," + std::to_string(g) + ",") == string::npos)
								{
									if (ttext == "NPC Tasks: ") ttext += std::to_string(g);
									else ttext += "," + std::to_string(g);
									std::strcpy(c, ttext.c_str());
									DrawText(0.5, 0.15, c);
								}
							}
						}
					}
				}
				
				
				//checking if current NPC is a story character and calculate max health (if its used is decided elsewhere)
				if (pedmap[peds[i]].isstorychar == 0) {
					int charstat = isPedStoryChar(peds[i]);
					int maxhealth;
					int minhealth;
					if (charstat == 1)
					{
						minhealth = ini_friendlystorypedhealthmin;
						maxhealth = ini_friendlystorypedhealthmax;
					}
					else if (charstat == 2)
					{
						minhealth = ini_otherstorypedhealthmin;
						maxhealth = ini_otherstorypedhealthmax;
					}
					else
					{
						if (pedmap[peds[i]].ispolice)
						{
							minhealth = ini_lawpedhealthmin;
							maxhealth = ini_lawpedhealthmax;
						}
						else
						{
							minhealth = ini_pedhealthmin;
							maxhealth = ini_pedhealthmax;
						}
					}

					pedmap[peds[i]].isstorychar = charstat;

					if (pedmap[peds[i]].maxhealth == 0)
					{
						pedmap[peds[i]].maxhealth = minhealth + (std::rand() % (maxhealth - minhealth + 1));
					}
				}

				//set health of story characters, if set in the ini
				//set if story characters are affected by the mod
				if (pedmap[peds[i]].isstorychar == 1)
				{
					if (ini_excludefriendlystorynpcs != 1) pedmap[peds[i]].isstorychar = 3;

					if (pedmap[peds[i]].usemaxhealth == 0)
					{
						if (ini_friendlystorypedhealthmin > 0 && ini_friendlystorypedhealthmax > 0)
						{
							pedmap[peds[i]].usemaxhealth = 2;
							if (PED::GET_PED_MAX_HEALTH(peds[i]) != pedmap[peds[i]].maxhealth)
							{
								PED::SET_PED_MAX_HEALTH(peds[i], pedmap[peds[i]].maxhealth);
								ENTITY::SET_ENTITY_HEALTH(peds[i], pedmap[peds[i]].maxhealth, 0);
							}
						}
					}
				}
				else if (pedmap[peds[i]].isstorychar == 2)
				{
					if (ini_excludeotherstorynpcs != 1) pedmap[peds[i]].isstorychar = 3;

					if (pedmap[peds[i]].usemaxhealth == 0)
					{
						if (ini_otherstorypedhealthmin > 0 && ini_otherstorypedhealthmax > 0)
						{
							pedmap[peds[i]].usemaxhealth = 2;
							if (PED::GET_PED_MAX_HEALTH(peds[i]) != pedmap[peds[i]].maxhealth)
							{
								PED::SET_PED_MAX_HEALTH(peds[i], pedmap[peds[i]].maxhealth);
								ENTITY::SET_ENTITY_HEALTH(peds[i], pedmap[peds[i]].maxhealth, 0);
							}
						}
					}
				}
				else if (pedmap[peds[i]].ispolice)
				{
					if (pedmap[peds[i]].usemaxhealth == 0)
					{
						if (ini_lawpedhealthmin > 0 && ini_lawpedhealthmax > 0)
						{
							pedmap[peds[i]].usemaxhealth = 2;
							if (PED::GET_PED_MAX_HEALTH(peds[i]) != pedmap[peds[i]].maxhealth)
							{
								PED::SET_PED_MAX_HEALTH(peds[i], pedmap[peds[i]].maxhealth);
								ENTITY::SET_ENTITY_HEALTH(peds[i], pedmap[peds[i]].maxhealth, 0);
							}
						}
					}
				}
				else pedmap[peds[i]].usemaxhealth = 1;

				//friendly fire
				if (friendlyfirebool)
				{
					if (PED::GET_RELATIONSHIP_BETWEEN_GROUPS(GAMEPLAY::GET_HASH_KEY("PLAYER"), GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS")) == 2)
					{
						PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, GAMEPLAY::GET_HASH_KEY("PLAYER"), GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS"));
						PED::SET_RELATIONSHIP_BETWEEN_GROUPS(0, GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS"), GAMEPLAY::GET_HASH_KEY("PLAYER"));
					}
				}
				else if (!ffchangeexecuted)
				{
					if (PED::GET_RELATIONSHIP_BETWEEN_GROUPS(GAMEPLAY::GET_HASH_KEY("PLAYER"), GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS")) != 2)
					{
						PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, GAMEPLAY::GET_HASH_KEY("PLAYER"), GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS"));
						PED::SET_RELATIONSHIP_BETWEEN_GROUPS(2, GAMEPLAY::GET_HASH_KEY("REL_GANG_DUTCHS"), GAMEPLAY::GET_HASH_KEY("PLAYER"));
					}
					ffchangeexecuted = true;
				}

				//horse health and invincibility
				if (ini_horsegod == 1)
				{
					if (PED::IS_PED_ON_MOUNT(playerPed))
					{
						Ped mount = PED::GET_MOUNT(playerPed);
						ENTITY::SET_ENTITY_INVINCIBLE(mount, true);
					}
				}
				else if (ini_horsehealth != 0)
				{
					if (PED::IS_PED_ON_MOUNT(playerPed))
					{
						Ped mount = PED::GET_MOUNT(playerPed);
						if (ENTITY::GET_ENTITY_MAX_HEALTH(mount, false) != ini_horsehealth)
						{
							ENTITY::SET_ENTITY_MAX_HEALTH(mount, ini_horsehealth);
							ENTITY::SET_ENTITY_HEALTH(mount, ENTITY::GET_ENTITY_MAX_HEALTH(mount, false), 0);
						}
					}
				}



//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								AFFECTING NPCS (ONLY HUMAN ONES), ID: PDO005
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

				//handle dying peds (for reactions in combat -> NPC fear feature)
				if (ENTITY::IS_ENTITY_DEAD(peds[i]) && pedmap[peds[i]].isdead == 0)
				{
					pedmap[peds[i]].isdead = 1;
					npcdowncounter++;
					if (pedmap[peds[i]].isincombatwithplayer)
					{
						pedmap[peds[i]].isincombatwithplayer = false;
						combatcounter--;
						if (combatcounter < 0) combatcounter = 0;
					}
				}
			
			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
			//
			//
			//			BEGIN: AFFECTING LIVING NPCs, ID: PDO006
			//
			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

				bool excludebodyguardspawner;

				if (ini_excludebodyguardmodnpcs == 1 && DECORATOR::DECOR_EXIST_ON(peds[i], "BodyguardSpawner")) excludebodyguardspawner = false;
				else excludebodyguardspawner = true;
			
				//if NPC is a non-story-char, is not dead and not the player himself, do stuff
				if (peds[i] != playerPed && excludebodyguardspawner && pedmap[peds[i]].isstorychar == 3 && PED::IS_PED_HUMAN(peds[i]) && !ENTITY::IS_ENTITY_DEAD(peds[i]) && !AI::GET_IS_TASK_ACTIVE(peds[i], 3)) //task 3 is the task for standoff situations (like duels or hostage situations, in which you have to headshot the NPC)
				{
					
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			BEGIN: SETTING UP VARIABLES AND BOOLEANs, ID: PDO007
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					//marking NPC if it is in combat with the player
					if (!PED::IS_PED_IN_COMBAT(peds[i], playerPed))
					{
						if (pedmap[peds[i]].isincombatwithplayer)
						{
							pedmap[peds[i]].isincombatwithplayer = false;
							combatcounter--;
						}
					}
					else
					{
						if (!pedmap[peds[i]].isincombatwithplayer)
						{
							pedmap[peds[i]].isincombatwithplayer = true;
							combatcounter++;
							if (combatcounter > 1) pedmap[peds[i]].wasnotaloneincombat = true;
						}
						else if (!pedmap[peds[i]].wasnotaloneincombat && combatcounter > 1) pedmap[peds[i]].wasnotaloneincombat = true;
					}

					//check if NPC is armed
					if (!pedmap[peds[i]].isarmed)
					{
						if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(peds[i]))//!WEAPON::xIS_PED_UNARMED(peds[i]))
						{
							pedmap[peds[i]].isarmed = true;
							pedmap[peds[i]].wasdisarmed = false;
						}
					}
										
					//marking NPC if player is shooting at it
					if (PED::IS_PED_SHOOTING(playerPed))
					{
						Entity playerTargeta, playerTargetb;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargeta))
						{
							if (playerTargeta == peds[i]) pedmap[peds[i]].shotbyplayer = GAMEPLAY::GET_GAME_TIMER();
						}
						if (PLAYER::GET_PLAYER_TARGET_ENTITY(player, &playerTargetb))
						{
							if (playerTargetb == peds[i]) pedmap[peds[i]].shotbyplayer = GAMEPLAY::GET_GAME_TIMER();
						}
					}

					//headshot bool
					bool headshot = false;
					
					//check if NPC is submerged in water
					bool submerged = false;
					bool drowning = false;
					if (ENTITY::GET_ENTITY_SUBMERGED_LEVEL(peds[i]) > 0.37f) submerged = true;
					if (ENTITY::GET_ENTITY_SUBMERGED_LEVEL(peds[i]) > 0.90f) drowning = true;
					
					//different variables for current NPC
					bool isnpconfire = FIRE::IS_ENTITY_ON_FIRE(peds[i]);
					int npchealth = ENTITY::GET_ENTITY_HEALTH(peds[i]);
					
					//checking if NPC is being hanged with TieYourLasso mod
					if (pedmap[peds[i]].ishanging && !DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged")) pedmap[peds[i]].ishanging = false;
					else if (!pedmap[peds[i]].ishanging && DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged"))
					{
						Vector3 vecfoot = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 45454));
						float groundzcoordped;
						GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecfoot.x, vecfoot.y, vecfoot.z, &groundzcoordped, true);
						if (vecfoot.z > groundzcoordped + 0.2 && peds[i] != PED::_0xD806CD2A4F2C2996(player)) //is not carried by player
						{
							pedmap[peds[i]].ishanging = true;
						}
					}					

					//set lasso bools
					if (AI::GET_IS_TASK_ACTIVE(peds[i], 399)) ishogtied = true;
					else ishogtied = false;
					if (AI::GET_IS_TASK_ACTIVE(peds[i], 400) || AI::GET_IS_TASK_ACTIVE(peds[i], 600)) islassoed = true;
					else islassoed = false;

					//set disarming stop for duelling npcs
					if (AI::GET_IS_TASK_ACTIVE(peds[i], 3)) pedmap[peds[i]].disarmingstop = true;

					//initialize variables for checking NPC stance
					//position of ground for NPC bone checks - is NPC on ground?
					Vector3 vechead = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 21030));
					Vector3 vechip = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 14415));
					Vector3 vecleg1 = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 43312));
					Vector3 vecleg2 = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 55120));
					Vector3 vecweaponhand = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 11300));
					float groundzcoordhead, groundzcoordhip, groundzcoordleg1, groundzcoordleg2;
					GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vechead.x, vechead.y, vechead.z, &groundzcoordhead, true);
					GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vechip.x, vechip.y, vechip.z, &groundzcoordhip, true);
					GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecleg1.x, vecleg1.y, vecleg1.z, &groundzcoordleg1, true);
					GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecleg2.x, vecleg2.y, vecleg2.z, &groundzcoordleg2, true);

					//set standing bool
					if (vechead.z > groundzcoordhead + ini_zvaluehead && vechead.z < groundzcoordhead + ini_zvalueheadup) isstanding = true;
					else isstanding = false;


					//check if NPC has been damaged by a weapon (needed for the bleeding effect)
					//if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(peds[i], 0, 2)) pedmap[peds[i]].hasbeendamagedbyweapon = true;
										
					map<int, map<int, char*>>::iterator mapit;
					for (auto mapit = allweaponmap.begin(); mapit != allweaponmap.end(); mapit++)
					{
						int weapontype = mapit->first;
						char* weaponname = mapit->second;
						if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(peds[i], GAMEPLAY::GET_HASH_KEY(weaponname), 0)) pedmap[peds[i]].hasbeendamagedbyweapon = true;
					}

					//bool for checking if NPC is in a state in which ragdolling him doesnt break anything
					bool pedisready = PED::IS_PED_RUNNING_RAGDOLL_TASK(peds[i]) && !isnpconfire && !PED::IS_PED_FALLING(peds[i]) && !PED::IS_PED_HANGING_ON_TO_VEHICLE(peds[i]);

					//randomizer for different pain sounds
					int painaudio = 0 + (std::rand() % (12 - 0 + 1));
					if (painaudio < 3) painaudio = 4;
					else if (painaudio < 7) painaudio = 5;
					else painaudio = 10;

					//randomizer for chance of pain sound playing
					int painaudiorand = 0 + (std::rand() % (999 - 0 + 1));

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			END: SETTING UP VARIABLES AND BOOLEANs
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			BEGIN: ARTERIAL BLEEDING BEHAVIOR (NPCs MUST NOT ENTER DYING STATES), ID: PDO008
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					
					//check if NPC is bleeding out and make sure it does not enter the dying states, if so
					/*
					if ((AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask01) || AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask02) || AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask03)
						|| AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask04) || AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask05)) && pedmap[peds[i]].status != 99)
					{
						if (pedmap[peds[i]].dyingstatebool == true)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], 38, 0);
							
							//ENTITY::SET_ENTITY_MAX_HEALTH(peds[i], ini_arteryshothealth);
							//ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], false), 0);
							pedmap[peds[i]].status = 99;
							pedmap[peds[i]].dyingstatebool = false;
						}
						pedmap[peds[i]].nobleeding = false;
						arteryshot[peds[i]] = true;
					}

					if (arteryshot[peds[i]])
					{
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 352) || AI::GET_IS_TASK_ACTIVE(peds[i], 353) || AI::GET_IS_TASK_ACTIVE(peds[i], 354) || AI::GET_IS_TASK_ACTIVE(peds[i], 355))
						{
							arteryshot[peds[i]] = false;
							pedmap[peds[i]].nobleeding = false;
							//pedmap[peds[i]].status = 6;
							if (ini_dyingstatechance > 0) pedmap[peds[i]].dyingstatebool = true;
						}
					}*/

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			END: ARTERY BLEEDING BEHAVIOR (NPCs MUST NOT ENTER DYING STATES)
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			BEGIN: POLICE BEHAVIOR, ID: PDO040
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					if (ini_policebehavior == 1 && pedmap[peds[i]].ispolice)
					{
						if (!(missionactive && ini_policebehaviordisabledinmissions == 1))
						{
							if (PED::IS_PED_ON_MOUNT(peds[i]) && !PED::IS_PED_ON_MOUNT(playerPed))
							{
								if (PED::IS_PED_IN_COMBAT(peds[i], playerPed) && PED::CAN_PED_IN_COMBAT_SEE_TARGET(peds[i], playerPed))
								{
									Vector3 vecheadplayer = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(playerPed, PED::GET_PED_BONE_INDEX(peds[i], 21030));
									float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vecheadplayer.x, vecheadplayer.y, vecheadplayer.z, vechead.x, vechead.y, vechead.z, false);
									if (distance < pedmap[peds[i]].policedismountdistance)
									{
										AI::CLEAR_PED_TASKS(peds[i], false, false);
										AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
										AI::_xTASK_DISMOUNT_ANIMAL(peds[i], 0, 0, 0, 0, 0);
									}
								}
							}
						}
					}

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			END: POLICE BEHAVIOR, ID: PDO009
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			BEGIN: SET NPC HEALTH, ID: PDO009
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					//if the NPC does not have the specified health yet, set the health accordingly
					if ((PED::GET_PED_MAX_HEALTH(peds[i]) != pedmap[peds[i]].maxhealth && PED::GET_PED_MAX_HEALTH(peds[i]) != ini_pedhealthvehicle)
						&& pedmap[peds[i]].status != 5 && pedmap[peds[i]].status != 4 && pedmap[peds[i]].status != 99 && pedmap[peds[i]].usemaxhealth == 1)
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(peds[i], false))
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_pedhealthvehicle);
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_pedhealthvehicle, 0);
							npchealth = ini_pedhealthvehicle;
						}
						else
						{
							PED::SET_PED_MAX_HEALTH(peds[i], pedmap[peds[i]].maxhealth);
							ENTITY::SET_ENTITY_HEALTH(peds[i], pedmap[peds[i]].maxhealth, 0);
							npchealth = pedmap[peds[i]].maxhealth;
						}
					}

					if (PED::GET_PED_MAX_HEALTH(peds[i]) == ini_pedhealthvehicle)
					{
						if (!PED::IS_PED_IN_ANY_VEHICLE(peds[i], false))
						{
							PED::SET_PED_MAX_HEALTH(peds[i], pedmap[peds[i]].maxhealth);
							if (npchealth <= ini_pedhealthvehicle * ini_pedhealthvehiclemodtods)
							{
								ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
								npchealth = ini_dyingmovementthreshold - 1;
							}
							else if (npchealth >= ini_pedhealthvehicle * ini_pedhealthvehiclemodtofull)
							{
								ENTITY::SET_ENTITY_HEALTH(peds[i], pedmap[peds[i]].maxhealth, 0);
								npchealth = pedmap[peds[i]].maxhealth;
							}
						}
					}

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			END: SET NPC HEALTH
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					
			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
			//
			//
			//			BEGIN: AFFECTING NPCs WITH PROPERLY SET HEALTH AND NOT CURRENTLY BURNING OR BURNING BEFORE, ID: PDO010
			//
			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
									   
					//if NPC has the specified health and was not burning before, check stuff and do stuff
					if (pedmap[peds[i]].status != 4 && pedmap[peds[i]].status != 5)
					{

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: SET NPC ACCURACY, ID: PDO011
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//set accuracy for NPCs
						if (ini_pedaccuracythreshold > 0)
						{
							if (npchealth < ini_pedaccuracythreshold)
							{
								if (pedmap[peds[i]].accuracy > ini_pedaccuracyinjuredmax || pedmap[peds[i]].accuracy < ini_pedaccuracyinjuredmin)
								{
									int randacc = ini_pedaccuracyinjuredmin + (std::rand() % (ini_pedaccuracyinjuredmax - ini_pedaccuracyinjuredmin + 1));
									pedmap[peds[i]].accuracy = randacc;
								}
							}
							else if (pedmap[peds[i]].accuracy > ini_pedaccuracyhealthymax || pedmap[peds[i]].accuracy < ini_pedaccuracyhealthymin)
							{
								int randacc = ini_pedaccuracyhealthymin + (std::rand() % (ini_pedaccuracyhealthymax - ini_pedaccuracyhealthymin + 1));
								pedmap[peds[i]].accuracy = randacc;
							}
							PED::SET_PED_ACCURACY(peds[i], pedmap[peds[i]].accuracy);
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: SET NPC ACCURACY
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: KILL NPCs WHEN IN DYING STATE TOO LONG, ID: PDO012
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//make sure NPCs don't stay in dying states for too long
						if (ini_maxdstime != 0 && !longerbleedoutbool)
						{
							if (ini_hogtiebleedingstop == 1)
							{
								if (!(islassoed || ishogtied))
								{
									if (ini_torsooverwrite == 0)
									{
										if (pedmap[peds[i]].dyingstate1entered != 0 && (pedmap[peds[i]].dyingstate1entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
										else if (pedmap[peds[i]].dyingstate2entered != 0 && (pedmap[peds[i]].dyingstate2entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
										else if (pedmap[peds[i]].dyingstate3entered != 0 && (pedmap[peds[i]].dyingstate3entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
									}
									else if (ini_onlybleedshottorso == 1 && ini_torsooverwrite == 1 && pedmap[peds[i]].torsohit == 1)
									{
										if (pedmap[peds[i]].dyingstate1entered != 0 && (pedmap[peds[i]].dyingstate1entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
										else if (pedmap[peds[i]].dyingstate2entered != 0 && (pedmap[peds[i]].dyingstate2entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
										else if (pedmap[peds[i]].dyingstate3entered != 0 && (pedmap[peds[i]].dyingstate3entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
									}
								}
							}
							else if (ini_onlybleedshottorso == 1 && ini_torsooverwrite == 1)
							{
								if (pedmap[peds[i]].torsohit == 1)
								{
									if (pedmap[peds[i]].dyingstate1entered != 0 && (pedmap[peds[i]].dyingstate1entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
									else if (pedmap[peds[i]].dyingstate2entered != 0 && (pedmap[peds[i]].dyingstate2entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
									else if (pedmap[peds[i]].dyingstate3entered != 0 && (pedmap[peds[i]].dyingstate3entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
								}
							}
							else
							{
								if (pedmap[peds[i]].dyingstate1entered != 0 && (pedmap[peds[i]].dyingstate1entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
								else if (pedmap[peds[i]].dyingstate2entered != 0 && (pedmap[peds[i]].dyingstate2entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
								else if (pedmap[peds[i]].dyingstate3entered != 0 && (pedmap[peds[i]].dyingstate3entered + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmap[peds[i]].tobekilled = 1;
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: KILL NPCs WHEN IN DYING STATE TOO LONG
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: STOP BLEEDING FOR KNOCKED OUT NPCs OR THOSE IN MELEE FIGHTS, ID: PDO013
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//make sure melee combat does not make NPCs bleed out
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 341)) pedmap[peds[i]].nobleeding = true;
						if (PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && npchealth < 10)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
							npchealth = ini_dyingmovementthreshold - 1;
							pedmap[peds[i]].nobleeding = true;
						}

						//make knocked out NPCs not bleed or die from being knocked out
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 11) && !(islassoed || ishogtied))
						{
							pedmap[peds[i]].nobleeding = true;
							ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], false), 0);
							pedmap[peds[i]].tobekilled = 0;
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: STOP BLEEDING FOR KNOCKED OUT NPCs
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: NPC FALLING CHECKS, ID: PDO014
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//handle the falling of an NPC (so it wont survive falls from unrealistic heights)
						if (PED::IS_PED_FALLING(peds[i]))
						{
							pedmap[peds[i]].isfalling = 1;
							if (pedmap[peds[i]].falltime == 0) pedmap[peds[i]].falltime = GAMEPLAY::GET_GAME_TIMER();
							else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].falltime + ini_falltimedeath) pedmap[peds[i]].tobekilled = 1;
							else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].falltime + ini_falltimedown) pedmap[peds[i]].tobedowned = 1;
						}
						else
						{
							pedmap[peds[i]].isfalling = 0;
							pedmap[peds[i]].falltime = 0;
							if (pedmap[peds[i]].tobekilled == 1)
							{
								if (!submerged)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
									npchealth = 0;
								}
								pedmap[peds[i]].tobekilled = 0;
							}
							else if (pedmap[peds[i]].tobedowned == 1)
							{
								if (!submerged)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
									npchealth = ini_dyingmovementthreshold - 1;
									PED::SET_PED_TO_RAGDOLL(peds[i], 2000, 2000, 0, false, false, false);
								}
								pedmap[peds[i]].tobedowned = 0;
							}

							if (!pedmap[peds[i]].ishanging && !AI::GET_IS_TASK_ACTIVE(peds[i], 11))
							{
								if (pedmap[peds[i]].handsup != 0)
								{
									if (PED::IS_PED_ON_MOUNT(peds[i]) && pedmap[peds[i]].dismount == 0)
									{
										pedmap[peds[i]].dismount = GAMEPLAY::GET_GAME_TIMER() + ini_dismounthandsupdelta;
										AI::CLEAR_PED_TASKS(peds[i], false, false);
										AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
										AI::_xTASK_DISMOUNT_ANIMAL(peds[i], 0, 0, 0, 0, 0);
										AUDIO::STOP_PED_SPEAKING(peds[i], true);
									}
									else if (PED::IS_PED_IN_ANY_VEHICLE(peds[i], false) && pedmap[peds[i]].dismount == 0)
									{
										pedmap[peds[i]].dismount = GAMEPLAY::GET_GAME_TIMER() + ini_dismounthandsupdelta;
										AI::CLEAR_PED_TASKS(peds[i], false, false);
										AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
										AI::TASK_LEAVE_ANY_VEHICLE(peds[i], 0, 0);
										AUDIO::STOP_PED_SPEAKING(peds[i], true);
									}
									
									if (pedmap[peds[i]].dismount != 0)
									{
										if (!PED::IS_PED_ON_MOUNT(peds[i]) && !PED::IS_PED_IN_ANY_VEHICLE(peds[i], false)) pedmap[peds[i]].dismount = 0;
										else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].dismount) pedmap[peds[i]].dismount = 0;
									}
									else
									{
										if (pedmap[peds[i]].handsup + ini_handsupdelta < GAMEPLAY::GET_GAME_TIMER() && pedmap[peds[i]].firsttimehandsup == 0)
										{
											pedmap[peds[i]].firsttimehandsup = GAMEPLAY::GET_GAME_TIMER();
										}
										
										if (pedmap[peds[i]].firsttimehandsup != 0 && pedmap[peds[i]].firsttimehandsup + ini_handsuptime > GAMEPLAY::GET_GAME_TIMER())
										{

											Vector3 vecheadplayer = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(playerPed, PED::GET_PED_BONE_INDEX(peds[i], 21030));
											float distance = GAMEPLAY::GET_DISTANCE_BETWEEN_COORDS(vecheadplayer.x, vecheadplayer.y, vecheadplayer.z, vechead.x, vechead.y, vechead.z, false);
											if (distance < ini_handsupradius)
											{
												AI::CLEAR_PED_TASKS(peds[i], false, false);
												AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
												AI::TASK_HANDS_UP(peds[i], ini_handsuptime, playerPed, 0, false);
												AUDIO::STOP_PED_SPEAKING(peds[i], true);
											}
											else
											{
												AI::CLEAR_PED_TASKS(peds[i], false, false);
												AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
												AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
											}
										}

										if (pedmap[peds[i]].handsup + ini_handsupdelta + ini_handsuptime < GAMEPLAY::GET_GAME_TIMER())
										{
											AI::CLEAR_PED_TASKS(peds[i], false, false);
											AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
											pedmap[peds[i]].handsup = 0;
											pedmap[peds[i]].firsttimehandsup = 0;
										}
									}
								}								
							}							
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: NPC FALLING CHECKS
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: HORSE FALLING DELTA, ID: PDO015
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//make sure falling off horses isn't lethal all the time
						if (PED::IS_PED_ON_MOUNT(peds[i])) pedmap[peds[i]].lasttimeonmount = GAMEPLAY::GET_GAME_TIMER() + ini_horsefallingdelta;
						if (pedmap[peds[i]].falltime != 0 && pedmap[peds[i]].lasttimeonmount > GAMEPLAY::GET_GAME_TIMER())
						{
							pedmap[peds[i]].falltime = 0;
							pedmap[peds[i]].tobekilled = 0;
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: HORSE FALLING DELTA
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: LASSO DISARMING, ID: PDO016
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

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
											AI::xTASK_FLEE(lastTarget, playerPed, 4, 0, -1.0f, -1, 0);

											//behavior randomizer
											int rand = 0 + (std::rand() % (99 - 0 + 1));
											if (rand < 5)
											{
												AI::TASK_COWER(lastTarget, 5, 1, 0);
												pedmap[lastTarget].health = ENTITY::GET_ENTITY_HEALTH(lastTarget);
											}
											else if (rand < 10)
											{
												pedmap[peds[i]].handsup = GAMEPLAY::GET_GAME_TIMER();
												pedmap[lastTarget].health = ENTITY::GET_ENTITY_HEALTH(lastTarget);
											}
											else if (rand < 55)
											{
												AI::xTASK_FLEE(lastTarget, playerPed, 4, 0, -1.0f, -1, 0);
											}
											else pedmap[lastTarget].health = ENTITY::GET_ENTITY_HEALTH(lastTarget); //remember health for later use
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

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: LASSO DISARMING
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: HOGTIE DISARMING, ID: PDO017
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//disarming by hogtying someone
						if (ini_hogtiedisarm == 1)
						{
							if (ishogtied)
							{
								WEAPON::REMOVE_ALL_PED_WEAPONS(peds[i], true, true);
								AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);

								//behavior randomizer
								int rand = 0 + (std::rand() % (99 - 0 + 1));
								if (rand < 5)
								{
									AI::TASK_COWER(peds[i], 5, 1, 0);
									pedmap[peds[i]].health = npchealth;
								}
								else if (rand < 10)
								{
									pedmap[peds[i]].handsup = GAMEPLAY::GET_GAME_TIMER();
									pedmap[peds[i]].health = npchealth;
								}
								else if (rand < 55)
								{
									AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
								}
								else pedmap[peds[i]].health = npchealth; //remember health for later use
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: HOGTIE DISARMING
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: NPC FEAR FEATURE (FLEEING DURING COMBAT), ID: PDO018
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//check if NPC should flee in combat
						if (ini_npcdownfearchance > 0)
						{
							Ped playertargetz;
							PLAYER::GET_PLAYER_TARGET_ENTITY(player, &playertargetz);
							PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playertargetz);

							if (pedmap[peds[i]].hasreacted > 10 && pedmap[peds[i]].hasreacted < GAMEPLAY::GET_GAME_TIMER() && !(playertargetz == peds[i]))
							{
								AI::CLEAR_PED_TASKS(peds[i], false, false);
								AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
								pedmap[peds[i]].isdown = 9;
							}
							else
							{
								if ((playertargetz == peds[i] || PED::IS_PED_IN_COMBAT(peds[i], playerPed) || PED::IS_PED_IN_COMBAT(playerPed, peds[i]) || PED::CAN_PED_IN_COMBAT_SEE_TARGET(peds[i], playerPed) ||
									PED::CAN_PED_IN_COMBAT_SEE_TARGET(playerPed, peds[i])) && pedmap[peds[i]].isdown == 0 && pedmap[peds[i]].hasreacted == 0)
								{
									int randfear = 0 + (std::rand() % (999 - 0 + 1));
									int randreact = 0 + (std::rand() % (99 - 0 + 1));
									if (npcdowncounter > ini_npcdownfearthreshold&& randfear < ini_npcdownfearchance)
									{
										if (randreact < ini_cowerchancefear)
										{
											AI::TASK_COWER(peds[i], -1, 1, 0);
											pedmap[peds[i]].health = npchealth;
											pedmap[peds[i]].hasreacted = 1;
										}
										else if (randreact < ini_cowerchancefear + ini_handsupchancefear)
										{
											pedmap[peds[i]].handsup = GAMEPLAY::GET_GAME_TIMER();
											pedmap[peds[i]].health = npchealth;
											pedmap[peds[i]].hasreacted = 1;
										}										
										else
										{
											AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
											pedmap[peds[i]].hasreacted = 2;
										}
									}
								}
								if (pedmap[peds[i]].hasreacted == 1 && ini_fearfleeultimately == 1)
								{
									int randtime = 4000 + (std::rand() % (15000 - 4000 + 1));
									pedmap[peds[i]].hasreacted = GAMEPLAY::GET_GAME_TIMER() + randtime;
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: NPC FEAR FEATURE (FLEEING DURING COMBAT)
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: SET NPC STATUS ACCORDING TO BODY PARTS BEING DAMAGED, ID: PDO019
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//if the NPC has been shot somewhere special, set the status accordingly and create proper behavior
						int actBone;
						if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &actBone) && pedmap[peds[i]].hasbeendamagedbyweapon)
						{
							//determining if NPC was hit in the torso
							for (vector<int>::size_type it = 0; it != torsobonesbleeding.size(); it++)
							{
								if (actBone == torsobonesbleeding[it])
								{
									if (pedmap[peds[i]].shotbyplayer != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmap[peds[i]].shotbyplayer + ini_npcbodydamagedelay) pedmap[peds[i]].torsohit = 1;
								}
							}
							
							//disarming and according behavior
							int randdis = 0 + (std::rand() % (99 - 0 + 1));
							if (randdis < ini_disarmingchance && !pedmap[peds[i]].disarmingstop)
							{
								if ((actBone == 11300 || actBone == 16749 || actBone == 16781 || actBone == 54187 || actBone == 22798 || actBone == 46065))// && !WEAPON::xIS_PED_UNARMED(peds[i]))
								{
									//disarming
									if (!WEAPON::xIS_PED_UNARMED(peds[i]))//WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(peds[i]))//!WEAPON::xIS_PED_UNARMED(peds[i]))
									{
										DisarmPed(peds[i], vecweaponhand);
										pedmap[peds[i]].wasdisarmed = true;
									}

									if (pedmap[peds[i]].isarmed)
									{
										//behavior randomizer
										int rand = 0 + (std::rand() % (99 - 0 + 1));
										if (rand < ini_disarmcowerchance)
										{
											AI::TASK_COWER(peds[i], -1, 1, 0);
											pedmap[peds[i]].health = npchealth;
										}
										else if (rand < ini_disarmcowerchance + ini_disarmhandsupchance)
										{										
											if (ini_combathandsuphostilenpcs == 1)
											{
												if (!pedmap[peds[i]].wasnotaloneincombat)
												{
													pedmap[peds[i]].handsup = GAMEPLAY::GET_GAME_TIMER();
													pedmap[peds[i]].health = npchealth;
												}
												else if (pedmap[peds[i]].wasnotaloneincombat && combatcounter <= ini_handsuphostilenpcthreshold)
												{
													pedmap[peds[i]].handsup = GAMEPLAY::GET_GAME_TIMER();
													pedmap[peds[i]].health = npchealth;
												}
												else AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
											}
											else
											{
												pedmap[peds[i]].handsup = GAMEPLAY::GET_GAME_TIMER();
												pedmap[peds[i]].health = npchealth;
											}
										}
										else if (rand < ini_disarmcowerchance + ini_disarmhandsupchance + ini_disarmfleeingchance)
										{
											AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0); 
										}
										else pedmap[peds[i]].health = npchealth; //remember health for later use
									}

									pedmap[peds[i]].isarmed = false;				
								}
							}

							//right leg
							if (actBone == 43312 || actBone == 6884 || actBone == 33646)
							{
								if (pedmap[peds[i]].status == 0) pedmap[peds[i]].status = 8;
								else if (pedmap[peds[i]].status == 3) pedmap[peds[i]].status = 9;
								else if (pedmap[peds[i]].status == 6) pedmap[peds[i]].status = 1;
								else if (pedmap[peds[i]].status == 7) pedmap[peds[i]].status = 2;
								
								if (pedmap[peds[i]].shotbyplayer != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmap[peds[i]].shotbyplayer + ini_npcbodydamagedelay) pedmap[peds[i]].rightleghitcounter++;
							}
							//left leg
							else if (actBone == 65478 || actBone == 55120 || actBone == 45454)
							{
								if (pedmap[peds[i]].status == 0) pedmap[peds[i]].status = 6;
								else if (pedmap[peds[i]].status == 3) pedmap[peds[i]].status = 7;
								else if (pedmap[peds[i]].status == 8) pedmap[peds[i]].status = 1;
								else if (pedmap[peds[i]].status == 9) pedmap[peds[i]].status = 2;
								if (pedmap[peds[i]].shotbyplayer != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmap[peds[i]].shotbyplayer + ini_npcbodydamagedelay) pedmap[peds[i]].leftleghitcounter++;
							}

							//spine paralysis
							if (actBone == 14410 || actBone == 14411 || actBone == 14412 || actBone == 14413 || actBone == 14414 || actBone == 14415)
							{
								int randpar = 0 + (std::rand() % (99 - 0 + 1));
								if (randpar < ini_parachance && ini_spineshotsparalyze == 1)
								{
									if (pedmap[peds[i]].shotbyplayer != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmap[peds[i]].shotbyplayer + ini_npcbodydamagedelay)
									{
										if (pedmap[peds[i]].status == 0
											|| pedmap[peds[i]].status == 1
											|| pedmap[peds[i]].status == 6
											|| pedmap[peds[i]].status == 8) pedmap[peds[i]].status = 10;
										else if (pedmap[peds[i]].status == 2
											|| pedmap[peds[i]].status == 3
											|| pedmap[peds[i]].status == 7
											|| pedmap[peds[i]].status == 10) pedmap[peds[i]].status = 11;
									}
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: SET NPC STATUS ACCORDING TO BODY PARTS BEING DAMAGED
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: MAKE INJURED NPCs MOVE WITH WOUNDED ANIMATIONS, ID: PDO020
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//make injured NPCs walk differently (in an injured style)
						if (pedmap[peds[i]].status == 6 || pedmap[peds[i]].status == 7 || pedmap[peds[i]].status == 8 || pedmap[peds[i]].status == 9 || pedmap[peds[i]].torsohit == 1
							|| pedmap[peds[i]].status == 1 || pedmap[peds[i]].status == 2)
						{
							if(pedmap[peds[i]].movementstyle == "" || pedmap[peds[i]].movementstyle == "injured_torso")// || pedmap[peds[i]].movementstyle == "injured_right_arm" || pedmap[peds[i]].movementstyle == "injured_left_arm" || pedmap[peds[i]].movementstyle == "injured_torso")
							{
								//int randlimp = 0 + (std::rand() % (99 - 0 + 1));

								if (pedmap[peds[i]].status == 6 || pedmap[peds[i]].status == 7) //left leg injured
								{
									pedmap[peds[i]].movementstyle = "injured_left_leg";
									//if (randlimp < 30) pedmap[peds[i]].movementstyle = "injured_general";
									//else if (randlimp < 60) pedmap[peds[i]].movementstyle = "injured_left_leg";
									//else pedmap[peds[i]].movementstyle = "injured_right_leg";
								}
								else if (pedmap[peds[i]].status == 8 || pedmap[peds[i]].status == 9) //right leg injured
								{
									pedmap[peds[i]].movementstyle = "injured_right_leg";
									//if (randlimp < 30) pedmap[peds[i]].movementstyle = "injured_general";
									//else if (randlimp < 60) pedmap[peds[i]].movementstyle = "injured_left_leg";
									//else pedmap[peds[i]].movementstyle = "injured_right_leg";
								}
								else if (pedmap[peds[i]].status == 1 || pedmap[peds[i]].status == 2) //both legs injured
								{
									pedmap[peds[i]].movementstyle = "injured_general";
									//if (randlimp < 30) pedmap[peds[i]].movementstyle = "injured_general";
									//else if (randlimp < 60) pedmap[peds[i]].movementstyle = "injured_left_leg";
									//else pedmap[peds[i]].movementstyle = "injured_right_leg";
								}
								else if (pedmap[peds[i]].torsohit == 1)
								{
									pedmap[peds[i]].movementstyle = "injured_torso";
									//if (randlimp < 30) pedmap[peds[i]].movementstyle = "injured_right_arm";
									//else if (randlimp < 60) pedmap[peds[i]].movementstyle = "injured_left_arm";
									//else pedmap[peds[i]].movementstyle = "injured_torso";
								}
							}
							else
							{
								if (PED::IS_PED_MALE(peds[i])) PED::xSET_STANCE(peds[i], "DEFAULT");
								else PED::xSET_STANCE(peds[i], "DEFAULT_FEMALE");
								PED::xSET_WALKING_STYLE(peds[i], pedmap[peds[i]].movementstyle);
							}
						}

						/*
						if (pedmap[peds[i]].status == 6 || pedmap[peds[i]].status == 7) //left leg injured
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_general");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_leg");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_right_leg");
						}
						else if (pedmap[peds[i]].status == 8 || pedmap[peds[i]].status == 9) //right leg injured
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_general");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_leg");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_right_leg");
						}
						else if (pedmap[peds[i]].status == 1 || pedmap[peds[i]].status == 2) //both legs injured
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_general");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_leg");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_right_leg");
						}
						else if (pedmap[peds[i]].torsohit == 1)
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_right_arm");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_arm");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_torso");
						}*/

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: MAKE INJURED NPCs MOVE WITH WOUNDED ANIMATIONS
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: SPINE PARALYSIS, ID: PDO021
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//put NPC down if paralyzed
						if (pedmap[peds[i]].status == 10 || pedmap[peds[i]].status == 11)
						{
							PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(peds[i], 1);
							if (PED::IS_PED_RAGDOLL(peds[i]) && ENTITY::GET_ENTITY_SPEED(peds[i]) < maxragdollspeed) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
							else
							{
								PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 3, false, false, false);
								PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
							}

							//disarming
							DisarmPed(peds[i], vecweaponhand);

							//randomizer for different pain sounds
							int painaudio = 9;

							//randomizer for chance of pain sound playing
							int painaudiorand = 0 + (std::rand() % (9999 - 0 + 1));

							//play pain sound
							if (painaudiorand < ini_paralysisaudiochance && !submerged && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
							{
								int randscream = 0 + (std::rand() % (99 - 0 + 1));
								if (randscream < 25) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
								else pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 4, 0, ini_ds2audiochancemod);
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: SPINE PARALYSIS
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: PUT NPCs DOWN WHEN HIT IN THE LEGS ENOUGH, ID: PDO022
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//put NPC down if hit in the legs enough
						if (npchealth > ini_dyingmovementthreshold && pedmap[peds[i]].status != 99
							&& pedmap[peds[i]].status != 10 && pedmap[peds[i]].status != 11 && ini_nopushingdown == 0)
						{
							if (isstanding)
							{
								if (pedmap[peds[i]].leftleghitcounter >= ini_legshitthreshold && pedmap[peds[i]].rightleghitcounter >= ini_legshitthreshold)
								{
									if (AI::IS_PED_RUNNING(peds[i]))
									{
										PED::SET_PED_TO_RAGDOLL(peds[i], 5000, 5000, 3, false, false, false);
										if (PED::IS_PED_MALE(peds[i])) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, ini_dforcepushy, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
										else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx / ini_femalepushmodifier, ini_dforcepushy / ini_femalepushmodifier, ini_dforcepushz / ini_femalepushmodifier, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									}
									else PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
									ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
									npchealth = ini_dyingmovementthreshold - 1;
									pedmap[peds[i]].legsdownsurvivaltimer = GAMEPLAY::GET_GAME_TIMER() + ini_bleedingsave;
								}
								else if (pedmap[peds[i]].leftleghitcounter > 0 && pedmap[peds[i]].rightleghitcounter > 0 && pedmap[peds[i]].legsdamageddowntime == 0) pedmap[peds[i]].legsdamageddowntime = GAMEPLAY::GET_GAME_TIMER() + ini_damagedlegsdowntime;

								//put NPC down if legs damaged and timer passed
								if (pedmap[peds[i]].legsdamageddowntime > 0 && pedmap[peds[i]].legsdamageddowntime < GAMEPLAY::GET_GAME_TIMER())
								{
									if (AI::IS_PED_RUNNING(peds[i]))
									{
										PED::SET_PED_TO_RAGDOLL(peds[i], 5000, 5000, 3, false, false, false);
										if (PED::IS_PED_MALE(peds[i])) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, ini_dforcepushy, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
										else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx / ini_femalepushmodifier, ini_dforcepushy / ini_femalepushmodifier, ini_dforcepushz / ini_femalepushmodifier, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									}
									else PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
									ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
									npchealth = ini_dyingmovementthreshold - 1;
									pedmap[peds[i]].legsdownsurvivaltimer = GAMEPLAY::GET_GAME_TIMER() + ini_bleedingsave;
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: PUT NPCs DOWN WHEN HIT IN THE LEGS ENOUGH
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: LEGS HURT - STUMBLING, ID: PDO023
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//check if NPC should stumble and create according behavior
						if (pedmap[peds[i]].status != 99 && pedmap[peds[i]].status != 10 && pedmap[peds[i]].status != 11)
						{
							//both legs damaged
							int xstumblechancebothlegs;
							int xstumblechanceoneleg;
							int xrunningthresholdbothlegs;
							int xrunningthresholdoneleg;

							if (npchealth <= ini_dyingmovementthreshold)
							{
								xstumblechancebothlegs = ini_dsstumblechancebothlegs;
								xstumblechanceoneleg = ini_dsstumblechanceoneleg;
								xrunningthresholdbothlegs = ini_dsrunningthresholdbothlegs;
								xrunningthresholdoneleg = ini_dsrunningthresholdoneleg;
							}
							else
							{
								xstumblechancebothlegs = ini_stumblechancebothlegs;
								xstumblechanceoneleg = ini_stumblechanceoneleg;
								xrunningthresholdbothlegs = ini_runningthresholdbothlegs;
								xrunningthresholdoneleg = ini_runningthresholdoneleg;
							}
							
							if ((pedmap[peds[i]].status == 1 || pedmap[peds[i]].status == 2) && !PED::IS_PED_RAGDOLL(peds[i]) && !AI::IS_PED_GETTING_UP(peds[i]) && (AI::IS_PED_WALKING(peds[i]) || AI::IS_PED_RUNNING(peds[i]) || AI::IS_PED_SPRINTING(peds[i]) || PED::IS_PED_GOING_INTO_COVER(peds[i])) && !PED::IS_PED_ON_MOUNT(peds[i]))
							{
								int fallrand = 0 + (std::rand() % (999 - 0 + 1));
								if (fallrand < xstumblechancebothlegs && ENTITY::GET_ENTITY_SPEED(peds[i]) > xrunningthresholdbothlegs)
								{
									PED::SET_PED_TO_RAGDOLL(peds[i], 1000, 1000, 3, false, false, false);
									ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_forcepushxboth, ini_forcepushyboth, ini_forcepushzboth, false, ini_dirrelbothlegs, ini_forrelbothlegs, false);
									PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
								}
							}
							//one leg damaged
							else if ((pedmap[peds[i]].status == 6 || pedmap[peds[i]].status == 7 || pedmap[peds[i]].status == 8 || pedmap[peds[i]].status == 9) && (AI::IS_PED_RUNNING(peds[i]) || AI::IS_PED_SPRINTING(peds[i]) || PED::IS_PED_GOING_INTO_COVER(peds[i])) && !PED::IS_PED_RAGDOLL(peds[i]) && !AI::IS_PED_GETTING_UP(peds[i]) && !PED::IS_PED_ON_MOUNT(peds[i]))
							{
								int fallrand = 0 + (std::rand() % (999 - 0 + 1));
								if (fallrand < xstumblechanceoneleg && ENTITY::GET_ENTITY_SPEED(peds[i]) > xrunningthresholdoneleg)
								{
									PED::SET_PED_TO_RAGDOLL(peds[i], 1000, 1000, 3, false, false, false);
									ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_forcepushx, ini_forcepushy, ini_forcepushz, false, ini_dirreloneleg, ini_forreloneleg, false);
									PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: LEGS HURT - STUMBLING
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: REACTION IF DISARMED NPCs ARE HARMED, ID: PDO024
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//if NPC has lost health since it has been flagged ("remember health for later use"), create according behavior
						if (ini_disarmingchance > 0 && pedmap[peds[i]].handsup != 0 && pedmap[peds[i]].handsup + ini_handsupdelta + ini_handsuptime > GAMEPLAY::GET_GAME_TIMER())//pedmap[peds[i]].wasdisarmed)
						{
							if (pedmap[peds[i]].health != 0)
							{
								if (pedmap[peds[i]].health > npchealth + 10)
								{
									pedmap[peds[i]].handsup = 0;
									AI::CLEAR_PED_TASKS(peds[i], false, false);
									AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
									AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
									pedmap[peds[i]].health = 0;
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: REACTION IF DISARMED NPCs ARE HARMED
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: REACTION OF NPCs WHEN HOGTIED (FOR BEING ATTACKED AND BEING AIMED AT), ID: PDO025
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//being attacked with a knife
						if (ishogtied && !isnpconfire)
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
										if (playerTarget == peds[i] && pedmap[peds[i]].limbhealth > npchealth)
										{
											int painaudiorand = 0 + (std::rand() % (99 - 0 + 1));
											if (painaudiorand < 20 && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))	AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
										}
									}
								}
							}
						}

						//being aimed at
						if (ishogtied && !isnpconfire && !pedmap[peds[i]].ishanging)
						{
							if (ini_ds1audio == 1)
							{
								Entity playerTargetped;
								if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetped))
								{
									if (playerTargetped == peds[i] && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
									{
										if (PED::IS_PED_SHOOTING(playerPed)) pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 4, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
										else
										{
											int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
											if (reactionrand < ini_reactiononaim)
											{
												if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
												{
													AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
													if (!pedmap[peds[i]].wasaimedat)
													{
														PedFear(peds[i], 3, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
														pedmap[peds[i]].wasaimedat = true;
													}
													else pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 1, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
												}
											}
										}
									}
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: REACTION OF NPCs WHEN HOGTIED AND AIMED AT
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: TIE YOUR LASSO FEATURES, ID: PDO026
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						
						if (ini_tylfeaturesenabled == 1)
						{
							if (DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged") && !isnpconfire && !ishogtied && !islassoed)
							{
								if (ini_mutenoosednpcs == 1)
								{
									AUDIO::STOP_PED_SPEAKING(peds[i], true);
									AUDIO::DISABLE_PED_PAIN_AUDIO(peds[i], true);
								}

								if (ini_noosefaceturn == 1) AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
								AI::TASK_STAND_STILL(peds[i], -1);

								Vector3 vecfoot = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 45454));
								float groundzcoordped;
								GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecfoot.x, vecfoot.y, vecfoot.z, &groundzcoordped, true);
								if (vecfoot.z > groundzcoordped + ini_hangingheight)
								{
									if (ini_nooseaudio == 1 && ini_mutenoosednpcs == 0) //react audibly if hanging
									{
										int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
										if (reactionrand < ini_nooseaudiochance && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
										{
											if (reactionrand < 80 && peds[i] != PED::_0xD806CD2A4F2C2996(player)) //is not carried by player
											{
												int reactionrand2 = 0 + (std::rand() % (10 - 0 + 1));
												if (reactionrand2 < 5) pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 5, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
												else PedPanic(peds[i]);
											}
										}
									}

									//health boost if hanging
									if (ini_nooselivelonger == 1)
									{
										if (pedmap[peds[i]].hangingtime == 0) pedmap[peds[i]].hangingtime = GAMEPLAY::GET_GAME_TIMER() + ini_noosehealthboostinterval;
										else if (pedmap[peds[i]].hangingtime < GAMEPLAY::GET_GAME_TIMER())
										{
											npchealth = ENTITY::GET_ENTITY_HEALTH(peds[i]);
											if (npchealth + ini_noosehealthboost < pedmap[peds[i]].maxhealth)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + ini_noosehealthboost, 0);
												npchealth += ini_noosehealthboost;
												pedmap[peds[i]].hangingtime = GAMEPLAY::GET_GAME_TIMER() + ini_noosehealthboostinterval;
											}
										}
									}
								}
								else
								{
									int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
									if (reactionrand < ini_nooseaudiochance && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
									{
										if (!pedmap[peds[i]].wasaimedat)
										{
											PedFear(peds[i], 3, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
											pedmap[peds[i]].wasaimedat = true;
										}
										else pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 2, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
									}
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: TIE YOUR LASSO FEATURES
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: BODY PART SPECIFIC DAMAGE, ID: PDO027
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						
						//checking limb damage and giving back health
						if (pedmap[peds[i]].limbhealth > npchealth && pedmap[peds[i]].status != 99)
						{
							int damagedone = pedmap[peds[i]].limbhealth - npchealth;
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
											if (npchealth < ini_dyingmovementthreshold && (healthback + npchealth) > ini_dyingmovementthreshold)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
												npchealth = ini_dyingmovementthreshold - 1;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + healthback, 0);
												npchealth += healthback;
											}
										}
										else
										{
											float dam = ini_legdamagemod - 1;
											dam = damagedone * dam;
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
											npchealth -= (int)dam;
										}
										pedmap[peds[i]].limbhealth = npchealth;
									}
								}

								for (vector<int>::size_type it = 0; it != armbones.size(); it++)
								{
									if (act_Bone == armbones[it])
									{
										int healthback = (int)(damagedone * ini_armdamagemod);
										if (ini_armdamagemod < 1)
										{
											if (npchealth < ini_dyingmovementthreshold && (healthback + npchealth) > ini_dyingmovementthreshold)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
												npchealth = ini_dyingmovementthreshold - 1;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + healthback, 0);
												npchealth += healthback;
											}
										}
										else
										{
											float dam = ini_armdamagemod - 1;
											dam = damagedone * dam;
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
											npchealth -= (int)dam;
										}
										pedmap[peds[i]].limbhealth = npchealth;
									}
								}

								for (vector<int>::size_type it = 0; it != torsobones.size(); it++)
								{
									if (act_Bone == torsobones[it])
									{
										int healthback = (int)(damagedone * ini_torsodamagemod);
										if (ini_torsodamagemod < 1)
										{
											if (npchealth < ini_dyingmovementthreshold && (healthback + npchealth) > ini_dyingmovementthreshold)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
												npchealth = ini_dyingmovementthreshold - 1;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + healthback, 0);
												npchealth += healthback;
											}
										}
										else
										{
											float dam = ini_torsodamagemod - 1;
											dam = damagedone * dam;
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
											npchealth -= (int)dam;
										}
										pedmap[peds[i]].limbhealth = npchealth;
									}
								}

								for (vector<int>::size_type it = 0; it != headbones.size(); it++)
								{
									if (act_Bone == headbones[it])
									{
										int healthback = (int)(damagedone * ini_headdamagemod);
										if (ini_headdamagemod < 1)
										{
											if (npchealth < ini_dyingmovementthreshold && (healthback + npchealth) > ini_dyingmovementthreshold)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
												npchealth = ini_dyingmovementthreshold - 1;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + healthback, 0);
												npchealth += healthback;
											}
										}
										else
										{
											float dam = ini_headdamagemod - 1;
											dam = damagedone * dam;
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
											npchealth -= (int)dam;
										}
										pedmap[peds[i]].limbhealth = npchealth;
									}
								}

								for (vector<int>::size_type it = 0; it != neckbones.size(); it++)
								{
									if (act_Bone == neckbones[it])
									{
										int healthback = (int)(damagedone * ini_neckdamagemod);
										if (ini_neckdamagemod < 1)
										{
											if (npchealth < ini_dyingmovementthreshold && (healthback + npchealth) > ini_dyingmovementthreshold)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
												npchealth = ini_dyingmovementthreshold - 1;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + healthback, 0);
												npchealth += healthback;
											}
										}
										else
										{
											float dam = ini_neckdamagemod - 1;
											dam = damagedone * dam;
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
											npchealth -= (int)dam;
										}
										pedmap[peds[i]].limbhealth = npchealth;
									}
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: BODY PART SPECIFIC DAMAGE
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


						//making sure artery shot NPCs don't stand still
						if (pedmap[peds[i]].status == 99)
						{
							//making sure NPCs don't stay in shock too long
							if (pedmap[peds[i]].headvectime == 0)
							{
								pedmap[peds[i]].headvecdelta = vechead;
								pedmap[peds[i]].headvectime = GAMEPLAY::GET_GAME_TIMER() + ini_headvecmaxtimeb;
							}
							else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].headvectime && pedmap[peds[i]].headvectime != -1)
							{
								if (isstanding && ENTITY::GET_ENTITY_SPEED(peds[i]) < maxragdollspeed && ini_nopushingdown == 0)
								{
									if ((vechead.x < pedmap[peds[i]].headvecdelta.x + ini_headvecmaxdeltab
										&& vechead.x > pedmap[peds[i]].headvecdelta.x - ini_headvecmaxdeltab)
										&&
										(vechead.y < pedmap[peds[i]].headvecdelta.y + ini_headvecmaxdeltab
											&& vechead.y > pedmap[peds[i]].headvecdelta.y - ini_headvecmaxdeltab)
										&&
										(vechead.z < pedmap[peds[i]].headvecdelta.z + ini_headvecmaxdeltab
											&& vechead.z > pedmap[peds[i]].headvecdelta.z - ini_headvecmaxdeltab))
									{
										PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 3, false, false, false);
										pedmap[peds[i]].status = 0;
									}
									else
									{
										pedmap[peds[i]].headvecdelta = vechead;
										pedmap[peds[i]].headvectime = GAMEPLAY::GET_GAME_TIMER() + ini_headvecmaxtimeb;
									}
								}
							}
						}

						//if DyingStates should apply to melee damage as well (-> people going into dying states after fist fights), set bool accordingly
						bool xhasbeendamagedbyweapon;
						if (ini_dyingstatesmelee == 1) xhasbeendamagedbyweapon = true;
						else xhasbeendamagedbyweapon = pedmap[peds[i]].hasbeendamagedbyweapon;
												
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: KNOCKING THE WIND OUT OF NPCs, ID: PDO028
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//if NPC can be knocked down, compute if he is being knocked down now (randomly when hit)
						if (npchealth < ini_painthreshold && npchealth > ini_dyingmovementthreshold
							&& pedmap[peds[i]].status != 99 && !pedmap[peds[i]].wasdisarmed && !pedmap[peds[i]].nobleeding && pedmap[peds[i]].status != 10 && pedmap[peds[i]].status != 11)
						{
							int painrand = 0 + (std::rand() % (999 - 0 + 1));
							int painaudio = 9;
							if (pedisready)
							{
								if (painrand < ini_knockbackchance)
								{
									PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
									if (painaudiorand < 5 && !submerged && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
								}
								int rand = 0 + (std::rand() % (999 - 0 + 1));
								if (rand < ini_dyingpushchance && isstanding && ENTITY::GET_ENTITY_SPEED(peds[i]) < maxragdollspeed && ini_nopushingdown == 0)
								{
									int randxyz = 0 + (std::rand() % (99 - 0 + 1));
									if (randxyz < 33) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, 0, 0, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									else if (randxyz < 66) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, ini_dforcepushy, 0, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
									else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, 0, 0, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
								}
							}
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: KNOCKING THE WIND OUT OF NPCs
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: PREVENTING ONE-SHOT-KILLS OF GOING LIMP, ID: PDO029
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//making sure NPC doesn't go limp when one-shotted
						if (npchealth < 2 && pedmap[peds[i]].dyingstatebool)
						{
							pedmap[peds[i]].dyingstatebool = false;
							ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
							npchealth = 0;
						}

						//making sure headshots kill or at least wound severely
						if (headshot && npchealth > 5)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], 5, 0);
							npchealth = 5;
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: PREVENTING ONE-SHOT-KILLS OF GOING LIMP 
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
											   
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			BEGIN: DYING STATES (GENERAL), ID: PDO030
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						else if (pedmap[peds[i]].dyingstatebool && !(islassoed || ishogtied) && npchealth <= ini_dyingmovementthreshold && xhasbeendamagedbyweapon
							&& pedmap[peds[i]].status != 99 && pedmap[peds[i]].status != 10 && pedmap[peds[i]].status != 11)
						{	
							//killing wounded NPCs, if enabled
							if (killwoundedbool)
							{
								ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
								npchealth = 0;
							}
							else
							{								
								//making sure NPCs don't die instantly after leg shots
								if (pedmap[peds[i]].legsdownsurvivaltimer != 0)
								{
									if (GAMEPLAY::GET_GAME_TIMER() < pedmap[peds[i]].legsdownsurvivaltimer)
									{
										ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
										npchealth = ini_dyingmovementthreshold - 1;
									}
								}

								//transition into ragdoll from fighting, horse, cover, etc.
								if (PED::IS_PED_ON_MOUNT(peds[i]) || PED::IS_PED_SITTING_IN_ANY_VEHICLE(peds[i]) || PED::IS_PED_IN_ANY_VEHICLE(peds[i], true) || PED::IS_PED_IN_COVER(peds[i], false, false)
									|| PED::IS_PED_IN_ANY_VEHICLE(peds[i], false) || PED::IS_PED_ON_VEHICLE(peds[i], false) || PED::IS_PED_ON_VEHICLE(peds[i], true))
								{
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);
								}
								else if (PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && ini_meleedyingstateknockout == 1)
								{
									AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false);
									PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);
									pedmap[peds[i]].isknockedout = 1;
								}


								if (isstanding)
								{
									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
									//
									//
									//			BEGIN: DYING STATE PUSH - PUT NPC DOWN (IF STANDING), ID: PDO031
									//
									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
									
									if (ini_dsforcedstumbling == 1)
									{
										if (pedmap[peds[i]].status != 1 && pedmap[peds[i]].status != 2)
										{
											int survivfir = 0 + (std::rand() % (99 - 0 + 1));
											if (survivfir < ini_survivfirchance) pedmap[peds[i]].status = 2;
											else pedmap[peds[i]].status = 1;
										}
									}
									
									if (ini_nopushingdown == 0)
									{
										//make sure NPC is able to be ragdolled
										PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(peds[i], 1);

										if (PED::IS_PED_RAGDOLL(peds[i]))
										{
											PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
											if (pedmap[peds[i]].euphstumblingenabled == 1)
											{
												if (pedmap[peds[i]].stumblingtime == 0)
												{
													int stumblingtime = ini_stumbletimemin + (std::rand() % (ini_stumbletimemax - ini_stumbletimemin + 1));
													pedmap[peds[i]].stumblingtime = GAMEPLAY::GET_GAME_TIMER() + stumblingtime;
												}
												else if (pedmap[peds[i]].stumblingtime > GAMEPLAY::GET_GAME_TIMER())
												{
													int randdir = 0 + (std::rand() % (9 - 0 + 1));
													if (randdir < 5)
													{
														if (PED::IS_PED_MALE(peds[i])) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx, ini_dforcepushy, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
														else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx / ini_femalepushmodifier, ini_dforcepushy / ini_femalepushmodifier, ini_dforcepushz / ini_femalepushmodifier, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
													}
													else
													{
														if (PED::IS_PED_MALE(peds[i])) ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx * (-1), ini_dforcepushy, ini_dforcepushz, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
														else ENTITY::APPLY_FORCE_TO_ENTITY_CENTER_OF_MASS(peds[i], 1, ini_dforcepushx / ini_femalepushmodifier * (-1), ini_dforcepushy / ini_femalepushmodifier, ini_dforcepushz / ini_femalepushmodifier, false, ini_ddirectionisrelative, ini_dforceisrelative, false);
													}
												}
												else PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
											}
											else PED::SET_PED_RAGDOLL_FORCE_FALL(peds[i]);
										}
									}		

									//preventing NPCs from getting up after being downed
									if ((pedmap[peds[i]].dyingstate1entered != 0 || pedmap[peds[i]].dyingstate2entered != 0 || pedmap[peds[i]].dyingstate3entered != 0) && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 35000, 35000, 3, false, false, false);

									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
									//
									//
									//			END: DYING STATE PUSH - PUT NPC DOWN (IF STANDING)
									//
									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

								}
								else if (!isstanding)
								{
									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
									//
									//
									//			BEGIN: DYING STATE BEHAVIOR - IF NPC IS NOT STANDING, ID: PDO032
									//
									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

									if (ini_blipfords == 1 && pedmap[peds[i]].blipset == 0)
									{
										Blip pedblip = RADAR::xBLIP_ADD_FOR_ENTITY(0x19365607, peds[i]);
										RADAR::xSET_BLIP_NAME_FROM_PLAYER_STRING(pedblip, "PDO - NPC in Dying States");
										pedmap[peds[i]].blipset = pedblip;
									}
									
									//disarming
									DisarmPed(peds[i], vecweaponhand);

									//make sure NPC is able to be ragdolled
									PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(peds[i], 1);

									//making sure melee knockout victims stay down
									if (pedmap[peds[i]].isknockedout != 0 && (pedmap[peds[i]].isknockedout == 1 || pedmap[peds[i]].isknockedout == 2))
									{
										pedmap[peds[i]].isknockedout = GAMEPLAY::GET_GAME_TIMER() + kotime;
									}
									else if (pedmap[peds[i]].isknockedout != 0 && GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].isknockedout && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);

									//preventing NPCs from getting up after being downed
									if ((pedmap[peds[i]].dyingstate1entered != 0 || pedmap[peds[i]].dyingstate2entered != 0 || pedmap[peds[i]].dyingstate3entered != 0) && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 35000, 35000, 3, false, false, false);

									if (pedisready)// && ENTITY::GET_ENTITY_SPEED(peds[i]) < maxragdollspeed)
									{
										//if NPC is not underwater, stop it from speaking (feels weird when they are down and still taunt the player like nothing happened)
										if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);

										//keep NPC on the ground
										if (npchealth > ini_dyingthreshold) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);

										//ensure reactiveness of NPC
										PED::SET_PED_CAN_ARM_IK(peds[i], true);
										PED::SET_PED_CAN_HEAD_IK(peds[i], true);
										PED::SET_PED_CAN_LEG_IK(peds[i], true);
										PED::SET_PED_CAN_TORSO_IK(peds[i], true);

										//setting the flag for ped being down (for NPC fear triggering)
										if (pedmap[peds[i]].isdown == 0)
										{
											pedmap[peds[i]].isdown = 1;
											npcdowncounter++;
										}

										//remembering health for pain reactions
										if (pedmap[peds[i]].healthds == 0) pedmap[peds[i]].healthds = npchealth;

										//pain reaction (when being shot)
										if (npchealth < pedmap[peds[i]].healthds)
										{
											if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) PedFear(peds[i], 4, 0, 100);//PedPain(peds[i]);
											pedmap[peds[i]].healthds = npchealth;
										}

										//pain sounds
										if (painaudiorand < 3 && !submerged && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);

										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
										//
										//
										//			BEGIN: DYING STATE 1, ID: PDO033
										//
										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

										if (npchealth > ini_dyingmovementthreshold2)
										{
											//reaction to being aimed at
											if (ini_ds1audio == 1 && !PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && !pedmap[peds[i]].ishanging)
											{
												Entity playerTargetped;
												if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetped))
												{
													if (playerTargetped == peds[i] && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
													{
														if (PED::IS_PED_SHOOTING(playerPed)) pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 4, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
														else
														{
															int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
															if (reactionrand < ini_reactiononaim)
															{
																if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
																{
																	AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
																	if (!pedmap[peds[i]].wasaimedat)
																	{
																		int pleadorno = 0 + (std::rand() % (9 - 0 + 1));
																		if (pleadorno < 5) PedFear(peds[i], 3, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
																		else PedFear(peds[i], 1, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
																		pedmap[peds[i]].wasaimedat = true;
																	}
																	else pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 1, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
																}
															}
														}
													}
												}
											}

											//panicking
											Entity playerTarget;
											if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
											{
												if (playerTarget == peds[i] && pedmap[peds[i]].limbhealth > (npchealth + ini_bleedingvalue))
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
																if (painaudiorand < 30 && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))	AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
															}
															else if (painaudiorand < ini_panicchancerag && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
														}
													}
													else
													{
														int painaudiorand = 0 + (std::rand() % (99999 - 0 + 1));
														if (painaudiorand < ini_panicchance && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
														{
															AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
															AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
														}
													}
												}
											}

											//if this is the first time entering this loop, create writhing behavior
											if (pedmap[peds[i]].animtrans == -1) pedmap[peds[i]].animtrans = GAMEPLAY::GET_GAME_TIMER() + ini_transdelta;
											else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].animtrans)
											{
												//transitioning into writhing animations
												if (pedmap[peds[i]].dyingstate1entered == 0 && pedmap[peds[i]].dsanimationused == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmap[peds[i]].dyingstate1entered = GAMEPLAY::GET_GAME_TIMER();
												}
												else if (pedmap[peds[i]].dyingstate1entered != 0 && pedmap[peds[i]].isknockedout == 1 && pedmap[peds[i]].dsanimationused == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmap[peds[i]].isknockedout = 2;
												}
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
										}

										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
										//
										//
										//			END: DYING STATE 1
										//
										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
										//
										//
										//			BEGIN: DYING STATE 2, ID: PDO034
										//
										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

										else if (npchealth > ini_dyingthreshold)
										{
											//reaction to being aimed at
											if (ini_ds2audio == 1 && !isnpconfire && !PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && !pedmap[peds[i]].ishanging)
											{
												Entity playerTargetped;
												if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetped))
												{
													if (playerTargetped == peds[i] && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
													{
														if (PED::IS_PED_SHOOTING(playerPed)) pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 4, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
														else
														{
															int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
															if (reactionrand < ini_reactiononaim)
															{
																if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
																{
																	AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
																	if (!pedmap[peds[i]].wasaimedat)
																	{
																		int pleadorno = 0 + (std::rand() % (9 - 0 + 1));
																		if (pleadorno < 5) PedFear(peds[i], 3, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
																		else PedFear(peds[i], 2, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
																		pedmap[peds[i]].wasaimedat = true;
																	}
																	else pedmap[peds[i]].lastfearaudio = PedFear(peds[i], 2, pedmap[peds[i]].lastfearaudio, ini_ds2audiochancemod);
																}
															}
														}
													}
												}
											}

											//panicking
											Entity playerTarget;
											if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
											{
												if (playerTarget == peds[i] && pedmap[peds[i]].limbhealth > (npchealth + ini_bleedingvalue))
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
																if (painaudiorand < 30 && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))	AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
															}
															else if (painaudiorand < ini_panicchancerag && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
														}
													}
													else
													{
														int painaudiorand = 0 + (std::rand() % (99999 - 0 + 1));
														if (painaudiorand < ini_panicchance && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
														{
															AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
															AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
														}
													}
												}
											}
											
											//if this is the first time entering this loop, create writhing behavior
											if (pedmap[peds[i]].animtrans == -1) pedmap[peds[i]].animtrans = GAMEPLAY::GET_GAME_TIMER() + ini_transdelta;
											else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].animtrans)
											{
												//transitioning into writhing animations
												if (pedmap[peds[i]].dyingstate1entered == 0 && pedmap[peds[i]].dyingstate2entered == 0 && pedmap[peds[i]].dsanimationused == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													int temp = GAMEPLAY::GET_GAME_TIMER();
													pedmap[peds[i]].dyingstate1entered = temp;
													pedmap[peds[i]].dyingstate2entered = temp;
												}
												else if (pedmap[peds[i]].dyingstate1entered != 0 && pedmap[peds[i]].dyingstate2entered != 0 && pedmap[peds[i]].isknockedout == 1 && pedmap[peds[i]].dsanimationused == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmap[peds[i]].isknockedout = 2;
												}
												else if (pedmap[peds[i]].dyingstate1entered != 0 && pedmap[peds[i]].dyingstate2entered == 0)
												{
													//AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::CLEAR_PED_TASKS(peds[i], false, false);
													AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmap[peds[i]].dyingstate2entered = GAMEPLAY::GET_GAME_TIMER();
												}													
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
										}

										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
										//
										//
										//			END: DYING STATE 2
										//
										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
										//
										//
										//			BEGIN: DYING STATE 3, ID: PDO035
										//
										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

										else
										{
											//if this is the first time entering this loop, create writhing behavior
											if (pedmap[peds[i]].dyingstate3entered == 0)
											{
												pedmap[peds[i]].dyingstate3entered = GAMEPLAY::GET_GAME_TIMER();
												AI::CLEAR_PED_TASKS(peds[i], false, false);
												AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
												PED::SET_PED_TO_RAGDOLL(peds[i], 35000, 35000, 0, false, false, false); //ragdoll the NPC
											}

											int painrand = 0 + (std::rand() % (999 - 0 + 1));
											int painaudio = 9;
											if (painrand < 5 && !submerged && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
										}

										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
										//
										//
										//			END: DYING STATE 3
										//
										//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

									}

									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
									//
									//
									//			END: DYING STATE BEHAVIOR - IF NPC IS NOT STANDING
									//
									//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

								}
							}

							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
							//
							//
							//			BEGIN: DYING STATE BLEEDING, ID: PDO36
							//
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

							if (!pedmap[peds[i]].nobleeding)
							{
								if (ini_hogtiebleedingstop == 1)
								{
									if (!(islassoed || ishogtied))
									{
										if (ini_onlybleedshottorso == 1)
										{
											if (pedmap[peds[i]].torsohit == 1)
											{
												if (ini_bleedwhendying == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
												{
													int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
													if (bleedrand < ini_bleedingchance_dying1)
													{
														if (npchealth - ini_bleedingvalue < 1)
														{
															ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
															npchealth = 0;
														}
														else
														{
															ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
															npchealth -= ini_bleedingvalue;
														}
														pedmap[peds[i]].healthds -= ini_bleedingvalue;
													}
												}
												//else pedmap[peds[i]].healthds = npchealth;
											}
										}
										else
										{
											if (ini_bleedwhendying == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
											{
												int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
												if (bleedrand < ini_bleedingchance_dying1)
												{
													if (npchealth - ini_bleedingvalue < 1)
													{
														ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
														npchealth = 0;
													}
													else
													{
														ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
														npchealth -= ini_bleedingvalue;
													}
													pedmap[peds[i]].healthds -= ini_bleedingvalue;
												}
											}
											//else pedmap[peds[i]].healthds = npchealth;
										}
									}
								}
								else if (ini_onlybleedshottorso == 1)
								{
									if (pedmap[peds[i]].torsohit == 1)
									{
										if (ini_bleedwhendying == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
										{
											int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
											if (bleedrand < ini_bleedingchance_dying1)
											{
												if (npchealth - ini_bleedingvalue < 1)
												{
													ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
													npchealth = 0;
												}
												else
												{
													ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
													npchealth -= ini_bleedingvalue;
												}
												pedmap[peds[i]].healthds -= ini_bleedingvalue;
											}
										}
										//else pedmap[peds[i]].healthds = npchealth;
									}
								}
								else if (ini_bleedwhendying == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
								{
									int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
									if (bleedrand < ini_bleedingchance_dying1)
									{
										if (npchealth - ini_bleedingvalue < 1)
										{
											ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
											npchealth = 0;
										}
										else
										{
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
											npchealth -= ini_bleedingvalue;
										}
										pedmap[peds[i]].healthds -= ini_bleedingvalue;
									}
								}
								//else pedmap[peds[i]].healthds = npchealth;
							}

							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
							//
							//
							//			END: DYING STATE BLEEDING
							//
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						}

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			END: DYING STATES (GENERAL)
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						
						

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: BLEEDING WHEN SHOT, ID: PDO037
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						if (!pedmap[peds[i]].nobleeding && (npchealth >= ini_dyingmovementthreshold))
						{
							if (ini_hogtiebleedingstop == 1)
							{
								if (!(islassoed || ishogtied))
								{
									if (ini_onlybleedshottorso == 1)
									{
										if (pedmap[peds[i]].torsohit == 1)
										{
											if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
											{
												int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
												if (bleedrand < ini_bleedingchance_dying1)
												{
													if (npchealth - ini_bleedingvalue < 1)
													{
														ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
														npchealth = 0;
													}
													else
													{
														ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
														npchealth -= ini_bleedingvalue;
													}
													pedmap[peds[i]].healthds -= ini_bleedingvalue;
												}
											}
											//else pedmap[peds[i]].healthds = npchealth;
										}
									}
									else
									{
										if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
										{
											int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
											if (bleedrand < ini_bleedingchance_dying1)
											{
												if (npchealth - ini_bleedingvalue < 1)
												{
													ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
													npchealth = 0;
												}
												else
												{
													ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
													npchealth -= ini_bleedingvalue;
												}
												pedmap[peds[i]].healthds -= ini_bleedingvalue;
											}
										}
										//else pedmap[peds[i]].healthds = npchealth;
									}
								}
							}
							else if (ini_onlybleedshottorso == 1)
							{
								if (pedmap[peds[i]].torsohit == 1)
								{
									if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
									{
										int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
										if (bleedrand < ini_bleedingchance_dying1)
										{
											if (npchealth - ini_bleedingvalue < 1)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
												npchealth = 0;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
												npchealth -= ini_bleedingvalue;
											}
											pedmap[peds[i]].healthds -= ini_bleedingvalue;
										}
									}
									//else pedmap[peds[i]].healthds = npchealth;
								}
							}
							else if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
							{
								int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
								if (bleedrand < ini_bleedingchance_shot)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
									npchealth -= ini_bleedingvalue;
									pedmap[peds[i]].healthds -= ini_bleedingvalue;
								}
							}
							//else pedmap[peds[i]].healthds = npchealth;
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: BLEEDING WHEN SHOT
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
				   
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: SPINE SHOT BLEEDING, ID: PDO038
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//bleeding for spine shot paralysis
						if (!pedmap[peds[i]].nobleeding && (pedmap[peds[i]].status == 10 || pedmap[peds[i]].status == 11))
						{
							if (ini_hogtiebleedingstop == 1)
							{
								if (!(islassoed || ishogtied))
								{
									if (ini_onlybleedshottorso == 1)
									{
										if (pedmap[peds[i]].torsohit == 1)
										{
											if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
											{
												int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
												if (bleedrand < ini_bleedingchance_dying1)
												{
													if (npchealth - ini_bleedingvalue < 1)
													{
														ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
														npchealth = 0;
													}
													else
													{
														ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
														npchealth -= ini_bleedingvalue;
													}
													pedmap[peds[i]].healthds -= ini_bleedingvalue;
												}
											}
											//else pedmap[peds[i]].healthds = npchealth;
										}
									}
									else
									{
										if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
										{
											int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
											if (bleedrand < ini_bleedingchance_dying1)
											{
												if (npchealth - ini_bleedingvalue < 1)
												{
													ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
													npchealth = 0;
												}
												else
												{
													ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
													npchealth -= ini_bleedingvalue;
												}
												pedmap[peds[i]].healthds -= ini_bleedingvalue;
											}
										}
										//else pedmap[peds[i]].healthds = npchealth;
									}
								}
							}
							else if (ini_onlybleedshottorso == 1)
							{
								if (pedmap[peds[i]].torsohit == 1)
								{
									if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
									{
										int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
										if (bleedrand < ini_bleedingchance_dying1)
										{
											if (npchealth - ini_bleedingvalue < 1)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
												npchealth = 0;
											}
											else
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
												npchealth -= ini_bleedingvalue;
											}
											pedmap[peds[i]].healthds -= ini_bleedingvalue;
										}
									}
									//else pedmap[peds[i]].healthds = npchealth;
								}
							}
							else if (ini_bleedwhenshot == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
							{
								int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
								if (bleedrand < ini_bleedingchance_shot)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
									npchealth -= ini_bleedingvalue;
									pedmap[peds[i]].healthds -= ini_bleedingvalue;
								}
							}
							//else pedmap[peds[i]].healthds = npchealth;
						}

						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			END: SPINE SHOT BLEEDING
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


					}

			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
			//
			//
			//			END: AFFECTING NPCs WITH PROPERLY SET HEALTH AND NOT CURRENTLY BURNING OR BURNING BEFORE
			//
			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			BEGIN: FIRE / BURNING, ID: PDO039
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					
					//make sure peds on fire play audio
					if (ini_diversefireaudio == 1)
					{
						if (isnpconfire && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]) && GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].fireaudiotime)
						{
							pedmap[peds[i]].fireaudiosample = PedFear(peds[i], 5, pedmap[peds[i]].fireaudiosample, ini_ds2audiochancemod);
							pedmap[peds[i]].fireaudiotime = GAMEPLAY::GET_GAME_TIMER() + fireaudiointerval;
						}
					}
					
					//if NPC was burning and is not anymore, create according behavior
					if ((pedmap[peds[i]].status == 4 || pedmap[peds[i]].status == 5) && !isnpconfire && !pedmap[peds[i]].nobleeding)
					{
						if (PED::GET_PED_MAX_HEALTH(peds[i]) != ini_firehealth)
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_firehealth);
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
							npchealth = ini_dyingthreshold - 1;
							//pedmap[peds[i]].dyingstate3entered = 0;
						}
						if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);
						if (PED::IS_PED_RAGDOLL(peds[i])) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
						else PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 3, false, false, false);
						int painrand = 0 + (std::rand() % (999 - 0 + 1));
						int painaudio = 9;
						if (painrand < 5 && !submerged && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
						if (ini_bleedwhendying == 1 && pedmap[peds[i]].hasbeendamagedbyweapon)
						{
							int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
							if (bleedrand < ini_bleedingchance_dying2)
							{
								if (npchealth < 2)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
									npchealth = 0;
								}
								else
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
									npchealth -= ini_bleedingvalue;
								}
							}
						}
					}
					//randomize if NPC can survive fire and set the status accordingly
					int survivfir = 0 + (std::rand() % (99 - 0 + 1));
					if (survivfir < ini_survivfirchance)
					{
						if (pedmap[peds[i]].status == 1) pedmap[peds[i]].status = 2;
						else if (pedmap[peds[i]].status == 0) pedmap[peds[i]].status = 3;
						else if (pedmap[peds[i]].status == 6) pedmap[peds[i]].status = 7;
						else if (pedmap[peds[i]].status == 8) pedmap[peds[i]].status = 9;
						else if (pedmap[peds[i]].status == 10) pedmap[peds[i]].status = 11;
					}
					else
					{
						if (pedmap[peds[i]].status == 2) pedmap[peds[i]].status = 1;
						else if (pedmap[peds[i]].status == 3) pedmap[peds[i]].status = 0;
						else if (pedmap[peds[i]].status == 7) pedmap[peds[i]].status = 6;
						else if (pedmap[peds[i]].status == 9) pedmap[peds[i]].status = 8;
						else if (pedmap[peds[i]].status == 11) pedmap[peds[i]].status = 10;
					}
					//if NPC should survive fire, make sure it does, otherwise make sure it doesnt
					if (isnpconfire)
					{
						if (pedmap[peds[i]].status == 2 || pedmap[peds[i]].status == 3 || pedmap[peds[i]].status == 4 || pedmap[peds[i]].status == 7 || pedmap[peds[i]].status == 9 || pedmap[peds[i]].status == 11)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], 800, 0);
							npchealth = 800;
							pedmap[peds[i]].status = 4;
						}
						else
						{
							if (pedmap[peds[i]].status != 5)
							{
								if (PED::GET_PED_MAX_HEALTH(peds[i]) != ini_firehealth)
								{
									PED::SET_PED_MAX_HEALTH(peds[i], ini_firehealth);
									ENTITY::SET_ENTITY_HEALTH(peds[i], ini_firehealth, 0);
									npchealth = ini_firehealth;
								}
								pedmap[peds[i]].status = 5;
							}
						}
					}

					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
					//
					//
					//			END: FIRE / BURNING
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					//clearing information containers to set them up for the next iteration
					PED::CLEAR_PED_LAST_DAMAGE_BONE(peds[i]);
					WEAPON::_CLEAR_PED_LAST_WEAPON_DAMAGE(peds[i]);
					if (pedmap[peds[i]].health != 0) pedmap[peds[i]].health = npchealth;
				}

			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
			//
			//
			//			END: AFFECTING LIVING NPCs
			//
			//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

			}
		}

		//draw messages on screen
		if (GetTickCount() < msgTime)
		{
			if (modScriptState)
			{
				DrawText(0.45, 0.45, "PedDamageOverhaul has been ENABLED.");
				if (iniexists) DrawText(0.58, 0.58, "ini file found.");
				else DrawText(0.58, 0.58, "ini file not found.");
			}
			else DrawText(0.45, 0.45, "PedDamageOverhaul has been DISABLED.");
		}

		//draw messages on screen
		if (GetTickCount() < msgTimeKillWounded)
		{
			if (killwoundedbool) DrawText(0.65, 0.65, "'Killing Wounded NPCs' has been ENABLED.");
			else DrawText(0.65, 0.65, "'Killing Wounded NPCs' has been DISABLED.");
		}

		//draw messages on screen
		if (GetTickCount() < msgTimeLongerBleedout)
		{
			if (longerbleedoutbool) DrawText(0.65, 0.65, "'Longer Bleedouts' has been ENABLED.");
			else DrawText(0.65, 0.65, "'Longer Bleedouts' has been DISABLED.");
		}

		//draw messages on screen
		if (GetTickCount() < msgTimeFriendlyFire)
		{
			if (friendlyfirebool) DrawText(0.65, 0.65, "'Friendly Fire' has been ENABLED.");
			else DrawText(0.65, 0.65, "'Friendly Fire' has been DISABLED.");
		}

		WAIT(1);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}