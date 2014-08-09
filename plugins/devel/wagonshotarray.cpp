//wagonshotarray is based on wagonshot.
//wagonshot is based on code from "probe.cpp" by peterix.
// for 34.07
#include <algorithm> // for std::transform
#include <assert.h>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <stdlib.h> // for rand()
#include <string>
#include <vector>
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

#include "DFHack.h"
#include "DataDefs.h"
#include <df/caste_raw.h>
#include <df/creature_raw.h>
#include <set>

#include "df/world.h"

using std::set;
using std::vector;
using std::string;
using namespace DFHack;
using namespace df::enums;
using df::global::world;
using df::global::ui;

DFHACK_PLUGIN("wagonshotarray");

command_result wagonshotarray (color_ostream &out, std::vector <std::string> & parameters);

// Mandatory init function. If you have some global state, create it here.
DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	// Fill the command list with your commands.
	commands.clear();
	commands.push_back(PluginCommand(
		"wagonshotarray",
		"End the targets violently.",
		wagonshotarray, false,
		"  In a perfect world you wouldn't need to kill critters by the wagonload that are bursting into your fortress.\nBut this isn't a perfect world. So drop some wagons on them.\n\n'wagonshotarray' takes no arguments. Just cursor over your target, and the wagons will do the rest.\n"
		));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result wagonshotarray (color_ostream &out, vector <string> & parameters)
{
	CoreSuspender suspend;
	int32_t cursorX, cursorY, cursorZ;
	Gui::getCursorCoords(cursorX,cursorY,cursorZ);
	uint32_t totalcount = 0; //Scuttle Counter.
	std::basic_string<char, std::char_traits<char>, std::allocator<char>> targetted, istarget;
	set<int> targetIDs;
	if (!parameters.empty()) //Takes no parameters. 'wagonshotarray' is a mouthful as it is.
		return CR_WRONG_USAGE;
	if(cursorX == -30000)
	{
		out.printerr("No cursor; place cursor over creature to murder it and its buds.\n");
	}
	else
	{
		for(size_t i = 0; i < world->units.all.size(); i++)
		{
			df::unit * unit = world->units.all[i];
			if(unit->pos.x == cursorX && unit->pos.y == cursorY && unit->pos.z == cursorZ && unit->civ_id != ui->civ_id) //make sure they are NOT 'friendlies'
			{
				targetted = world->raws.creatures.all[Units::GetCreature(i)->race]->creature_id.c_str();
				//out.print("Targetting: %s\n",targetted.c_str()); //Now to just apply flags3.bits.scuttle = 1; to every id that is a 'targetted' and totalcount++ the shebang.
				for(size_t j = 0; j < world->units.all.size(); j++)
				{
					df::unit * unit2 = world->units.all[j];
					istarget = world->raws.creatures.all[Units::GetCreature(j)->race]->creature_id.c_str();
					if(unit2->civ_id != ui->civ_id && istarget == targetted && (unit2->flags2.bits.killed != 1 || unit2->flags1.bits.dead != 1))
					{
						unit2->flags3.bits.scuttle = 1;
						totalcount++;
						continue;
					}
				}
				continue;
			}
			if (unit->pos.x == cursorX && unit->pos.y == cursorY && unit->pos.z == cursorZ && unit->civ_id == ui->civ_id) //you've targetted a friendly.
			{
				out.print("I know them! I'm not dropping a wagon on any one of them even if they kill YOU.\n");
				continue;
			}
		}
		//out.print("Boom! Wagonshot! Every single %s we could see has a wagon on'em.\nBy my count that was %d in total.\n", targetted.c_str(), totalcount);
	}
	return CR_OK;
}