// Note that these headers are automatically included in from project properties:
// 
// - C:\Users\jerem\OneDrive\Desktop\SAGE\headers
// - C:\Users\jerem\OneDrive\Desktop\SAGE\licenses
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\wxwidgets\include\wx*
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\boost_source\boost*
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\sfml_source\include\SFML*
// - D:\SteamLibrary\steamapps\common\AoE2DE\Tools_Builds\docs\Source\lz4_source\lib* (Yes, the headers is actually here.)
// Note: wx, boost, sfml, and lz4 libs are also included in the project properties.

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <errno.h>
#include <fstream>
#include <functional>
#include <iconv.h>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <list>
#include <map>
#include <math.h>
#include <memory>
#include <ostream>
#include <psapi.h>
#include <set>
#include <sstream>
#include <stdarg.h>
#include <stddef.h>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <windows.h>

#include "A_AboutDialog.h"
#include "A_AGE_Frame.h"
#include "A_BaseMainFrame.h"
#include "A_CustomComboBoxes.h"
#include "A_CustomTextControls.h"
#include "A_CustomWidgets.h"
#include "A_DataCopies.hpp"
#include "A_DelayedMessageRelay.h"
#include "A_EditableVersion.h"
#include "A_LinkedCheckBox.h"
#include "A_LinkedControl.h"
#include "A_Loaders.h"
#include "A_OpenDialog.h"
#include "A_OpenSaveDialog.h"
#include "A_SaveDialog.h"
#include "A_SharedComboPopup.h"
#include "A_Sprites.h"
#include "G_AttackOrArmor.h"
#include "G_BinaFile.h"
#include "G_Bird.h"
#include "G_Building.h"
#include "G_Civ.h"
#include "G_Color.h"
#include "G_Compressor.h"
#include "G_Creatable.h"
#include "G_DamageGraphic.h"
#include "G_DatFile.h"
#include "G_DeadFish.h"
#include "G_DrsFile.h"
#include "G_Graphic.h"
#include "G_GraphicAttackSound.h"
#include "G_GraphicDelta.h"
#include "G_IFile.h"
#include "G_ISerializable.h"
#include "G_LangFile.h"
#include "G_Logger.h"
#include "G_MapDescription.h"
#include "G_PalFile.h"
#include "G_PlayerColour.h"
#include "G_Projectile.h"
#include "G_RandomMap.h"
#include "G_Research.h"
#include "G_ResourceUsage.h"
#include "G_ScnFile.h"
#include "G_ScnPlayerData.h"
#include "G_ScnResource.h"
#include "G_SlpFile.h"
#include "G_SlpFrame.h"
#include "G_SmpFile.h"
#include "G_SmpFrame.h"
#include "G_SmxFile.h"
#include "G_SmxFrame.h"
#include "G_Sound.h"
#include "G_SoundItem.h"
#include "G_SpriteFile.h"
#include "G_Techage.h"
#include "G_TechageEffect.h"
#include "G_TechTree.h"
#include "G_Terrian.h"
#include "G_TerrianBlock.h"
#include "G_TerrianBorder.h"
#include "G_TerrianCommon.h"
#include "G_TerrianPassGraphic.h"
#include "G_TerrianRestriction.h"
#include "G_Trigger.h"
#include "G_Type50.h"
#include "G_Types.h"
#include "G_Unit.h"
#include "G_UnitCommand.h"
#include "G_UnitHeader.h"
#include "G_UnitLine.h"
#include "L_boost.h"
#include "L_GPLv3.h"
#include "L_LGPLv2.h"
#include "L_liblz4.h"
#include "L_SFML.h"
#include "L_wxWidgets.h"
#include "P_pcrdef.h"
#include "P_pcrio.h"

#include "boost/interprocess/streams/vectorstream.hpp"
#include "boost/iostreams/copy.hpp"
#include "boost/iostreams/detail/config/dyn_link.hpp"
#include "boost/iostreams/device/back_inserter.hpp"
#include "boost/iostreams/filter/zlib.hpp"
#include "boost/iostreams/filtering_streambuf.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/throw_exception.hpp"
#include "lexical_cast.hpp"

#include "wx/app.h"
#include "wx/arrstr.h"
#include "wx/bitmap.h"
#include "wx/button.h"
#include "wx/checkbox.h"
#include "wx/clrpicker.h"
#include "wx/combo.h"
#include "wx/combobox.h"
#include "wx/confbase.h"
#include "wx/config.h"
#include "wx/dc.h"
#include "wx/dcbuffer.h"
#include "wx/dcclient.h"
#include "wx/defs.h"
#include "wx/dialog.h"
#include "wx/dir.h"
#include "wx/display.h"
#include "wx/event.h"
#include "wx/ffile.h"
#include "wx/fileconf.h"
#include "wx/filename.h"
#include "wx/filepicker.h"
#include "wx/frame.h"
#include "wx/generic/textdlgg.h"
#include "wx/hyperlink.h"
#include "wx/log.h"
#include "wx/menu.h"
#include "wx/msgdlg.h"
#include "wx/msw/registry.h"
#include "wx/notebook.h"
#include "wx/odcombo.h"
#include "wx/panel.h"
#include "wx/radiobox.h"
#include "wx/renderer.h"
#include "wx/scrolwin.h"
#include "wx/settings.h"
#include "wx/sizer.h"
#include "wx/statbmp.h"
#include "wx/stattext.h"
#include "wx/stdpaths.h"
#include "wx/string.h"
#include "wx/textctrl.h"
#include "wx/textdlg.h"
#include "wx/timer.h"
#include "wx/tokenzr.h"
#include "wx/toolbar.h"
#include "wx/vlbox.h"
#include "wx/window.h"
#include "wx/wrapsizer.h"

#include "SFML/Audio/Sound.hpp"
#include "SFML/Audio/SoundBuffer.hpp"

#include "lz4hc.h"

#include "zlib.h"

#include "P_pcrdef.h"
#include "P_pcrio.h"

#include "AppIcon24.xpm"
#include "AppIcon32.xpm"
#include "AppIcon64.xpm"
#include "AutoBackup.xpm"
#include "DRSunlock.xpm"
#include "G_BinaFile.h"
#include "float.xpm"
#include "GateClosed.xpm"
#include "GateOpen.xpm"
#include "G_Logger.h"
#include "Ox.xpm"
#include "Paste.xpm"
#include "Question.xpm"
#include "Reselection.xpm"
#include "Villager32.xpm"

// This is going to break things... I know... I am going to fix it soon...

// #include <wx/app.h>

// All headers in this header should be precompiled

/*

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

#ifdef WIN32
#include <wx/msw/registry.h>
#endif

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

// Simple and Fast Multimedia Library

#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

*/

namespace boost { namespace iostreams { struct zlib_params; } }

using boost::lexical_cast;
using boost::bad_lexical_cast;