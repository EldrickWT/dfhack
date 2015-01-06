// Fix Happiness - Dwarven Brainwashing. Based in part on cprobe by peterix
// for 34.07

#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <climits>
#include <stdlib.h> // for rand()
#include <algorithm> // for std::transform
#include <vector>
#include <list>
#include <iterator>
using namespace std;

#include "DFHack.h"
#include "Core.h"
#include "Console.h"
#include "Export.h"
#include "PluginManager.h"
#include "DataDefs.h"
#include <df/caste_raw.h>
#include <df/creature_raw.h>
#include <df/unit.h>
#include <df/unit_soul.h>

#include "df/world.h"
#include "df/historical_entity.h"
#include "df/entity_raw.h"
#include "df/entity_position.h"
#include "df/entity_position_responsibility.h"
#include "df/entity_position_assignment.h"

#include <set>

using std::set;
using std::vector;
using std::string;
using namespace DFHack;
using namespace df::enums;
using df::global::world;
using df::global::ui;



command_result fixhappiness (color_ostream &out, vector <string> & parameters);

DFHACK_PLUGIN("fixhappiness");

DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	commands.push_back(PluginCommand(
		"fixhappiness",
		"For fixing the happiness of the discontented simply target with cursor and invoke",
		fixhappiness, false,
		"\n"
		"Place the Cursor on a distraught unit. Invoke command. They are now Happiness 100 and have forgotten why they were upset.\n"
		"Note: Avoid use on happy units as it will blast their pleasant thoughts away.\n"
		));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result fixhappiness (color_ostream &out, vector <string> & parameters)
{
	CoreSuspender suspend;
	int32_t cursorX, cursorY, cursorZ;
	Gui::getCursorCoords(cursorX,cursorY,cursorZ);
	if(cursorX == -30000)
	{
		out.printerr("No cursor; place cursor over creature to probe.\n");
	}
	else
	{
		for(size_t i = 0; i < world->units.all.size(); i++)
		{
			df::unit * unit = world->units.all[i];
			if(unit->pos.x == cursorX && unit->pos.y == cursorY && unit->pos.z == cursorZ)
			{
				if(unit->status.current_soul->personality.stress_level >= 101)
					continue;
				out.print("fixing happiness... for unit id:%d aka %s\n", unit->id, Translation::TranslateName(&unit->name, false).c_str());
//				for(auto iter = unit->status.current_soul->personality.emotions.begin(); ; iter != unit->status.current_soul->personality.emotions.end(); iter++)
//				{ //0-129, 212-216, 220-221 are bad mmmkay... aka if unit->status.recent_events[*].type = 0-129, 212-216, 220-221 then delete the bugger.
//					delete *iter;
//				}
				unit->status.current_soul->personality.stress_level = 10;
				continue;
			}
		}
	}
	return CR_OK;
}
