#include "wordcompletion.h"
#include <wx/xrc/xmlres.h>
#include "WordCompletionThread.h"
#include <wx/stc/stc.h>
#include "clKeyboardManager.h"
#include <wx/app.h>
#include "WordCompletionSettingsDlg.h"
#include "event_notifier.h"
#include "cl_command_event.h"
#include "wxCodeCompletionBoxManager.h"
#include "WordCompletionDictionary.h"

static WordCompletionPlugin* thePlugin = NULL;

// Define the plugin entry point
extern "C" EXPORT IPlugin* CreatePlugin(IManager* manager)
{
    if(thePlugin == 0) {
        thePlugin = new WordCompletionPlugin(manager);
    }
    return thePlugin;
}

extern "C" EXPORT PluginInfo GetPluginInfo()
{
    PluginInfo info;
    info.SetAuthor(wxT("Eran Ifrah"));
    info.SetName(wxT("Word Completion"));
    info.SetDescription(wxT("Suggest completion based on words typed in the editor"));
    info.SetVersion(wxT("v1.0"));
    return info;
}

extern "C" EXPORT int GetPluginInterfaceVersion() { return PLUGIN_INTERFACE_VERSION; }

WordCompletionPlugin::WordCompletionPlugin(IManager* manager)
    : IPlugin(manager)
{
    m_longName = wxT("Suggest completion based on words typed in the editor");
    m_shortName = wxT("Word Completion");

    wxTheApp->Bind(wxEVT_MENU, &WordCompletionPlugin::OnWordComplete, this, XRCID("text_word_complete"));
    wxTheApp->Bind(wxEVT_MENU, &WordCompletionPlugin::OnWordComplete, this, XRCID("word_complete_no_single_insert"));
    wxTheApp->Bind(wxEVT_MENU, &WordCompletionPlugin::OnSettings, this, XRCID("text_word_complete_settings"));

    m_dictionary = new WordCompletionDictionary();

    clKeyboardManager::Get()->AddGlobalAccelerator(
        "text_word_complete", "Ctrl-ENTER", "Plugins::Word Completion::Show word completion");
}

WordCompletionPlugin::~WordCompletionPlugin() {}

clToolBar* WordCompletionPlugin::CreateToolBar(wxWindow* parent)
{
    wxUnusedVar(parent);
    clToolBar* tb(NULL);
    return tb;
}

void WordCompletionPlugin::CreatePluginMenu(wxMenu* pluginsMenu)
{
    wxMenu* menu = new wxMenu;
    menu->Append(XRCID("text_word_complete"), _("Show Word Completion"));
    menu->AppendSeparator();
    menu->Append(XRCID("text_word_complete_settings"), _("Settings"));
    pluginsMenu->Append(wxID_ANY, GetShortName(), menu);
}

void WordCompletionPlugin::UnPlug()
{
    wxDELETE(m_dictionary);
    wxTheApp->Unbind(wxEVT_MENU, &WordCompletionPlugin::OnWordComplete, this, XRCID("text_word_complete"));
    wxTheApp->Unbind(wxEVT_MENU, &WordCompletionPlugin::OnWordComplete, this, XRCID("word_complete_no_single_insert"));
    wxTheApp->Unbind(wxEVT_MENU, &WordCompletionPlugin::OnSettings, this, XRCID("text_word_complete_settings"));
}

void WordCompletionPlugin::OnWordComplete(wxCommandEvent& event)
{
    event.Skip();
    IEditor* activeEditor = m_mgr->GetActiveEditor();
    CHECK_PTR_RET(activeEditor);

    // Build the suggetsion list
    wxString suggestString;
    wxCodeCompletionBoxEntry::Vec_t entries;
    wxCodeCompletionBox::BmpVec_t bitmaps;
    bitmaps.push_back(m_images.Bitmap("m_bmpWord"));

    WordCompletionSettings settings;
    settings.Load();

    // Filter (what the user has typed so far)
    wxStyledTextCtrl* stc = activeEditor->GetSTC();
    int curPos = stc->GetCurrentPos();
    int start = stc->WordStartPosition(stc->GetCurrentPos(), true);
    if(curPos <= start) return;

    wxString filter = stc->GetTextRange(start, curPos);
    wxString lcFilter = filter.Lower();

    wxStringSet_t words = m_dictionary->GetWords();
    wxStringSet_t filterdSet;
    for(wxStringSet_t::iterator iter = words.begin(); iter != words.end(); ++iter) {
        wxString word = *iter;
        wxString lcWord = word.Lower();
        if(settings.GetComparisonMethod() == WordCompletionSettings::kComparisonStartsWith) {
            if(lcWord.StartsWith(lcFilter) && filter != word) {
                filterdSet.insert(word);
            }
        } else {
            if(lcWord.Contains(lcFilter) && filter != word) {
                filterdSet.insert(word);
            }
        }
    }

    for(wxStringSet_t::iterator iter = filterdSet.begin(); iter != filterdSet.end(); ++iter) {
        entries.push_back(wxCodeCompletionBoxEntry::New(*iter, 0));
    }

    wxCodeCompletionBoxManager::Get().ShowCompletionBox(activeEditor->GetSTC(),
                                                        entries,
                                                        bitmaps,
                                                        event.GetId() == XRCID("text_word_complete") ?
                                                            wxCodeCompletionBox::kInsertSingleMatch :
                                                            wxCodeCompletionBox::kNone);
}

void WordCompletionPlugin::OnSettings(wxCommandEvent& event)
{
    WordCompletionSettingsDlg dlg(EventNotifier::Get()->TopFrame());
    dlg.ShowModal();
}
