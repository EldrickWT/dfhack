//command-prompt a one line command entry at the top of the screen for quick commands

#include "Core.h"
#include <Console.h>
#include <Export.h>
#include <PluginManager.h>
#include <ColorText.h>
#include <LineEditor.h>

#include <modules/Screen.h>
#include <modules/Gui.h>

#include <set>
#include <list>
#include <utility>
#include <vector>

#include "df/interface_key.h"
#include "df/ui.h"
#include "df/graphic.h"
#include "df/enabler.h"

using namespace DFHack;
using namespace df::enums;

using df::global::ui;
using df::global::gps;
using df::global::enabler;

CommandHistory cmdprompt_history;

class viewscreen_commandpromptst;
class prompt_ostream:public buffered_color_ostream
{
    viewscreen_commandpromptst *parent_;
    protected:
        void flush_proxy();
    public:
        prompt_ostream(viewscreen_commandpromptst* parent):parent_(parent){}
        bool empty(){return buffer.empty();}
};
class viewscreen_commandpromptst : public dfhack_viewscreen {
public:
    void feed(std::set<df::interface_key> *events);

    void logic() {
        dfhack_viewscreen::logic();
    }

    void render();
    void help() { }
    int8_t movies_okay() { return 0; }

    std::string getFocusString() { return "commandprompt"; }
    viewscreen_commandpromptst(std::string entry):is_response(false)
    {
        show_fps=gps->display_frames;
        gps->display_frames=0;
        frame = 0;
        line_editor = new LineEditor;
        cmdprompt_history.add("");
        line_editor->history = cmdprompt_history;
        line_editor->history_index = 0;
    }
    ~viewscreen_commandpromptst()
    {
        gps->display_frames=show_fps;
        delete line_editor;
    }

