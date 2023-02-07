#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "Helpers.h"
#include "ui_ToolEnqueue.h"

class ToolEnqueue : public QMainWindow
{
	Q_OBJECT

public:
	/********** CONSTRUCTOR **********/

	/*
	 * Constructs the ToolEnqueue object.
	 *
	 * @param	std::string	f		- Default folder to open
	 * @param	QWidget*	parent	- Parent widget
	 */
	ToolEnqueue(std::string f, QWidget *parent = nullptr);


	/********** CONSTRUCTOR **********/

	/*
	 * Destructs the ToolEnqueue object.
	 */
	~ToolEnqueue();

	/********** PUBLIC FUNCTIONS **********/


private:
	// The listwidget drag&drop reordering capability is set inside the ui file
	Ui::ToolEnqueueClass ui;

	// List of files to open
	QStringList iso_files;
	// Destination file name
	QString destination;

	/*
		Folder to open when clicking the "Load Files" button, initialized with the value contained into
		the config file and updated each time a file is loaded from a different folder to always open
		the last used folder.
	*/
	std::string folder;

	/********** PRIVATE FUNCTIONS **********/

	/*
	 * Opens the file browser to select the source file.
	 *
	 * @return void
	 */
	void addFile();

	/*
	 * Removes the selected files from the list.
	 *
	 * @return void
	 */
	void removeFiles();

	/*
	 * Processes the given files and save the result.
	 *
	 * @return void
	 */
	void enqueueFiles();

	/*
	 * Resets the window for a new job.
	 *
	 * @return void
	 */
	void reset();

	/*
	 * Opens the file browser to select the folder where to save the new file.
	 *
	 * @return QString	-	The path and file name where to save the new file. Returns a null QString if the dialog
	 * is closed without a selection.
	 */
	QString saveDestination();

	/*
	 * Validates the data, checking that at least one file is selected and that all the selected files
	 * have valid extension and content.
	 *
	 * @return True if the inserted data are correct, false otherwise.
	 */
	bool checkData();

	/*
	 * Closes the widget.
	 *
	 * @return void
	 */
	void closeWindow();
};
