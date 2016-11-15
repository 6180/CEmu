#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QShortcut>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QFileDialog>
#include <QtCore/QSettings>
#include <QtGui/QTextCursor>

#include "cemuopts.h"
#include "lcdwidget.h"
#include "romselection.h"
#include "emuthread.h"
#include "lcdpopout.h"
#include "keypad/qtkeypadbridge.h"
#include "qhexedit/qhexedit.h"

#include "../../core/vat.h"
#include "../../core/debug/debug.h"
#include "../../core/debug/disasm.h"
#include "../../core/debug/profiler.h"

enum breakpointIndex {
    BREAK_LABEL_LOC=0,
    BREAK_ADDR_LOC,
    BREAK_ENABLE_LOC
};

enum watchpointIndex {
    WATCH_LABEL_LOC=0,
    WATCH_ADDR_LOC,
    WATCH_SIZE_LOC,
    WATCH_VALUE_LOC,
    WATCH_READ_LOC,
    WATCH_WRITE_LOC
};

enum profilerIndex {
    PROFILE_LABEL_LOC=0,
    PROFILE_ADDR_LOC,
    PROFILE_SIZE_LOC,
    PROFILE_CYCLE_LOC
};

enum portIndex {
    PORT_ADDR_LOC=0,
    PORT_VALUE_LOC,
    PORT_READ_LOC,
    PORT_WRITE_LOC,
    PORT_FREEZE_LOC
};

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(CEmuOpts opts,QWidget *p = Q_NULLPTR);
    ~MainWindow();

public slots:
    // Misc.
    virtual void closeEvent(QCloseEvent*) Q_DECL_OVERRIDE;

    // Drag & Drop
    virtual void dropEvent(QDropEvent*) Q_DECL_OVERRIDE;
    virtual void dragEnterEvent(QDragEnterEvent*) Q_DECL_OVERRIDE;

    // Console
    void consoleStr(QString);
    void consoleErrStr(QString);

    // Saved/Restored State
    void saved(bool);
    void started(bool);
    void restored(bool);

    // Other
    void isBusy(bool busy);
    bool restoreEmuState();
    void saveEmuState();
    void restoreFromFile();
    void saveToFile();
    void exportRom();
    void setImagePath();

    // Debugging
    void debuggerGUIDisable();
    void debuggerGUIEnable();

    // Sending keys
    void sendASMKey();

    // LCD Popouts
    void createLCD();

signals:
    // Debugging
    void debuggerSendNewState(bool);
    void triggerEmuSendState();
    void debugInputRequested();
    void debuggerCommand(QString);
    void setDebugStepInMode();
    void setDebugStepOverMode();
    void setDebugStepNextMode();
    void setDebugStepOutMode();
    void setRunUntilMode();

    // Linking
    void setSendState(bool);
    void sendVariable(std::string);
    void setReceiveState(bool);

    // Speed
    void setEmuSpeed(int);
    void changedThrottleMode(bool);

    // Reset
    void resetTriggered();

