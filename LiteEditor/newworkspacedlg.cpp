//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : newworkspacedlg.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "newworkspacedlg.h"
#include "windowattrmanager.h"
#include "globals.h"
#include "wx/msgdlg.h"
#include "wx/dirdlg.h"
#include "wx/filename.h"

NewWorkspaceDlg::NewWorkspaceDlg(wxWindow* parent)
    : NewWorkspaceBase(parent)
{
    m_textCtrlWorkspacePath->SetValue(wxGetCwd());
    m_textCtrlWorkspaceName->SetFocus();
    Centre();
    WindowAttrManager::Load(this, wxT("NewWorkspaceDlg"), NULL);
}

NewWorkspaceDlg::~NewWorkspaceDlg() { WindowAttrManager::Save(this, wxT("NewWorkspaceDlg"), NULL); }

void NewWorkspaceDlg::OnWorkspacePathUpdated(wxCommandEvent& event)
{
    // update the static text control with the actual path

    wxString workspacePath;
    workspacePath << m_textCtrlWorkspacePath->GetValue();

    workspacePath = workspacePath.Trim().Trim(false);

    wxString tmpSep(wxFileName::GetPathSeparator());
    if(!workspacePath.EndsWith(tmpSep) && workspacePath.IsEmpty() == false) {
        workspacePath << wxFileName::GetPathSeparator();
    }

    if(m_textCtrlWorkspaceName->GetValue().Trim().Trim(false).IsEmpty()) {
        m_staticTextWorkspaceFileName->SetLabel(wxEmptyString);
        return;
    }

    if(m_checkBoxCreateSeparateDir->IsChecked()) {
        workspacePath << m_textCtrlWorkspaceName->GetValue();
        workspacePath << wxFileName::GetPathSeparator();
    }

    workspacePath << m_textCtrlWorkspaceName->GetValue();
    workspacePath << wxT(".workspace");
    m_staticTextWorkspaceFileName->SetLabel(workspacePath);
}

void NewWorkspaceDlg::OnWorkspaceDirPicker(wxCommandEvent& event)
{
    const wxString& dir = wxDirSelector(_("Choose a folder:"));
    if(!dir.empty()) {

        static wxString INVALID_CHARS = " ,'()";
        if(dir.find_first_of(INVALID_CHARS) != wxString::npos) {
            int answer = ::wxMessageBox(wxString() << _("The selected project path '") << dir
                                                   << _("'\nContains some invalid characters\nContinue anyways?"),
                                        "CodeLite",
                                        wxYES_NO | wxCANCEL | wxICON_WARNING,
                                        this);
            if(answer != wxYES) {
                return;
            }
        }
        
        // Use SetValue to ensure that an TEXT_UPDATE event is fired
        m_textCtrlWorkspacePath->SetValue(dir);
    }
}

void NewWorkspaceDlg::OnButtonCreate(wxCommandEvent& event)
{
    // validate that the path part is valid
    m_workspacePath = m_staticTextWorkspaceFileName->GetLabel();
    wxFileName fn(m_workspacePath);

    if(m_checkBoxCreateSeparateDir->IsChecked()) {
        // dont test the result
        fn.Mkdir(fn.GetPath(), 0777, wxPATH_MKDIR_FULL);
    }

    if(!wxDirExists(fn.GetPath())) {
        wxMessageBox(_("Invalid path: ") + fn.GetPath(), _("Error"), wxOK | wxICON_HAND);
        return;
    }
    EndModal(wxID_OK);
}
void NewWorkspaceDlg::OnOKUI(wxUpdateUIEvent& event)
{
    event.Enable(!m_textCtrlWorkspaceName->IsEmpty());
}
