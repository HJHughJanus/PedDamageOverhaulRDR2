#include "script.h"
#include "keyboard.h"

#include <string>
#include <ctime>

using namespace std;

void DrawText(float x, float y, char* str)
{
	UI::DRAW_TEXT(GAMEPLAY::CREATE_STRING(10, "LITERAL_STRING", str), x, y);
}

void main()
{
	bool modScriptState{ true };
	constexpr int toggleKey{ VK_F8 };
	int msgTime{ 0 };
	char c[40];
	int act = 99999;
	std::string text = "Last Damaged Bone = ";

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

			if (WEAPON::IS_PED_WEAPON_READY_TO_SHOOT(playerPed))
			{
				strcpy(c, text.c_str());
				DrawText(0.4, 0.4, c);
			}
			else
			{
				if (PED::IS_PED_SHOOTING(playerPed))
				{
					Entity playerTarget;
					if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(player, &playerTarget))
					{
						int actBone;
						if (PED::GET_PED_LAST_DAMAGE_BONE(playerTarget, &actBone))
						{
							act = actBone;
							text = "Last Damaged Bone = " + std::to_string(act);
							strcpy(c, text.c_str());
							DrawText(0.4, 0.4, c);
						}
					}
				}
			}
		}
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}