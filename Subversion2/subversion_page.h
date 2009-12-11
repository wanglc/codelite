#ifndef __subversion_page__
#define __subversion_page__

/**
@file
Subclass of SubversionPageBase, which is generated by wxFormBuilder.
*/

#include "subversion2_ui.h"
#include "svncommand.h"
#include "svntreedata.h"

class Subversion2;
class wxMenu;

class SvnPageSelectionInfo {

public:
	wxArrayString            m_paths;
	SvnTreeData::SvnNodeType m_selectionType;

public:
	SvnPageSelectionInfo() : m_selectionType(SvnTreeData::SvnNodeTypeInvalid) {}
	~SvnPageSelectionInfo() {}

	void Clear() {
		m_paths.Clear();
		m_selectionType = SvnTreeData::SvnNodeTypeInvalid;
	}
};

/** Implementing SubversionPageBase */
class SubversionPage : public SubversionPageBase
{
	Subversion2*         m_plugin;
	wxTreeItemId         m_modifiedItems;
	SvnCommand           m_simpleCommand;
	SvnPageSelectionInfo m_selectionInfo;

protected:
	// Handlers for SubversionPageBase events.
	void                     OnChangeRootDir ( wxCommandEvent& event );
	void                     OnTreeMenu      ( wxTreeEvent&    event );
	void                     CreatGUIControls();
	void                     ClearAll();
	void                     DoAddNode(const wxString &title, int imgId, SvnTreeData::SvnNodeType nodeType, const wxArrayString &files);
	wxString                 DoGetSvnExeName();
	int                      DoGetIconIndex(const wxString &filename);
	SvnTreeData::SvnNodeType DoGetSelectionType(const wxArrayTreeItemIds &items);
	void                     DoGetPaths(const wxTreeItemId &parent, wxArrayString &paths);
protected:
	// Menu management
	void CreateFileMenu(wxMenu *menu);
	void CreateRootMenu(wxMenu *menu);
	void CreateSecondRootMenu(wxMenu *menu);

protected:

	// Events
	void OnWorkspaceLoaded(wxCommandEvent &event);
	void OnWorkspaceClosed(wxCommandEvent &event);

	// Svn events
	void OnCommit         (wxCommandEvent &event);
	void OnUpdate         (wxCommandEvent &event);
	void OnRevert         (wxCommandEvent &event);
	void OnAdd            (wxCommandEvent &event);

public:
	/** Constructor */
	SubversionPage( wxWindow* parent, Subversion2 *plugin);

	void UpdateTree(const wxArrayString& modifiedFiles, const wxArrayString &conflictedFiles, const wxArrayString &unversionedFiles, const wxArrayString& newFiles, const wxArrayString& deletedFiles);
	void BuildTree();
};

#endif // __subversion_page__
