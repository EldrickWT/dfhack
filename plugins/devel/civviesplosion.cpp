// Civvie( Ex)plosion based on catsplosion
// By Zhentar , Further modified by dark_rabite, peterix, belal
// This work of evil makes Civilians (aka Dwarves) pregnant
// and due within 2 in-game hours...
// for 34.11

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
#include "MiscUtils.h"


#include <set>

#include "df/world.h"

using std::set;
using std::vector;
using std::string;
using namespace DFHack;
using namespace df::enums;
using df::global::world;
using df::global::ui;

command_result civviesplosion2 (color_ostream &out, std::vector <std::string> & parameters);

DFHACK_PLUGIN("civviesplosion2");

// Mandatory init function. If you have some global state, create it here.
DFhackCExport command_result plugin_init (color_ostream &out, std::vector <PluginCommand> &commands)
{
	// Fill the command list with your commands.
	commands.push_back(PluginCommand("civviesplosion2",
		"Begins Malicious Breeding Program.",
		civviesplosion2));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result civviesplosion2 (color_ostream &out, std::vector <std::string> & parameters)
{
	CoreSuspender suspend;
	int32_t cursorX, cursorY, cursorZ;
	Gui::getCursorCoords(cursorX,cursorY,cursorZ);
	std::basic_string<char, std::char_traits<char>, std::allocator<char>> targetted;
	bool skipped;
	skipped=false;
	if(cursorX == -30000)
	{
		out.printerr("No cursor; place cursor over creature to convince it to engage with someone.\n");
	}
	else
	{
		out.print("Cursor XYZ is %d %d %d\n", cursorX, cursorY, cursorZ); //I hate debugging lines.
		for(size_t i = 0; i < world->units.active.size(); i++)
		{
			df::unit * unit = world->units.active[i];
			if(unit->pos.x == cursorX && unit->pos.y == cursorY && unit->pos.z == cursorZ)
			{
				if(unit->civ_id == ui->civ_id)
				{
					if ((!unit->flags1.bits.dead) && (!unit->flags3.bits.ghostly) && (!unit->flags3.bits.scuttle) && (!unit->flags2.bits.killed) && (!unit->flags2.bits.for_trade))
					{
						targetted = world->raws.creatures.all[Units::GetCreature(i)->race]->creature_id.c_str();
						out.print("Targetting: %s at %d %d %d\n",targetted.c_str(), cursorX, cursorY, cursorZ);
						out.print("which is hopefully the same as: %d %d %d\n", unit->pos.x, unit->pos.y, unit->pos.z);
						out.print("which is hopefully the same as: %d %d %d\n", df::global::cursor->x, df::global::cursor->y, df::global::cursor->z);
						skipped=false;
						continue;
					}
					else
					{
						out.print("Given target (%s) unusable. Pick a different unit.\n", world->raws.creatures.all[Units::GetCreature(i)->race]->creature_id.c_str());
						skipped=true;
						continue;
					}
				}
				else
				{
					out.print("Not aligned with your fortress. Skipping.\n");
					skipped=true;
					continue;
				}
			}
		}
		if (!skipped)
			out.print("All %s should be engaging soon.\n", targetted.c_str());
		else
			out.print("Given target unusable for whatever reason. Pick a different unit. Printing active creature list.\n");
		list<string> s_creatures;
		// only cursor target.
		s_creatures.push_back(targetted);
		s_creatures.unique();
		uint32_t numCreatures;
		if(!(numCreatures = Units::getNumCreatures()))
		{
			cerr << "Can't get any creatures." << endl;
			return CR_FAILURE;
		}

		int totalcount=0;
		int totalchanged=0;
		int totalcreated=0;
		string sextype;

		// shows all the creatures and returns.

		int maxlength = 0;
		map<string, vector <df::unit *> > male_counts;
		map<string, vector <df::unit *> > female_counts;

		// classify
		for(uint32_t i =0;i < numCreatures;i++)
		{
			df::unit * creature = Units::GetCreature(i);
			df::creature_raw *raw = df::global::world->raws.creatures.all[creature->race];
			if(creature->sex == 0) // female
			{
				female_counts[raw->creature_id].push_back(creature);
				male_counts[raw->creature_id].size();
			}
			else // male, other, etc.
			{
				male_counts[raw->creature_id].push_back(creature);
				female_counts[raw->creature_id].size(); //auto initialize the females as well
			}
		}

		// print (optional)
		//if (showcreatures == 1)
		{
			out.print("Type                 Male # Female #\n");
			for(auto it1 = male_counts.begin();it1!=male_counts.end();it1++)
			{
				out.print("%20s %6d %8d\n", it1->first.c_str(), it1->second.size(), female_counts[it1->first].size());
			}
		}


		// process
		for (list<string>::iterator it = s_creatures.begin(); it != s_creatures.end(); ++it)
		{
			std::string clinput = *it;
			std::transform(clinput.begin(), clinput.end(), clinput.begin(), ::toupper);
			vector <df::unit *> &females = female_counts[clinput];
			uint32_t sz_fem = females.size();
			totalcount += sz_fem;
			for(uint32_t i = 0; i < sz_fem; i++)// max 1 pregnancy
			{
				df::unit * female = females[i];
				// accelerate
				if(female->relations.pregnancy_timer != 0)
				{
					female->relations.pregnancy_timer = rand() % 100 + 1;
					totalchanged++;
				}
				else if(!female->relations.pregnancy_genes)
				{
					df::unit_genes *preg = new df::unit_genes;
					preg->appearance = female->appearance.genes.appearance;
					preg->colors = female->appearance.genes.colors;
					female->relations.pregnancy_genes = preg;
					female->relations.pregnancy_timer = rand() % 100 + 1;
					female->relations.pregnancy_caste = 1; // WTF is this?
					totalcreated ++;
				}
			}
		}
		if(totalchanged)
			out.print("%d pregnancies accelerated.\n", totalchanged);
		if(totalcreated)
			out.print("%d pregnancies created.\n", totalcreated);
		out.print("Total creatures checked: %d\n", totalcount);
	}
	return CR_OK;
}
