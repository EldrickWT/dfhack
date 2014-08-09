// Wagonshot is based on code from "probe.cpp" by peterix.
// for 34.07
#include <iostream>
#include <iomanip>
#include <climits>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdio>
using namespace std;

#include "Core.h"
#include "Console.h"
#include "Export.h"
#include "PluginManager.h"
#include "modules/Units.h"
#include "modules/Maps.h"
#include "modules/Gui.h"
#include "modules/Materials.h"
#include "modules/MapCache.h"
#include "MiscUtils.h"

#include "df/world.h"


using std::vector;
using std::string;
using namespace DFHack;
using namespace df::enums;
using df::global::world;

DFHACK_PLUGIN("wagonshot");

command_result wagonshot (color_ostream &out, std::vector <std::string> & parameters);

// Mandatory init function. If you have some global state, create it here.
DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	// Fill the command list with your commands.
	commands.clear();
	commands.push_back(PluginCommand(
		"wagonshot",
		"End the target violently.",
		wagonshot, false,
		"  In a perfect world you wouldn't need to kill critters bursting into your fortress. But this isn't a perfect world. So drop a wagon on them.\n"
		));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result wagonshot (color_ostream &out, vector <string> & parameters)
{
	CoreSuspender suspend;
	int32_t cursorX, cursorY, cursorZ;
	Gui::getCursorCoords(cursorX,cursorY,cursorZ);
	if(cursorX == -30000)
	{
		out.printerr("No cursor; place cursor over creature to murder.\n");
	}
	else
	{
		for(size_t i = 0; i < world->units.all.size(); i++)
		{
			df::unit * unit = world->units.all[i];
			if(unit->pos.x == cursorX && unit->pos.y == cursorY && unit->pos.z == cursorZ)
			{
				if (parameters.size() == 1 && (parameters[0] == "--kill" || parameters[0] == "-k"))
				{
					unit->flags3.bits.scuttle = 1;
					//out.print("Boom! Wagonshot!\n");
					continue;
				}
				else
				{
					out.print("'wagonshot --kill' short and sweet\n(or 'wagonshot -k' even shorter). No condiments needed. That's what the wagon provides.\nNow show me where we drop the wagon.\n");
					break;
				}
			}
		}
	}
	return CR_OK;
}

