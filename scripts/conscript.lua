-- Turns babies and children into adults, and assigns all labors to everyone in the fortress -also checks for messed up labors, and taming.
--I know it doesn't look it but I am trying to not just tabulate every single creature in the objects directory. Really I swear.
--Lazy Variable Declarations... doesn't need all this readability
local debug = false
local mine = false
local fish = false
local hunt = false
local wood = false
local bees = false
--Lazy Argument Checking... extra lines for readability
for _,arg in ipairs({...}) do
    if string.lower(arg) == "debug" then debug = true end

    if string.lower(arg) == "mine" then mine = true end
    if string.lower(arg) == "fish" then fish = true end
    if string.lower(arg) == "hunt" then hunt = true end
    if string.lower(arg) == "wood" then wood = true end
    if string.lower(arg) == "bees" then bees = true end

    if string.lower(arg) == "nomine" then mine = false end
    if string.lower(arg) == "nofish" then fish = false end
    if string.lower(arg) == "nohunt" then hunt = false end
    if string.lower(arg) == "nowood" then wood = false end
    if string.lower(arg) == "nobees" then bees = false end

end
--Lazy Help Section
--TODO
--Lazy Pre-Run Variable Check
--if debug==true then
--print('Debug is (duh) ',debug,'Mine is ',mine,'Fish is ',fish,'Hunt is ',hunt,'Wood is ',wood,'Bees is ',bees)
--end
--Lazy Code
for _,v in ipairs(df.global.world.units.active) do
    if not (v.flags1.dead == true) then
        if v.civ_id == df.global.ui.civ_id then
            if (v.profession >= 103) or (v.profession2 >= 103) then
                v.profession = 102
                v.profession2 = 102
                if (v.flags1.diplomat == false)
                  or (v.flags1.merchant == false)
                  or (v.flags1.forest == false) then
                    v.relations.following = nil
                    v.flags1.ridden=false
                    v.flags1.rider=false
                    v.relations.rider_mount_id=-1
                end
                if v.name.has_name == false then
                    namey = 'This one'
                else
                    namey = dfhack.TranslateName(v.name,true)
                end
                print(namey.. " is all grown up!")
            end
            --I could probably reuse it somehow... but I'm just gonna make a second "namey"... should probably check for nicknames... maybe later.
            if v.name.has_name == false then
                namey2 = 'without a name'
            else
                namey2 = 'named ' ..dfhack.TranslateName(v.name,true)
            end
            -- We don't check for POWER, should we? We also don't get too nitty and gritty about the entity races.
            -- Training_level 9 is -SOME KIND OF NORMAL- when the tame flag is false. df.global.ui.civ_id members are typically level 9
            if (df.creature_raw.find(v.race).creature_id == "DOWNTRODDEN")
              or (df.creature_raw.find(v.race).creature_id == "FABULOUSA")
              or (df.creature_raw.find(v.race).creature_id == "FOOCUBI")
              or (df.creature_raw.find(v.race).creature_id == "KOLCHA")
              or (df.creature_raw.find(v.race).creature_id == "MEWLI")
              or (df.creature_raw.find(v.race).creature_id == "MESSIANIC_FLUFFBALL")
              or (df.creature_raw.find(v.race).creature_id == "QUEEN_SUCCUBUS")
              or (df.creature_raw.find(v.race).creature_id == "TROLL")
              or (df.creature_raw.find(v.race).creature_id == "WRAITH") then
                if (debug == true) then print("Untaming a " ..df.creature_raw.find(v.race).creature_id.. " " ..namey2) end
                v.flags1.tame = false
                v.training_level = 9
            elseif (v.race == df.global.ui.race_id) then
                if (debug == true) then print("Race match for a " ..df.creature_raw.find(v.race).creature_id.. " " ..namey2) end
                v.flags1.tame = false
                v.training_level = 9
            elseif (df.creature_raw.find(v.race).flags.CASTE_CAN_SPEAK == false) or (df.creature_raw.find(v.race).flags.CASTE_CAN_LEARN == false) then
                if (debug == true) then print("Impairment detected: " ..df.creature_raw.find(v.race).creature_id.. " " ..namey2) end
                v.flags1.tame = true
                v.training_level = 7
            --Need to spell out the higher order/entity races in case of "tweak makeown" and "mercenaries". Animalmen'll still be boned but they kinda should be. At current anyway.
            --In either event, we're talking about CIV members already anyway. Aaand we already covered "pets" so now Mercenaries.
            --Could potentially be cleaned up to pull from the master entity list or the entity raws ... but this should be quick at least.
            elseif (df.creature_raw.find(v.race).creature_id == "ANGEL") --ANGEL_CIV
              or (df.creature_raw.find(v.race).creature_id == "SEPUTUS") --CULTIST
              or (df.creature_raw.find(v.race).creature_id == "DWARF") --MOUNTAIN
              or (df.creature_raw.find(v.race).creature_id == "ELF") --FOREST
              or (df.creature_raw.find(v.race).creature_id == "HUMAN") --PLAINS
              or (df.creature_raw.find(v.race).creature_id == "GOBLIN") --EVIL
              or (df.creature_raw.find(v.race).creature_id == "KOBOLD") --SKULKING...
              or (df.creature_raw.find(v.race).creature_id == "FABULOUSA") --FABULOUSA
              or (df.creature_raw.find(v.race).creature_id == "FALLEN") --FALLEN
              or (df.creature_raw.find(v.race).creature_id == "KAPA") --KAPA...
              or (df.creature_raw.find(v.race).creature_id == "KOLCHA") --KOLCHA
              or (df.creature_raw.find(v.race).creature_id == "LOLI") --LOLI_CIV
              or (df.creature_raw.find(v.race).creature_id == "MEWLI") --MEWLI
              or (df.creature_raw.find(v.race).creature_id == "MLA") --MLA_CIV
              or (df.creature_raw.find(v.race).creature_id == "PEDO") --PEDO ...
              or (df.creature_raw.find(v.race).creature_id == "FOOCUBI") --W_FOOCUBI_CIV
              or (df.creature_raw.find(v.race).creature_id == "PROUDHORN") --PROUDHORN_CIV
              or (df.creature_raw.find(v.race).creature_id == "VILEHORN") --VILEHORN_CIV
              or (df.creature_raw.find(v.race).creature_id == "GARDOHN") --GARDOHN
              or (df.creature_raw.find(v.race).creature_id == "WRAITH") --WRAITH
              or (df.creature_raw.find(v.race).creature_id == "YIFFIAN") then --YIFFIAN_CIV
                if (debug == true) then print("Mercenary " ..df.creature_raw.find(v.race).creature_id.. " " ..namey2) end
                v.flags1.tame = false
                v.training_level = 9
            --Spot open for Semi-megabeasts here...
            --Spot open for Megabeasts here...
            elseif not (v.race == df.global.ui.race_id) then
                if (debug == true) then print("Race doesn't match for a " ..df.creature_raw.find(v.race).creature_id.. " " ..namey2) end
                v.flags1.tame = true
                v.training_level = 7
            else --WTF?
                if (debug == true) then print("No criteria for a " ..df.creature_raw.find(v.race).creature_id.. " "..named..", so taming them.") end
                v.flags1.tame = false
                v.training_level = 9
            end
