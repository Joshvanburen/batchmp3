/*
Josh Van Buren
Main file for the mp3 rename
*/

//Include the header with function and class definitions
#include "mp3Rename.h"

//Macro to implement the app
IMPLEMENT_APP(mp3Rename)

//Using the standard namespace
using namespace std;

//Define the initialization method
bool mp3Rename::OnInit() 
{
	//Create the wxFrame
    mainFrame * content = new mainFrame();
	
	//Center the frame
	content->Centre();
	
	//Show the frame
    content->Show(true);

	//Return
    return true;
}

//Constructor for the main class
mainFrame::mainFrame() : wxFrame(NULL, wxID_ANY, L"Batch MP3 Rename", wxDefaultPosition, wxSize(225,200)) 
{
	//Create the menubar
	wxMenuBar * menuBar = new wxMenuBar();
	
	//Create the file menu, help menu, edit menu
	wxMenu * fileMenu = new wxMenu();
	wxMenu * helpMenu = new wxMenu();
	wxMenu * editMenu = new wxMenu();
	
	//Append items to the file menu
	fileMenu->Append(idSF, L"&Open Folder\t Ctrl-O", L"Select the folder");
	fileMenu->Append(idRN, L"&Rename Files\t Ctrl-R", L"Start file rename");
	fileMenu->AppendCheckItem(idSS, L"Recurse Through Subd&irectories\t Ctrl-I", L"Search subdirectories");
	fileMenu->Append(idCF, L"&Close Folder\t Ctrl-C", L"Close the current directory");
	fileMenu->AppendSeparator();
	fileMenu->Append(idEx, L"E&xit\t Ctrl-F4", L"Exits the program");
	
	//Append to the help menu
	helpMenu->Append(wxID_ABOUT, L"&About...\tF1", L"Show about dialog");
	
	//Append to the edit menu
	editMenu->Append(idOp, L"&Preferences\t Ctrl-P", L"Edit preferences");
	
	//Add the file menu to the menu bar
	menuBar->Append(fileMenu, L"&File");
	menuBar->Append(editMenu, L"&Edit");
	menuBar->Append(helpMenu, L"&Help");
	
	//Set the menubar
	SetMenuBar(menuBar);
	
	//Create the status bar
    CreateStatusBar(1);
    SetStatusText(L"Batch MP3 Rename");
	
	//Sets the default naming scheme
	namingScheme = L"<TrackNumber> <Artist> - <Title>";
	
	//String array for the field id's
	fieldNames.Add(L"<Album>");
	fieldNames.Add(L"<Artist>");
	fieldNames.Add(L"<Genre>");
	fieldNames.Add(L"<Title>");
	fieldNames.Add(L"<TrackNumber>");
	fieldNames.Add(L"<Year>");
	
	//Sets default field presence
	fieldPresent[0] = false;
	fieldPresent[1] = true;
	fieldPresent[2] = false;
	fieldPresent[3] = true;
	fieldPresent[4] = true;
	fieldPresent[5] = false;
	
	//Set the icon
	SetIcon(wxIcon(L"id"));
	
	//Create the sizers for the frame
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *oSizer = new wxBoxSizer(wxHORIZONTAL);
	
	//Panel for the frame
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	
	//Set the sizer for the panel
	panel->SetSizer(oSizer);
	
	//Create the buttons, check box
	wxButton * selDir = new wxButton(panel, idSFB, L"&Open Folder");
	wxButton * sConv = new wxButton(panel, idRNB, L"&Rename Files");
	wxCheckBox * sSubs = new wxCheckBox(panel, idSSC, L"Recurs&ive");
	wxButton * exitButton = new wxButton(panel, idExB, L"E&xit");
	
	//Disable the button, menu item
	sConv->Enable(0);
	fileMenu->Enable(idRN, 0);
	fileMenu->Enable(idCF, 0);
	
	//Add the items to the sizer
	sizer->Add(selDir, 1, wxEXPAND | wxALL, 5);
	sizer->Add(sConv, 1, wxEXPAND | wxALL, 5);
	sizer->Add(exitButton, 1, wxEXPAND | wxALL, 5);
	oSizer->Add(sizer, 1, wxEXPAND | wxALL, 5);
	oSizer->Add(sSubs, 1, wxEXPAND | wxALL, 5);
}

