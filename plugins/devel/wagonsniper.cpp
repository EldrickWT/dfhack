// Wagonsniper.cpp for when they are skulking about, you know they are there, and they need to go to the !!circus!!
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

DFHACK_PLUGIN("wagonsniper");

command_result wagonsniper (color_ostream &out, vector <string> & parameters);

DFhackCExport command_result plugin_init (color_ostream &out, vector <PluginCommand> &commands)
{
	commands.push_back(PluginCommand("wagonsniper",
		"Call out a target and the wagonsniper takes them out without any hand holding. (!!CAN BE DANGEROUS!!)",
		wagonsniper,
		false,
"  \n"
"  \n"
"Options:\n"
"  --kill - Tell the sniper to actually take the shot.\n"
"  --ind - Tell the sniper to target indiscriminately... allows targetting\n          pet races... and your civilians.(!!CAN BE DANGEROUS!!)\n"
"  --verb - Increased Verbosity.\n"
"  --skip - Skip the complete list of targets when there are no targets.\n"
"  --all - Target EVERYONE in sight. WARNING Ignores Indiscriminate Setting. Wagongeddon.\n"
"  NAME - The Target(s). Any creature ID(s) separated by spaces.\n          Creature IDs can be found in the Raws.\n          Generated Creatures (DEMON_66) require a tad more work to target.\n"
"If no targets are given, all valid targets will be listed.\n"
"  \n"
"  \n"
"Some Examples: Let's pick on BADGERs\n"
"\n  wagonsniper BADGER :\n          This will Dryrun targetting all the BADGER on the map except those\n          who are in your civilization.\n"
"\n  wagonsniper -k BADGER :\n          This will kill all the BADGER on the map except those who are in\n          your civilization.\n"
"\n  wagonsniper -k -i BADGER :\n          This will kill all the BADGER on the map even those who are in\n          your civilization. Bad Thoughts will happen.\n"
"\n  wagonsniper -k -i -v BADGER :\n          This will kill all the BADGER on the map even yours;\n          Bad Thoughts will happen;\n          AND the Console will fill with data as it lists who was excluded.\n          This could be spammy during an invasion.\n"
"\n  wagonsniper -a :\n          This will Dryrun a target list of everyone on the map. EVERYONE.\n          It's to show how much damage a true Wagongeddon will achieve.\n"
"\n  wagonsniper -a -k :\n          Wagongeddon. Everyone is targetted and killed.\n          This is the shiny button to end it all.\n"
"\n  wagonsniper -k -i BADGER BADGER_MAN :\n          This will kill all the BADGER and BADGER_MAN on the map\n          even those who are in your civilization. Bad Thoughts will happen.\n"
"  \n"
"  \n"
"Some BAD Examples: Let's pick on RAVENs and DOGs\n"
"\n  wagonsniper RAVENS :\n          This is a typo so it'll spam you with the entire creature list.\n"
"\n  wagonsniper DOG :\n          This would give you a Dryrun listing all the DOGs but you 'forgot' -i\n          so it COULD assume there are no targets and spam you with the entire creature list\n          if say the Elves didn't bring any.\n"
"\n  wagonsniper RAVEN -a :\n          Doesn't just list all the RAVENs - it lists everyone on the map.\n          Infact it does not care you typed in RAVEN.\n          The option --all does NOT maintain a list of names.\n"
"\n  wagonsniper TOAD :\n          Critters that are spawned vermin style can't be targetted.\n          If you edit the code to allow them to be targetted, it will not do anything.\n          wagonshot allows targetting them and it can't harm them.\n"
"\n  wagonsniper -k -i SQUID MAN :\n          This critter would need quotes around their name to target them properly.\n          Currently it would read it like:\n\nwagonsniper -k -i SQUID\nwagonsniper -k -i MAN\n\n          The first will work -if there are squid on the map-\n          the second one will cause it to drop the creature list on you.\n"
		));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result wagonsniper (color_ostream &out, vector <string> & parameters)
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
		else if(parameters[h] == "--kill" || parameters[h] == "-kill" || parameters[h] == "--k" || parameters[h] == "-k")
		{
			kill = true;
			continue;
		}
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
			out.print("It was a nice outpost I guess... WAGONGEDDON!\n");
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
			//out.print("Beginning targetNames.size() > 0... ");
			for (set<string>::const_iterator it0 = targetNames.begin(); it0 != targetNames.end(); it0++)
			{
				//if(verbose)
				//	out.print("Names left %6d.\nIDs left %6d.\n", count, targetNames.size(), targetIDs.size());
				//if(verbose)
				//	out.print("     Beginning it0 loop... ");
				//auto sam = it0->c_str();
				//auto harry = bob.c_str();
				//if(verbose)
				//	out.print("it0->c_str() is (%s) and bob.c_str() is (%s)...\n", it0->c_str(), bob.c_str());
				if (strcmp(it0->c_str(), bob.c_str()) != 0)
				{
					//if(verbose)
					//	out.print("Non-target...");
					continue;
				}
				else if (strcmp(it0->c_str(), bob.c_str()) == 0)
				{
					//if(verbose)
					//	out.print("Starting indiscrimancy check...");
					if (indis)
					{
						//targetNames.erase(it0);
						targetIDs.insert(world->units.all[j]->id);
						//if(verbose)
						//	out.print("it0==bob: Indis is On and %d is j.\n", j+1);
						//if(verbose)
						//	out.print("Names left %6d.\nIDs left %6d.\n", count, targetNames.size(), targetIDs.size());
					}
					else
					{
						//auto joe = world->units.all[j]->civ_id;
						if(world->units.all[j]->civ_id < 0) //Many critters do not have anything here. So zap it anyway. Your civ is "Wilderness Population Source".
						{
							//targetNames.erase(it0);
							targetIDs.insert(world->units.all[j]->id);
							continue;
						}
						//auto mark = ui->civ_id;
						if(ui->civ_id < 0) //Should be Impossible. Your civ is "Wilderness Population Source" or something.
						{
							//targetNames.erase(it0);
							targetIDs.insert(world->units.all[j]->id);
							continue;
						}
						//if(verbose)
						//	out.print("world->units.all[j]->civ_id is (%d) and ui->civ_id is (%d)...\n", world->units.all[j]->civ_id, ui->civ_id);
						if (world->units.all[j]->civ_id == ui->civ_id)
							targetNames.erase(it0);
						else
						{
							//if(verbose)
							//	out.print("it0!=bob: J is (%d), and the number of targetNames remaining is (%d)\n", j+1, targetNames.size());
						}
						//if(verbose)
						//	out.print("it0==bob: Indis is Off and %d is j.\n", j+1);
						//if(verbose)
						//	out.print("Names left %6d.\nIDs left %6d.\n", count, targetNames.size(), targetIDs.size()); // Somewhere after here... KABLAM due to tree/set<> oddness.
					}
				}
				else
				{
					//if(verbose)
					//	out.print("it0: Exiting it0 == bob check. J is (%d), and the number of targetNames remaining is (%d)\n", j+1, targetNames.size());
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
	//	out.print("Don't confuse the sniper with bad wagon orders! You specified:\n");
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
			//if(verbose)
			//	out.print("Gonna Wagon Something... out of these %d IDs\n", world->units.all.size());
			for (set<int>::const_iterator it2 = targetIDs.begin(); it2 != targetIDs.end(); it2++)
			{
				//if(verbose)
				//	out.print("Beginning targetID iterator it2...\n");
				//if(verbose)
				//	out.print("unit->id is (%d) and *it2 is (%d)... and they are equal (Right?)\n", unit->id, *it2);
				if (unit->id == *it2)
				{
					//if(verbose)
					//	out.print("unit->id is (%d) and *it2 is (%d)... and they are equal (Right?)\n", unit->id, *it2);
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
		out.print("Wagons dropped on %d targets.\n", count);
	//if(verbose)
	//	out.print("Names left %6d.\nIDs left %6d.\n", count, targetNames.size(), targetIDs.size());
	return CR_OK;
}