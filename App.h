#pragma once
#include <wx/wx.h>
#include "MainFrame.h"

const int MainWindowWidth = 800;
const int MainWindowHeight = 600;

class App : public wxApp
{
public: 
	bool OnInit();
};