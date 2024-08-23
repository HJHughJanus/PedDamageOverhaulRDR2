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
ID: PDO015x NPC SURRENDERING
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
#include "MiscFunctions.h"
#include "SetUpFunctions.h"
#include "Structs.h"

#include <unordered_map>
#include <vector>
#include <string>
#include <map>
#include <array>

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
	float ini_meleemodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MeleeModifier", 100, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_npcweaponmodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCWeaponModifier", 230, ".\\PedDamageOverhaul.ini")) / 100;
	float ini_npcmeleemodifier = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCMeleeModifier", 100, ".\\PedDamageOverhaul.ini")) / 100;
	int ini_friendlystorypedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FriendlyStoryNPCHealthMin", 450, ".\\PedDamageOverhaul.ini");
	int ini_friendlystorypedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FriendlyStoryNPCHealthMax", 450, ".\\PedDamageOverhaul.ini");
	int ini_otherstorypedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OtherStoryNPCHealthMin", 100, ".\\PedDamageOverhaul.ini");
	int ini_otherstorypedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OtherStoryNPCHealthMax", 100, ".\\PedDamageOverhaul.ini");
	int ini_lawpedhealthmin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PoliceNPCHealthMin", 75, ".\\PedDamageOverhaul.ini");
	int ini_lawpedhealthmax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PoliceNPCHealthMax", 75, ".\\PedDamageOverhaul.ini");
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
	float ini_zvaluehead = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHeadBone", 500, ".\\PedDamageOverhaul.ini")) / 1000;
	float ini_zvaluehip = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHipBone", 500, ".\\PedDamageOverhaul.ini")) / 1000;
	int ini_ignoreupvec = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IgnoreUpVector", 1, ".\\PedDamageOverhaul.ini");
	int ini_dforceisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingForceRelative", 0, ".\\PedDamageOverhaul.ini");
	int ini_ddirectionisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingDirectionRelative", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingpushchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingPushChance", 600, ".\\PedDamageOverhaul.ini");
	float ini_bleedingchance_shot = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceShot", 210, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_bleedingchance_dying1 = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying1", 120, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_bleedingchance_dying2 = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying2", 60, ".\\PedDamageOverhaul.ini")) / 10;
	float ini_longerbleedingchance = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LongerBleedingChance", 30, ".\\PedDamageOverhaul.ini")) / 10;
	int ini_longerbleedingvalue = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LongerBleedingValue", 4, ".\\PedDamageOverhaul.ini");
	int ini_dirreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeOneLeg", 1, ".\\PedDamageOverhaul.ini");
	int ini_forreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeOneLeg", 0, ".\\PedDamageOverhaul.ini");
	int ini_dirrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeBothLegs", 1, ".\\PedDamageOverhaul.ini");
	int ini_forrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeBothLegs", 0, ".\\PedDamageOverhaul.ini");
	int ini_lassodisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "LassoDisarms", 0, ".\\PedDamageOverhaul.ini");
	int ini_hogtiedisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "HogtyingDisarms", 1, ".\\PedDamageOverhaul.ini");
	int ini_disarmingchance = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisarmingChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_disarmingwhendown = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisarmingWhenDown", 1, ".\\PedDamageOverhaul.ini");
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
	float ini_legdamagemod = 1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LegDamageModifier", 1, ".\\PedDamageOverhaul.ini")) / 100;
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
	int ini_longerbleedoutdisabledinmissions = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DisableLongerBleedoutInMissions", 1, ".\\PedDamageOverhaul.ini");
	int ini_excludebodyguardmodnpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ExcludeBodyGuardSpawnerBodyguards", 1, ".\\PedDamageOverhaul.ini");
	int ini_tickinterval = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TickInterval", 200, ".\\PedDamageOverhaul.ini");
	int ini_clearnpcsaftertickinterval = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ClearNPCsAfterTickIntervals", 100, ".\\PedDamageOverhaul.ini");
	bool ini_dropnpcindyingstate = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DropNPCInDyingState", 1, ".\\PedDamageOverhaul.ini");
	
	//storing certain ini values in one object, so it can be passed to functions needing them
	std::unordered_map<std::string, float> iniValues;
	iniValues["ini_excludebodyguardmodnpcs"] = ini_excludebodyguardmodnpcs;
	iniValues["ini_policedismountdistancemin"] = ini_policedismountdistancemin;
	iniValues["ini_policedismountdistancemax"] = ini_policedismountdistancemax;
	iniValues["ini_dyingstatechance"] = ini_dyingstatechance;
	iniValues["ini_useanimchance"] = ini_useanimchance;
	iniValues["ini_euphoriastumblechance"] = ini_euphoriastumblechance;
	iniValues["ini_friendlystorypedhealthmin"] = ini_friendlystorypedhealthmin;
	iniValues["ini_friendlystorypedhealthmax"] = ini_friendlystorypedhealthmax;
	iniValues["ini_otherstorypedhealthmin"] = ini_otherstorypedhealthmin;
	iniValues["ini_otherstorypedhealthmax"] = ini_otherstorypedhealthmax;
	iniValues["ini_lawpedhealthmin"] = ini_lawpedhealthmin;
	iniValues["ini_lawpedhealthmax"] = ini_lawpedhealthmax;
	iniValues["ini_pedhealthmin"] = ini_pedhealthmin;
	iniValues["ini_pedhealthmax"] = ini_pedhealthmax;
	iniValues["ini_excludefriendlystorynpcs"] = ini_excludefriendlystorynpcs;
	iniValues["ini_excludeotherstorynpcs"] = ini_excludeotherstorynpcs;
	iniValues["ini_shownpchealth"] = ini_shownpchealth;
	iniValues["ini_showpedmodel"] = ini_showpedmodel;
	iniValues["ini_showpedtasks"] = ini_showpedtasks;
	
	//setting up hotkeys
	int toggleKey = VK_F9;
	bool toggleKeyBound;
	int killwoundedkey = VK_F8;
	bool killWoundedKeyBound;
	int longerbleedoutkey = VK_F7;
	bool longerBleedoutKeyBound;
	int friendlyfirekey = VK_F2;
	bool friendlyFireKeyBound;

	toggleKeyBound = SetKey(&toggleKey, ini_togglekey);
	killWoundedKeyBound = SetKey(&killwoundedkey, ini_killwoundedkey);
	longerBleedoutKeyBound = SetKey(&longerbleedoutkey, ini_longerbleedoutkey);
	friendlyFireKeyBound = SetKey(&friendlyfirekey, ini_friendlyfirekey);

	//setting up correct speed thresholds for stumbling and ragdolling
	const float runningthresholdbothlegs = ini_runningthresholdbothlegs / 10;
	const float runningthresholdoneleg = ini_runningthresholdoneleg / 10;
	const float dsrunningthresholdbothlegs = ini_dsrunningthresholdbothlegs / 10;
	const float dsrunningthresholdoneleg = ini_dsrunningthresholdoneleg / 10;
	const float maxragdollspeed = ini_maxragdollspeed / 10;
	const float maxragdollspeed2 = ini_maxragdollspeed2 / 10;

	//making sure euphoria settings do not make NPCs in cover sack down when using euphoria mods
	/*
	if (ini_nopushingdown == 1) {
		ini_zvaluehead = 2.0f;
	}
	*/

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

	//container for NPCs and their attributes
	std::tr1::unordered_map<Ped, PedAttributes> pedmap;

	//container for story NPCs
	const std::vector<Hash> friendlyStoryNPCs = getFriendlyStoryPedList();
	const std::vector<Hash> otherStoryNPCs = getOtherStoryPedList();

	//container for weapons which cause bleeding
	std::array<char*, 56> allweaponmap = SetUpAllWeaponMap();

	//container for special weapons (affected by special weapon damage modifier)
	std::array<Hash, 14> specialweaponmap = SetUpSpecialWeaponMap();

	//container for special ammo (affected by special weapon damage modifier)
	std::map<Hash, Hash> specialammomap = SetUpSpecialAmmoMap();

	//container for knives (affected by knife damage modifier)
	std::array<char*, 7> knives = SetUpKnivesMap();

	//container for bones (needed for bleeding and locational damage)
	std::array<int, 6> legbones = SetUpLegBonesMap(), torsobones = SetUpTorsoBonesMap(), neckbones = SetUpNeckBonesMap();
	std::array<int, 17> armbones = SetUpArmBonesMap();
	std::array<int, 1> headbones = SetUpHeadBonesMap();
	std::array<int, 14> torsobonesbleeding = SetUpTorsoBonesBleedingMap();	
	
	//loop counter (increments each script loop)
	int loopcounter = 0;

	//fire audio interval (needed so pain audio from NPCs on fire does not loop)
	const int fireaudiointerval = 4000;

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
	const int kotime = 20000;

	//variables for downed NPCs (used when deciding if NPCs flee out of fear) 
	int npcdowncounter = 0;
	int npcdowntimeintervallx = 0;

	//values for drowning (needed so NPCs with high health (if someone changed the ini values) still drown properly)
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

	//longer bleedout settings
	bool longerbleedoutbool = false;
	int msgTimeLongerBleedout = 0;
	int longerbleedingvalue = ini_longerbleedingvalue;
	int longerbleedingchance = ini_longerbleedingchance;
	int bleedingvaluetemp = ini_bleedingvalue;
	int bleedingchancetemp = ini_bleedingchance_dying2;
	
	//variables for printing on screen text (if the corresponding ini values are set)
	bool drawStuffOnScreen = false;
	if (ini_shownpchealth != 0 || ini_showpedmodel != 0 || ini_showpedtasks != 0) drawStuffOnScreen = true;
	std::array<char, 60> c, d, e, f, g, h, j;
	std::unordered_map<char, std::array<char, 60>*> charsets;
	charsets['c'] = &c;
	charsets['d'] = &d;
	charsets['e'] = &e;
	charsets['f'] = &f;
	charsets['g'] = &g;
	charsets['h'] = &h;
	charsets['j'] = &j;
	//std::string text = "NPC Health: ", textpm = "Hostile NPC Count: ", textstat = "Ped Status: ", ttext = "NPC Tasks: ", mtext = "NPC Model: ", torso = "Max HP/Use Max HP: ", weapondam = "Damaged by weapon: ", legshit = "Legs hit/Torso Hit: ", dyingbool = "DS enabled: ", dshistory = "DS visited (1, 2, 3): ", lastdambone = "Last dmg bone/StoryNPC: ";
	std::string text, textpm, textstat, ttext, mtext, torso, weapondam, legshit, dyingbool, dshistory, lastdambone;
	int taskid = 99999;
	int lastbonedamaged = 0;
	int lastbonedamagedtemp = 0;
	Entity playerTargetyolo = PLAYER::PLAYER_PED_ID();
	Entity playerTargetOld = playerTargetyolo;

	//get player and player ped as well as the identifying hash for the lasso weapon and a variable for NPCs getting caught by it and another one for checking if player damages hogtied NPCs
	Player player = PLAYER::PLAYER_ID();
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	Hash lassoHash = GAMEPLAY::GET_HASH_KEY("WEAPON_LASSO");
	Ped lastTarget = playerPed; //used for lasso disarming
	Ped lastTarget2 = playerPed; //used for hogtied panicking
	Ped lastTargetx = playerPed; //used for special hogtie fix for dying movement writhing animation
	Ped lastTargety = playerPed; //used for NPC screaming on being attacked

	//containers for storing information needed for on screen display
	std::unordered_map<std::string, Ped*> pedInfo;
	pedInfo["playerTargetyolo"] = &playerTargetyolo;
	pedInfo["playerTargetOld"] = &playerTargetOld;
	pedInfo["playerPed"] = &playerPed;

	std::unordered_map<std::string, std::string*> textInfo;
	textInfo["text"] = &text;
	textInfo["textpm"] = &textpm;
	textInfo["textstat"] = &textstat;
	textInfo["ttext"] = &ttext;
	textInfo["mtext"] = &mtext;
	textInfo["torso"] = &torso;
	textInfo["weapondam"] = &weapondam;
	textInfo["legshit"] = &legshit;
	textInfo["dyingbool"] = &dyingbool;
	textInfo["dshistory"] = &dshistory;
	textInfo["lastdambone"] = &lastdambone;

	std::unordered_map<std::string, int*> miscInfo;
	miscInfo["lastbonedamaged"] = &lastbonedamaged;
	miscInfo["lastbonedamagedtemp"] = &lastbonedamagedtemp;
	miscInfo["combatcounter"] = &combatcounter;

	
	//main script
	while (true)
	{
		//check if script is being enabled or disabled
		if (toggleKeyBound)
		{
			if (IsKeyJustUp(toggleKey))
			{
				modScriptState = !modScriptState;
				msgTime = GetTickCount() + 3000;
			}
		}

		//check if kill wounded function is being enabled or disabled
		if (killWoundedKeyBound)
		{
			if (IsKeyJustUp(killwoundedkey))
			{
				killwoundedbool = !killwoundedbool;
				msgTimeKillWounded = GetTickCount() + 3000;
			}
		}

		//check if longer bleedout function is being enabled or disabled
		if (longerBleedoutKeyBound)
		{
			if (IsKeyJustUp(longerbleedoutkey))
			{
				longerbleedoutbool = !longerbleedoutbool;
				msgTimeLongerBleedout = GetTickCount() + 3000;
			}
		}

		//check if friendly fire function is being enabled or disabled
		if (friendlyFireKeyBound)
		{
			if (IsKeyJustUp(friendlyfirekey))
			{
				friendlyfirebool = !friendlyfirebool;
				msgTimeFriendlyFire = GetTickCount() + 3000;
				if (!friendlyfirebool) ffchangeexecuted = false;
			}
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
			//setting the longer bleeding, firendly fire and killing wounded ini overwrites
			if (ini_alwaysbleedoutlonger == 1) longerbleedoutbool = true;
			if (ini_alwayskillwounded == 1) killwoundedbool = true;
			if (ini_friendlyfire == 1) friendlyfirebool = true;

			//configures longer bleedout function, if enabled (depending on mission status and ini settings)
			bool ismissionandbleedoutshouldbedisabled = false;
			
			if (ini_longerbleedoutdisabledinmissions == 1 && missionactive) {
				ismissionandbleedoutshouldbedisabled = true;
			}

			if (longerbleedoutbool)
			{
				if (!ismissionandbleedoutshouldbedisabled) {
					ini_bleedingchance_dying2 = longerbleedingchance;
					ini_bleedingvalue = longerbleedingvalue;
				}
				else {
					ini_bleedingchance_dying2 = bleedingchancetemp;
					ini_bleedingvalue = bleedingvaluetemp;
				}
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

			//core depletion and money loss after death
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
			const int ARR_SIZE = 256;
			Ped peds[ARR_SIZE];
			int count = worldGetAllPeds(peds, ini_modeffectrange);
			
			if (ini_disableitemglow != 0) //disable item glow
			{
				const int pickupsRange = 128;
				//objectsRange = 128;
				Object pickups[pickupsRange];
				//objects[objectsRange];

				int pickupcount = worldGetAllPickups(pickups, pickupsRange);
				//int objectcount = worldGetAllObjects(objects, objectsRange);

				for (int i = 0; i < pickupcount; i++)
				{
					GRAPHICS::x_0x50C14328119E1DD1(pickups[i], true);
				}

				/*
				for (int i = 0; i < objectcount; i++)
				{
					GRAPHICS::x_0x50C14328119E1DD1(objects[i], true);
				}
				*/
			}

			//filling container for peds and giving them default values
			for (int i = 0; i < count; i++)
			{
				if (pedmap.find(peds[i]) == pedmap.end())
				{
					PedAttributes p;
					
					SetUpPed(&peds[i], &p, &iniValues, &friendlyStoryNPCs, &otherStoryNPCs);

					pedmap[peds[i]] = p;
				}
			}


//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								PLAYER DAMAGE MODIFIERS, ID: PDO003
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


			//set damage modifiers depending on the current weapon and target
			if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(player) || PLAYER::IS_PLAYER_FREE_AIMING(player))
			{
				//knife bool for checking if knife is being used
				bool knife = false;
				//special weapon bool for checking if a special weapon is being used
				bool specialweapon = false;
				//special ammo bool for checking if special ammo is being used
				bool specialammo = false;
				std::map<Hash, Hash>::iterator a_it = specialammomap.begin();
				
				//check if knife is used
				for(char*& k: knives)
				{
					Hash weaponhash;
					if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, false, 0, false))
					{
						if (weaponhash == GAMEPLAY::GET_HASH_KEY(k)) knife = true;
					}
				}

				//check if special weapon is used
				for(Hash& w: specialweaponmap)
				{
					Hash weaponhash;
					if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, false, 0, false))
					{
						if (weaponhash == w) specialweapon = true;
					}
				}

				//check if special ammo is used
				while (a_it != specialammomap.end())
				{
					Hash ammo = a_it->first;
					Hash weapon = a_it->second;
					Hash weaponhash;
					if (WEAPON::GET_CURRENT_PED_WEAPON(playerPed, &weaponhash, false, 0, false))
					{
						if (weaponhash == weapon)
						{
							if (WEAPON::GET_PED_AMMO_TYPE_FROM_WEAPON(playerPed, weapon) == ammo) specialammo = true;
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
				//excludes NPCs from the "Bodyguard Spawner" mod, if set in the ini
				if (!pedmap[peds[i]].isFromBodyguardSpawner)
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

					//drawing on screen information, if set in the ini
					if (drawStuffOnScreen) DrawStatsOnScreen(loopcounter, &iniValues, &charsets, &player, &pedInfo, &textInfo, &pedmap, &miscInfo);

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

						//if NPC is a non-story-char, is not dead and not the player himself, do stuff
					if (peds[i] != playerPed && pedmap[peds[i]].isstorychar == 3 && PED::IS_PED_HUMAN(peds[i]) && !ENTITY::IS_ENTITY_DEAD(peds[i]) && !AI::GET_IS_TASK_ACTIVE(peds[i], 3)) //task 3 is the task for standoff situations (like duels or hostage situations, in which you have to headshot the NPC)
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
						for (char*& w : allweaponmap)
						{
							if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(peds[i], GAMEPLAY::GET_HASH_KEY(w), 0)) pedmap[peds[i]].hasbeendamagedbyweapon = true;
						}

						//bool for checking if NPC is in a state in which ragdolling him doesnt break anything
						bool pedisready;
						
						if (ini_nopushingdown == 0)
						{
							pedisready = PED::IS_PED_RUNNING_RAGDOLL_TASK(peds[i]) && !isnpconfire && !PED::IS_PED_FALLING(peds[i]) && !PED::IS_PED_HANGING_ON_TO_VEHICLE(peds[i]);
						}
						else
						{
							pedisready = !isstanding && PED::IS_PED_RUNNING_RAGDOLL_TASK(peds[i]) && !isnpconfire && !PED::IS_PED_FALLING(peds[i]) && !PED::IS_PED_HANGING_ON_TO_VEHICLE(peds[i]);
						}

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

						if (PED::IS_PED_IN_ANY_VEHICLE(peds[i], false)) {
							pedmap[peds[i]].isinvehicle = true;
						}

						if (pedmap[peds[i]].isinvehicle) {
							if (PED::IS_PED_IN_ANY_VEHICLE(peds[i], false) && PED::GET_PED_MAX_HEALTH(peds[i]) != ini_pedhealthvehicle)
							{
								PED::SET_PED_MAX_HEALTH(peds[i], ini_pedhealthvehicle);
								ENTITY::SET_ENTITY_HEALTH(peds[i], ini_pedhealthvehicle, 0);
								npchealth = ini_pedhealthvehicle;
							}
							else
							{
								pedmap[peds[i]].isinvehicle = false;
							}
						}
						
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
								ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth, 0);
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
						//			END: SET NPC HEALTH, ID: PDO009
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


				//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
				//
				//
				//			BEGIN: AFFECTING NPCs WITH PROPERLY SET HEALTH AND NOT CURRENTLY BURNING OR BURNING BEFORE, ID: PDO010
				//
				//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//if NPC was not burning before, check stuff and do stuff (otherwise execute burning or post-burning behavior)
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
							//			END: SET NPC ACCURACY, ID: PDO011
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
							//			END: KILL NPCs WHEN IN DYING STATE TOO LONG, ID: PDO012
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
							//			END: STOP BLEEDING FOR KNOCKED OUT NPCs, ID: PDO013
							//
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
							//
							//
							//			BEGIN: NPC FALLING CHECKS, ID: PDO014
							//
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

							//handle the falling of an NPC (so it wont survive falls from unrealistic heights)
							if (ini_falltimedown > 0 || ini_falltimedeath > 0)
							{
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
								}

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
								//			END: HORSE FALLING DELTA, ID: PDO015
								//
								//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

							}
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
							//
							//
							//			END: NPC FALLING CHECKS, ID: PDO014
							//
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

							
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
							//
							//
							//			BEGIN: NPC SURRENDERING, ID: PDO015x
							//
							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________


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

							//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
							//
							//
							//			END: NPC SURRENDERING, ID: PDO015x
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
								for (std::vector<int>::size_type it = 0; it != torsobonesbleeding.size(); it++)
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
								if (pedmap[peds[i]].movementstyle == "" || pedmap[peds[i]].movementstyle == "injured_torso")// || pedmap[peds[i]].movementstyle == "injured_right_arm" || pedmap[peds[i]].movementstyle == "injured_left_arm" || pedmap[peds[i]].movementstyle == "injured_torso")
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
								if (ini_disarmingwhendown == 1) {
									DisarmPed(peds[i], vecweaponhand);
								}

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
								for (std::vector<char*>::size_type it = 0; it != knives.size(); it++)
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
							if (pedmap[peds[i]].limbhealth > npchealth&& pedmap[peds[i]].status != 99)
							{
								int damagedone = pedmap[peds[i]].limbhealth - npchealth;
								int act_Bone;
								if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &act_Bone))
								{
									for (std::vector<int>::size_type it = 0; it != legbones.size(); it++)
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
												float dam = ini_legdamagemod;
												dam = damagedone * dam;
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
												npchealth -= (int)dam;
											}
											pedmap[peds[i]].limbhealth = npchealth;
										}
									}

									for (std::vector<int>::size_type it = 0; it != armbones.size(); it++)
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
												float dam = ini_armdamagemod;
												dam = damagedone * dam;
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
												npchealth -= (int)dam;
											}
											pedmap[peds[i]].limbhealth = npchealth;
										}
									}

									for (std::vector<int>::size_type it = 0; it != torsobones.size(); it++)
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
												float dam = ini_torsodamagemod;
												dam = damagedone * dam;
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
												npchealth -= (int)dam;
											}
											pedmap[peds[i]].limbhealth = npchealth;
										}
									}

									for (std::vector<int>::size_type it = 0; it != headbones.size(); it++)
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
												float dam = ini_headdamagemod;
												dam = damagedone * dam;
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - (int)dam, 0);
												npchealth -= (int)dam;
											}
											pedmap[peds[i]].limbhealth = npchealth;
										}
									}

									for (std::vector<int>::size_type it = 0; it != neckbones.size(); it++)
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
												float dam = ini_neckdamagemod;
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
								else if (GAMEPLAY::GET_GAME_TIMER() > pedmap[peds[i]].headvectime&& pedmap[peds[i]].headvectime != -1)
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
										if (ini_disarmingwhendown == 1) {
											DisarmPed(peds[i], vecweaponhand);
										}

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
															for (std::vector<char*>::size_type it = 0; it != knives.size(); it++)
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
												if (pedmap[peds[i]].animtrans == -1) {
													pedmap[peds[i]].animtrans = GAMEPLAY::GET_GAME_TIMER() + ini_transdelta;
												}
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
															for (std::vector<char*>::size_type it = 0; it != knives.size(); it++)
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
									else
									{
										if (ini_dropnpcindyingstate && npchealth <= ini_dyingthreshold)
										{
											PED::SET_PED_TO_RAGDOLL(peds[i], 35000, 35000, 0, false, false, false);
										}
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

		WAIT(ini_tickinterval);
		loopcounter++;

		if (loopcounter % ini_clearnpcsaftertickinterval == 0)
		{
			for (auto& it : pedmap) {
				if (!ENTITY::DOES_ENTITY_EXIST(it.first))
				{
					pedmap.erase(it.first);
				}
			}
		}
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}