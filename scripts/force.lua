-- Forces an event.


local EventType = ...


if EventType == "caravan" or EventType == "diplomat" or EventType == "siege" or EventType == "migrants" then
	if dfhack.gui.getSelectedUnit().civ_id==nil then
		qerror('Caravan, diplomat and siege require a civ unit to be highlighted.')
end
end

--code may be kind of bad below :V Putnam ain't experienced in lua...
if EventType ~= "caravan" and EventType ~= "migrants" and EventType ~= "diplomat" and EventType ~= "megabeast" and EventType ~= "curiousbeast" and EventType ~= "mischievousbeast" and EventType ~= "mischeviousbeast" and EventType ~= "flier" and EventType ~= "siege" and EventType ~= "nightcreature" then
	qerror('Invalid argument. Valid arguments are caravan, migrants, diplomat, megabeast, curiousbeast, mischievousbeast, flier, siege and nightcreature.')
end

function force_megabeast()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.Megabeast, season = df.global.cur_season, season_ticks = df.global.cur_season_tick } )
end

function force_migrants()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.Migrants, season = df.global.cur_season, season_ticks = df.global.cur_season_tick, entity = df.historical_entity.find(dfhack.gui.getSelectedUnit().civ_id) } )
end

function force_caravan()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.Caravan, season = df.global.cur_season, season_ticks = df.global.cur_season_tick, entity = df.historical_entity.find(dfhack.gui.getSelectedUnit().civ_id) } )
end

function force_diplomat()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.Diplomat, season = df.global.cur_season, season_ticks = df.global.cur_season_tick, entity = df.historical_entity.find(dfhack.gui.getSelectedUnit().civ_id) } )
end

function force_curious()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.WildlifeCurious, season = df.global.cur_season, season_ticks = df.global.cur_season_tick } )
end

function force_mischievous()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.WildlifeMichievous, season = df.global.cur_season, season_ticks = df.global.cur_season_tick } )
end

function force_flier()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.WildlifeFlier, season = df.global.cur_season, season_ticks = df.global.cur_season_tick } )
end

function force_siege()
df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.CivAttack, season = df.global.cur_season, season_ticks = df.global.cur_season_tick, entity = df.historical_entity.find(dfhack.gui.getSelectedUnit().civ_id) } )
end

function force_nightcreature()
	df.global.timed_events:insert('#', { new = df.timed_event, type = df.timed_event_type.NightCreature, season = df.global.cur_season, season_ticks = df.global.cur_season_tick } )
end

--this code may be bad too :V

if EventType=="caravan" then force_caravan()
	elseif EventType=="migrants" then force_migrants()
	elseif EventType=="diplomat" then force_diplomat()
	elseif EventType=="megabeast" then force_megabeast()
	elseif EventType=="curiousbeast" then force_curious()
	elseif EventType=="mischievousbeast" or EventType=="mischeviousbeast" then force_mischievous()
	elseif EventType=="flier" then force_flier()
	elseif EventType=="siege" then force_siege()
	elseif EventType=="nightcreature" then force_nightcreature()
end
	