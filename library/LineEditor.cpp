/*
https://github.com/peterix/dfhack
Copyright (c) 2009-2012 Petr Mrázek (peterix@gmail.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "LineEditor.h"

using namespace DFHack;

void LineEditor::check_cursor()
{
    if (cursor < 0)
        cursor = 0;
    else if (cursor > line.size())
        cursor = line.size();
}

bool LineEditor::insert (char ch)
{
    check_cursor();
    line.insert(cursor, 1, ch);
    cursor++;
    return true;
}

bool LineEditor::backspace ()
{
    check_cursor();
    if (cursor == 0)
        return false;
    line.erase(cursor - 1, 1);
    cursor--;
    return true;
}

bool LineEditor::fwd_delete ()
{
    check_cursor();
    if (cursor == line.size())
        return false;
    line.erase(cursor, 1);
    return true;
}

bool LineEditor::cursor_left (int dist)
{
    if (cursor == 0)
        return false;
    cursor -= dist;
    check_cursor();
    return true;
}

bool LineEditor::cursor_right (int dist)
{
    if (cursor == line.size())
        return false;
    cursor += dist;
    check_cursor();
    return true;
}

bool LineEditor::cursor_left_word ()
{
    cursor_left(10);
}

bool LineEditor::cursor_right_word ()
{
    cursor_right(10);
}

bool LineEditor::cursor_start ()
{
    check_cursor();
    if (cursor == 0)
        return false;
    cursor = 0;
    return true;
}

bool LineEditor::cursor_end ()
{
    check_cursor();
    if (cursor == line.size())
        return false;
    cursor = line.size();
    return true;
}

