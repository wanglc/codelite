//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: continousbuildbasepane.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef CONTINOUSBUILDBASEPANE_BASE_CLASSES_H
#define CONTINOUSBUILDBASEPANE_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/panel.h>
#include <wx/artprov.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/stattext.h>

class ContinousBuildBasePane : public wxPanel
{
protected:
    wxCheckBox* m_checkBox1;
    wxButton* m_buttonCancel;
    wxListBox* m_listBoxQueue;
    wxStaticText* m_staticText4;
    wxListBox* m_listBoxFailedFiles;

protected:
    virtual void OnEnableCB(wxCommandEvent& event) { event.Skip(); }
    virtual void OnStopAll(wxCommandEvent& event) { event.Skip(); }
    virtual void OnStopUI(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnEnableContBuildUI(wxUpdateUIEvent& event) { event.Skip(); }

public:
    wxCheckBox* GetCheckBox1() { return m_checkBox1; }
    wxButton* GetButtonCancel() { return m_buttonCancel; }
    wxListBox* GetListBoxQueue() { return m_listBoxQueue; }
    wxStaticText* GetStaticText4() { return m_staticText4; }
    wxListBox* GetListBoxFailedFiles() { return m_listBoxFailedFiles; }
    ContinousBuildBasePane(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(389,188), long style = wxTAB_TRAVERSAL);
    virtual ~ContinousBuildBasePane();
};

#endif
