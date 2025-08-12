#pragma once
#include <wx/wx.h>


class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title);
private:
	void BindObjects();
	void CallCalculator(wxCommandEvent& event);

	wxPanel* panel;
	wxStaticText* display_screen;
	wxButton* equals_button;

};