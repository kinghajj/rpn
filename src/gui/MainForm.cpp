#include "gui/MainForm.h"

MainForm::MainForm(const wxString& title)
         : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150))
{
	Centre();
}
