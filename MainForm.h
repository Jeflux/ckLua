#pragma once

#include <Windows.h>
#include "ckLua.h"
#include <iostream>
#include <sstream>
#include <msclr\marshal_cppstd.h>
#include "utils.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


public ref class MainForm : public System::Windows::Forms::Form
{
public:
	MainForm(ckLua* lua, std::ostringstream* out) : minimizedFirstTime(true) {
		InitializeComponent();
		this->Show();

		this->lua = lua;
		this->out = out;

		populateComboBoxes();
		this->updateTimer->Start();

		loadSelectedKeymap();
		loadSelectedScript();

		System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
		this->Icon = gcnew System::Drawing::Icon(L"..\\icon.ico");
		this->minimizeIcon->Icon = gcnew System::Drawing::Icon(L"..\\icon.ico");
	}

private:
	System::Void MainForm_Shown(System::Object^  sender, System::EventArgs^  e) {
	}

protected:
	~MainForm(){
		if (components){
			delete components;
		}
	}

private: System::ComponentModel::IContainer^  components;
protected:

// Form components
private: System::Windows::Forms::RichTextBox^	txtOutput;
private: System::Windows::Forms::Label^			lblOutput;
private: System::Windows::Forms::Button^		btnClearConsole;
private: System::Windows::Forms::ComboBox^		cbScripts;
private: System::Windows::Forms::Label^			lblScript;

private: System::Windows::Forms::Button^		btnReloadScript;
private: System::Windows::Forms::Label^			lblKeyConfig;
private: System::Windows::Forms::ComboBox^		cbKeyConfig;

private: System::Windows::Forms::Button^		btnReloadConfig;

private: System::Windows::Forms::CheckBox^		chkPause;
private: System::Windows::Forms::ContextMenuStrip^  menuTray;
private: System::Windows::Forms::NotifyIcon^  minimizeIcon;
private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  restoreToolStripMenuItem;
private: System::Windows::Forms::ToolStripMenuItem^  notifyScriptMenu;
private: System::Windows::Forms::ToolStripSeparator^  toolStripSeparator1;


private: System::Windows::Forms::Timer^			updateTimer;

	// Initialize components
	void InitializeComponent(void){
		this->components = (gcnew System::ComponentModel::Container());
		this->txtOutput = (gcnew System::Windows::Forms::RichTextBox());
		this->lblOutput = (gcnew System::Windows::Forms::Label());
		this->btnClearConsole = (gcnew System::Windows::Forms::Button());
		this->cbScripts = (gcnew System::Windows::Forms::ComboBox());
		this->lblScript = (gcnew System::Windows::Forms::Label());
		this->btnReloadScript = (gcnew System::Windows::Forms::Button());
		this->lblKeyConfig = (gcnew System::Windows::Forms::Label());
		this->cbKeyConfig = (gcnew System::Windows::Forms::ComboBox());
		this->btnReloadConfig = (gcnew System::Windows::Forms::Button());
		this->updateTimer = (gcnew System::Windows::Forms::Timer(this->components));
		this->chkPause = (gcnew System::Windows::Forms::CheckBox());
		this->menuTray = (gcnew System::Windows::Forms::ContextMenuStrip(this->components));
		this->notifyScriptMenu = (gcnew System::Windows::Forms::ToolStripMenuItem());
		this->restoreToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
		this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
		this->minimizeIcon = (gcnew System::Windows::Forms::NotifyIcon(this->components));
		this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
		this->menuTray->SuspendLayout();
		this->SuspendLayout();
		// 
		// txtOutput
		// 
		this->txtOutput->BackColor = System::Drawing::SystemColors::Desktop;
		this->txtOutput->Cursor = System::Windows::Forms::Cursors::Arrow;
		this->txtOutput->ForeColor = System::Drawing::SystemColors::Window;
		this->txtOutput->Location = System::Drawing::Point(15, 172);
		this->txtOutput->Name = L"txtOutput";
		this->txtOutput->ReadOnly = true;
		this->txtOutput->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::ForcedVertical;
		this->txtOutput->Size = System::Drawing::Size(382, 246);
		this->txtOutput->TabIndex = 4;
		this->txtOutput->Text = L"";
		// 
		// lblOutput
		// 
		this->lblOutput->AutoSize = true;
		this->lblOutput->BackColor = System::Drawing::SystemColors::Control;
		this->lblOutput->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->lblOutput->Location = System::Drawing::Point(12, 156);
		this->lblOutput->Name = L"lblOutput";
		this->lblOutput->Size = System::Drawing::Size(42, 13);
		this->lblOutput->TabIndex = 1;
		this->lblOutput->Text = L"Output:";
		// 
		// btnClearConsole
		// 
		this->btnClearConsole->Location = System::Drawing::Point(306, 393);
		this->btnClearConsole->Name = L"btnClearConsole";
		this->btnClearConsole->Size = System::Drawing::Size(75, 23);
		this->btnClearConsole->TabIndex = 2;
		this->btnClearConsole->Text = L"Clear";
		this->btnClearConsole->UseVisualStyleBackColor = true;
		this->btnClearConsole->Click += gcnew System::EventHandler(this, &MainForm::btnClearConsole_Click);
		// 
		// cbScripts
		// 
		this->cbScripts->FormattingEnabled = true;
		this->cbScripts->Location = System::Drawing::Point(15, 25);
		this->cbScripts->Name = L"cbScripts";
		this->cbScripts->Size = System::Drawing::Size(301, 21);
		this->cbScripts->TabIndex = 5;
		// 
		// lblScript
		// 
		this->lblScript->AutoSize = true;
		this->lblScript->Location = System::Drawing::Point(12, 9);
		this->lblScript->Name = L"lblScript";
		this->lblScript->Size = System::Drawing::Size(37, 13);
		this->lblScript->TabIndex = 6;
		this->lblScript->Text = L"Script:";
		// 
		// btnReloadScript
		// 
		this->btnReloadScript->Location = System::Drawing::Point(322, 23);
		this->btnReloadScript->Name = L"btnReloadScript";
		this->btnReloadScript->Size = System::Drawing::Size(75, 23);
		this->btnReloadScript->TabIndex = 8;
		this->btnReloadScript->Text = L"Load";
		this->btnReloadScript->UseVisualStyleBackColor = true;
		this->btnReloadScript->Click += gcnew System::EventHandler(this, &MainForm::btnReloadScript_Click);
		// 
		// lblKeyConfig
		// 
		this->lblKeyConfig->AutoSize = true;
		this->lblKeyConfig->Location = System::Drawing::Point(12, 69);
		this->lblKeyConfig->Name = L"lblKeyConfig";
		this->lblKeyConfig->Size = System::Drawing::Size(61, 13);
		this->lblKeyConfig->TabIndex = 9;
		this->lblKeyConfig->Text = L"Key Config:";
		// 
		// cbKeyConfig
		// 
		this->cbKeyConfig->FormattingEnabled = true;
		this->cbKeyConfig->Location = System::Drawing::Point(15, 85);
		this->cbKeyConfig->Name = L"cbKeyConfig";
		this->cbKeyConfig->Size = System::Drawing::Size(301, 21);
		this->cbKeyConfig->TabIndex = 10;
		// 
		// btnReloadConfig
		// 
		this->btnReloadConfig->Location = System::Drawing::Point(322, 83);
		this->btnReloadConfig->Name = L"btnReloadConfig";
		this->btnReloadConfig->Size = System::Drawing::Size(75, 23);
		this->btnReloadConfig->TabIndex = 12;
		this->btnReloadConfig->Text = L"Load";
		this->btnReloadConfig->UseVisualStyleBackColor = true;
		this->btnReloadConfig->Click += gcnew System::EventHandler(this, &MainForm::btnReloadConfig_Click);
		// 
		// updateTimer
		// 
		this->updateTimer->Interval = 30;
		this->updateTimer->Tick += gcnew System::EventHandler(this, &MainForm::updateTimer_Tick);
		// 
		// chkPause
		// 
		this->chkPause->AutoSize = true;
		this->chkPause->Location = System::Drawing::Point(341, 155);
		this->chkPause->Name = L"chkPause";
		this->chkPause->Size = System::Drawing::Size(56, 17);
		this->chkPause->TabIndex = 13;
		this->chkPause->Text = L"Pause";
		this->chkPause->UseVisualStyleBackColor = true;
		// 
		// menuTray
		// 
		this->menuTray->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
			this->restoreToolStripMenuItem,
				this->notifyScriptMenu, this->toolStripSeparator1, this->exitToolStripMenuItem
		});
		this->menuTray->Name = L"menuTray";
		this->menuTray->Size = System::Drawing::Size(153, 98);
		// 
		// notifyScriptMenu
		// 
		this->notifyScriptMenu->Name = L"notifyScriptMenu";
		this->notifyScriptMenu->Size = System::Drawing::Size(152, 22);
		this->notifyScriptMenu->Text = L"Change Script";
		// 
		// restoreToolStripMenuItem
		// 
		this->restoreToolStripMenuItem->Name = L"restoreToolStripMenuItem";
		this->restoreToolStripMenuItem->Size = System::Drawing::Size(152, 22);
		this->restoreToolStripMenuItem->Text = L"Restore";
		// 
		// exitToolStripMenuItem
		// 
		this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
		this->exitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
		this->exitToolStripMenuItem->Text = L"Exit";
		this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
		// 
		// minimizeIcon
		// 
		this->minimizeIcon->ContextMenuStrip = this->menuTray;
		this->minimizeIcon->Text = L"NotifyIcon";
		this->minimizeIcon->Visible = true;
		this->minimizeIcon->DoubleClick += gcnew System::EventHandler(this, &MainForm::minimizeIcon_DoubleClick);
		this->minimizeIcon->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &MainForm::minimizeIcon_MouseClick);
		// 
		// toolStripSeparator1
		// 
		this->toolStripSeparator1->Name = L"toolStripSeparator1";
		this->toolStripSeparator1->Size = System::Drawing::Size(149, 6);
		// 
		// MainForm
		// 
		this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
		this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
		this->ClientSize = System::Drawing::Size(412, 430);
		this->Controls->Add(this->cbKeyConfig);
		this->Controls->Add(this->cbScripts);
		this->Controls->Add(this->btnReloadConfig);
		this->Controls->Add(this->lblKeyConfig);
		this->Controls->Add(this->btnReloadScript);
		this->Controls->Add(this->lblScript);
		this->Controls->Add(this->btnClearConsole);
		this->Controls->Add(this->txtOutput);
		this->Controls->Add(this->lblOutput);
		this->Controls->Add(this->chkPause);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
		this->MaximizeBox = false;
		this->Name = L"MainForm";
		this->Text = L"CKLua";
		this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::formClosed);
		this->Shown += gcnew System::EventHandler(this, &MainForm::MainForm_Shown);
		this->SizeChanged += gcnew System::EventHandler(this, &MainForm::MainForm_SizeChanged);
		this->menuTray->ResumeLayout(false);
		this->ResumeLayout(false);
		this->PerformLayout();

	}
	

	// Private members
	ckLua* lua;
	std::ostringstream* out;
	bool minimizedFirstTime;