//Function that sets the folder
void mainFrame::setFolder(wxCommandEvent &event)
{
	//Creates the directory dialog
	wxDirDialog * chooseDir = new wxDirDialog(this, L"Choose a folder", L"", 
		wxDD_DEFAULT_STYLE);
		
	//If the user selects a directory
	if(chooseDir->ShowModal() == wxID_OK)
	{
		//Sets the path
		dirPath = wstring(chooseDir->GetPath().wc_str());
		dirPath.append(L"\\");
		
		//Get the menu bar from the frame
		wxMenuBar * fMenu = GetMenuBar();
		
		//Look for the edit menu
		int fMenuIndex = fMenu->FindMenu(L"File");
		
		//Gets the edit menu
		wxMenu * fileMenu = fMenu->GetMenu(fMenuIndex);
		
		//Find the button to disable
		wxButton * sConv = (wxButton *)FindWindow(idRNB);
		
		//Enable the button, menu item
		sConv->Enable(1);
		fileMenu->Enable(idRN, 1);
		fileMenu->Enable(idCF, 1);
	}
	
	//Delete the dialog
	delete chooseDir;
}

//Method that sets the mode
void mainFrame::setMode(wxCommandEvent &event)
{
	//If the event is from the menu item 
	if(event.IsChecked() == true)
	{
		//Search sub directories
		mode = 1;
		
		//Find the checkbox to disable
		wxCheckBox * sSubs = (wxCheckBox *)FindWindow(idSSC);
		
		//Check the box
		sSubs->SetValue(true);
		
		//Get the menu bar
		wxMenuBar * mBar = GetMenuBar();
		
		//Get the file menu
		wxMenu * fMenu = mBar->GetMenu(0);
		
		//Check the menu item
		fMenu->Check(idSS, true);
	}
	//Else not checked
	else
	{
		//Do not search sub directories
		mode = 0;
		
		//Find the checkbox to disable
		wxCheckBox * sSubs = (wxCheckBox *)FindWindow(idSSC);
		
		//Check the box
		sSubs->SetValue(false);
		
		//Get the menu bar
		wxMenuBar * mBar = GetMenuBar();
		
		//Get the file menu
		wxMenu * fMenu = mBar->GetMenu(0);
		
		//Uncheck the menu item
		fMenu->Check(idSS, false);
	}
}

//Method that exits the application
void mainFrame::exitApp(wxCommandEvent &event)
{
	//Destroy and close the window
	this->Destroy();
}

//Method that displays the about dialog
void mainFrame::about(wxCommandEvent& event)
{
	//Creates the dialog and sets the properties
	wxAboutDialogInfo aboutInfo;
	aboutInfo.SetName(L"Batch MP3 Rename");
	aboutInfo.SetVersion(L"1.0");
	aboutInfo.SetDescription(_(L"Batch Rename of MP3 Files"));
	//aboutInfo.SetCopyright(L"(C) 2011");
	aboutInfo.SetWebSite(L"https://github.com/Joshvanburen");
	aboutInfo.AddDeveloper(L"Josh Van Buren");
	aboutInfo.SetIcon(wxIcon(L"id"));

	//Displays the box
	wxAboutBox(aboutInfo);
}

//Method that opens the preferences dialog
void mainFrame::prefDialog(wxCommandEvent& event)
{
	//Create the preferences dialog
	preferencesDialog * pDialog = new preferencesDialog();
	
	//Set the reference to the other class
	pDialog->setMFRef(this);
	
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Disable the menu item
	editMenu->Enable(idOp, false);
}

//Method that closes the open directory
void mainFrame::closeDir(wxCommandEvent &event)
{
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int fMenuIndex = fMenu->FindMenu(L"File");
	
	//Gets the edit menu
	wxMenu * fileMenu = fMenu->GetMenu(fMenuIndex);
	
	//Find the button to disable
	wxButton * sConv = (wxButton *)FindWindow(idRNB);
	
	//Disable the button, menu items
	sConv->Enable(0);
	fileMenu->Enable(idRN, 0);
	fileMenu->Enable(idCF, 0);
	
	//Clear the current directory
	dirPath.clear();
}

