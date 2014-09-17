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

#pragma once

#include <string>

#include "Export.h"

namespace DFHack
{
    class DFHACK_EXPORT LineEditor
    {
    public:
        LineEditor()
           :cursor(0),
            line(""),
            yank_clipboard("")
        { }
        int cursor;
        std::string line;
        std::string yank_clipboard;
        std::string prompt;
        void check_cursor();

        bool insert (char ch);
        bool backspace ();
        bool fwd_delete ();
        bool cursor_left (int dist = 1);
        bool cursor_right (int dist = 1);
        bool cursor_left_word ();
        bool cursor_right_word ();
        bool cursor_start ();
        bool cursor_end ();

        bool yank_left ();
        bool yank_right ();
        bool yank_paste ();
        bool transpose ();
    };
}
