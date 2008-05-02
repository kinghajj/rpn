#include "gui/App.h"
#include "gui/MainForm.h"

IMPLEMENT_APP(App)

bool App::OnInit()
{
	MainForm *main_form = new MainForm(wxT("RPN-GUI Main Form"));
	main_form->Show(true);
	return true;
}