//Main search method method
void mainFrame::mainSearch(wxCommandEvent &event)
{
	//Call the particular search depending on the mode
	if(mode == 1)
		//Search sub directories
		wxDirSearchSub(dirPath, L"*.mp3");
	//Else do not search sub directories
	else
		//Do not search sub directories
		wxDirSearch(dirPath, L"*.mp3");
	
	//Rename the files
	renameFiles();
}

//Search directories without searching sub directories
void mainFrame::wxDirSearch(wstring dirPath, wstring extension)
{
	//Create the directory
	wxDir sDir(dirPath);
	
	//If error, return
	if(!sDir.IsOpened())
	{
		//Return
		return;
	}
	
	//First file to find
    wxString fileName;

	//Find the first file
    bool res = sDir.GetFirst(&fileName, extension);
	
	//Create the progress dialog
	wxProgressDialog fileProg(L"Please wait", L"Processing Files", 100, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_SMOOTH);
	
	//While there are more files to find, process them
    while(res)
    {
		//Pulse the dialog
		fileProg.Pulse(dirPath + fileName, NULL);
		
		//Process the file
        processFile(dirPath + fileName);
		
		//Get the next file
		res = sDir.GetNext(&fileName);
    }
	
	//Update to finished
	fileProg.Update(100, L"Finished", NULL);
}

//Search directory and subs
void mainFrame::wxDirSearchSub(wstring dirPath, wstring extension)
{
	//String array
	wxArrayString files;
	
	//Search the directory and sub directories
	wxDir::GetAllFiles(dirPath, &files, extension);
	
	//Get the file count
	unsigned int count = files.GetCount();
	
	//Create the progress dialog
	wxProgressDialog fileProg(L"Please wait", L"Processing Files", count, this, wxPD_APP_MODAL | wxPD_AUTO_HIDE | wxPD_SMOOTH);
	
	//For loop to loop through the files
	for(int i = 0; i<count; i++)
	{
		//Update to finished
		fileProg.Update(i+1, files.Item(i), NULL);
		
		//Process the file
		processFile(files.Item(i));
	}
	
	//Update to finished
	fileProg.Update(count, L"Finished", NULL);
}

