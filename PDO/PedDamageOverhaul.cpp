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
	float ini_sweaponmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpecialWeaponModifier", 190, ".\\PedDamageOverhaul.ini") / 100;
	float ini_weaponmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "WeaponModifier", 60, ".\\PedDamageOverhaul.ini") / 100;
	float ini_knifemodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "KnifeModifier", 100, ".\\PedDamageOverhaul.ini") / 100;
	float ini_meleemodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MeleeModifier", 150, ".\\PedDamageOverhaul.ini") / 100;
	float ini_npcweaponmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCWeaponModifier", 100, ".\\PedDamageOverhaul.ini") / 100;
	float ini_npcmeleemodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCMeleeModifier", 100, ".\\PedDamageOverhaul.ini") / 100;
	int ini_storypedhealth = GetPrivateProfileInt("PedDamageConfiguration_Basic", "StoryNPCHealth", 0, ".\\PedDamageOverhaul.ini");
	int ini_togglekey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "ToggleKey", 9, ".\\PedDamageOverhaul.ini");
	int ini_friendlyfirekey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "FirendlyFireKey", 2, ".\\PedDamageOverhaul.ini");
	int ini_killwoundedkey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "KillWoundedKey", 8, ".\\PedDamageOverhaul.ini");
	int ini_longerbleedoutkey = GetPrivateProfileInt("PedDamageConfiguration_Basic", "LongerBleedoutKey", 7, ".\\PedDamageOverhaul.ini");
	int ini_alwayskillwounded = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "AlwaysKillWounded", 0, ".\\PedDamageOverhaul.ini");
	int ini_alwaysbleedoutlonger = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "AlwaysBleedoutLonger", 0, ".\\PedDamageOverhaul.ini");
	int ini_enabled = GetPrivateProfileInt("PedDamageConfiguration_Basic", "EnableScriptByDefault", 1, ".\\PedDamageOverhaul.ini");
	int ini_playerinvincibility = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PlayerInvincibility", 0, ".\\PedDamageOverhaul.ini");
	int ini_survivfirchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FireSurvivalChance", 23, ".\\PedDamageOverhaul.ini");
	int ini_firehealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FireHealth", 300, ".\\PedDamageOverhaul.ini");
	int ini_pedhealth = GetPrivateProfileInt("PedDamageConfiguration_Basic", "NPCHealth", 75, ".\\PedDamageOverhaul.ini");
	int ini_pedhealthvehicle = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCHealthInVehicles", 70, ".\\PedDamageOverhaul.ini");
	float ini_pedhealthvehiclemodtods = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCHealthVehicleToDyingState", 75, ".\\PedDamageOverhaul.ini") / 100;
	float ini_pedhealthvehiclemodtofull = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCHealthVehicleToFull", 90, ".\\PedDamageOverhaul.ini") / 100;
	int ini_painthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "KnockbackThreshold", 230, ".\\PedDamageOverhaul.ini");
	int ini_knockbackchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "KnockbackChance", 11, ".\\PedDamageOverhaul.ini");
	int ini_dyingthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingThreshold", 60, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementthreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingMovementThreshold", 215, ".\\PedDamageOverhaul.ini");
	int ini_dyingmovementthreshold2 = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DyingMovementThreshold2", 130, ".\\PedDamageOverhaul.ini");
	int ini_stumblechancebothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumbleChanceBothLegs", 100, ".\\PedDamageOverhaul.ini");
	int ini_stumblechanceoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumbleChanceOneLeg", 15, ".\\PedDamageOverhaul.ini");
	int ini_dsstumblechancebothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSStumbleChanceBothLegs", 100, ".\\PedDamageOverhaul.ini");
	int ini_dsstumblechanceoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSStumbleChanceOneLeg", 15, ".\\PedDamageOverhaul.ini");
	int ini_bleedwhendying = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedWhenDying", 1, ".\\PedDamageOverhaul.ini");
	int ini_bleedwhenshot = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedWhenShot", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingvalue = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingValue", 2, ".\\PedDamageOverhaul.ini");
	int ini_runningthresholdbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RunningThresholdBothLegs", 30, ".\\PedDamageOverhaul.ini");
	int ini_runningthresholdoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RunningThresholdOneLeg", 50, ".\\PedDamageOverhaul.ini");
	int ini_dsrunningthresholdbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSRunningThresholdBothLegs", 30, ".\\PedDamageOverhaul.ini");
	int ini_dsrunningthresholdoneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSRunningThresholdOneLeg", 50, ".\\PedDamageOverhaul.ini");
	float ini_femalepushmodifier = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FemalePushModifier", 3, ".\\PedDamageOverhaul.ini");
	float ini_forcepushx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushX", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushy = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushY", 320, ".\\PedDamageOverhaul.ini");
	float ini_forcepushz = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "OneLegStumbleForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushxboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushX", 0, ".\\PedDamageOverhaul.ini");
	float ini_forcepushyboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushY", 375, ".\\PedDamageOverhaul.ini");
	float ini_forcepushzboth = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BothLegsStumbleForcePushZ", 0, ".\\PedDamageOverhaul.ini");
	float ini_dforcepushx = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushX", 35, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dforcepushy = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushY", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dforcepushz = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingForcePushZ", 0, ".\\PedDamageOverhaul.ini") / 10;
	float ini_dyingmovementenablespeed = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementEnableWhenSpeedLowerThan", 10, ".\\PedDamageOverhaul.ini") / 10;
	float ini_zvaluehead = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHeadBone", 500, ".\\PedDamageOverhaul.ini") / 1000;
	float ini_zvaluehip = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHipBone", 250, ".\\PedDamageOverhaul.ini") / 1000;
	int ini_ignoreupvec = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IgnoreUpVector", 1, ".\\PedDamageOverhaul.ini");
	int ini_dforceisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingForceRelative", 0, ".\\PedDamageOverhaul.ini");
	int ini_ddirectionisrelative = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDyingDirectionRelative", 1, ".\\PedDamageOverhaul.ini");
	int ini_dyingpushchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingPushChance", 1000, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_shot = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceShot", 8, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_dying1 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying1", 4, ".\\PedDamageOverhaul.ini");
	int ini_bleedingchance_dying2 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingChanceDying2", 8, ".\\PedDamageOverhaul.ini");
	int ini_longerbleedingchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LongerBleedingChance", 3, ".\\PedDamageOverhaul.ini");
	int ini_longerbleedingvalue = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LongerBleedingValue", 4, ".\\PedDamageOverhaul.ini");
	int ini_dirreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeOneLeg", 1, ".\\PedDamageOverhaul.ini");
	int ini_forreloneleg = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeOneLeg", 0, ".\\PedDamageOverhaul.ini");
	int ini_dirrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsDirectionRelativeBothLegs", 1, ".\\PedDamageOverhaul.ini");
	int ini_forrelbothlegs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "IsForceRelativeBothLegs", 0, ".\\PedDamageOverhaul.ini");
	int ini_lassodisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "LassoDisarms", 0, ".\\PedDamageOverhaul.ini");
	int ini_hogtiedisarm = GetPrivateProfileInt("PedDamageConfiguration_Basic", "HogtyingDisarms", 1, ".\\PedDamageOverhaul.ini");
	int ini_disarmingchance = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisarmingChance", 1, ".\\PedDamageOverhaul.ini");
	int ini_xrollchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementXRollChance", 40, ".\\PedDamageOverhaul.ini");
	int ini_dmaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudioChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_painaudio01 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio01", 4, ".\\PedDamageOverhaul.ini");
	int ini_painaudio02 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio02", 5, ".\\PedDamageOverhaul.ini");
	int ini_painaudio03 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingMovementAudio03", 9, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamounthealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountHealth", 5, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamountstamina = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountStamina", 5, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionamountdeadeye = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAmountDeadeye", 5, ".\\PedDamageOverhaul.ini");
	int ini_coredepletiontime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionTime", 300000, ".\\PedDamageOverhaul.ini");
	int ini_coredepletion = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletion", 0, ".\\PedDamageOverhaul.ini");
	int ini_coredepletionafterdeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "CoreDepletionAfterDeath", 0, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletionamounthealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletionAmountHealth", 5, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletionamountstamina = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletionAmountStamina", 5, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletiontime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletionTime", 300000, ".\\PedDamageOverhaul.ini");
	int ini_horsecoredepletion = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseCoreDepletion", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingstatesmelee = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStatesForMelee", 0, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyhealthymin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyHealthyMin", 45, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyinjuredmin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyInjuredMin", 10, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyhealthymax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyHealthyMax", 65, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracyinjuredmax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCAccuracyInjuredMax", 20, ".\\PedDamageOverhaul.ini");
	int ini_pedaccuracythreshold = GetPrivateProfileInt("PedDamageConfiguration_Basic", "NPCAccuracyThreshold", 0, ".\\PedDamageOverhaul.ini");
	int ini_dyingstatechance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateChance", 100, ".\\PedDamageOverhaul.ini");
	int ini_panicchance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PanicChance", 2, ".\\PedDamageOverhaul.ini");
	int ini_panicchancerag = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PanicChanceHelpless", 3, ".\\PedDamageOverhaul.ini");
	int ini_excludestorynpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ExcludeStoryNPCs", 1, ".\\PedDamageOverhaul.ini");
	float ini_legdamagemod =  1 - ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LegDamageModifier", 100, ".\\PedDamageOverhaul.ini") / 100);
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
	int ini_friendlyfire = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FriendlyFire", 0, ".\\PedDamageOverhaul.ini");
	int ini_npcdowntimeintervall = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCsDownIntervall", 10000, ".\\PedDamageOverhaul.ini");
	int ini_npcdownfearthreshold = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCsDownThreshold", 5, ".\\PedDamageOverhaul.ini");
	float ini_npcdownfearchance = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NPCFearChance", 250, ".\\PedDamageOverhaul.ini") / 10);
	int ini_cowerchancefear = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearCowerChance", 5, ".\\PedDamageOverhaul.ini");
	int ini_handsupchancefear = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearHandsUpChance", 55, ".\\PedDamageOverhaul.ini");
	int ini_fleechancefear = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearFleeingChance", 40, ".\\PedDamageOverhaul.ini");	
	int ini_fearfleeultimately = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "FearFleeUltimately", 1, ".\\PedDamageOverhaul.ini");
	float ini_playersneaknoisemult = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SneakNoiseMultiplier", 0, ".\\PedDamageOverhaul.ini") / 100);
	int ini_reactiononaim = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DyingStateAimReaction", 20, ".\\PedDamageOverhaul.ini");
	int ini_ds1audio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DS1Audio", 1, ".\\PedDamageOverhaul.ini");
	int ini_ds2audio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DS2Audio", 1, ".\\PedDamageOverhaul.ini");
	int ini_horsehealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseHealth", 0, ".\\PedDamageOverhaul.ini");
	int ini_horsegod = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseInvincibility", 0, ".\\PedDamageOverhaul.ini");
	int ini_hogtiebleedingstop = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HogtieBleedingStop", 1, ".\\PedDamageOverhaul.ini");
	int ini_arteryshothealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ArteryShotHealth", 400, ".\\PedDamageOverhaul.ini");
	int ini_horsefallingdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HorseFallingDelta", 6000, ".\\PedDamageOverhaul.ini");
	int ini_maxdstime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MaximumDyingStateTime", 25000, ".\\PedDamageOverhaul.ini");
	int ini_disableitemglow = GetPrivateProfileInt("PedDamageConfiguration_Basic", "DisableItemGlow", 0, ".\\PedDamageOverhaul.ini");
	int ini_zvalueheadup = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "ZValueForHeadBone2", 1500, ".\\PedDamageOverhaul.ini");
	int ini_stumbletimemin = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSEuphoriaStumbleMin", 1000, ".\\PedDamageOverhaul.ini");
	int ini_stumbletimemax = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSEuphoriaStumbleMax", 15000, ".\\PedDamageOverhaul.ini");
	int ini_torsooverwrite = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TorsoShotBleedingOverwrite", 0, ".\\PedDamageOverhaul.ini");
	int ini_npcbodydamagedelay = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BodyDamageInterval", 500, ".\\PedDamageOverhaul.ini");
	int ini_headvecmaxdeltab = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadVecMaxDeltaB", 30, ".\\PedDamageOverhaul.ini") / 1000;
	int ini_headvecmaxtimeb = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadVecMaxTimeB", 2000, ".\\PedDamageOverhaul.ini");
	int ini_playerhealth = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "PlayerHealth", 500, ".\\PedDamageOverhaul.ini");
	int ini_nooseaudio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseAudio", 0, ".\\PedDamageOverhaul.ini");
	int ini_nooseaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseAudioChance", 70, ".\\PedDamageOverhaul.ini");
	int ini_nooselivelonger = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NoosedNPCsLiveLonger", 1, ".\\PedDamageOverhaul.ini");
	int ini_noosehealthboost = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseHealthBoost", 1, ".\\PedDamageOverhaul.ini");
	int ini_moneylossafterdeath = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MoneyLossAfterDeath", 0, ".\\PedDamageOverhaul.ini");
	float ini_moneylosspercentage = ((float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MoneyLossPercentage", 0, ".\\PedDamageOverhaul.ini") / 100);
	int ini_legshitthreshold = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "LegsHitThreshold", 2, ".\\PedDamageOverhaul.ini");
	int ini_damagedlegsdowntime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DamagedLegsDownTime", 15000, ".\\PedDamageOverhaul.ini");
	int ini_spineshotsparalyze = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpineShotsParalyze", 0, ".\\PedDamageOverhaul.ini");
	int ini_paralysisaudiochance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpineParalysisAudioChance", 30, ".\\PedDamageOverhaul.ini");
	int ini_parachance = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "SpineParalysisChance", 10, ".\\PedDamageOverhaul.ini");
	int ini_bleedingsave = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingSave", 100000, ".\\PedDamageOverhaul.ini");
	int ini_headvecstumble = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HeadVecStumble", 1, ".\\PedDamageOverhaul.ini");
	int ini_maxragdollspeed = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "RagdollingSpeedThreshold", 1, ".\\PedDamageOverhaul.ini");
	int ini_maxragdollspeed2 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DSRagdollingSpeedThreshold", 1, ".\\PedDamageOverhaul.ini");
	int ini_diversefireaudio = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DiverseFireAudio", 0, ".\\PedDamageOverhaul.ini");
	int ini_ds2audiochancemod = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "DS2AudioChanceModifier", 75, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask01 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask01", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask02 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask02", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask03 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask03", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask04 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask04", 0, ".\\PedDamageOverhaul.ini");
	int ini_bleedingtask05 = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "BleedingTask05", 0, ".\\PedDamageOverhaul.ini");
	int ini_noosehealthboostinterval = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseHealthBoostInterval", 1500, ".\\PedDamageOverhaul.ini");
	int ini_tylfeaturesenabled = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "TYLFeaturesEnabled", 1, ".\\PedDamageOverhaul.ini");
	int ini_mutenoosednpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MuteNoosedNPCs", 0, ".\\PedDamageOverhaul.ini");
	float ini_hangingheight = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseHangingHeight", 20, ".\\PedDamageOverhaul.ini") / 100;
	int ini_noosefaceturn = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "NooseFaceTurn", 1, ".\\PedDamageOverhaul.ini");
	int ini_nopushingdown = GetPrivateProfileInt("PedDamageConfiguration_Basic", "EuphoriaModInstalled", 0, ".\\PedDamageOverhaul.ini");
	int ini_handsuptime = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpTime", 8000, ".\\PedDamageOverhaul.ini");
	int ini_combathandsuphostilenpcs = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpOnDisarmingOnlyWhenAlone", 0, ".\\PedDamageOverhaul.ini");
	int ini_handsuphostilenpcthreshold = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpOnDisarmingAllyCount", 2, ".\\PedDamageOverhaul.ini");
	int ini_handsupdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpDelta", 2000, ".\\PedDamageOverhaul.ini");
	float ini_handsupradius = (float)GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpRadius", 20000, ".\\PedDamageOverhaul.ini") / 100;
	int ini_dsforcedstumbling = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "StumblingBelowDSThreshold", 1, ".\\PedDamageOverhaul.ini");
	int ini_dismounthandsupdelta = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "HandsUpDeltaMounted", 1000, ".\\PedDamageOverhaul.ini");
	int ini_blipfords = GetPrivateProfileInt("PedDamageConfiguration_Advanced", "MapBlipForNPCsInDS", 0, ".\\PedDamageOverhaul.ini");
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
	std::map<Ped, int> pedmapdyingstate1entered;
	//container for peds and a value for dying movement stage 2 (for remembering if it has been entered before)
	std::map<Ped, int> pedmapdyingstate2entered;
	//container for peds and a value for dying movement stage 3 (for remembering if it has been entered before)
	std::map<Ped, int> pedmapdyingstate3entered;
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
	//container for the ped's accuracy
	std::map<Ped, int> pedmapaccuracy;
	//container for flag if ped is down
	std::map<Ped, int> pedmapisdown;
	//container for flag if ped is dead
	std::map<Ped, int> pedmapisdead;
	//container for flag if ped is has reacted to other peds dying in combat
	std::map<Ped, int> pedmaphasreacted;
	//container for bool if ped should bleed
	std::map<Ped, bool> pedmapnobleeding;
	//container for time when ped was last on a mount
	std::map<Ped, int> pedmaplasttimeonmount;
	//container for delaying the dying state effects, so euphoria motions can be fully executed
	std::map<Ped, int> pedmapdsdelaytimer;
	//container for stumbling time
	std::map<Ped, int> pedmapstumbling;
	//container for making sure peds don't stand still
	std::map<Ped, Vector3> pedmapheadvecdelta;
	std::map<Ped, int> pedmapheadvectime;
	//container for last fear audio (to prevent repeating audio)
	std::map<Ped, int> pedmaplastfearaudio;
	//container for how many times the right leg of an NPC was hit
	std::map<Ped, int> pedmaprightleghitcounter;
	//container for how many times the left leg of an NPC was hit
	std::map<Ped, int> pedmapleftleghitcounter;
	//container for damaged legs triggering falling down into dying states
	std::map<Ped, int> pedmaplegsdamageddowntime;
	//container for timer of peds being saved from bleeing out (to not die from leg shots)
	std::map<Ped, int> pedmaplegsdown;
	//container for timer (fire audio interval)
	std::map<Ped, int> pedmapfireaudiotime;
	//container for last sound used (to prevent looping)
	std::map<Ped, int> pedmapfireaudiosample;
	//container for bool if ped is bleeding from an artery shot
	std::map<Ped, bool> pedmaparteryshot;
	//container for bool if ped has been aimed at when down
	std::map<Ped, bool> pedmapwasaimedat;
	//container for bool if ped has been aimed at when down
	std::map<Ped, bool> pedmaphasbeendamagedbyweapon;
	//container for bool if ped is hanging (TieYourLasso mod)
	std::map<Ped, bool> pedmapishanging;
	//container for hanging time (TieYourLasso mod)
	std::map<Ped, int> pedmaphangingtime;
	//container for the last time the ped was shot by the player
	std::map<Ped, int> pedmapshotbyplayer;
	//container for bool if ped was fighting against the player alone
	std::map<Ped, bool> pedmapwasnotaloneincombat;
	//container for bool if ped is in combat with player
	std::map<Ped, bool> pedmapisincombatwithplayer;
	//container for when ped should be in hands-up task
	std::map<Ped, int> pedmaphandsup;
	//container for the first time a ped was forced into "hands up"
	std::map<Ped, int> pedmapfirsttimehandsup;
	//container for the information if a ped is or was armed and has been disarmed
	std::map<Ped, bool> pedmapisarmed;
	std::map<Ped, bool> pedmapwasdisarmed;
	//container for remembering which movement style ped is supposed to have
	std::map<Ped, char*> pedmapmovement;
	//container for remembering if ped blip is set
	std::map<Ped, Blip> pedmapblipset;
	//container for remembering when ped did a forced dismount (for surrendering)
	std::map<Ped, int> pedmapdismount;
	

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


