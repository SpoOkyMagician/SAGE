#pragma once

#include <I_Headers.h>

/*
#include <wx/dialog.h>
*/

class AboutDialog : public wxDialog
{
public:
    AboutDialog(wxWindow *parent, const wxFont &font);

    static const wxString AGE_VER;
};