//Process the matching file
void mainFrame::processFile(wxString filePath)
{
	//Get the mp3 tag of the file
	TagLib::FileRef f(filePath.wc_str());
	
	//If the tag is not null
	if(!f.isNull() && f.tag())
	{
		//Get the tag
		TagLib::Tag * tag = f.tag();
		
		//Backslash character
		wchar_t backS = '\\';
		
		//Gets the file name only, i.e. after the last slash
		wxString folderPath = filePath.BeforeLast(wxUniChar(backS));
		
		//Temporarily copy the naming scheme
		wxString newName = namingScheme;
		
		//If album field present, check
		if(fieldPresent[0] == true)
		{
			//If empty, return, else replace
			if((tag->album().isEmpty() == true) || (tag->album().isNull() == true))
			{
				//Field empty, so return
				return;
			}
			//Else, replace
			else
			{
				//Replace the string
				newName.Replace(L"<Album>", tag->album().toCString(), true);
			}
		}
			
		//If artist field present, check
		if(fieldPresent[1] == true)
		{
			//If empty, return
			if((tag->artist().isEmpty() == true) || (tag->artist().isNull() == true))
			{
				//Field empty, so return
				return;
			}
			//Else, replace
			else
			{
				//Replace the string
				newName.Replace(L"<Artist>", tag->artist().toCString(), true);
			}
		}
			
		//If genre field present, check
		if(fieldPresent[2] == true)
		{
			//If empty, return
			if((tag->genre().isEmpty() == true) || (tag->genre().isNull() == true))
			{
				//Field empty, so return
				return;
			}
			//Else, replace
			else
			{
				//Replace the string
				newName.Replace(L"<Genre>", tag->genre().toCString(), true);
			}
		}
			
		//If title field present, check
		if(fieldPresent[3] == true)
		{
			//If empty, return
			if((tag->title().isEmpty() == true) || (tag->title().isNull() == true))
			{
				//Field empty, so return
				return;
			}
			//Else, replace
			else
			{
				//Replace the string
				newName.Replace(L"<Title>", tag->title().toCString(), true);
			}
		}
			
		//If track field present, check
		if(fieldPresent[4] == true)
		{
			//If empty, return
			if(tag->track() <= 0)
			{
				//Field empty, so return
				return;
			}
			//Else, replace
			else
			{
				//Temporary character array
				wchar_t * tempA = new wchar_t[3];
				
				//Write track to temp array
				wsprintf(tempA, L"%02d", tag->track());
				
				//Replace the string
				newName.Replace(L"<TrackNumber>", tempA, true);
				
				//Delete the temp array
				delete[] tempA;
			}
		}
			
		//If year field present, check
		if(fieldPresent[5] == true)
		{
			//If empty, return
			if(tag->year() <= 0)
			{
				//Field empty, so return
				return;
			}
			//Else, replace
			else
			{
				//Temporary character array
				wchar_t * tempA = new wchar_t[3];
				
				//Write track to temp array
				wsprintf(tempA, L"%02d", tag->year());
				
				//Replace the string
				newName.Replace(L"<Year>", tempA, true);
				
				//Delete the temp array
				delete[] tempA;
			}
		}
	
		//Create the strings and add them to the vectors
		oldPaths.push_back(filePath.ToStdWstring());
		newPaths.push_back(folderPath.ToStdWstring() + wstring(L"\\") + newName.ToStdWstring() + wstring(L".mp3"));
	}
}

//Function that renames the files
void mainFrame::renameFiles()
{
	//For loop to rename the files
	for(unsigned int i = 0; i<oldPaths.size(); i++)
	{
		//Rename the files
		_wrename(_wcsdup(oldPaths[i].c_str()), _wcsdup(newPaths[i].c_str()));
	}
}

//Function that sets the naming scheme from the preferences dialog
void mainFrame::setNamingScheme(wxString scheme)
{
	//Sets the naming scheme
	namingScheme = scheme;
	
	//Get the count of the items in the string array
	for(int i = 0; i<fieldNames.GetCount(); i++)
	{
		//Get the item at the index
		wstring item = fieldNames.Item(i).wc_str();
		
		//Check for the text in the field
		if(namingScheme.ToStdString().find(fieldNames.Item(i).ToStdString()) == string::npos)
		{
			//Not found, so not present
			fieldPresent[i] = false;
		}
		//Else, set to remove item
		else
		{
			//Found, so present
			fieldPresent[i] = true;
		}
	}
}

//Function that lets the main frame know that the child has closed
void mainFrame::childClosed()
{
	//Get the menu bar from the frame
	wxMenuBar * fMenu = GetMenuBar();
	
	//Look for the edit menu
	int eMenuIndex = fMenu->FindMenu(L"Edit");
	
	//Gets the edit menu
	wxMenu * editMenu = fMenu->GetMenu(eMenuIndex);
	
	//Enable the menu item
	editMenu->Enable(idOp, true);	
}

//Function that returns the naming scheme
wxString mainFrame::getNamingScheme()
{
	//Returns the naming scheme
	return namingScheme;
}

