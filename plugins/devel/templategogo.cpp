// templategogo.cpp for list based targetting of things on the map
// for 34.07

#include <iostream>
#include <iomanip>
#include <climits>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdio>
#include <set>
using namespace std;

#include "Core.h"
#include "Console.h"
#include "Export.h"
#include "PluginManager.h"
#include "modules/Units.h"
#include "modules/Gui.h"
#include "modules/Translation.h"
#include "DataDefs.h"
#include <df/caste_raw.h>
#include "df/creature_raw.h"
#include "MiscUtils.h"

#include "df/world.h"


using std::vector;
using std::string;
using std::set;
using namespace DFHack;
using namespace df::enums;
using df::global::world;
using df::global::ui;

DFHACK_PLUGIN("templategogo");

command_result templategogo (color_ostream &out, vector <string> & parameters);

DFhackCExport command_result plugin_init (color_ostream &out, vector <PluginCommand> &commands)
{
	commands.push_back(PluginCommand("templategogo",
		"Do something to a list of targets. Has options (!!CAN BE DANGEROUS!!)",
		templategogo,
		false,
"  \n"
"  \n"
"Options:\n"
"templategogo (--ind --verb --skip --all NAME (NAME))\n"
"  --ind - Target indiscriminately... allows targetting\n          pet races... and your civilians.(!!CAN BE DANGEROUS!!)\n"
"  --verb - Increased Verbosity.\n"
"  --skip - Skip the complete list of targets when there are no targets.\n"
"  --all - Target EVERYONE in sight. WARNING Ignores Indiscriminate Setting.\n"
"  NAME - The Target(s). Any creature ID(s) separated by spaces.\n          Creature IDs can be found in the Raws.\n          Generated Creatures (DEMON_66) require a tad more work to target.\n"
"If no targets are given, all valid targets will be listed.\n"
"  \n"
"  \n"
"Some Examples: Let's pick on BADGERs\n"
"\n  templategogo BADGER :\n          This will Dryrun targetting all the BADGER on the map except those\n          who are in your civilization.\n"
"\n  templategogo -a :\n          This will Dryrun a target list of everyone on the map. EVERYONE.\n"
"  \n"
"  \n"
"Some BAD Examples: Let's pick on RAVENs and DOGs\n"
"\n  templategogo RAVENS :\n          This is a typo so it'll spam you with the entire creature list.\n"
"\n  templategogo DOG :\n          This would give you a Dryrun listing all the DOGs but you 'forgot' -i\n          so it COULD assume there are no targets and spam you with the entire creature list\n          if say the Elves didn't bring any.\n"
"\n  templategogo RAVEN -a :\n          Doesn't just list all the RAVENs - it lists everyone on the map.\n          Infact it does not care you typed in RAVEN.\n          The option --all does NOT maintain a list of names.\n"
		));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result templategogo (color_ostream &out, vector <string> & parameters)
{
	set<int> targetIDs;
	targetIDs.clear();
	set<string> targetNames;
	targetNames.clear();
	bool kill = false;
	bool indis = false;
	bool all = false;
	bool verbose = false;
	bool skip = false;
	//bool exclude = false;
	std::basic_string<char, std::char_traits<char>, std::allocator<char>> bob;
	bob.clear();
	int count = 0;
	for (size_t h = 0; h < parameters.size(); h++)
	{
		if(parameters[h] == "help" || parameters[h] == "?" || parameters[h] == "-?" || parameters[h] == "--?" || parameters[h] == "--help" || parameters[h] == "-help" || parameters[h] == "-h" || parameters[h] == "--h")
			return CR_WRONG_USAGE;
		else if(parameters[h] == "--ind" || parameters[h] == "-ind" || parameters[h] == "--i" || parameters[h] == "-i")
		{
			indis = true;
			continue;
		}
		else if(parameters[h] == "--verb" || parameters[h] == "-verb" || parameters[h] == "--v" || parameters[h] == "-v")
		{
			verbose = true;
			continue;
		}
		else if(parameters[h] == "--skip" || parameters[h] == "-skip" || parameters[h] == "--s" || parameters[h] == "-s")
		{
			skip = true;
			continue;
		}
		else if(parameters[h] == "--all" || parameters[h] == "-all" || parameters[h] == "--a" || parameters[h] == "-a")
		{
			all = true;
			for (size_t j = 0; j < world->units.all.size(); j++)
			{
				targetIDs.insert(world->units.all[j]->id);
			}
			out.print("Targetting everything on the current map.\n");
			continue;
		}
		else
		{
			targetNames.insert(parameters[h]);
			continue;
		}
	}

	CoreSuspender suspend;

	if(all)
		targetNames.clear(); //No sense leaving them full of things that'll just get ignored.

	for (size_t j = 0; j < world->raws.creatures.all.size(); j++)
	{
		//Mandatory to avoid the Crash of the Toad. Access Violations suck. Actually... blame the magic
		if (targetNames.empty())
			break;
		if(world->raws.creatures.all[Units::GetCreature(j)->race]->flags.is_set(df::creature_raw_flags::VERMIN_EATER))
			continue;
		if(world->raws.creatures.all[Units::GetCreature(j)->race]->flags.is_set(df::creature_raw_flags::VERMIN_GROUNDER))
			continue;
		if(world->raws.creatures.all[Units::GetCreature(j)->race]->flags.is_set(df::creature_raw_flags::VERMIN_ROTTER))
			continue;
		if(world->raws.creatures.all[Units::GetCreature(j)->race]->flags.is_set(df::creature_raw_flags::VERMIN_SOIL))
			continue;
		if(world->raws.creatures.all[Units::GetCreature(j)->race]->flags.is_set(df::creature_raw_flags::VERMIN_SOIL_COLONY))
			continue;
		if(world->raws.creatures.all[Units::GetCreature(j)->race]->flags.is_set(df::creature_raw_flags::VERMIN_FISH))
			continue;
		if(verbose)
			out.print("%d ... %s ... of %d ... \n", j+1, world->raws.creatures.all[Units::GetCreature(j)->race]->creature_id.c_str(), Units::getNumCreatures());
		bob = world->raws.creatures.all[Units::GetCreature(j)->race]->creature_id.c_str();
		if (targetNames.size() > 0)
		{
			for (set<string>::const_iterator it0 = targetNames.begin(); it0 != targetNames.end(); it0++)
			{
				if (strcmp(it0->c_str(), bob.c_str()) != 0)
				{
					continue;
				}
				else if (strcmp(it0->c_str(), bob.c_str()) == 0)
				{
					if (indis)
					{
						targetIDs.insert(world->units.all[j]->id);
					}
					else
					{
						if(world->units.all[j]->civ_id < 0) //Many critters do not have anything here. So zap it anyway. Your civ is "Wilderness Population Source".
						{
							targetIDs.insert(world->units.all[j]->id);
							continue;
						}
						if(ui->civ_id < 0) //Should be Impossible. Your civ is "Wilderness Population Source" or something.
						{
							targetIDs.insert(world->units.all[j]->id);
							continue;
						}
						if (world->units.all[j]->civ_id == ui->civ_id)
							targetNames.erase(it0);
						else
						{
							//if(verbose)
						}
						//if(verbose)
					}
				}
				else
				{
					//if(verbose)
				}
			}
			if (j >= Units::getNumCreatures()-1) //magic leaks depending on what you set @.@ //On *2* separate maps this is VITAL to avoid access violations/segfaults.
			{
				//if(verbose)
				//	out.print("J is Greater than or Equal to getNumCreatures (%d), and the number of targetNames remaining is (%d)\n The targetIDs count is (%d).\n", j+1, targetNames.size(), targetIDs.size());
				break;
			}
		}
	}
	//if (targetNames.size() > 0 && !skip) //should be empty by now... unless something is misspelled.
	//{
	//	out.print("There are bad entries! You specified:\n");
	//	for (set<string>::const_iterator it1 = targetNames.begin(); it1 != targetNames.end(); it1++)
	//	{
	//		//auto ellen = it1->c_str();
	//		out.print("%s", it1->c_str());
	//		out.print("\n");
	//		//targetNames.erase(it1);
	//	}
	//	return CR_FAILURE;
	//}
	if (targetIDs.size() == 0)
	{
		out.print("\nNo targets... so... let me show you some targets you COULD have selected. Grab a throne:\n");
		for (size_t k = 0; k < world->raws.creatures.all.size(); k++)
		{
			if(skip)
			{
				out.print("\n'Skip it?' Fine. I hate reading anyway.\n");
				break;
			}
			df::creature_raw *creature = world->raws.creatures.all[k];
			out.print("%6d) %s - %s\n", k+1, creature->creature_id.c_str(), creature->name[0].c_str());
		}
		return CR_OK;
	}
	count = 0;
	for(size_t l = 0; l < world->units.all.size(); l++)
	{
		df::unit * unit = world->units.all[l];
		if (kill)
		{
			for (set<int>::const_iterator it2 = targetIDs.begin(); it2 != targetIDs.end(); it2++)
			{
				if (unit->id == *it2)
				{
					unit->flags3.bits.scuttle = 1;
					//if(verbose)
					//	out.print("Target %d is a %s. Wagoned.\n", l, world->raws.creatures.all[Units::GetCreature(l)->race]->creature_id.c_str());
					count++;
					//targetIDs.erase(it2);
				}
				continue;
			}
		}
		else if (!kill)
		{
			for (set<int>::const_iterator it3 = targetIDs.begin(); it3 != targetIDs.end(); it3++)
			{
				out.print("Dryrun results:");
				if (targetIDs.empty())
				{
					out.print("Wait... nothing to list.\n");
					break;
				}
				out.print("%6d", *it3);
				out.print("\n");
				targetIDs.erase(it3);
			}
		}
	}
	if (count)
		out.print("There were %d targets impacted.\n", count);
	//if(verbose)
	//	out.print("Names left %6d.\nIDs left %6d.\n", count, targetNames.size(), targetIDs.size());
	return CR_OK;
}