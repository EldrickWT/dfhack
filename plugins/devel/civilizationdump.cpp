// Civilizationdump - SUPPOSED to dump all the civilizations
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



command_result civilizationdump (color_ostream &out, std::vector <std::string> & parameters);
command_result cprobe2 (color_ostream &out, vector <string> & parameters);

DFHACK_PLUGIN("civilizationdump");

// Mandatory init function. If you have some global state, create it here.
DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	// Fill the command list with your commands.
	commands.push_back(PluginCommand(
		"civilizationdump",
		"Dumps all current civilizations and religions.",
		civilizationdump));
	commands.push_back(PluginCommand(
		"cprobe2",
		"A creature probe",
		cprobe2));

	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result civilizationdump (color_ostream &out, std::vector <std::string> & parameters)
{
	CoreSuspender suspend;
	for(size_t i = 0; i < world->entities.all.size(); i++)
	{
		df::historical_entity *ent = world->entities.all[i];
		//if (df::historical_entity::find(i)->name.first_name.empty())
		//	continue;
		switch (ent->type){
		case 0:
			{
				out.print("%s is a Civilization(0)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 1:
			{
				out.print("%s is an Embark Group(1)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 2:
			{
				out.print("%s is a Vessel Crew(2)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 3:
			{
				out.print("%s is a Survivor/Migrating Group(3)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 4:
			{
				out.print("%s is a Nomadic Group(4)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 5:
			{
				out.print("%s is a Religion(5)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 6:
			{
				out.print("%s is a Military Group(6)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		case 7:
			{
				out.print("%s is an Outcast Group(7)... that's %s in English\n", Translation::TranslateName(&df::historical_entity::find(i)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(i)->name, true).c_str());
				continue;
			}
		default:
			continue;
		}
	}
	return CR_OK;
}
command_result cprobe2 (color_ostream &out, vector <string> & parameters)
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
				out.print("Creature %d, race %d (%x), civ %d (%x)\n", unit->id, unit->race, unit->race, unit->civ_id, unit->civ_id);
				if (!unit->name.first_name.empty())
					out.print(" %s is their name. %s is their English name.\n", Translation::TranslateName(&unit->name, false).c_str(), Translation::TranslateName(&unit->name, true).c_str());
				if (!unit->name.nickname.empty())
					out.print(" %s is their nickname.\n", unit->name.nickname.c_str());
				if (df::historical_entity::find(unit->civ_id))
					out.print(" %s is their Civilization. The translation for that is %s.\n", Translation::TranslateName(&df::historical_entity::find(unit->civ_id)->name, false).c_str(), Translation::TranslateName(&df::historical_entity::find(unit->civ_id)->name, true).c_str());
				break;
			}
		}
	}
	return CR_OK;
}