//Constructor for the preferences dialog class
preferencesDialog::preferencesDialog() : wxDialog(NULL, -1, L"Preferences", wxDefaultPosition, wxSize(320,200), wxDEFAULT_DIALOG_STYLE)
{	
	//Panel for the dialog
	wxPanel *panel = new wxPanel(this, wxID_ANY);
	
	//Create the sizers
	wxBoxSizer * sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * sizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * sizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticBox * staticBox = new wxStaticBox(panel, wxID_ANY, L"Naming Options");
	wxStaticBoxSizer * staticSizer = new wxStaticBoxSizer(staticBox, wxVERTICAL);
	
	//Set the sizer for the panel
	panel->SetSizer(mainSizer);
	
	//String array for the combo box of number of fields
	numList.Add(L"Album");
	numList.Add(L"Artist");
	numList.Add(L"Genre");
	numList.Add(L"Title");
	numList.Add(L"Track Number");
	numList.Add(L"Year");
	
	//String array for the field id's
	fieldNames.Add(L"<Album>");
	fieldNames.Add(L"<Artist>");
	fieldNames.Add(L"<Genre>");
	fieldNames.Add(L"<Title>");
	fieldNames.Add(L"<TrackNumber>");
	fieldNames.Add(L"<Year>");
	
	//Create the combo box
	numCombo = new wxComboBox(panel, idNC,
		L"Album", wxDefaultPosition, wxDefaultSize, numList, wxCB_READONLY);
		
	//Create the text control
	nameField = new wxTextCtrl(panel, idTF, L"<TrackNumber> <Artist> - <Title>", wxDefaultPosition, wxDefaultSize,  
		wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr);
		
	//Create the buttons
	aRButton = new wxButton(panel, idAR, L"&Add Field");
	okButton = new wxButton(panel, wxID_OK, L"&OK");
	canButton = new wxButton(panel, wxID_CANCEL, L"&Cancel");
	applyButton = new wxButton(panel, wxID_APPLY, L"&Apply");
	
	//Disable the apply button on opening
	applyButton->Enable(0);
		
	//Add the items to the sizers
	sizer2->Add(numCombo, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 5);
	sizer2->Add(aRButton, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxEXPAND | wxALL, 5);
	sizer3->Add(nameField, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND, 5);
	staticSizer->Add(sizer3, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND, 5);
	staticSizer->Add(sizer2, 1, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALL | wxEXPAND, 5);
	sizer->Add(okButton, 1, wxLEFT | wxRIGHT, 5);
	sizer->Add(canButton, 1, wxLEFT | wxRIGHT, 5);
	sizer->Add(applyButton, 1, wxLEFT | wxRIGHT, 5);
	mainSizer->Add(staticSizer, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);
	mainSizer->Add(sizer, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTRE_VERTICAL | wxALIGN_CENTRE_HORIZONTAL | wxALL, 5);
	
	//Fit the sizer to the panel
	mainSizer->Fit(panel);
	
	//Show the dialog
	Show(true);
}

//Function that handles the events for the ok button
void preferencesDialog::onOK(wxCommandEvent &event)
{
	//Get the text from the field
	wxString fieldText = nameField->GetLineText(0);
	
	//Check the file name convention
	if(checkNamingScheme())
	{
		//Sets the naming scheme
		mainFrameRef->setNamingScheme(fieldText);
	
		//Enable the apply button
		applyButton->Enable(0);
		
		//Close the frame
		Close();
		
		//Destroy the dialog
		Destroy();
	}
	//Else, illegal character, warn
	else
	{
		//Alert the user
		wxMessageBox(L"Illegal Characters in the File Name", L"Error", wxOK | wxICON_ERROR, this);
	}
}

//Function that handles the events for the cancel button
void preferencesDialog::onCancel(wxCommandEvent &event)
{
	//Close the dialog
	Close();
	
	//Destroy the dialog
	Destroy();
}

//Function that handles the events for the apply button
void preferencesDialog::onApply(wxCommandEvent &event)
{
	//Get the text from the field
	wxString fieldText = nameField->GetLineText(0);
	
	//Check the file name convention
	if(checkNamingScheme())
	{
		//Sets the naming scheme
		mainFrameRef->setNamingScheme(fieldText);
	
		//Enable the apply button
		applyButton->Enable(0);
	}
	//Else, illegal character, warn
	else
	{
		//Alert the user
		wxMessageBox(L"Illegal Characters in the File Name", L"Error", wxOK | wxICON_ERROR, this);
	}
}