//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
//
//
//								ACTUAL MOD FEATURES STARTING, ID: PDO002
//
//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

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
						ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 0, 0);
						ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 1, 0);
						ATTRIBUTE::_0xC6258F41D86676E0(playerPed, 2, 0);
					}
					
					if (ini_moneylossafterdeath == 1)
					{
						if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 10000) CASH::PLAYER_DECREASE_CASH(10000);
						else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 1000) CASH::PLAYER_DECREASE_CASH(1000);
						else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 100) CASH::PLAYER_DECREASE_CASH(100);
						else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney + 10) CASH::PLAYER_DECREASE_CASH(10);
						else if (CASH::GET_PLAYER_CURRENT_CASH() > playermoney) CASH::PLAYER_DECREASE_CASH(1);
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
				if (pedmapstat.find(peds[i]) == pedmapstat.end())
				{
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
					//status: 0 = default value, >0 = was already in dying state 3 at least once
					pedmapdyingstate3entered[peds[i]] = 0;
				}
				if (pedmapdyingstate2entered.find(peds[i]) == pedmapdyingstate2entered.end())
				{
					//status: 0 = default value, >0 = was already in dying state 2 at least once
					pedmapdyingstate2entered[peds[i]] = 0;
				}
				if (pedmapdyingstate1entered.find(peds[i]) == pedmapdyingstate1entered.end())
				{
					//status: 0 = default value, >0 = was already in dying state 1 at least once
					pedmapdyingstate1entered[peds[i]] = 0;
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
				if (pedmapaccuracy.find(peds[i]) == pedmapaccuracy.end())
				{
					//status: 0 = default value
					pedmapaccuracy[peds[i]] = 0;
				}
				if (pedmapisdown.find(peds[i]) == pedmapisdown.end())
				{
					//status: 0 = not down, 1 = down
					pedmapisdown[peds[i]] = 0;
				}
				if (pedmapisdead.find(peds[i]) == pedmapisdead.end())
				{
					//status: 0 = not dead, 1 = dead
					pedmapisdead[peds[i]] = 0;
				}
				if (pedmaphasreacted.find(peds[i]) == pedmaphasreacted.end())
				{
					//status: 0 = has not reacted to friends dying, 1 = has reacted to friends dying (hands up or cowering), 2 = has reacted to friends dying (fleeing), 9 = fleeing after status 1
					pedmaphasreacted[peds[i]] = 0;
				}
				if (pedmapnobleeding.find(peds[i]) == pedmapnobleeding.end())
				{
					pedmapnobleeding[peds[i]] = false;
				}
				if (pedmaplasttimeonmount.find(peds[i]) == pedmaplasttimeonmount.end())
				{
					//status: 0 = has been on a mount
					pedmaplasttimeonmount[peds[i]] = 0;
				}
				if (pedmapdsdelaytimer.find(peds[i]) == pedmapdsdelaytimer.end())
				{
					pedmapdsdelaytimer[peds[i]] = 0;
				}
				if (pedmaphandsup.find(peds[i]) == pedmaphandsup.end())
				{
					pedmaphandsup[peds[i]] = 0;
				}
				if (pedmapfirsttimehandsup.find(peds[i]) == pedmapfirsttimehandsup.end())
				{
					pedmapfirsttimehandsup[peds[i]] = 0;
				}
				if (pedmapstumbling.find(peds[i]) == pedmapstumbling.end())
				{
					//status: 0 = no stumbling time calculated yet
					pedmapstumbling[peds[i]] = 0;
				}
				if (pedmapheadvecdelta.find(peds[i]) == pedmapheadvecdelta.end())
				{
					pedmapheadvecdelta[peds[i]].x = 0;
					pedmapheadvecdelta[peds[i]].y = 0;
					pedmapheadvecdelta[peds[i]].z = 0;
				}
				if (pedmapheadvectime.find(peds[i]) == pedmapheadvectime.end())
				{
					pedmapheadvectime[peds[i]] = 0;
				}
				if (pedmaplastfearaudio.find(peds[i]) == pedmaplastfearaudio.end())
				{
					pedmaplastfearaudio[peds[i]] = 0;
				}
				if (pedmaprightleghitcounter.find(peds[i]) == pedmaprightleghitcounter.end())
				{
					pedmaprightleghitcounter[peds[i]] = 0;
				}
				if (pedmapleftleghitcounter.find(peds[i]) == pedmapleftleghitcounter.end())
				{
					pedmapleftleghitcounter[peds[i]] = 0;
				}
				if (pedmaplegsdamageddowntime.find(peds[i]) == pedmaplegsdamageddowntime.end())
				{
					pedmaplegsdamageddowntime[peds[i]] = 0;
				}
				if (pedmaplegsdown.find(peds[i]) == pedmaplegsdown.end())
				{
					pedmaplegsdown[peds[i]] = 0;
				}
				if (pedmapfireaudiotime.find(peds[i]) == pedmapfireaudiotime.end())
				{
					pedmapfireaudiotime[peds[i]] = 0;
				}
				if (pedmapfireaudiosample.find(peds[i]) == pedmapfireaudiosample.end())
				{
					pedmapfireaudiosample[peds[i]] = 0;
				}
				if (pedmaparteryshot.find(peds[i]) == pedmaparteryshot.end())
				{
					pedmaparteryshot[peds[i]] = false;
				}
				if (pedmapwasaimedat.find(peds[i]) == pedmapwasaimedat.end())
				{
					pedmapwasaimedat[peds[i]] = false;
				}
				if (pedmaphasbeendamagedbyweapon.find(peds[i]) == pedmaphasbeendamagedbyweapon.end())
				{
					pedmaphasbeendamagedbyweapon[peds[i]] = false;
				}
				if (pedmapishanging.find(peds[i]) == pedmapishanging.end())
				{
					pedmapishanging[peds[i]] = false;
				}
				if (pedmaphangingtime.find(peds[i]) == pedmaphangingtime.end())
				{
					pedmaphangingtime[peds[i]] = 0;
				}
				if (pedmapshotbyplayer.find(peds[i]) == pedmapshotbyplayer.end())
				{
					pedmapshotbyplayer[peds[i]] = 0;
				}
				if (pedmapwasnotaloneincombat.find(peds[i]) == pedmapwasnotaloneincombat.end())
				{
					pedmapwasnotaloneincombat[peds[i]] = false;
				}
				if (pedmapisincombatwithplayer.find(peds[i]) == pedmapisincombatwithplayer.end())
				{
					pedmapisincombatwithplayer[peds[i]] = false;
				}
				if (pedmapisarmed.find(peds[i]) == pedmapisarmed.end())
				{
					pedmapisarmed[peds[i]] = false;
				}
				if (pedmapwasdisarmed.find(peds[i]) == pedmapwasdisarmed.end())
				{
					pedmapwasdisarmed[peds[i]] = false;
				}
				if (pedmapmovement.find(peds[i]) == pedmapmovement.end())
				{
					pedmapmovement[peds[i]] = "";
				}
				if (pedmapblipset.find(peds[i]) == pedmapblipset.end())
				{
					pedmapblipset[peds[i]] = 0;
				}
				if (pedmapdismount.find(peds[i]) == pedmapdismount.end())
				{
					pedmapdismount[peds[i]] = 0;
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
					if (pedmapblipset[peds[i]] != 0)
					{
						if (ENTITY::IS_ENTITY_DEAD(peds[i]) && RADAR::DOES_BLIP_EXIST(pedmapblipset[peds[i]])) RADAR::REMOVE_BLIP(&pedmapblipset[peds[i]]);
					}
				}
				
				//show ped health
				if (ini_shownpchealth == 1)
				{
					strcpy(c, text.c_str());
					strcpy(d, torso.c_str());
					strcpy(e, weapondam.c_str());
					strcpy(f, legshit.c_str());
					strcpy(g, dyingbool.c_str());
					strcpy(h, dshistory.c_str());
					strcpy(j, lastdambone.c_str());
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
						strcpy(c, text.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						PED::GET_PED_LAST_DAMAGE_BONE(playerTargetyolo, &lastbonedamagedtemp);
						if (lastbonedamagedtemp != 0) lastbonedamaged = lastbonedamagedtemp;
						
						text = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(playerTargetyolo));
						torso = "Torso hit: " + std::to_string(pedmaptorsohit[playerTargetyolo]);
						weapondam = "Damaged by weapon: " + std::to_string(pedmaphasbeendamagedbyweapon[playerTargetyolo]);
						legshit = "Legs hit: " + std::to_string(pedmaplegsdown[playerTargetyolo]);
						dyingbool = "DS enabled: " + std::to_string(pedmapdyingstatebool[playerTargetyolo]);
						dshistory = "DS visited (1, 2, 3): " + std::to_string(pedmapdyingstate1entered[playerTargetyolo]) + ", " + std::to_string(pedmapdyingstate2entered[playerTargetyolo]) + ", " + std::to_string(pedmapdyingstate3entered[playerTargetyolo]);
						lastdambone = "Last damaged bone: " + std::to_string(lastbonedamaged);
						strcpy(c, text.c_str());
						strcpy(d, torso.c_str());
						strcpy(e, weapondam.c_str());
						strcpy(f, legshit.c_str());
						strcpy(g, dyingbool.c_str());
						strcpy(h, dshistory.c_str());
						strcpy(j, textpm.c_str());
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
					strcpy(c, text.c_str());
					strcpy(d, torso.c_str());
					strcpy(e, textstat.c_str());
					strcpy(f, legshit.c_str());
					strcpy(g, dyingbool.c_str());
					strcpy(h, dshistory.c_str());
					strcpy(j, textpm.c_str());
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
						strcpy(c, text.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						PED::GET_PED_LAST_DAMAGE_BONE(playerTargetyolo, &lastbonedamagedtemp);
						if (lastbonedamagedtemp != 0) lastbonedamaged = lastbonedamagedtemp;

						text = "NPC Health: " + std::to_string(ENTITY::GET_ENTITY_HEALTH(playerTargetyolo));
						torso = "Torso hit: " + std::to_string(pedmaptorsohit[playerTargetyolo]);
						legshit = "Legs hit: " + std::to_string(pedmaplegsdown[playerTargetyolo]);
						dyingbool = "DS enabled: " + std::to_string(pedmapdyingstatebool[playerTargetyolo]);
						dshistory = "DS visited (1, 2, 3): " + std::to_string(pedmapdyingstate1entered[playerTargetyolo]) + ", " + std::to_string(pedmapdyingstate2entered[playerTargetyolo]) + ", " + std::to_string(pedmapdyingstate3entered[playerTargetyolo]);
						textpm = "Hostile NPC Count: " + std::to_string(combatcounter);
						textstat = "NPC Status: " + std::to_string(pedmapstat[playerTargetyolo]);
						strcpy(c, text.c_str());
						strcpy(d, torso.c_str());
						strcpy(e, textstat.c_str());
						strcpy(f, legshit.c_str());
						strcpy(g, dyingbool.c_str());
						strcpy(h, dshistory.c_str());
						strcpy(j, textpm.c_str());
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
						strcpy(c, mtext.c_str());
					}
					if (playerTargetyolo != playerPed)
					{
						mtext = "NPC Model: " + std::to_string(ENTITY::GET_ENTITY_MODEL(playerTargetyolo));
						strcpy(c, mtext.c_str());
						DrawText(0.5, 0.15, c);
					}
				}

				//show task ids
				if (ini_showpedtasks == 1)
				{
					strcpy(c, ttext.c_str());
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
						strcpy(c, ttext.c_str());
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
									strcpy(c, ttext.c_str());
									DrawText(0.5, 0.15, c);
								}
							}
						}
					}
				}
				
				
				//bool for checking if current NPC is a story character
				bool isstorychar = false;
				if (ini_excludestorynpcs == 1) isstorychar = isPedStoryChar(peds[i]);

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

				//set health of story characters, if set in the ini
				if (isstorychar && ini_excludestorynpcs == 1)
				{
					if (ini_storypedhealth > 0 && PED::GET_PED_MAX_HEALTH(peds[i]) != ini_storypedhealth)
					{
						PED::SET_PED_MAX_HEALTH(peds[i], ini_storypedhealth);
						ENTITY::SET_ENTITY_HEALTH(peds[i], ini_storypedhealth, 0);
					}
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
				if (ENTITY::IS_ENTITY_DEAD(peds[i]) && pedmapisdead[peds[i]] == 0)
				{
					pedmapisdead[peds[i]] = 1;
					npcdowncounter++;
					if (pedmapisincombatwithplayer[peds[i]])
					{
						pedmapisincombatwithplayer[peds[i]] = false;
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
				if (peds[i] != playerPed && !isstorychar && PED::IS_PED_HUMAN(peds[i]) && !ENTITY::IS_ENTITY_DEAD(peds[i]))
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
						if (pedmapisincombatwithplayer[peds[i]])
						{
							pedmapisincombatwithplayer[peds[i]] = false;
							combatcounter--;
						}
					}
					else
					{
						if (!pedmapisincombatwithplayer[peds[i]])
						{
							pedmapisincombatwithplayer[peds[i]] = true;
							combatcounter++;
							if (combatcounter > 1) pedmapwasnotaloneincombat[peds[i]] = true;
						}
						else if (!pedmapwasnotaloneincombat[peds[i]] && combatcounter > 1) pedmapwasnotaloneincombat[peds[i]] = true;
					}

					//check if NPC is armed
					if (!pedmapisarmed[peds[i]])
					{
						if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(peds[i]))//!WEAPON::xIS_PED_UNARMED(peds[i]))
						{
							pedmapisarmed[peds[i]] = true;
							pedmapwasdisarmed[peds[i]] = false;
						}
					}
										
					//marking NPC if player is shooting at it
					if (PED::IS_PED_SHOOTING(playerPed))
					{
						Entity playerTargeta, playerTargetb;
						if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargeta))
						{
							if (playerTargeta == peds[i]) pedmapshotbyplayer[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
						}
						if (PLAYER::GET_PLAYER_TARGET_ENTITY(player, &playerTargetb))
						{
							if (playerTargetb == peds[i]) pedmapshotbyplayer[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
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
					if (pedmapishanging[peds[i]] && !DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged")) pedmapishanging[peds[i]] = false;
					else if (!pedmapishanging[peds[i]] && DECORATOR::DECOR_EXIST_ON(peds[i], "TYL_hanged"))
					{
						Vector3 vecfoot = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(peds[i], PED::GET_PED_BONE_INDEX(peds[i], 45454));
						float groundzcoordped;
						GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(vecfoot.x, vecfoot.y, vecfoot.z, &groundzcoordped, true);
						if (vecfoot.z > groundzcoordped + 0.2 && peds[i] != PED::_0xD806CD2A4F2C2996(player)) //is not carried by player
						{
							pedmapishanging[peds[i]] == true;
						}
					}					

					//set lasso bools
					if (AI::GET_IS_TASK_ACTIVE(peds[i], 399)) ishogtied = true;
					else ishogtied = false;
					if (AI::GET_IS_TASK_ACTIVE(peds[i], 400) || AI::GET_IS_TASK_ACTIVE(peds[i], 600)) islassoed = true;
					else islassoed = false;

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
					//if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(peds[i], 0, 2)) pedmaphasbeendamagedbyweapon[peds[i]] = true;
										
					map<int, map<int, char*>>::iterator mapit;
					for (auto mapit = allweaponmap.begin(); mapit != allweaponmap.end(); mapit++)
					{
						int weapontype = mapit->first;
						char* weaponname = mapit->second;
						if (WEAPON::HAS_ENTITY_BEEN_DAMAGED_BY_WEAPON(peds[i], GAMEPLAY::GET_HASH_KEY(weaponname), 0)) pedmaphasbeendamagedbyweapon[peds[i]] = true;
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
						|| AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask04) || AI::GET_IS_TASK_ACTIVE(peds[i], ini_bleedingtask05)) && pedmapstat[peds[i]] != 99)
					{
						if (pedmapdyingstatebool[peds[i]] == true)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], 38, 0);
							
							//ENTITY::SET_ENTITY_MAX_HEALTH(peds[i], ini_arteryshothealth);
							//ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], false), 0);
							pedmapstat[peds[i]] = 99;
							pedmapdyingstatebool[peds[i]] = false;
						}
						pedmapnobleeding[peds[i]] = false;
						pedmaparteryshot[peds[i]] = true;
					}

					if (pedmaparteryshot[peds[i]])
					{
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 352) || AI::GET_IS_TASK_ACTIVE(peds[i], 353) || AI::GET_IS_TASK_ACTIVE(peds[i], 354) || AI::GET_IS_TASK_ACTIVE(peds[i], 355))
						{
							pedmaparteryshot[peds[i]] = false;
							pedmapnobleeding[peds[i]] = false;
							//pedmapstat[peds[i]] = 6;
							if (ini_dyingstatechance > 0) pedmapdyingstatebool[peds[i]] = true;
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
					//			BEGIN: SET NPC HEALTH, ID: PDO009
					//
					//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

					//if the NPC does not have the specified health yet, set the health accordingly
					if ((PED::GET_PED_MAX_HEALTH(peds[i]) != ini_pedhealth && PED::GET_PED_MAX_HEALTH(peds[i]) != ini_pedhealthvehicle) && pedmapstat[peds[i]] != 5 && pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 99)
					{
						if (PED::IS_PED_IN_ANY_VEHICLE(peds[i], false))
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_pedhealthvehicle);
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_pedhealthvehicle, 0);
							npchealth = ini_pedhealthvehicle;
						}
						else
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_pedhealth);
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_pedhealth, 0);
							npchealth = ini_pedhealth;
						}
					}

					if (PED::GET_PED_MAX_HEALTH(peds[i]) == ini_pedhealthvehicle)
					{
						if (!PED::IS_PED_IN_ANY_VEHICLE(peds[i], false))
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_pedhealth);
							if (npchealth <= ini_pedhealthvehicle * ini_pedhealthvehiclemodtods)
							{
								ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
								npchealth = ini_dyingmovementthreshold - 1;
							}
							else if (npchealth >= ini_pedhealthvehicle * ini_pedhealthvehiclemodtofull)
							{
								ENTITY::SET_ENTITY_HEALTH(peds[i], ini_pedhealth, 0);
								npchealth = ini_pedhealth;
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
					if ((PED::GET_PED_MAX_HEALTH(peds[i]) == ini_pedhealth || PED::GET_PED_MAX_HEALTH(peds[i]) == ini_pedhealthvehicle) && pedmapstat[peds[i]] != 4 && pedmapstat[peds[i]] != 5)
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
								if (pedmapaccuracy[peds[i]] > ini_pedaccuracyinjuredmax || pedmapaccuracy[peds[i]] < ini_pedaccuracyinjuredmin)
								{
									int randacc = ini_pedaccuracyinjuredmin + (std::rand() % (ini_pedaccuracyinjuredmax - ini_pedaccuracyinjuredmin + 1));
									pedmapaccuracy[peds[i]] = randacc;
								}
							}
							else if (pedmapaccuracy[peds[i]] > ini_pedaccuracyhealthymax || pedmapaccuracy[peds[i]] < ini_pedaccuracyhealthymin)
							{
								int randacc = ini_pedaccuracyhealthymin + (std::rand() % (ini_pedaccuracyhealthymax - ini_pedaccuracyhealthymin + 1));
								pedmapaccuracy[peds[i]] = randacc;
							}
							PED::SET_PED_ACCURACY(peds[i], pedmapaccuracy[peds[i]]);
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
										if (pedmapdyingstate1entered[peds[i]] != 0 && (pedmapdyingstate1entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
										else if (pedmapdyingstate2entered[peds[i]] != 0 && (pedmapdyingstate2entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
										else if (pedmapdyingstate3entered[peds[i]] != 0 && (pedmapdyingstate3entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
									}
									else if (ini_onlybleedshottorso == 1 && ini_torsooverwrite == 1 && pedmaptorsohit[peds[i]] == 1)
									{
										if (pedmapdyingstate1entered[peds[i]] != 0 && (pedmapdyingstate1entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
										else if (pedmapdyingstate2entered[peds[i]] != 0 && (pedmapdyingstate2entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
										else if (pedmapdyingstate3entered[peds[i]] != 0 && (pedmapdyingstate3entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
									}
								}
							}
							else if (ini_onlybleedshottorso == 1 && ini_torsooverwrite == 1)
							{
								if (pedmaptorsohit[peds[i]] == 1)
								{
									if (pedmapdyingstate1entered[peds[i]] != 0 && (pedmapdyingstate1entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
									else if (pedmapdyingstate2entered[peds[i]] != 0 && (pedmapdyingstate2entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
									else if (pedmapdyingstate3entered[peds[i]] != 0 && (pedmapdyingstate3entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
								}
							}
							else
							{
								if (pedmapdyingstate1entered[peds[i]] != 0 && (pedmapdyingstate1entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
								else if (pedmapdyingstate2entered[peds[i]] != 0 && (pedmapdyingstate2entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
								else if (pedmapdyingstate3entered[peds[i]] != 0 && (pedmapdyingstate3entered[peds[i]] + ini_maxdstime) < GAMEPLAY::GET_GAME_TIMER()) pedmapkill[peds[i]] = 1;
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
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 341)) pedmapnobleeding[peds[i]] = true;
						if (PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && npchealth < 10)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
							npchealth = ini_dyingmovementthreshold - 1;
							pedmapnobleeding[peds[i]] = true;
						}

						//make knocked out NPCs not bleed or die from being knocked out
						if (AI::GET_IS_TASK_ACTIVE(peds[i], 11) && !(islassoed || ishogtied))
						{
							pedmapnobleeding[peds[i]] = true;
							ENTITY::SET_ENTITY_HEALTH(peds[i], ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], false), 0);
							pedmapkill[peds[i]] = 0;
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
								if (!submerged)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], 0, 0);
									npchealth = 0;
								}
								pedmapkill[peds[i]] = 0;
							}
							else if (pedmapdown[peds[i]] == 1)
							{
								if (!submerged)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingmovementthreshold - 1, 0);
									npchealth = ini_dyingmovementthreshold - 1;
									PED::SET_PED_TO_RAGDOLL(peds[i], 2000, 2000, 0, false, false, false);
								}
								pedmapdown[peds[i]] = 0;
							}

							if (!pedmapishanging[peds[i]])
							{
								if (pedmaphandsup[peds[i]] != 0)
								{
									if (PED::IS_PED_ON_MOUNT(peds[i]) && pedmapdismount[peds[i]] == 0)
									{
										pedmapdismount[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_dismounthandsupdelta;
										AI::CLEAR_PED_TASKS(peds[i], false, false);
										AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
										AI::_xTASK_DISMOUNT_ANIMAL(peds[i], 0, 0, 0, 0, 0);
										AUDIO::STOP_PED_SPEAKING(peds[i], true);
									}
									else if (PED::IS_PED_IN_ANY_VEHICLE(peds[i], false) && pedmapdismount[peds[i]] == 0)
									{
										pedmapdismount[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_dismounthandsupdelta;
										AI::CLEAR_PED_TASKS(peds[i], false, false);
										AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
										AI::TASK_LEAVE_ANY_VEHICLE(peds[i], 0, 0);
										AUDIO::STOP_PED_SPEAKING(peds[i], true);
									}
									
									if (pedmapdismount[peds[i]] != 0)
									{
										if (!PED::IS_PED_ON_MOUNT(peds[i]) && !PED::IS_PED_IN_ANY_VEHICLE(peds[i], false)) pedmapdismount[peds[i]] = 0;
										else if (GAMEPLAY::GET_GAME_TIMER() > pedmapdismount[peds[i]]) pedmapdismount[peds[i]] = 0;
									}
									else
									{
										if (pedmaphandsup[peds[i]] + ini_handsupdelta < GAMEPLAY::GET_GAME_TIMER() && pedmapfirsttimehandsup[peds[i]] == 0)
										{
											pedmapfirsttimehandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
										}
										
										if (pedmapfirsttimehandsup[peds[i]] != 0 && pedmapfirsttimehandsup[peds[i]] + ini_handsuptime > GAMEPLAY::GET_GAME_TIMER())
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

										if (pedmaphandsup[peds[i]] + ini_handsupdelta + ini_handsuptime < GAMEPLAY::GET_GAME_TIMER())
										{
											AI::CLEAR_PED_TASKS(peds[i], false, false);
											AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
											pedmaphandsup[peds[i]] = 0;
											pedmapfirsttimehandsup[peds[i]] = 0;
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
						if (PED::IS_PED_ON_MOUNT(peds[i])) pedmaplasttimeonmount[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_horsefallingdelta;
						if (pedmapfalltime[peds[i]] != 0 && pedmaplasttimeonmount[peds[i]] > GAMEPLAY::GET_GAME_TIMER())
						{
							pedmapfalltime[peds[i]] = 0;
							pedmapkill[peds[i]] = 0;
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
												pedmaphealth[lastTarget] = ENTITY::GET_ENTITY_HEALTH(lastTarget);
											}
											else if (rand < 10)
											{
												pedmaphandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
												pedmaphealth[lastTarget] = ENTITY::GET_ENTITY_HEALTH(lastTarget);
											}
											else if (rand < 55)
											{
												AI::xTASK_FLEE(lastTarget, playerPed, 4, 0, -1.0f, -1, 0);
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
									pedmaphealth[peds[i]] = npchealth;
								}
								else if (rand < 10)
								{
									pedmaphandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
									pedmaphealth[peds[i]] = npchealth;
								}
								else if (rand < 55)
								{
									AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
								}
								else pedmaphealth[peds[i]] = npchealth; //remember health for later use
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

							if (pedmaphasreacted[peds[i]] > 10 && pedmaphasreacted[peds[i]] < GAMEPLAY::GET_GAME_TIMER() && !(playertargetz == peds[i]))
							{
								AI::CLEAR_PED_TASKS(peds[i], false, false);
								AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
								pedmapisdown[peds[i]] = 9;
							}
							else
							{
								if ((playertargetz == peds[i] || PED::IS_PED_IN_COMBAT(peds[i], playerPed) || PED::IS_PED_IN_COMBAT(playerPed, peds[i]) || PED::CAN_PED_IN_COMBAT_SEE_TARGET(peds[i], playerPed) ||
									PED::CAN_PED_IN_COMBAT_SEE_TARGET(playerPed, peds[i])) && pedmapisdown[peds[i]] == 0 && pedmaphasreacted[peds[i]] == 0)
								{
									int randfear = 0 + (std::rand() % (999 - 0 + 1));
									int randreact = 0 + (std::rand() % (99 - 0 + 1));
									if (npcdowncounter > ini_npcdownfearthreshold&& randfear < ini_npcdownfearchance)
									{
										if (randreact < ini_cowerchancefear)
										{
											AI::TASK_COWER(peds[i], -1, 1, 0);
											pedmaphealth[peds[i]] = npchealth;
											pedmaphasreacted[peds[i]] = 1;
										}
										else if (randreact < ini_cowerchancefear + ini_handsupchancefear)
										{
											pedmaphandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
											pedmaphealth[peds[i]] = npchealth;
											pedmaphasreacted[peds[i]] = 1;
										}										
										else
										{
											AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
											pedmaphasreacted[peds[i]] = 2;
										}
									}
								}
								if (pedmaphasreacted[peds[i]] == 1 && ini_fearfleeultimately == 1)
								{
									int randtime = 4000 + (std::rand() % (15000 - 4000 + 1));
									pedmaphasreacted[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + randtime;
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
						if (PED::GET_PED_LAST_DAMAGE_BONE(peds[i], &actBone) && pedmaphasbeendamagedbyweapon[peds[i]])
						{
							//determining if NPC was hit in the torso
							for (vector<int>::size_type it = 0; it != torsobonesbleeding.size(); it++)
							{
								if (actBone == torsobonesbleeding[it])
								{
									if (pedmapshotbyplayer[peds[i]] != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmapshotbyplayer[peds[i]] + ini_npcbodydamagedelay) pedmaptorsohit[peds[i]] = 1;
								}
							}
							
							//disarming and according behavior
							int randdis = 0 + (std::rand() % (99 - 0 + 1));
							if (randdis < ini_disarmingchance)
							{
								if ((actBone == 11300 || actBone == 16749 || actBone == 16781 || actBone == 54187 || actBone == 22798 || actBone == 46065))// && !WEAPON::xIS_PED_UNARMED(peds[i]))
								{
									//disarming
									if (!WEAPON::xIS_PED_UNARMED(peds[i]))//WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(peds[i]))//!WEAPON::xIS_PED_UNARMED(peds[i]))
									{
										DisarmPed(peds[i], vecweaponhand);
										pedmapwasdisarmed[peds[i]] = true;
									}

									if (pedmapisarmed[peds[i]])
									{
										//behavior randomizer
										int rand = 0 + (std::rand() % (99 - 0 + 1));
										if (rand < ini_disarmcowerchance)
										{
											AI::TASK_COWER(peds[i], -1, 1, 0);
											pedmaphealth[peds[i]] = npchealth;
										}
										else if (rand < ini_disarmcowerchance + ini_disarmhandsupchance)
										{										
											if (ini_combathandsuphostilenpcs == 1)
											{
												if (!pedmapwasnotaloneincombat[peds[i]])
												{
													pedmaphandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
													pedmaphealth[peds[i]] = npchealth;
												}
												else if (pedmapwasnotaloneincombat[peds[i]] && combatcounter <= ini_handsuphostilenpcthreshold)
												{
													pedmaphandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
													pedmaphealth[peds[i]] = npchealth;
												}
												else AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
											}
											else
											{
												pedmaphandsup[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
												pedmaphealth[peds[i]] = npchealth;
											}
										}
										else if (rand < ini_disarmcowerchance + ini_disarmhandsupchance + ini_disarmfleeingchance)
										{
											AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0); 
										}
										else pedmaphealth[peds[i]] = npchealth; //remember health for later use
									}

									pedmapisarmed[peds[i]] = false;				
								}
							}

							//right leg
							if (actBone == 43312 || actBone == 6884 || actBone == 33646)
							{
								if (pedmapstat[peds[i]] == 0) pedmapstat[peds[i]] = 8;
								else if (pedmapstat[peds[i]] == 3) pedmapstat[peds[i]] = 9;
								else if (pedmapstat[peds[i]] == 6) pedmapstat[peds[i]] = 1;
								else if (pedmapstat[peds[i]] == 7) pedmapstat[peds[i]] = 2;
								
								if (pedmapshotbyplayer[peds[i]] != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmapshotbyplayer[peds[i]] + ini_npcbodydamagedelay) pedmaprightleghitcounter[peds[i]]++;
							}
							//left leg
							else if (actBone == 65478 || actBone == 55120 || actBone == 45454)
							{
								if (pedmapstat[peds[i]] == 0) pedmapstat[peds[i]] = 6;
								else if (pedmapstat[peds[i]] == 3) pedmapstat[peds[i]] = 7;
								else if (pedmapstat[peds[i]] == 8) pedmapstat[peds[i]] = 1;
								else if (pedmapstat[peds[i]] == 9) pedmapstat[peds[i]] = 2;
								if (pedmapshotbyplayer[peds[i]] != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmapshotbyplayer[peds[i]] + ini_npcbodydamagedelay) pedmapleftleghitcounter[peds[i]]++;
							}

							//spine paralysis
							if (actBone == 14410 || actBone == 14411 || actBone == 14412 || actBone == 14413 || actBone == 14414 || actBone == 14415)
							{
								int randpar = 0 + (std::rand() % (99 - 0 + 1));
								if (randpar < ini_parachance && ini_spineshotsparalyze == 1)
								{
									if (pedmapshotbyplayer[peds[i]] != 0 && GAMEPLAY::GET_GAME_TIMER() < pedmapshotbyplayer[peds[i]] + ini_npcbodydamagedelay)
									{
										if (pedmapstat[peds[i]] == 0
											|| pedmapstat[peds[i]] == 1
											|| pedmapstat[peds[i]] == 6
											|| pedmapstat[peds[i]] == 8) pedmapstat[peds[i]] = 10;
										else if (pedmapstat[peds[i]] == 2
											|| pedmapstat[peds[i]] == 3
											|| pedmapstat[peds[i]] == 7
											|| pedmapstat[peds[i]] == 10) pedmapstat[peds[i]] = 11;
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
						if (pedmapstat[peds[i]] == 6 || pedmapstat[peds[i]] == 7 || pedmapstat[peds[i]] == 8 || pedmapstat[peds[i]] == 9 || pedmaptorsohit[peds[i]] == 1
							|| pedmapstat[peds[i]] == 1 || pedmapstat[peds[i]] == 2)
						{
							if(pedmapmovement[peds[i]] == "" || pedmapmovement[peds[i]] == "injured_torso")// || pedmapmovement[peds[i]] == "injured_right_arm" || pedmapmovement[peds[i]] == "injured_left_arm" || pedmapmovement[peds[i]] == "injured_torso")
							{
								//int randlimp = 0 + (std::rand() % (99 - 0 + 1));

								if (pedmapstat[peds[i]] == 6 || pedmapstat[peds[i]] == 7) //left leg injured
								{
									pedmapmovement[peds[i]] = "injured_left_leg";
									//if (randlimp < 30) pedmapmovement[peds[i]] = "injured_general";
									//else if (randlimp < 60) pedmapmovement[peds[i]] = "injured_left_leg";
									//else pedmapmovement[peds[i]] = "injured_right_leg";
								}
								else if (pedmapstat[peds[i]] == 8 || pedmapstat[peds[i]] == 9) //right leg injured
								{
									pedmapmovement[peds[i]] = "injured_right_leg";
									//if (randlimp < 30) pedmapmovement[peds[i]] = "injured_general";
									//else if (randlimp < 60) pedmapmovement[peds[i]] = "injured_left_leg";
									//else pedmapmovement[peds[i]] = "injured_right_leg";
								}
								else if (pedmapstat[peds[i]] == 1 || pedmapstat[peds[i]] == 2) //both legs injured
								{
									pedmapmovement[peds[i]] = "injured_general";
									//if (randlimp < 30) pedmapmovement[peds[i]] = "injured_general";
									//else if (randlimp < 60) pedmapmovement[peds[i]] = "injured_left_leg";
									//else pedmapmovement[peds[i]] = "injured_right_leg";
								}
								else if (pedmaptorsohit[peds[i]] == 1)
								{
									pedmapmovement[peds[i]] = "injured_torso";
									//if (randlimp < 30) pedmapmovement[peds[i]] = "injured_right_arm";
									//else if (randlimp < 60) pedmapmovement[peds[i]] = "injured_left_arm";
									//else pedmapmovement[peds[i]] = "injured_torso";
								}
							}
							else
							{
								if (PED::IS_PED_MALE(peds[i])) PED::xSET_STANCE(peds[i], "DEFAULT");
								else PED::xSET_STANCE(peds[i], "DEFAULT_FEMALE");
								PED::xSET_WALKING_STYLE(peds[i], pedmapmovement[peds[i]]);
							}
						}

						/*
						if (pedmapstat[peds[i]] == 6 || pedmapstat[peds[i]] == 7) //left leg injured
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_general");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_leg");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_right_leg");
						}
						else if (pedmapstat[peds[i]] == 8 || pedmapstat[peds[i]] == 9) //right leg injured
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_general");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_leg");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_right_leg");
						}
						else if (pedmapstat[peds[i]] == 1 || pedmapstat[peds[i]] == 2) //both legs injured
						{
							PED::xSET_STANCE(peds[i], "DEFAULT");
							if (randlimp < 30) PED::xSET_WALKING_STYLE(peds[i], "injured_general");
							else if (randlimp < 60) PED::xSET_WALKING_STYLE(peds[i], "injured_left_leg");
							else PED::xSET_WALKING_STYLE(peds[i], "injured_right_leg");
						}
						else if (pedmaptorsohit[peds[i]] == 1)
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
						if (pedmapstat[peds[i]] == 10 || pedmapstat[peds[i]] == 11)
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
								else pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 4, 0, ini_ds2audiochancemod);
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
						if (ENTITY::GET_ENTITY_MAX_HEALTH(peds[i], false) == ini_pedhealth && npchealth > ini_dyingmovementthreshold&& pedmapstat[peds[i]] != 99
							&& pedmapstat[peds[i]] != 10 && pedmapstat[peds[i]] != 11 && ini_nopushingdown == 0)
						{
							if (isstanding)
							{
								if (pedmapleftleghitcounter[peds[i]] >= ini_legshitthreshold && pedmaprightleghitcounter[peds[i]] >= ini_legshitthreshold)
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
									pedmaplegsdown[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_bleedingsave;
								}
								else if (pedmapleftleghitcounter[peds[i]] > 0 && pedmaprightleghitcounter[peds[i]] > 0 && pedmaplegsdamageddowntime[peds[i]] == 0) pedmaplegsdamageddowntime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_damagedlegsdowntime;

								//put NPC down if legs damaged and timer passed
								if (pedmaplegsdamageddowntime[peds[i]] > 0 && pedmaplegsdamageddowntime[peds[i]] < GAMEPLAY::GET_GAME_TIMER())
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
									pedmaplegsdown[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_bleedingsave;
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
						if (pedmapstat[peds[i]] != 99 && pedmapstat[peds[i]] != 10 && pedmapstat[peds[i]] != 11)
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
							
							if ((pedmapstat[peds[i]] == 1 || pedmapstat[peds[i]] == 2) && !PED::IS_PED_RAGDOLL(peds[i]) && !AI::IS_PED_GETTING_UP(peds[i]) && (AI::IS_PED_WALKING(peds[i]) || AI::IS_PED_RUNNING(peds[i]) || AI::IS_PED_SPRINTING(peds[i]) || PED::IS_PED_GOING_INTO_COVER(peds[i])) && !PED::IS_PED_ON_MOUNT(peds[i]))
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
							else if ((pedmapstat[peds[i]] == 6 || pedmapstat[peds[i]] == 7 || pedmapstat[peds[i]] == 8 || pedmapstat[peds[i]] == 9) && (AI::IS_PED_RUNNING(peds[i]) || AI::IS_PED_SPRINTING(peds[i]) || PED::IS_PED_GOING_INTO_COVER(peds[i])) && !PED::IS_PED_RAGDOLL(peds[i]) && !AI::IS_PED_GETTING_UP(peds[i]) && !PED::IS_PED_ON_MOUNT(peds[i]))
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
						if (ini_disarmingchance > 0 && pedmaphandsup[peds[i]] != 0 && pedmaphandsup[peds[i]] + ini_handsupdelta + ini_handsuptime > GAMEPLAY::GET_GAME_TIMER())//pedmapwasdisarmed[peds[i]])
						{
							if (pedmaphealth[peds[i]] != 0)
							{
								if (pedmaphealth[peds[i]] > npchealth + 10)
								{
									pedmaphandsup[peds[i]] = 0;
									AI::CLEAR_PED_TASKS(peds[i], false, false);
									AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
									AI::xTASK_FLEE(peds[i], playerPed, 4, 0, -1.0f, -1, 0);
									pedmaphealth[peds[i]] = 0;
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
										if (playerTarget == peds[i] && pedmaplimbhealth[peds[i]] > npchealth)
										{
											int painaudiorand = 0 + (std::rand() % (99 - 0 + 1));
											if (painaudiorand < 20 && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))	AUDIO::PLAY_PAIN(peds[i], 1, 1.0f, false, false);
										}
									}
								}
							}
						}

						//being aimed at
						if (ishogtied && !isnpconfire && !pedmapishanging[peds[i]])
						{
							if (ini_ds1audio == 1)
							{
								Entity playerTargetped;
								if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetped))
								{
									if (playerTargetped == peds[i] && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
									{
										if (PED::IS_PED_SHOOTING(playerPed)) pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 4, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
										else
										{
											int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
											if (reactionrand < ini_reactiononaim)
											{
												if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
												{
													AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
													if (!pedmapwasaimedat[peds[i]])
													{
														PedFear(peds[i], 3, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
														pedmapwasaimedat[peds[i]] = true;
													}
													else pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 1, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
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
												if (reactionrand2 < 5) pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 5, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
												else PedPanic(peds[i]);
											}
										}
									}

									//health boost if hanging
									if (ini_nooselivelonger == 1)
									{
										if (pedmaphangingtime[peds[i]] == 0) pedmaphangingtime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_noosehealthboostinterval;
										else if (pedmaphangingtime[peds[i]] < GAMEPLAY::GET_GAME_TIMER())
										{
											npchealth = ENTITY::GET_ENTITY_HEALTH(peds[i]);
											if (npchealth + ini_noosehealthboost < ini_pedhealth)
											{
												ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth + ini_noosehealthboost, 0);
												npchealth += ini_noosehealthboost;
												pedmaphangingtime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_noosehealthboostinterval;
											}
										}
									}
								}
								else
								{
									int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
									if (reactionrand < ini_nooseaudiochance && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
									{
										if (!pedmapwasaimedat[peds[i]])
										{
											PedFear(peds[i], 3, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
											pedmapwasaimedat[peds[i]] = true;
										}
										else pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 2, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
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
						if (pedmaplimbhealth[peds[i]] > npchealth && pedmapstat[peds[i]] != 99)
						{
							int damagedone = pedmaplimbhealth[peds[i]] - npchealth;
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
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - dam, 0);
											npchealth -= dam;
										}
										pedmaplimbhealth[peds[i]] = npchealth;
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
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - dam, 0);
											npchealth -= dam;
										}
										pedmaplimbhealth[peds[i]] = npchealth;
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
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - dam, 0);
											npchealth -= dam;
										}
										pedmaplimbhealth[peds[i]] = npchealth;
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
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - dam, 0);
											npchealth -= dam;
										}
										pedmaplimbhealth[peds[i]] = npchealth;
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
											ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - dam, 0);
											npchealth -= dam;
										}
										pedmaplimbhealth[peds[i]] = npchealth;
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
						if (pedmapstat[peds[i]] == 99)
						{
							//making sure NPCs don't stay in shock too long
							if (pedmapheadvectime[peds[i]] == 0)
							{
								pedmapheadvecdelta[peds[i]] = vechead;
								pedmapheadvectime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_headvecmaxtimeb;
							}
							else if (GAMEPLAY::GET_GAME_TIMER() > pedmapheadvectime[peds[i]] && pedmapheadvectime[peds[i]] != -1)
							{
								if (isstanding && ENTITY::GET_ENTITY_SPEED(peds[i]) < maxragdollspeed && ini_nopushingdown == 0)
								{
									if ((vechead.x < pedmapheadvecdelta[peds[i]].x + ini_headvecmaxdeltab
										&& vechead.x > pedmapheadvecdelta[peds[i]].x - ini_headvecmaxdeltab)
										&&
										(vechead.y < pedmapheadvecdelta[peds[i]].y + ini_headvecmaxdeltab
											&& vechead.y > pedmapheadvecdelta[peds[i]].y - ini_headvecmaxdeltab)
										&&
										(vechead.z < pedmapheadvecdelta[peds[i]].z + ini_headvecmaxdeltab
											&& vechead.z > pedmapheadvecdelta[peds[i]].z - ini_headvecmaxdeltab))
									{
										PED::SET_PED_TO_RAGDOLL(peds[i], 3000, 3000, 3, false, false, false);
										pedmapstat[peds[i]] = 0;
									}
									else
									{
										pedmapheadvecdelta[peds[i]] = vechead;
										pedmapheadvectime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_headvecmaxtimeb;
									}
								}
							}
						}

						//if DyingStates should apply to melee damage as well (-> people going into dying states after fist fights), set bool accordingly
						bool xhasbeendamagedbyweapon;
						if (ini_dyingstatesmelee == 1) xhasbeendamagedbyweapon = true;
						else xhasbeendamagedbyweapon = pedmaphasbeendamagedbyweapon[peds[i]];
												
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________
						//
						//
						//			BEGIN: KNOCKING THE WIND OUT OF NPCs, ID: PDO028
						//
						//______________________________________________________________________________________________________________________________________________________________________________________________________________________________

						//if NPC can be knocked down, compute if he is being knocked down now (randomly when hit)
						if (npchealth < ini_painthreshold && npchealth > ini_dyingmovementthreshold
							&& pedmapstat[peds[i]] != 99 && !pedmapwasdisarmed[peds[i]] && !pedmapnobleeding[peds[i]] && pedmapstat[peds[i]] != 10 && pedmapstat[peds[i]] != 11)
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
						if (npchealth < 2 && pedmapdyingstatebool[peds[i]])
						{
							pedmapdyingstatebool[peds[i]] = false;
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

						else if (pedmapdyingstatebool[peds[i]] && !(islassoed || ishogtied) && npchealth <= ini_dyingmovementthreshold && xhasbeendamagedbyweapon
							&& pedmapstat[peds[i]] != 99 && pedmapstat[peds[i]] != 10 && pedmapstat[peds[i]] != 11)
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
								if (pedmaplegsdown[peds[i]] != 0)
								{
									if (GAMEPLAY::GET_GAME_TIMER() < pedmaplegsdown[peds[i]])
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
									pedmapko[peds[i]] = 1;
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
										if (pedmapstat[peds[i]] != 1 && pedmapstat[peds[i]] != 2)
										{
											int survivfir = 0 + (std::rand() % (99 - 0 + 1));
											if (survivfir < ini_survivfirchance) pedmapstat[peds[i]] = 2;
											else pedmapstat[peds[i]] = 1;
										}
									}
									
									if (ini_nopushingdown == 0)
									{
										//make sure NPC is able to be ragdolled
										PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(peds[i], 1);

										if (PED::IS_PED_RAGDOLL(peds[i]))
										{
											PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
											if (pedmapeuph[peds[i]] == 1)
											{
												if (pedmapstumbling[peds[i]] == 0)
												{
													int stumblingtime = ini_stumbletimemin + (std::rand() % (ini_stumbletimemax - ini_stumbletimemin + 1));
													pedmapstumbling[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + stumblingtime;
												}
												else if (pedmapstumbling[peds[i]] > GAMEPLAY::GET_GAME_TIMER())
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
									if ((pedmapdyingstate1entered[peds[i]] != 0 || pedmapdyingstate2entered[peds[i]] != 0 || pedmapdyingstate3entered[peds[i]] != 0) && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 35000, 35000, 3, false, false, false);

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

									if (ini_blipfords == 1 && pedmapblipset[peds[i]] == 0)
									{
										Blip pedblip = RADAR::xBLIP_ADD_FOR_ENTITY(0x19365607, peds[i]);
										RADAR::xSET_BLIP_NAME_FROM_PLAYER_STRING(pedblip, "PDO - NPC in Dying States");
										pedmapblipset[peds[i]] = pedblip;
									}
									
									//disarming
									DisarmPed(peds[i], vecweaponhand);

									//make sure NPC is able to be ragdolled
									PED::_RESET_PED_RAGDOLL_BLOCKING_FLAGS(peds[i], 1);

									//making sure melee knockout victims stay down
									if (pedmapko[peds[i]] != 0 && (pedmapko[peds[i]] == 1 || pedmapko[peds[i]] == 2))
									{
										pedmapko[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + kotime;
									}
									else if (pedmapko[peds[i]] != 0 && GAMEPLAY::GET_GAME_TIMER() > pedmapko[peds[i]] && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false);

									//preventing NPCs from getting up after being downed
									if ((pedmapdyingstate1entered[peds[i]] != 0 || pedmapdyingstate2entered[peds[i]] != 0 || pedmapdyingstate3entered[peds[i]] != 0) && !PED::IS_PED_RAGDOLL(peds[i])) PED::SET_PED_TO_RAGDOLL(peds[i], 35000, 35000, 3, false, false, false);

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
										if (pedmapisdown[peds[i]] == 0)
										{
											pedmapisdown[peds[i]] = 1;
											npcdowncounter++;
										}

										//remembering health for pain reactions
										if (pedmaphealthds[peds[i]] == 0) pedmaphealthds[peds[i]] = npchealth;

										//pain reaction (when being shot)
										if (npchealth < pedmaphealthds[peds[i]])
										{
											if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) PedFear(peds[i], 4, 0, 100);//PedPain(peds[i]);
											pedmaphealthds[peds[i]] = npchealth;
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
											if (ini_ds1audio == 1 && !PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && !pedmapishanging[peds[i]])
											{
												Entity playerTargetped;
												if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetped))
												{
													if (playerTargetped == peds[i] && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
													{
														if (PED::IS_PED_SHOOTING(playerPed)) pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 4, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
														else
														{
															int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
															if (reactionrand < ini_reactiononaim)
															{
																if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
																{
																	AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
																	if (!pedmapwasaimedat[peds[i]])
																	{
																		int pleadorno = 0 + (std::rand() % (9 - 0 + 1));
																		if (pleadorno < 5) PedFear(peds[i], 3, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
																		else PedFear(peds[i], 1, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
																		pedmapwasaimedat[peds[i]] = true;
																	}
																	else pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 1, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
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
												if (playerTarget == peds[i] && pedmaplimbhealth[peds[i]] > (npchealth + ini_bleedingvalue))
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
											if (pedmapanimtrans[peds[i]] == -1) pedmapanimtrans[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_transdelta;
											else if (GAMEPLAY::GET_GAME_TIMER() > pedmapanimtrans[peds[i]])
											{
												//transitioning into writhing animations
												if (pedmapdyingstate1entered[peds[i]] == 0 && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmapdyingstate1entered[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
												}
												else if (pedmapdyingstate1entered[peds[i]] != 0 && pedmapko[peds[i]] == 1 && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmapko[peds[i]] = 2;
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
											if (ini_ds2audio == 1 && !isnpconfire && !PED::IS_PED_IN_MELEE_COMBAT(peds[i]) && !pedmapishanging[peds[i]])
											{
												Entity playerTargetped;
												if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTargetped))
												{
													if (playerTargetped == peds[i] && WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
													{
														if (PED::IS_PED_SHOOTING(playerPed)) pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 4, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
														else
														{
															int reactionrand = 0 + (std::rand() % (9999 - 0 + 1));
															if (reactionrand < ini_reactiononaim)
															{
																if (!AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]))
																{
																	AI::TASK_TURN_PED_TO_FACE_ENTITY(peds[i], playerPed, 4000, 0, 0, 0);
																	if (!pedmapwasaimedat[peds[i]])
																	{
																		int pleadorno = 0 + (std::rand() % (9 - 0 + 1));
																		if (pleadorno < 5) PedFear(peds[i], 3, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
																		else PedFear(peds[i], 2, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
																		pedmapwasaimedat[peds[i]] = true;
																	}
																	else pedmaplastfearaudio[peds[i]] = PedFear(peds[i], 2, pedmaplastfearaudio[peds[i]], ini_ds2audiochancemod);
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
												if (playerTarget == peds[i] && pedmaplimbhealth[peds[i]] > (npchealth + ini_bleedingvalue))
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
											if (pedmapanimtrans[peds[i]] == -1) pedmapanimtrans[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + ini_transdelta;
											else if (GAMEPLAY::GET_GAME_TIMER() > pedmapanimtrans[peds[i]])
											{
												//transitioning into writhing animations
												if (pedmapdyingstate1entered[peds[i]] == 0 && pedmapdyingstate2entered[peds[i]] == 0 && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													int temp = GAMEPLAY::GET_GAME_TIMER();
													pedmapdyingstate1entered[peds[i]] = temp;
													pedmapdyingstate2entered[peds[i]] = temp;
												}
												else if (pedmapdyingstate1entered[peds[i]] != 0 && pedmapdyingstate2entered[peds[i]] != 0 && pedmapko[peds[i]] == 1 && pedmapanimuse[peds[i]] == 1)
												{
													AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmapko[peds[i]] = 2;
												}
												else if (pedmapdyingstate1entered[peds[i]] != 0 && pedmapdyingstate2entered[peds[i]] == 0)
												{
													//AI::CLEAR_PED_TASKS_IMMEDIATELY(peds[i], false, false); //stop whatever the NPC is doing
													AI::CLEAR_PED_TASKS(peds[i], false, false);
													AI::CLEAR_PED_SECONDARY_TASK(peds[i]);
													AI::TASK_STAY_IN_COVER(peds[i]); //tell NPC to stay in cover (just a trick to make them go into writhing animations when ragdolled)
													PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 0, false, false, false); //ragdoll the NPC (forcing it into writhing animation, knowing that they should execute the cover task)
													pedmapdyingstate2entered[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
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
											if (pedmapdyingstate3entered[peds[i]] == 0)
											{
												pedmapdyingstate3entered[peds[i]] = GAMEPLAY::GET_GAME_TIMER();
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

							if (!pedmapnobleeding[peds[i]])
							{
								if (ini_hogtiebleedingstop == 1)
								{
									if (!(islassoed || ishogtied))
									{
										if (ini_onlybleedshottorso == 1)
										{
											if (pedmaptorsohit[peds[i]] == 1)
											{
												if (ini_bleedwhendying == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
														pedmaphealthds[peds[i]] -= ini_bleedingvalue;
													}
												}
												//else pedmaphealthds[peds[i]] = npchealth;
											}
										}
										else
										{
											if (ini_bleedwhendying == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
													pedmaphealthds[peds[i]] -= ini_bleedingvalue;
												}
											}
											//else pedmaphealthds[peds[i]] = npchealth;
										}
									}
								}
								else if (ini_onlybleedshottorso == 1)
								{
									if (pedmaptorsohit[peds[i]] == 1)
									{
										if (ini_bleedwhendying == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
												pedmaphealthds[peds[i]] -= ini_bleedingvalue;
											}
										}
										//else pedmaphealthds[peds[i]] = npchealth;
									}
								}
								else if (ini_bleedwhendying == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
										pedmaphealthds[peds[i]] -= ini_bleedingvalue;
									}
								}
								//else pedmaphealthds[peds[i]] = npchealth;
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

						if (!pedmapnobleeding[peds[i]] && (npchealth >= ini_dyingmovementthreshold))
						{
							if (ini_hogtiebleedingstop == 1)
							{
								if (!(islassoed || ishogtied))
								{
									if (ini_onlybleedshottorso == 1)
									{
										if (pedmaptorsohit[peds[i]] == 1)
										{
											if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
													pedmaphealthds[peds[i]] -= ini_bleedingvalue;
												}
											}
											//else pedmaphealthds[peds[i]] = npchealth;
										}
									}
									else
									{
										if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
												pedmaphealthds[peds[i]] -= ini_bleedingvalue;
											}
										}
										//else pedmaphealthds[peds[i]] = npchealth;
									}
								}
							}
							else if (ini_onlybleedshottorso == 1)
							{
								if (pedmaptorsohit[peds[i]] == 1)
								{
									if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
											pedmaphealthds[peds[i]] -= ini_bleedingvalue;
										}
									}
									//else pedmaphealthds[peds[i]] = npchealth;
								}
							}
							else if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
							{
								int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
								if (bleedrand < ini_bleedingchance_shot)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
									npchealth -= ini_bleedingvalue;
									pedmaphealthds[peds[i]] -= ini_bleedingvalue;
								}
							}
							//else pedmaphealthds[peds[i]] = npchealth;
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
						if (!pedmapnobleeding[peds[i]] && (pedmapstat[peds[i]] == 10 || pedmapstat[peds[i]] == 11))
						{
							if (ini_hogtiebleedingstop == 1)
							{
								if (!(islassoed || ishogtied))
								{
									if (ini_onlybleedshottorso == 1)
									{
										if (pedmaptorsohit[peds[i]] == 1)
										{
											if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
													pedmaphealthds[peds[i]] -= ini_bleedingvalue;
												}
											}
											//else pedmaphealthds[peds[i]] = npchealth;
										}
									}
									else
									{
										if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
												pedmaphealthds[peds[i]] -= ini_bleedingvalue;
											}
										}
										//else pedmaphealthds[peds[i]] = npchealth;
									}
								}
							}
							else if (ini_onlybleedshottorso == 1)
							{
								if (pedmaptorsohit[peds[i]] == 1)
								{
									if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
											pedmaphealthds[peds[i]] -= ini_bleedingvalue;
										}
									}
									//else pedmaphealthds[peds[i]] = npchealth;
								}
							}
							else if (ini_bleedwhenshot == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
							{
								int bleedrand = 0 + (std::rand() % (999 - 0 + 1));
								if (bleedrand < ini_bleedingchance_shot)
								{
									ENTITY::SET_ENTITY_HEALTH(peds[i], npchealth - ini_bleedingvalue, 0);
									npchealth -= ini_bleedingvalue;
									pedmaphealthds[peds[i]] -= ini_bleedingvalue;
								}
							}
							//else pedmaphealthds[peds[i]] = npchealth;
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
						if (isnpconfire && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i]) && GAMEPLAY::GET_GAME_TIMER() > pedmapfireaudiotime[peds[i]])
						{
							pedmapfireaudiosample[peds[i]] = PedFear(peds[i], 5, pedmapfireaudiosample[peds[i]], ini_ds2audiochancemod);
							pedmapfireaudiotime[peds[i]] = GAMEPLAY::GET_GAME_TIMER() + fireaudiointerval;
						}
					}
					
					//if NPC was burning and is not anymore, create according behavior
					if ((pedmapstat[peds[i]] == 4 || pedmapstat[peds[i]] == 5) && !isnpconfire && !pedmapnobleeding[peds[i]])
					{
						if (PED::GET_PED_MAX_HEALTH(peds[i]) != ini_firehealth)
						{
							PED::SET_PED_MAX_HEALTH(peds[i], ini_firehealth);
							ENTITY::SET_ENTITY_HEALTH(peds[i], ini_dyingthreshold - 1, 0);
							npchealth = ini_dyingthreshold - 1;
							//pedmapdyingstate3entered[peds[i]] = 0;
						}
						if (!submerged) AUDIO::STOP_CURRENT_PLAYING_AMBIENT_SPEECH(peds[i], 0);
						if (PED::IS_PED_RAGDOLL(peds[i])) PED::RESET_PED_RAGDOLL_TIMER(peds[i]);
						else PED::SET_PED_TO_RAGDOLL(peds[i], 3500, 3500, 3, false, false, false);
						int painrand = 0 + (std::rand() % (999 - 0 + 1));
						int painaudio = 9;
						if (painrand < 5 && !submerged && !AUDIO::IS_ANY_SPEECH_PLAYING(peds[i])) AUDIO::PLAY_PAIN(peds[i], painaudio, 1.0f, false, false);
						if (ini_bleedwhendying == 1 && pedmaphasbeendamagedbyweapon[peds[i]])
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
						if (pedmapstat[peds[i]] == 1) pedmapstat[peds[i]] = 2;
						else if (pedmapstat[peds[i]] == 0) pedmapstat[peds[i]] = 3;
						else if (pedmapstat[peds[i]] == 6) pedmapstat[peds[i]] = 7;
						else if (pedmapstat[peds[i]] == 8) pedmapstat[peds[i]] = 9;
						else if (pedmapstat[peds[i]] == 10) pedmapstat[peds[i]] = 11;
					}
					else
					{
						if (pedmapstat[peds[i]] == 2) pedmapstat[peds[i]] = 1;
						else if (pedmapstat[peds[i]] == 3) pedmapstat[peds[i]] = 0;
						else if (pedmapstat[peds[i]] == 7) pedmapstat[peds[i]] = 6;
						else if (pedmapstat[peds[i]] == 9) pedmapstat[peds[i]] = 8;
						else if (pedmapstat[peds[i]] == 11) pedmapstat[peds[i]] = 10;
					}
					//if NPC should survive fire, make sure it does, otherwise make sure it doesnt
					if (isnpconfire)
					{
						if (pedmapstat[peds[i]] == 2 || pedmapstat[peds[i]] == 3 || pedmapstat[peds[i]] == 4 || pedmapstat[peds[i]] == 7 || pedmapstat[peds[i]] == 9 || pedmapstat[peds[i]] == 11)
						{
							ENTITY::SET_ENTITY_HEALTH(peds[i], 800, 0);
							npchealth = 800;
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
									npchealth = ini_firehealth;
								}
								pedmapstat[peds[i]] = 5;
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
					if (pedmaphealth[peds[i]] != 0) pedmaphealth[peds[i]] = npchealth;
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