    void add_response(color_value v, std::string s)
    {
        std::stringstream ss(s);
        std::string part;
        while (std::getline(ss, part))
        {
            responses.push_back(std::make_pair(v, part + '\n'));
        }
    }

protected:
    std::list<std::pair<color_value,std::string> > responses;
    LineEditor* line_editor;
    bool is_response;
    bool show_fps;
    int frame;
    void submit();
};
void prompt_ostream::flush_proxy()
{
    if (buffer.empty())
        return;
    for(auto it=buffer.begin();it!=buffer.end();it++)
        parent_->add_response(it->first,it->second);
    buffer.clear();
}
void viewscreen_commandpromptst::render()
{
    ++frame;
    if (frame >= enabler->gfps)
        frame = 0;
    if (Screen::isDismissed(this))
        return;

    dfhack_viewscreen::render();

    auto dim = Screen::getWindowSize();
    parent->render();
    if(is_response)
    {
        auto it=responses.begin();
        for(int i=0;i<dim.y && it!=responses.end();i++,it++)
        {
            Screen::fillRect(Screen::Pen(' ', 7, 0),0,i,dim.x,i);
            std::string cur_line=it->second;
            Screen::paintString(Screen::Pen(' ',it->first,0),0,i,cur_line.substr(0,cur_line.size()-1));
        }
    }
    else
    {
        std::string entry = line_editor->line;
        Screen::fillRect(Screen::Pen(' ', 7, 0),0,0,dim.x,0);
        Screen::paintString(Screen::Pen(' ', 7, 0), 0, 0,"[DFHack]#");
        std::string cursor = (frame < enabler->gfps / 2) ? "_" : " ";
        if(line_editor->cursor < (dim.x - 10))
        {
            Screen::paintString(Screen::Pen(' ', 7, 0), 10,0 , entry);
            if (entry.size() > dim.x - 10)
                Screen::paintTile(Screen::Pen('\032', 7, 0), dim.x - 1, 0);
            if (cursor != " ")
                Screen::paintString(Screen::Pen(' ', 10, 0), 10 + line_editor->cursor, 0, cursor);
        }
        else
        {
            size_t start = line_editor->cursor - dim.x + 10 + 1;
            Screen::paintTile(Screen::Pen('\033', 7, 0), 9, 0);
            Screen::paintString(Screen::Pen(' ', 7, 0), 10, 0, entry.substr(start));
            if (cursor != " ")
                Screen::paintString(Screen::Pen(' ', 10, 0), dim.x - 1, 0, cursor);
        }
    }
}
void viewscreen_commandpromptst::submit()
{
    CoreSuspendClaimer suspend;
    if(is_response || line_editor->line.size() == 0)
    {
        Screen::dismiss(this);
        return;
    }
    line_editor->history.remove();
    line_editor->history.add(line_editor->line);
    line_editor->history.save("command-prompt.history");
    prompt_ostream out(this);
    Core::getInstance().runCommand(out, line_editor->line);
    if(out.empty() && responses.empty())
        Screen::dismiss(this);
    else
    {
        is_response=true;
    }
}
void viewscreen_commandpromptst::feed(std::set<df::interface_key> *events)
{
    int old_pos = line_editor->cursor;
    bool leave_all = events->count(interface_key::LEAVESCREEN_ALL);
    if (leave_all || events->count(interface_key::LEAVESCREEN))
    {
        events->clear();
        Screen::dismiss(this);
        if (leave_all)
        {
            events->insert(interface_key::LEAVESCREEN);
            parent->feed(events);
            events->clear();
        }
        return;
    }
    if (events->count(interface_key::SELECT))
    {
        submit();
        return;
    }
    if (is_response)
        return;
    for (auto it = events->begin(); it != events->end(); ++it)
    {
        auto key = *it;
        if (key==interface_key::STRING_A000) //delete
        {
            line_editor->backspace();
            continue;
        }
        int charcode = Screen::keyToChar(key);
        if (charcode > 0)
        {
            line_editor->insert(char(charcode));
            return;
        }
    }
    // Prevent number keys from moving cursor
    if(events->count(interface_key::CURSOR_RIGHT))
    {
        line_editor->cursor_right();
    }
    else if(events->count(interface_key::CURSOR_LEFT))
    {
        line_editor->cursor_left();
    }
    else if(events->count(interface_key::CURSOR_RIGHT_FAST))
    {
        line_editor->cursor_right_word();
    }
    else if(events->count(interface_key::CURSOR_LEFT_FAST))
    {
        line_editor->cursor_left_word();
    }
    else if(events->count(interface_key::CUSTOM_CTRL_A))
    {
        line_editor->cursor_start();
    }
    else if(events->count(interface_key::CUSTOM_CTRL_E))
    {
        line_editor->cursor_end();
    }
    else if(events->count(interface_key::CUSTOM_CTRL_U) ||
            events->count(interface_key::CUSTOM_CTRL_I))
    {
        // Ctrl-I avoids conflict with interface_key::PREFIX (Ctrl-U by default)
        line_editor->yank_left();
    }
    else if(events->count(interface_key::CUSTOM_CTRL_K))
    {
        line_editor->yank_right();
    }
    else if(events->count(interface_key::CUSTOM_CTRL_Y))
    {
        line_editor->yank_paste();
    }
    else if(events->count(interface_key::CUSTOM_CTRL_T))
    {
        line_editor->transpose();
    }
    else if(events->count(interface_key::CURSOR_UP))
    {
        line_editor->history_back();
    }
    else if(events->count(interface_key::CURSOR_DOWN))
    {
        line_editor->history_fwd();
    }
    if (old_pos != line_editor->cursor)
        frame = 0;

}
DFHACK_PLUGIN("command-prompt");
command_result show_prompt(color_ostream &out, std::vector <std::string> & parameters)
{
    if (Gui::getCurFocus() == "dfhack/commandprompt")
    {
        Screen::dismiss(Gui::getCurViewscreen(true));
    }
    std::string params;
    for(size_t i=0;i<parameters.size();i++)
        params+=parameters[i]+" ";
    Screen::show(new viewscreen_commandpromptst(params));
    return CR_OK;
}
bool hotkey_allow_all(df::viewscreen *top)
{
    return true;
}
DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
    commands.push_back(PluginCommand(
        "command-prompt","Shows a command prompt on window.",show_prompt,hotkey_allow_all,
        "command-prompt [entry] - shows a cmd prompt in df window. Entry is used for default prefix (e.g. ':lua')"
        ));
    cmdprompt_history.load("command-prompt.history");
    return CR_OK;
}

DFhackCExport command_result plugin_shutdown ( color_ostream &out )
{
    return CR_OK;
}
