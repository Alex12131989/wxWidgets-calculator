#include "MainFrame.h"
#include "App.h"
#include "Calculator.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	panel = new wxPanel(this);

	//DISPLAY SCREEN SETUP
	wxFont screen_font = this->GetFont();
	screen_font.SetPointSize(13);
	screen_font.SetWeight(wxFONTWEIGHT_BOLD);
	wxPanel* up_screen_padding = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));
	display_screen = new wxStaticText(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE);
	display_screen->SetFont(screen_font);
	wxPanel* down_screen_padding = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxSize(-1, 20));

	up_screen_padding->SetBackgroundColour(*wxLIGHT_GREY);
	display_screen->SetBackgroundColour(*wxLIGHT_GREY);
	down_screen_padding->SetBackgroundColour(*wxLIGHT_GREY);

	wxBoxSizer* display_screen_sizer = new wxBoxSizer(wxVERTICAL);
	wxSizerFlags screen_flags = wxSizerFlags().Expand();
	display_screen_sizer->Add(up_screen_padding, screen_flags);
	display_screen_sizer->Add(display_screen, screen_flags);
	display_screen_sizer->Add(down_screen_padding, screen_flags);


	//BUTTONS
	std::vector<wxString> labels = 
	{ "CA", "<-", "^", "÷", 
		"7", "8", "9", "×", 
		"4", "5", "6", "-", 
		"1", "2", "3", "+", 
		".", "0", "", "=" };
	wxFont button_font = this->GetFont();
	button_font.SetPointSize(22);
	button_font.SetWeight(wxFONTWEIGHT_EXTRABOLD);
	for (wxString label : labels)
	{
		wxButton* button = new wxButton(panel, wxID_ANY, label, wxDefaultPosition, wxSize(80, 80));
		button->SetFont(button_font);
		calculator_buttons.push_back(button);
	}
	wxGridSizer* buttons = new wxGridSizer(6, 4, wxDefaultSize);
	wxSizerFlags buttons_flags = wxSizerFlags().Proportion(1).Expand();

	for (wxButton* button : calculator_buttons)
		buttons->Add(button, buttons_flags);

	//LAYOUT
	wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);
	wxSizerFlags display_screen_flags = wxSizerFlags().Expand();
	layout->Add(display_screen_sizer, display_screen_flags);
	layout->AddSpacer(10);
	layout->Add(buttons);

	wxGridSizer* interactive_part = new wxGridSizer(1, 1, wxSize(0, 0));
	wxSizerFlags border = wxSizerFlags().Border(wxALL, 20).Center();
	interactive_part->Add(layout, border);

	BindObjects();
	panel->Layout();
	panel->SetSizer(interactive_part);
	interactive_part->SetSizeHints(this);
}

void MainFrame::BindObjects()
{
	for (auto button : calculator_buttons)
		button->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {Perform(event, button->GetLabel());});
}

void MainFrame::Perform(wxCommandEvent& event, wxString label)
{
	std::string previous_text = display_screen->GetLabel().ToStdString(), appendage;
	if (label == "=")
	{
		wxString expression = display_screen->GetLabel();
		float result = Calculate(expression.ToStdString());
		previous_text = "";
		appendage = std::to_string(result);
	}
	else if (label == "CA")
	{
		previous_text = "";
	}
	else if (label == "<-")
	{
		if (previous_text.size())
			previous_text.resize(previous_text.size() - 1);
	}
	else
	{
		if (previous_text.size())
		{
			if (label == "÷" || label == "×" || label == "+" || label == "^")
			{
				if (previous_text[previous_text.size() - 1] != '÷' && previous_text[previous_text.size() - 1] != '×' && previous_text[previous_text.size() - 1] != '+' && 
					previous_text[previous_text.size() - 1] != '-' && previous_text[previous_text.size() - 1] != '^')
					appendage = label;
			}
			else
				appendage = label;
		}
		else 
			if (label != "÷" && label != "×" && label != "+" && label != "^")
				appendage = label;

	}
	display_screen->SetLabel(previous_text + appendage);
}