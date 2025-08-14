#pragma once
#include <wx/wx.h>
#include <vector>


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	void BindObjects();
	void Perform(wxCommandEvent& event, wxString label);

	wxPanel* panel;
	wxStaticText* display_screen;
	std::vector<wxButton*> calculator_buttons;
};