//Function that handles the events for the add remove button
void preferencesDialog::addRemItem(wxCommandEvent &event)
{
	//Gets the currently selected item
	size_t selItem = numCombo->GetCurrentSelection();
	
	//Gets the item text
	wxString itemText = numCombo->GetValue();
	
	//Get the text from the field
	wxString fieldText = nameField->GetLineText(0);
	
	//Convert the strings to standard strings
	string nIT = itemText.ToStdString();
	string nFT = fieldText.ToStdString();
	
	//Get the count of the items in the string array
	for(int i = 0; i<numList.GetCount(); i++)
	{
		//Get the item at the index
		string item = numList.Item(i).ToStdString();
		
		//Look for the selected field in the array
		if(item.compare(nIT) == 0)
		{
			//Check for the text in the field
			if(nFT.find(fieldNames.Item(i).ToStdString()) == string::npos)
			{
				//Not found, so add it
				nameField->WriteText(fieldNames.Item(i));
				
				//Set the button label
				aRButton->SetLabel(L"Remove Field");
			}
			//Else, set to remove item
			else
			{
				//Remove the field
				fieldText.Replace(fieldNames.Item(i), "", true);
				
				//Replace the text
				nameField->Remove(0, nameField->GetLastPosition());
				
				//Add the text
				nameField->AppendText(fieldText);
				
				//Set the button label
				aRButton->SetLabel(L"Add Field");
			}
		}
	}
	
	//Enable the apply button
	applyButton->Enable(1);
}

//Function that handles the events for the ok button
void preferencesDialog::itemSelect(wxCommandEvent &event)
{
	//Gets the currently selected item
	size_t selItem = numCombo->GetCurrentSelection();
	
	//Gets the item text
	wxString itemText = numCombo->GetValue();
	
	//Get the text from the field
	wxString fieldText = nameField->GetLineText(0);
	
	//Convert the strings to standard strings
	string nIT = itemText.ToStdString();
	string nFT = fieldText.ToStdString();
	
	//Get the count of the items in the string array
	for(int i = 0; i<numList.GetCount(); i++)
	{
		//Get the item at the index
		string item = numList.Item(i).ToStdString();
		
		//Look for the selected field in the array
		if(item.compare(nIT) == 0)
		{
			//Check for the text in the field
			if(nFT.find(fieldNames.Item(i).ToStdString()) == string::npos)
			{
				//Not found, so set to add item
				aRButton->SetLabel(L"Add Field");
			}
			//Else, set to remove item
			else
			{
				//Set text to remove item
				aRButton->SetLabel(L"Remove Field");
			}
		}
	}
}

//Function that handles the event for the text control
void preferencesDialog::fieldChange(wxCommandEvent &event)
{
	//Enable the apply button
	applyButton->Enable(1);
}

//Function that handles the close event
void preferencesDialog::OnClose(wxCloseEvent& event)
{
	//Alert the parent that it's closed
	mainFrameRef->childClosed();
	
	//Continue event handling
	event.Skip();
}

//Function that sets the class reference
void preferencesDialog::setMFRef(mainFrame * ref)
{
	//Sets the reference
	mainFrameRef = ref;
	
	//Clears the text
	nameField->Clear();
	
	//Sets the insertion point
	nameField->SetInsertionPoint(0);
	
	//Writes the text to the field
	nameField->WriteText(mainFrameRef->getNamingScheme());
	
	//Disable the apply button
	applyButton->Enable(0);
}

//Function that checks the naming scheme for illegal characters
bool preferencesDialog::checkNamingScheme()
{
	//Get the text from the field
	wxString fieldText = nameField->GetLineText(0);
	
	//Get the count of the items in the string array
	for(int i = 0; i<fieldNames.GetCount(); i++)
	{
		//Get the item at the index
		string item = fieldNames.Item(i).ToStdString();
		
		//Replace the text in naming convention
		fieldText.Replace(item.c_str(), "");
	}
	
	//Check for illegal characters
	if(fieldText.ToStdString().find("?")  != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find("<") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find(">") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find(":") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find("*") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find("|") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find("/") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find("\\") != string::npos)
		return false;
		
	//Check for illegal characters
	if(fieldText.ToStdString().find("\"") != string::npos)
		return false;		
	
	//No illegal characters found, so return true
	return true;
}
