#include "A_CustomTextControls.h"
#include "A_LinkedCheckBox.h"
#include <boost/lexical_cast.hpp>

LinkedCheckBox::LinkedCheckBox(wxWindow* parent, wxString label, AGETextCtrl* link, bool connect) :
    wxCheckBox(parent, wxID_ANY, label), LinkedControl(link)
{
    if (connect)
    {
        Bind(wxEVT_CHECKBOX, &LinkedCheckBox::OnChoose, this);
    }
}

void LinkedCheckBox::OnChoose(wxCommandEvent&)
{
    TextControl->ChangeValue(boost::lexical_cast<std::string>(GetValue()));
    TextControl->SaveEdits();
}
