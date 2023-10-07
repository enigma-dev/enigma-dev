#pragma once
#include <ctime>
#include <stack>
#include <string>
#include <thread>
#include <vector>
#include <functional>
#include <unordered_map>
#include <algorithm> // std::min, std::max

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include "ghc/filesystem.hpp"
#include "filesystem.hpp"

#define IFD_DIALOG_FILE      0
#define IFD_DIALOG_DIRECTORY 1
#define IFD_DIALOG_SAVE      2

#define IFD_DIALOG_WIDTH     (int)((!ngs::fs::environment_get_variable("IMGUI_DIALOG_WIDTH" ).empty()) ? strtoul(ngs::fs::environment_get_variable("IMGUI_DIALOG_WIDTH" ).c_str(), nullptr, 10) : 640)
#define IFD_DIALOG_HEIGHT    (int)((!ngs::fs::environment_get_variable("IMGUI_DIALOG_HEIGHT").empty()) ? strtoul(ngs::fs::environment_get_variable("IMGUI_DIALOG_HEIGHT").c_str(), nullptr, 10) : 360)

namespace ifd {
  class FileDialog {
  public:
    static inline FileDialog& Instance() {
      static FileDialog ret;
      return ret;
    }

    FileDialog();
    ~FileDialog();

    bool Save(const std::string& key, const std::string& title, const std::string& filter, const std::string& startingFile = "", const std::string& startingDir = "");

    bool Open(const std::string& key, const std::string& title, const std::string& filter, bool isMultiselect = false, const std::string& startingFile = "", const std::string& startingDir = "");

    bool IsDone(const std::string& key);

    inline bool HasResult() { return m_result.size(); }
    inline const ghc::filesystem::path& GetResult() { return m_result[0]; }
    inline const std::vector<ghc::filesystem::path>& GetResults() { return m_result; }

    void Close();

    void RemoveFavorite(std::string path);
    void AddFavorite(std::string path);
    inline const std::vector<std::string>& GetFavorites() { return m_favorites; }

    inline void SetZoom(float z) { 
      m_zoom = std::min<float>(25.0f, std::max<float>(1.0f, z)); 
      m_refreshIconPreview();
    }
    inline float GetZoom() { return m_zoom; }

    std::function<void*(uint8_t*, int, int, char)> CreateTexture; // char -> fmt -> { 0 = BGRA, 1 = RGBA }
    std::function<void(void*)> DeleteTexture;

    class FileTreeNode {
    public:
      #ifdef _WIN32
      FileTreeNode(const std::wstring& path) {
        Path = ghc::filesystem::path(path);
        Read = false;
      }
      #endif

      FileTreeNode(const std::string& path) {
        Path = ghc::filesystem::path(path);
        Read = false;
      }

      ghc::filesystem::path Path;
      bool Read;
      std::vector<FileTreeNode*> Children;
    };
    class FileData {
    public:
      FileData(const ghc::filesystem::path& path);

      ghc::filesystem::path Path;
      bool IsDirectory;
      size_t Size;
      time_t DateModified;

      bool HasIconPreview;
      void* IconPreview;
      uint8_t* IconPreviewData;
      int IconPreviewWidth, IconPreviewHeight;
    };

  private:
    std::string m_currentKey;
    std::string m_currentTitle;
    ghc::filesystem::path m_currentDirectory;
    bool m_isMultiselect;
    bool m_isOpen;
    uint8_t m_type;
    char m_inputTextbox[1024];
    char m_pathBuffer[1024];
    char m_newEntryBuffer[1024];
    char m_searchBuffer[128];
    std::vector<std::string> m_favorites;
    bool m_calledOpenPopup;
    std::stack<ghc::filesystem::path> m_backHistory, m_forwardHistory;
    float m_zoom;

    std::vector<ghc::filesystem::path> m_selections;
    int m_selectedFileItem;
    void m_select(const ghc::filesystem::path& path, bool isCtrlDown = false);

    std::vector<ghc::filesystem::path> m_result;
    bool m_finalize(const std::string& filename = "");

    std::string m_filter;
    std::vector<std::vector<std::string>> m_filterExtensions;
    size_t m_filterSelection;
    void m_parseFilter(const std::string& filter);

    std::vector<int> m_iconIndices;
    std::vector<std::string> m_iconFilepaths; // m_iconIndices[x] <-> m_iconFilepaths[x]
    std::unordered_map<std::string, void *> m_icons;
    void *m_getIcon(const ghc::filesystem::path& path);
    void m_clearIcons();
    void m_refreshIconPreview();
    void m_clearIconPreview();

    std::thread* m_previewLoader;
    bool m_previewLoaderRunning;
    void m_stopPreviewLoader();
    void m_loadPreview();

    std::vector<FileTreeNode*> m_treeCache;
    void m_clearTree(FileTreeNode* node);
    void m_renderTree(FileTreeNode* node);

    unsigned int m_sortColumn;
    unsigned int m_sortDirection;
    std::vector<FileData> m_content;
    void m_setDirectory(ghc::filesystem::path p, bool addHistory = true, bool clearFileName = true);
    void m_sortContent(unsigned int column, unsigned int sortDirection);
    void m_renderContent();

    void m_renderPopups();
    void m_renderFileDialog();
  };
}
