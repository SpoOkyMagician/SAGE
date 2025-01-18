#include <I_Headers.h>

/*
#include "A_CustomTextControls.h"
#include "A_LinkedControl.h"
*/

LinkedControl::LinkedControl(AGETextCtrl* link) :
    TextControl(link)
{
    TakeControl();
}

void LinkedControl::TakeControl(void)
{
    TextControl->LinkedBox = this;
}
