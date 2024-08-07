#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <qevent.h>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QProgressDialog>
#include <QTranslator>
#include <fstream>
#include <QVector2D>
#include <QVector3D>
#include <QFile>
#include <QTimer>
#include <QCursor>
#include "json.hpp"
#include "ui_CncTools.h"
#include "CoordManager.h"
#include "CncGraphicsScene.h"
#include "Helpers.h"
#include "Config.h"
#include "SceneZoom.h"
#include "ToolTranslation.h"
#include "ToolEnqueue.h"

using json = nlohmann::json;

class CncTools : public QMainWindow
{
    Q_OBJECT

public:
    /********** CONSTRUCTOR **********/

    /*
     * Constructs the CncTools object.
     *
     * @param	QWidget*	parent	- Parent widget
     */
    CncTools(QWidget* parent = Q_NULLPTR);


    /********** DESTRUCTOR **********/

    /*
     * Destructs the CncTools object.
     */
    ~CncTools();


    /********** OVERRIDES **********/

    /*
     * Overrides the QMainWindow resizeEvent() called when the window is resized.
     *
     * @param   QResizeEvent*    event
     *
     * @return void
     */
    void resizeEvent(QResizeEvent* event) override;

    /*
     * Overrides the QMainWindow showEvent() called when the window is shown.
     *
     * @param   QShowEvent*    event
     *
     * @return void
     */
    void showEvent(QShowEvent* event) override;

    /********** PUBLIC FUNCTIONS **********/

    /*
     * Changes the zoom level.
     *
     * @return void
     */
    void zoomLevel();


    /********** INIT FUNCTIONS **********/

    /*
     * Reads the config file and sets the corresponding checkboxes.
     *
     * @return void
     */
    void initOptions();


    /********** UPDATE FUNCTIONS **********/

    /*
     * Updates the value of config file relative to the provided key.
     *
     * @param   const std::string&      key    - Name of the option to change.
     * @param   const std::string&      value  - Value to set.
     *
     * @return void
     */
    void updateOptions(const std::string& key, const std::string& value);

    /*
     * Toggles the setting value for the fit option inside the config file.
     *
     * @return void
     */
    void toggleFit();

    /*
     * Toggles the setting value for the autoresize option inside the config file.
     *
     * @return void
     */
    void toggleAutoresize();

    /*
     * Toggles the setting value for the color option inside the config file.
     *
     * @return void
     */
    void toggleColor();

    /*
     * Toggles the setting value for the gradient option inside the config file.
     *
     * @return void
     */
    void toggleGradient();

    /*
     * Toggles the setting value for the z max option inside the config file.
     *
     * @return void
     */
    void toggleZMax();

    /*
     * Updates the last folder used to load a file.
     *
     * @param   const QString&      f      - Path to the folder
     *
     * @return void
     */
    void updateFolder(const QString& f);

    /*
     * Updates the language to use for the UI.
     *
     * @param   const QString&      l      - Language code (it, en...)
     *
     * @return void
     */
    void updateLanguage(const QString& l);


    /********** ACCESSORS **********/

    /*
     * Gets and shows the mouse pointer coordinates.
     *
     * @param   const QPointF&     pos - Mouse position as point
     *
     * @return void
     */
    void mousePosition(const QPointF& pos);

    /*
     * Gets the code of the key pressed and triggers the appropriate action to perform.
     *
     * @param   const int&          key - Code of the key pressed
     *
     * @return void
     */
    void keyPressed(const int& key);

    /*
     * Gets the visible size of the canvas, applying a compensation
     * for the px difference introduced by the antialiasing if necessary.
     *
     * @return QVector2D - The size of the canvas
     */
    QVector2D getCanvasSize();


    /********** MODIFIERS **********/


    /********** RESETTERS **********/

    /*
     * Resets all the input fields, the errors  and the scene.
     *
     * @return void
     */
    void fullReset();

    /*
     * Resets the input fields, the selected files label and the status bar to
     * remove eventual error messages and relative highlighting.
     *
     * @return void
     */
    void resetErrors();

    /*
     * Resets the scene to the initial state.
     *
     * @param   bool    reset_to_draw - If false also reset the selected files, defaults to false.
     *
     * @return void
     */
    void resetScene(bool reset_to_draw = false);

    /*
     * Resets the both the scene and the coordinates' limits.
     *
     * @return void
     */
    void cancelDrawing();


    /********** OTHERS **********/

    /*
     * Opens the file browser.
     *
     * @return void
     */
    void browseFile();

    /*
     * Calculates the scale factor to resize the drawing to fit the scene size.
     *
     * @param   int    w    - Width of the slab in use for the job.
     * @param   int    h    - Height of the slab in use for the job.
     *
     * @return True if the data are correct, false otherwise.
     */
    float scaleFactor(int w, int h);

    /*
     * Set the scen to contain the drawing just elaborated.
     *
     * @return void
     */
    void setScene();

    /*
     * Estimates the working time in seconds.
     *
     * @param const float&     eng_dst     - Engraving distance
     * @param const float&     pos_dst     - Repositioning distance
     *
     * @return int  The estimated time in seconds.
     */
    int workingTime(const float& eng_dst, const float& pos_dst);

