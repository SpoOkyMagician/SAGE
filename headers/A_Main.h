#pragma once
// #include <wx/app.h>

// Note that these headers are automatically included in from project properties:

// - C:\Users\jerem\OneDrive\Desktop\SAGE\headers
// - C:\Users\jerem\OneDrive\Desktop\SAGE\licenses
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\wxwidgets\include\wx*
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\boost_source\boost*
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\sfml_source\include\SFML*
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\lz4_source\lib* (Yes, the headers is actually here.)

// Note: wx, boost, sfml, and lz4 libs are also included in the project properties.
 
// All headers in this header should be precompiled

// Standard (may still need these...)
#include <cstdint>
#include <algorithm>
#include <array>
#include <chrono>
#include <functional>
#include <iomanip>
#include <ios>
#include <list>
#include <map>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

// Win32 (may still need these...)
#ifdef WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#endif

// boost
#include <lexical_cast.hpp>

// wxWidgets
/*
#include <wx/arrstr.h>
#include <wx/bitmap.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>
#include <wx/config.h>
#include <wx/dc.h>
#include <wx/dcbuffer.h>
#include <wx/dialog.h>
#include <wx/dir.h>
#include <wx/display.h>
#include <wx/event.h>
#include <wx/ffile.h>
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/filepicker.h>
#include <wx/frame.h>
#include <wx/hyperlink.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
*/
#ifdef WIN32
#include <wx/msw/registry.h>
#endif
/*
#include <wx/notebook.h>
#include <wx/odcombo.h>
#include <wx/panel.h>
#include <wx/radiobox.h>
#include <wx/renderer.h>
#include <wx/scrolwin.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>
#include <wx/toolbar.h>
#include <wx/vlbox.h>
#include <wx/window.h>
#include <wx/wrapsizer.h>
*/

// Simple and Fast Multimedia Library
/*
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
*/

namespace boost { namespace iostreams { struct zlib_params; } }

using boost::lexical_cast;
using boost::bad_lexical_cast;

class AGE : public wxApp
{
    virtual bool OnInit() override;
};

void DECLARE_APP(AGE);
