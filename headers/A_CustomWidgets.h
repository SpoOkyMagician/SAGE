#pragma once
#include "I_Headers.h"

/*
#include <vector>
#include <wx/panel.h>
#include <wx/scrolwin.h>
#include <wx/stattext.h>
#include <wx/vlbox.h>

#include <wx/app.h>
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
#include <wx/radiobox.h>
#include <wx/renderer.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/textdlg.h>
#include <wx/timer.h>
#include <wx/tokenzr.h>
#include <wx/toolbar.h>
#include <wx/window.h>
#include <wx/wrapsizer.h>
*/

class ProperList : public wxVListBox
{
public:
    ProperList(wxWindow* parent, const wxSize& size);
    void EnsureVisible(size_t n);
    inline void Sweep() { names.clear(); indexes.clear(); }
    //inline void Add(const wxString &name, size_t i);

    wxArrayString names;
    std::vector<size_t> indexes;

private:
    inline wxCoord OnMeasureItem(size_t) const override { return row_height; }
    void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const override;
    void OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const override;

    int row_height = 0;
};

class SolidText : public wxStaticText
{
public:
    SolidText(wxWindow* parent, const wxString& label, long style = 0, const wxSize& size = wxDefaultSize);
};

class APanel : public wxPanel
{
public:
    APanel();
    APanel(wxWindow* parent, const wxSize& size = wxDefaultSize);

private:
    void DisableAutoScroll(void);
};

class ATabPage : public APanel
{
public:
    ATabPage(wxWindow* parent, const wxSize& size = wxDefaultSize);

    bool Show(bool show = true) override;

private:
    wxWindow* parent;
};

class AScrolled : public wxScrolled<APanel>
{
public:
    AScrolled(wxWindow* parent);

    static float rate;

private:
    float remainder;
};