private:

	// Finds files in folder and populates combo box
	void populateComboBoxesHelper(ComboBox^ cb, LPCWSTR f, WIN32_FIND_DATA &data) {
		HANDLE h = FindFirstFile(f, &data);

		if (h != INVALID_HANDLE_VALUE)
		{
			do
			{
				char* nPtr = new char[lstrlen(data.cFileName) + 1];
				for (int i = 0; i < lstrlen(data.cFileName); i++)
					nPtr[i] = char(data.cFileName[i]);

				nPtr[lstrlen(data.cFileName)] = '\0';

				cb->Items->Add(gcnew String(nPtr));
				delete nPtr;

			} while (FindNextFile(h, &data));
		}
		else
			std::cout << "Scripts not found" << std::endl;

		FindClose(h);
	}

	// Load all scripts and keyconfigs
	void populateComboBoxes() {
		WIN32_FIND_DATA data;
		populateComboBoxesHelper(this->cbScripts, L"scripts/*.lua", data);
		for each (auto var in this->cbScripts->Items){
			ToolStripMenuItem^ child = gcnew ToolStripMenuItem(var->ToString());
			child->Click += gcnew EventHandler(this, &MainForm::scriptMenu_Click);
			notifyScriptMenu->DropDownItems->Add(child);
		}

		if (cbScripts->Items->Count > 0) {
			cbScripts->SelectedIndex = 0;
		}

		populateComboBoxesHelper(this->cbKeyConfig, L"keymaps/*.keyconf", data);
		if (cbKeyConfig->Items->Count > 0)
			cbKeyConfig->SelectedIndex = 0;
	}

	void loadSelectedScript(){
		if (cbScripts->SelectedIndex == -1) return;
		String^ fileName = "scripts/";
		fileName = String::Concat(fileName, this->cbScripts->Items[this->cbScripts->SelectedIndex]->ToString());

		lua->changeScript(msclr::interop::marshal_as<std::string>(fileName));
	}

	void loadSelectedKeymap(){
		if (cbKeyConfig->SelectedIndex == -1) return;
		// Convert from managed string to standard string and load keymap
		String^ fileName = "keymaps/";
		fileName = String::Concat(fileName, this->cbKeyConfig->Items[this->cbKeyConfig->SelectedIndex]->ToString());

		lua->changeKeyConfig(msclr::interop::marshal_as<std::string>(fileName));
	}
	

	// We need to kill model when exiting
	System::Void formClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
		lua->kill();
		Application::Exit();
	}

	// When pressing clear button
	private: System::Void btnClearConsole_Click(System::Object^  sender, System::EventArgs^  e) {
		txtOutput->Clear();
	}
	
	// Update console on set interval
	private: System::Void updateTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		if (out->str() == "") return;
		
		// Silent?
		if (!chkPause->Checked) {
			txtOutput->AppendText(gcnew String(out->str().c_str()));

			txtOutput->SelectionStart = txtOutput->Text->Length; //Set the current caret position at the end
			txtOutput->ScrollToCaret(); //Now scroll it automatically
		}
		
		out->clear();
		out->str("");
	}
	
	
	System::Void btnReloadScript_Click(System::Object^  sender, System::EventArgs^  e) {
		loadSelectedScript();
	}
	System::Void btnReloadConfig_Click(System::Object^  sender, System::EventArgs^  e) {
		loadSelectedKeymap();
	}
	
	// Used for minimizing to tray
	System::Void MainForm_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
		if (this->WindowState == FormWindowState::Minimized) {
			this->minimizeIcon->Visible = true;

			if (minimizedFirstTime) {
				this->minimizeIcon->ShowBalloonTip(500, L"ckLua", L"Program is still running", ToolTipIcon::Info);
				minimizedFirstTime = false;
			}

			this->ShowInTaskbar = false;
			this->Hide();
		}
		else if (this->WindowState == FormWindowState::Normal) {
			this->minimizeIcon->Visible = false;
			this->ShowInTaskbar = true;
		}
	}
	System::Void minimizeIcon_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
		System::Windows::Forms::MouseEventArgs^ ee = (System::Windows::Forms::MouseEventArgs^)e;
		switch (ee->Button)
		{
		case ::MouseButtons::Left:
			this->Show();
			this->WindowState = FormWindowState::Normal;
			break;
		}

		
	}

	// Used to display context menu
	System::Void minimizeIcon_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		switch (e->Button)
		{
		case ::MouseButtons::Right:
			minimizeIcon->ContextMenuStrip->Show();
			break;
		}
	}
	
	
	// Context menu
	System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Application::Exit();
	}

	// Change script from context menu
	System::Void scriptMenu_Click(System::Object^  sender, System::EventArgs^  e) {
		String^ fileName = "scripts/";
		fileName = String::Concat(fileName, sender->ToString());

		lua->changeScript(msclr::interop::marshal_as<std::string>(fileName));
	}
};
