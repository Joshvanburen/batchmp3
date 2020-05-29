/*
Josh Van Buren
Header functions for the mp3 rename
*/

#ifndef MP3RENAME_H
#define MP3RENAME_H

//Use unicode
#define UNICODE

//Use the static taglib
#define TAGLIB_STATIC

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/aboutdlg.h>
#include <wx/progdlg.h>
#include <wx/combobox.h>
#include <wx/textctrl.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <taglib\fileref.h>
#include <taglib\tag.h>

//Using the standard namespace
using namespace std;

//Define the main class
class mp3Rename : public wxApp 
{
	//Public method and variable declarations
    public:
        virtual bool OnInit();
}; //End the class definition

//Define the frame class
class mainFrame : public wxFrame
{
	//Public methods and variable declarations
	public:
		mainFrame();
		void mainSearch(wxCommandEvent &event);
		void setFolder(wxCommandEvent &event);
		void setMode(wxCommandEvent &event);
		void setNamingScheme(wxString scheme);
		void childClosed();
		wxString getNamingScheme();
	
	//Private method and variable declarations
	private:
		void wxDirSearch(wstring dirPath, wstring extension);
		void wxDirSearchSub(wstring dirPath, wstring extension);
		void processFile(wxString filePath);
		void renameFiles();
		void exitApp(wxCommandEvent &event);
		void about(wxCommandEvent& event);
		void closeDir(wxCommandEvent& event);
		void prefDialog(wxCommandEvent& event);
		wstring dirPath;
		int mode;
		vector <wstring> oldPaths;
		vector <wstring> newPaths;
		wxMenu * fileMenu;
		wxMenu * helpMenu;
		wxMenu * editMenu;
		wxMenuBar * menuBar;
		wxButton * selDir;
		wxButton * sConv;
		wxButton * exitButton;
		wxCheckBox * sSubs;
		wxString namingScheme;
		wxArrayString fieldNames;
		bool fieldPresent [6];
		DECLARE_EVENT_TABLE()
		
		//Enumeration for the event ids
		enum
		{
			//Select folder, start rename, searchSubs, exit
			idSF = 1000,
			idRN,
			idSS,
			idEx,
			idSFB,
			idRNB,
			idSSC,
			idExB,
			idCF,
			idOp
		};
};

//Define the preferences dialog class
class preferencesDialog : public wxDialog
{
	//Public methods and variables
	public:
		preferencesDialog();
		void setMFRef(mainFrame * ref);
		
	//Private methods and variables
	private:
		void onOK(wxCommandEvent &event);
		void onCancel(wxCommandEvent &event);
		void onApply(wxCommandEvent &event);
		void addRemItem(wxCommandEvent &event);
		void itemSelect(wxCommandEvent &event);	
		void fieldChange(wxCommandEvent &event);
		void OnClose(wxCloseEvent& event);
		bool checkNamingScheme();
		wxComboBox* numCombo;
		wxButton * aRButton;
		wxButton * okButton;
		wxButton * canButton;
		wxButton * applyButton;
		wxTextCtrl * nameField;
		mainFrame * mainFrameRef;
		wxArrayString fieldNames;
		wxArrayString numList;
		DECLARE_EVENT_TABLE()
		
		//Enumeration for the event ids
		enum
		{
			//Event id's
			idNC = 2000,
			idAR,
			idTF
		};
};

//Begin the event table declaration
BEGIN_EVENT_TABLE(mainFrame, wxFrame) 
    EVT_MENU(idSF, mainFrame::setFolder) 
    EVT_MENU(idRN, mainFrame::mainSearch) 
    EVT_MENU(idSS, mainFrame::setMode)
	EVT_MENU(idEx, mainFrame::exitApp)
	EVT_MENU(idCF, mainFrame::closeDir)
    EVT_BUTTON(idSFB, mainFrame::setFolder) 
    EVT_BUTTON(idRNB, mainFrame::mainSearch) 
    EVT_CHECKBOX(idSSC, mainFrame::setMode)
	EVT_BUTTON(idExB, mainFrame::exitApp)
	EVT_MENU(wxID_ABOUT, mainFrame::about)
	EVT_MENU(idOp, mainFrame::prefDialog)
END_EVENT_TABLE() //End the event table declaration

//Begin the event table declaration
BEGIN_EVENT_TABLE(preferencesDialog, wxDialog) 
	EVT_BUTTON(wxID_OK, preferencesDialog::onOK)
	EVT_BUTTON(wxID_CANCEL, preferencesDialog::onCancel)
	EVT_BUTTON(wxID_APPLY, preferencesDialog::onApply)
	EVT_BUTTON(idAR, preferencesDialog::addRemItem)
	EVT_COMBOBOX(idNC, preferencesDialog::itemSelect)
	EVT_TEXT(idTF, preferencesDialog::fieldChange)
	EVT_CLOSE(preferencesDialog::OnClose)
END_EVENT_TABLE() //End the event table declaration

#endif