    /*
     * Maps a value from a range to another.
     * Here it is used to convert a depth value to a color, the deeper is the engraving, the higher is the color value.
     *
     * @param const float&     value          - Value to be converted.
     * @param const float&     source_min     - Source range min value.
     * @param const float&     source_max     - Source range max value.
     * @param const float&     target_min     - Target range min value.
     * @param const float&     target_max     - Target range max value.
     *
     * @return int  The mapped value.
     */
    int mapRange(const float& value, const float& source_min, const float& source_max, const float& target_min, const float& target_max);

    /*
     * Displays the drawing.
     *
     * @return void
     */
    void draw();

protected:
    /*
     * Overrides the QMainWindow changeEvent() called when there is a change in the window.
     * Here it is used to manage the language changes.
     *
     * @param   QEvent*    event
     *
     * @return void
     */
    void changeEvent(QEvent* event) override;

protected slots:
    /*
     * This slot is called by the language menu actions to translate the UI
     *
     * @param   QAction*    action
     *
     * @return void
     */
    void slotLanguageChanged(QAction* action);

    /*
     * This slot is called by the Tool's Translate Coords menu actions to activate the coordinates' translation tool
     *
     * @return void
     */
    void slotToolsTranslateCoords();

    /*
     * This slot is called by the Tool's Enqueue PGR files menu actions to activate the enqueue tool
     *
     * @return void
     */
    void slotToolsEnqueuePgr();

private:
    // Radius of the circle to highlight the max depth points
    const int ZMAX_CIRCLE_RADIUS = 7;

    // Code of the language to use
    QString language;

    // List of strings containing the selected ISO files
    QStringList iso_files;
    // Scale factor to use to resize the drawing
    float scale_factor = 1;

    // Scene width
    int scene_w = 0;
    // Scene height
    int scene_h = 0;

    // Name of the configuration file
    const std::string config_file = "config.cfg";

    const std::string default_folder = "C:/helios1/archivio";

    // Folder to open when clicking the "Load Files" button, initialized with the value contained into
    // the config file and updated each time a file is loaded from a different folder to always open
    // the last used folder.
    std::string folder;

    // By setting the antialiasing for the canvas, active by default,
    // the canvas area is extended by 4 pixels over the visible area (2px around), and the mouse position
    // is offset by 4px, this will compensate this difference.
    // Must be set to 0 if the antialiasing is not in use.
    int canvas_expanded = 2;

    // Last Z position, to be updated for each segment, this will be used to know
    // how much the tool must be raised to leave the working surface
    float last_z = 0;

    // Says whether anything was drawn or not
    bool iso_drawn = false;

    // Says whether the timer to delay the drawing regeneration is active or not
    bool resize_timer_running = false;
    // Says whether the timer must be used or not
    bool delay_enabled = true;
    // Delay in milliseconds;
    int delay_timeout = 500;

    // Contains the full string of the selected files, only their names
    // If needed there will be another variable containing a cut string returned by elideText()
    QString selected_files_full_string = "";

    // Timer to redraw upon window resize
    QTimer* reset_timer;

    // Scene where the drawing will be displayed
    CncGraphicsScene* scene;

    // Configuration manager
    Config* config;

    SceneZoom* scene_zoom;

    // Coordinates manager
    CoordManager* coord_manager;

    // Translation tool instance
    ToolTranslation* tool_translation;

    // Enqueue tool instance
    ToolEnqueue* tool_enqueue;

    // User interface
    Ui::CncToolsClass ui;

    // Application root folder
    const QString m_application_root = QApplication::applicationDirPath();

    // Contains the translations for this application
    QTranslator m_translator;

    // Contains the translations for qt
    QTranslator m_translatorQt;

    // Contains the currently loaded language
    QString m_currLang = "";

    // Name of the folder containing the language files. This is always "languages".
    const QString m_langFolder = "languages";

    // Full path to the folder containing the language files.
    const QString m_langFullPath = m_application_root + "/" + m_langFolder;

    QList<QVector3D> coords;

    /********** PRIVATE FUNCTIONS **********/

    /*
     * Validates the data.
     *
     * @return True if the inserted data are correct, false otherwise.
     */
    bool checkData();

    /*
     * Loads a language by the given language shortcut
     *
     * @param   const QString&      rLanguage     - The language code (e.g. it, en)
     *
     * @return void
     */
    void loadLanguage(const QString& rLanguage);

    /*
     * Creates the language menu dynamically from the content of m_langPath
     *
     * @return void
     */
    void createLanguageMenu();

    /*
     * Updates the translator to use according to the selected language
     *
     * @param   QTranslator&        translator
     * @param   const QString&      filename
     *
     * @return void
     */
    void switchTranslator(QTranslator& translator, const QString& filename);

    /*
     * Moves the mouse pointer to the given coordinates over the QGraphicsScene
     *
     * @param   const float   x
     * @param   const float   y
     *
     * @return void
     */
    void moveMousePointerToClosestPoint(const float x, const float y);

    /*
     * Closes the main window.
     *
     * @return void
     */
    void closeWindow();
};