private:
    // Save/Restore
    void saveToPath(QString path);
    bool restoreFromPath(QString path);

    // Actions
    bool runSetup(void);
    void screenshot(void);
    void screenshotGIF(void);
    void screenshotSave(QString, QString, QString);
    void recordGIF(void);
    void changeFrameskip(int);
    void changeFramerate(void);
    void checkForUpdates(bool);
    void showAbout(void);
    void batteryIsCharging(bool);
    void batteryChangeStatus(int);
    void setSaveOnClose(bool b);
    void setRestoreOnOpen(bool b);
    void changeSnapshotPath();

    // Debugger
    void breakpointGUIAdd();
    void watchpointGUIAdd();
    void debuggerGUIPopulate();
    void debuggerGUISetState(bool);

    void debugCommand();
    void debuggerRaise();
    void debuggerLeave();
    void debuggerUpdateChanges();
    void debuggerChangeState();
    void debuggerExecuteCommand(uint32_t, uint8_t);
    void debuggerProcessCommand(int, uint32_t);

    void portRemoveSelected();

    void portUpdate(int);
    void watchpointUpdate(int);
    void profilerUpdate(int);

    void portSetPreviousAddress(QTableWidgetItem*);
    void breakpointSetPreviousAddress(QTableWidgetItem*);
    void watchpointSetPreviousAddress(QTableWidgetItem*);

    void portDataChanged(QTableWidgetItem*);
    void breakpointDataChanged(QTableWidgetItem*);
    void watchpointDataChanged(QTableWidgetItem*);
    void profilerDataChange(QTableWidgetItem*);

    void updateDisasmView(const int, const bool);
    void drawNextDisassembleLine();
    void scrollDisasmView(int);

    void stepInPressed();
    void stepOverPressed();
    void stepNextPressed();
    void stepOutPressed();

    void updateTIOSView();
    void updateStackView();

    void gotoPressed();

    void disasmContextMenu(const QPoint &);
    void variablesContextMenu(const QPoint&);
    void vatContextMenu(const QPoint &);
    void opContextMenu(const QPoint &);

    void breakpointRemoveAddress(uint32_t);
    void watchpointRemoveAddress(uint32_t);

    void debuggerZeroClockCounter();
    void debuggerTabSwitched(int);

    void profilerZero();
    void profilerExport();
    void profilerRemoveAll();
    void profilerChangeGranularity(int);

    // For linking to the buttons
    void breakpointSlotAdd();
    void watchpointSlotAdd();
    void profilerSlotAdd();
    void portSlotAdd();

    // Removal from widgets
    bool profilerRemoveSelected();
    bool breakpointRemoveSelectedRow();
    bool watchpointRemoveSelectedRow();

    // Get labels
    QString watchpointNextLabel();
    QString breakpointNextLabel();
    QString profilerNextLabel();

    // Adding watchpoints from disassembly
    void watchpointReadGUIAdd();
    void watchpointWriteGUIAdd();
    void watchpointReadWriteGUIAdd();

    // Debugging files
    void debuggerImportFile();
    void debuggerExportFile();

    // MAIN IMPLEMENTATION ROUTINES
    bool portAdd(uint16_t, unsigned);
    bool breakpointAdd(QString, uint32_t, bool);
    bool watchpointAdd(QString, uint32_t, uint8_t, unsigned);
    bool profilerAdd(QString, uint32_t, uint32_t, uint64_t);

    void screenContextMenu(const QPoint &);
    void equatesAddDialog();
    void equatesAddFile(QString);
    void equatesClear();
    void equatesRefresh();
    void selectKeypadColor();
    void setKeypadColor(unsigned color);

    // Speed
    void setEmulatedSpeed(int);
    void setThrottleMode(int);
    void showActualSpeed(int);

    // Console
    void showStatusMsg(QString);
    void consoleOutputChanged();
    void consoleAppend(QString str, QColor color = Qt::black);

    // Settings
    void adjustScreen();
    void setSkinToggle(bool);
    void setLCDScale(int);
    void setLCDRefresh(int);
    void setAlwaysOnTop(int);
    void setAutoCheckForUpdates(int);
    void setSpaceDisasm(bool);
    void setUIMode(bool);
    void toggleUIEditMode(void);
    int setReprintScale(int);

    // Linking
    QStringList showVariableFileDialog(QFileDialog::AcceptMode, QString name_filter);
    void selectFiles();
    void refreshVariableList();
    void variableClicked(QTableWidgetItem*);
    void saveSelected();

    // Autotester
    void dispAutotesterError(int errCode);
    int openJSONConfig(const QString& jsonPath);
    void prepareAndOpenJSONConfig();
    void reloadJSONConfig();
    void launchTest();
    void updateCRCParamsFromPreset(int comboBoxIndex);
    void refreshCRC();

    // Hex Editor
    void flashUpdate();
    void flashGotoPressed();
    void flashSearchPressed();
    void flashSyncPressed();
    void ramUpdate();
    void ramGotoPressed();
    void ramSearchPressed();
    void ramSyncPressed();
    void memUpdate(uint32_t);
    void memGoto(QString address);
    void memGotoPressed();
    void memSearchPressed();
    void memSyncPressed();
    // Others
    void syncHexView(int, QHexEdit *);
    void searchEdit(QHexEdit *);

    // Keypad
    void keymapChanged();
    void setKeymap(const QString &);

    // Font
    void setFont(int);

    // Reset
    void reloadROM();
    void resetCalculator();

#ifdef _WIN32
    // Win32 Console Toggle
    void toggleConsole();
    void installToggleConsole();
#endif

    // Members
    unsigned watchpointGUIMask = DBG_NO_HANDLE;

    QString getAddressString(QString);
    QString searchingString;

    Ui::MainWindow *ui = nullptr;
    QtKeypadBridge keypadBridge{this};
    QLabel statusLabel;
    QSettings *settings = nullptr;
    QDockWidget *debuggerDock = nullptr;
    QTextCursor disasmOffset;
    bool disasmOffsetSet;
    bool fromPane;
    int32_t addressPane;
    int memSize;

    QDir currentDir;
    QString currentEquateFile;
    EmuThread emu;

    bool uiEditMode = false;
    bool nativeConsole = false;
    bool closeAfterSave = false;
    bool isResumed = false;
    bool hexSearch = true;
    bool canScroll = false;
    bool usingLoadedImage = false;
    CEmuOpts opts;

    uint32_t prevBreakpointAddress = 0;
    uint32_t prevWatchpointAddress = 0;
    uint32_t prevDisasmAddress = 0;
    uint16_t prevPortAddress = 0;
    QPalette colorback, nocolorback;

    QShortcut *stepInShortcut;
    QShortcut *stepOverShortcut;
    QShortcut *stepNextShortcut;
    QShortcut *stepOutShortcut;
    QShortcut *debuggerShortcut;
    QShortcut *asmShortcut;

    QList<calc_var_t> vars;
    QIcon runIcon, stopIcon; // help speed up stepping
    QTextCharFormat consoleFormat;
};

#endif
