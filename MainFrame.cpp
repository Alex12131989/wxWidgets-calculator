#include "MainFrame.h"
#include "Calculator.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{

	panel = new wxPanel(this);
	display_screen = new wxStaticText(panel, wxID_ANY, "1󫎿", wxPoint(0, 0), wxSize(1000, 50));

	equals_button = new wxButton(panel, wxID_ANY, "=", wxPoint(0, 50), wxSize(100, 35));

	BindObjects();
}

void MainFrame::BindObjects()
{
	equals_button->Bind(wxEVT_BUTTON, &MainFrame::CallCalculator, this);
}

void MainFrame::CallCalculator(wxCommandEvent& event)
{
	std::string expression = "1+2-2^3�3-4�2+1-2^4";
	float result = Calculate(expression);
	display_screen->SetLabel(std::to_string(result));
}