--Lazy SELECT CASTE ALL knockoff applied to LUA
            v.flags1.on_ground=false
            if v.mood==8 then
                v.mood=-1
            end
            if v.counters.soldier_mood==2 then
                v.counters.soldier_mood_countdown=1
            end
            if mine==true then v.status.labors.MINE = true v.military.pickup_flags.update = true else v.status.labors.MINE = false end
            v.status.labors.HAUL_STONE = true
            v.status.labors.HAUL_WOOD = true
            v.status.labors.HAUL_BODY = true
            v.status.labors.HAUL_FOOD = true
            v.status.labors.HAUL_REFUSE = true
            v.status.labors.HAUL_ITEM = true
            v.status.labors.HAUL_FURNITURE = true
            v.status.labors.HAUL_ANIMALS = true
            v.status.labors.CLEAN = true
            if wood==true then v.status.labors.CUTWOOD = true v.military.pickup_flags.update = true else v.status.labors.CUTWOOD = false end
            v.status.labors.CARPENTER = true
            v.status.labors.DETAIL = true
            v.status.labors.MASON = true
            v.status.labors.ARCHITECT = true
            v.status.labors.ANIMALTRAIN = true
            v.status.labors.ANIMALCARE = true
            v.status.labors.DIAGNOSE = true
            v.status.labors.SURGERY = true
            v.status.labors.BONE_SETTING = true
            v.status.labors.SUTURING = true
            v.status.labors.DRESSING_WOUNDS = true
            v.status.labors.FEED_WATER_CIVILIANS = true
            v.status.labors.RECOVER_WOUNDED = true
            v.status.labors.BUTCHER = true
            v.status.labors.TRAPPER = true
            v.status.labors.DISSECT_VERMIN = true
            v.status.labors.LEATHER = true
            v.status.labors.TANNER = true
            v.status.labors.BREWER = true
            v.status.labors.ALCHEMIST = true
            v.status.labors.SOAP_MAKER = true
            v.status.labors.WEAVER = true
            v.status.labors.CLOTHESMAKER = true
            v.status.labors.MILLER = true
            v.status.labors.PROCESS_PLANT = true
            v.status.labors.MAKE_CHEESE = true
            v.status.labors.MILK = true
            v.status.labors.COOK = true
            v.status.labors.PLANT = true
            v.status.labors.HERBALIST = true
            if fish==true then v.status.labors.FISH = true else v.status.labors.FISH = false end
            v.status.labors.CLEAN_FISH = true
            v.status.labors.DISSECT_FISH = true
            if hunt==true then v.status.labors.HUNT = true v.military.pickup_flags.update = true else v.status.labors.HUNT = false end
            v.status.labors.SMELT = true
            v.status.labors.FORGE_WEAPON = true
            v.status.labors.FORGE_ARMOR = true
            v.status.labors.FORGE_FURNITURE = true
            v.status.labors.METAL_CRAFT = true
            v.status.labors.CUT_GEM = true
            v.status.labors.ENCRUST_GEM = true
            v.status.labors.WOOD_CRAFT = true
            v.status.labors.STONE_CRAFT = true
            v.status.labors.BONE_CARVE = true
            v.status.labors.GLASSMAKER = true
            v.status.labors.EXTRACT_STRAND = true
            v.status.labors.SIEGECRAFT = true
            v.status.labors.SIEGEOPERATE = true
            v.status.labors.BOWYER = true
            v.status.labors.MECHANIC = true
            v.status.labors.POTASH_MAKING = true
            v.status.labors.LYE_MAKING = true
            v.status.labors.DYER = true
            v.status.labors.BURN_WOOD = true
            v.status.labors.OPERATE_PUMP = true
            v.status.labors.SHEARER = true
            v.status.labors.SPINNER = true
            v.status.labors.POTTERY = true
            v.status.labors.GLAZING = true
            v.status.labors.PRESSING = true
            if bees==true then v.status.labors.BEEKEEPING = true else v.status.labors.BEEKEEPING = false end
            v.status.labors.WAX_WORKING = true
            v.status.labors.HANDLE_VEHICLES = true
            v.status.labors.HAUL_TRADE = true
            v.status.labors.PULL_LEVER = true
            v.status.labors.REMOVE_CONSTRUCTION = true
            v.status.labors.HAUL_WATER = true
        end
        --/Old/ code for checking for mysterious extra labors.
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[78] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.78") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[79] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.79") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[80] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.80") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[81] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.81") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[82] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.82") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[83] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.83") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[84] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.84") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[85] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.85") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[86] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.86") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[87] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.87") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[88] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.88") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[89] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.89") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[90] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.90") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[91] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.91") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[92] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.92") end
        if (v.civ_id == df.global.ui.civ_id) and not (v.status.labors[93] == false) then print(dfhack.TranslateName(v.name,true).." has a true value for status.labors.93") end
    end
end