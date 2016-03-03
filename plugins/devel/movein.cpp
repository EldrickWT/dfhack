// movein is based on code from "probe.cpp" by peterix. tries to make the target under cursor move in (prealpha)
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
#include "df/historical_entity.h"
#include "df/historical_figure.h"
#include "df/ui.h"
#include "df/world.h"


using std::vector;
using std::string;
using namespace DFHack;
using namespace df::enums;
using df::global::world;
using df::global::ui;

DFHACK_PLUGIN("movein");

command_result movein (color_ostream &out, std::vector <std::string> & parameters);

// Mandatory init function. If you have some global state, create it here.
DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	// Fill the command list with your commands.
	commands.clear();
	commands.push_back(PluginCommand(
		"movein",
		"Make the target join your fortress.",
		movein));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result movein (color_ostream &out, vector <string> & parameters)
{
	CoreSuspender suspend;
	int32_t cursorX, cursorY, cursorZ;
	Gui::getCursorCoords(cursorX,cursorY,cursorZ);
	if(cursorX == -30000)
	{
		out.printerr("No cursor; place cursor over creature to murder.\n");
	}
	else if (!parameters.empty())
	{
		return CR_WRONG_USAGE;
	}
	else
	{
		for(size_t i = 0; i < world->units.all.size(); i++)
		{
			df::unit * unit = world->units.all[i];
			if(unit->pos.x == cursorX && unit->pos.y == cursorY && unit->pos.z == cursorZ)
			{
				unit->flags1.bits.diplomat = 0;
				unit->flags2.bits.resident = 0;
				unit->flags1.bits.merchant = 0;
				unit->flags1.bits.forest = 0;
				unit->civ_id = df::global::ui->civ_id;
				//add them to fortress_entity unit_ids and that should avoid them becoming pets
				//add them to fortress_entity hist_figures and that should avoid them becoming pets
				out.print("The target should movein!\n");
				continue;
			}
		}
	}
	return CR_OK;
}

