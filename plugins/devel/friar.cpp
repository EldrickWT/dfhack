// Friar - For Adding a Super Noble who can handle everything short of Monarchy for your Embark Group. Based on: Fixpositions by Quietust
// for 34.07
#include <algorithm> // for std::transform
#include <assert.h>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <set>
#include <stdlib.h> // for rand()
#include <vector>
using namespace std;

#include "Console.h"
#include "Core.h"
#include "DataDefs.h"
#include "DFHack.h"
#include "Export.h"
#include "PluginManager.h"

#include <df/caste_raw.h>
#include <df/creature_raw.h>
#include "df/entity_position.h"
#include "df/entity_position_assignment.h"
#include "df/entity_position_responsibility.h"
#include "df/entity_raw.h"
#include "df/historical_entity.h"
#include "df/historical_figure.h"
#include "df/ui.h"
#include "df/world.h"

using std::set;
using std::string;
using std::vector;

using namespace df::enums;
using namespace DFHack;
using df::global::ui;
using df::global::world;

DFHACK_PLUGIN("friar");

command_result friaradd (color_ostream &out, vector<string> &parameters);

DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	commands.push_back(PluginCommand(
		"friaradd",
		"Add a Friar who can handle many management tasks themselves.",
		friaradd, false,
		"\n"
		"\"friaradd\" takes no arguments. Invoke it, and it's done.\n"
		"\n"
		"Troubleshooting: Currently You must go to the military screen, create a squad off the 'Friar' in the list,\n"
		"and assign in the \"Vacant\" spot who-so-ever you wish to be the Friar.\n"
		"\n"
		"After this has been done you may use the Nobles screen to reassign who is the Friar forever-after.\n"
		"When this has been fixed this message will be removed.\n"
		));
	return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
	return CR_OK;
}

command_result friaradd (color_ostream &out, vector<string> &parameters)
{
	if (!parameters.empty())
		return CR_WRONG_USAGE;

	CoreSuspender suspend;
	int checked = 0, fixed = 0;
	for (int i = 0; i < world->entities.all.size(); i++)
	{
		df::historical_entity *ent = world->entities.all[i];
		checked++;
		if (ent->id != ui->main.fortress_entity->id)
			continue;
		bool update = true;
		df::entity_position *pos = NULL;
		// see if we need to add a new position or modify an existing one
		for (int j = 0; j < ent->positions.own.size(); j++)
		{
			pos = ent->positions.own[j];
			if (pos->responsibilities[entity_position_responsibility::ACCOUNTING] &&
				pos->responsibilities[entity_position_responsibility::BUILD_MORALE] &&
				pos->responsibilities[entity_position_responsibility::HEALTH_MANAGEMENT] &&
				pos->responsibilities[entity_position_responsibility::LAW_ENFORCEMENT] &&
				pos->responsibilities[entity_position_responsibility::LAW_MAKING] &&
				pos->responsibilities[entity_position_responsibility::MAKE_TOPIC_AGREEMENTS] &&
				pos->responsibilities[entity_position_responsibility::MANAGE_PRODUCTION] &&
				pos->responsibilities[entity_position_responsibility::MEET_WORKERS] &&
				pos->responsibilities[entity_position_responsibility::MILITARY_GOALS] &&
				pos->responsibilities[entity_position_responsibility::MILITARY_STRATEGY] &&
				pos->responsibilities[entity_position_responsibility::RECEIVE_DIPLOMATS] &&
				pos->responsibilities[entity_position_responsibility::RELIGION] &&
				pos->responsibilities[entity_position_responsibility::TRADE] &&
				(pos->precedence = 130))

			{
				// a friar position exists with the proper responsibilities - skip to the end
				update = false;
				break;
			}
			// Friar position already exists, but has the wrong options - modify it instead of creating a new one
			if (pos->code == "FRIAR")
				break;
			pos = NULL;
		}
		if (update)
		{
			// either there's no friar, or there is one and it's got the wrong responsibilities
			if (!pos)
			{
				// there was no frair - create it
				pos = new df::entity_position;
				ent->positions.own.push_back(pos);
				pos->code = "FRIAR";
				pos->id = ent->positions.next_position_id++;
				pos->name[0] = "Friar";
				pos->name[1] = "Friars";
				pos->flags.set(entity_position_flags::MENIAL_WORK_EXEMPTION);
				pos->flags.set(entity_position_flags::COLOR);
				pos->flags.set(entity_position_flags::HAS_RESPONSIBILITIES);
				pos->flags.set(entity_position_flags::IS_LEADER);
				pos->flags.set(entity_position_flags::unk_12);
				pos->flags.set(entity_position_flags::unk_13);
				pos->flags.set(entity_position_flags::unk_1a);
				pos->flags.set(entity_position_flags::unk_1b);
				pos->precedence = 130;
				pos->flags.set(entity_position_flags::FLASHES);
				pos->flags.set(entity_position_flags::EXPORTED_IN_LEGENDS);
				pos->flags.set(entity_position_flags::BRAG_ON_KILL);
				pos->flags.set(entity_position_flags::CHAT_WORTHY);
				pos->flags.set(entity_position_flags::DO_NOT_CULL);
				pos->squad[0]= "congregator";
				pos->squad[1]= "congregation";
				pos->squad_size = 10;
				pos->color[0] = 5;
				pos->color[1] = 0;
				pos->color[2] = 0;
				pos->flags.set(entity_position_flags::DUTY_BOUND);
				pos->flags.set(entity_position_flags::RULES_FROM_LOCATION);
			}
			// assign responsibilities
			pos->responsibilities[entity_position_responsibility::ACCOUNTING] = true;
			pos->responsibilities[entity_position_responsibility::BUILD_MORALE] = true;
			pos->responsibilities[entity_position_responsibility::HEALTH_MANAGEMENT] = true;
			pos->responsibilities[entity_position_responsibility::LAW_ENFORCEMENT] = true;
			pos->responsibilities[entity_position_responsibility::LAW_MAKING] = true;
			pos->responsibilities[entity_position_responsibility::MAKE_TOPIC_AGREEMENTS] = true;
			pos->responsibilities[entity_position_responsibility::MANAGE_PRODUCTION] = true;
			pos->responsibilities[entity_position_responsibility::MEET_WORKERS] = true;
			pos->responsibilities[entity_position_responsibility::MILITARY_GOALS] = true;
			pos->responsibilities[entity_position_responsibility::MILITARY_STRATEGY] = true;
			pos->responsibilities[entity_position_responsibility::RECEIVE_DIPLOMATS] = true;
			pos->responsibilities[entity_position_responsibility::RELIGION] = true;
			pos->responsibilities[entity_position_responsibility::TRADE] = true;
		}

		// make sure the friar position, whether we created it or not, is set up for proper assignment
		bool assign = true;
		for (int j = 0; j < ent->positions.assignments.size(); j++)
		{
			if (ent->positions.assignments[j]->position_id == pos->id)
			{
				// it is - nothing more to do here
				assign = false;
				break;
			}
		}
		if (assign)
		{
			// it isn't - set it up
			df::entity_position_assignment *asn = new df::entity_position_assignment;
			ent->positions.assignments.push_back(asn);

			asn->id = ent->positions.next_assignment_id++;
			asn->position_id = pos->id;
			asn->flags.extend(0x1F); // make room for 32 flags
			asn->flags.set(0);  // and set the first one
		}
		if (update || assign)
			fixed++;
	}
	out.print("Fixed %d of %d civilization, embark, survivor, and religious groups to enable friars.\n", fixed, checked);
	return CR_OK;
}