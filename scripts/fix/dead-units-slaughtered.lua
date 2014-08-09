-- Remove slaughtered dead units from the unit list.

local units = df.global.world.units.active
local count = 0

for i=#units-1,0,-1 do
    local unit = units[i]
    local flags1 = unit.flags1
    local flags2 = unit.flags2
    if flags1.dead and flags2.slaughter and flags2.killed and not unit.name.has_name then
        count = count + 1
        units:erase(i)
    end
end

print('Units removed from active: '..count)
