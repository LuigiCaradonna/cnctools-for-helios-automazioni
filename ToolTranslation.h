#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include "ui_ToolTranslation.h"
#include "Helpers.h"

class ToolTranslation : public QMainWindow
{
	Q_OBJECT

public:
	/********** CONSTRUCTOR **********/

	/*
	 * Constructs the ToolTranslation object.
	 *
	 * @param	std::string	f		- Default folder to open
	 * @param	QWidget*	parent	- Parent widget
	 */
	ToolTranslation(std::string f, QWidget *parent = Q_NULLPTR);


	/********** CONSTRUCTOR **********/

	/*
	 * Destructs the ToolTranslation object.
	 */
	~ToolTranslation();

	/********** PUBLIC FUNCTIONS **********/

private:
	Ui::ToolTranslationClass ui;

	// Source file name
	QString source;
	// Destination file name
	QString destination;

	// Folder to open when clicking the "Load Files" button, initialized with the value contained into
	// the config file and updated each time a file is loaded from a different folder to always open
	// the last used folder.
	std::string folder;

	/********** PRIVATE FUNCTIONS **********/

	/*
	 * Opens the file browser to select the source file.
	 *
	 * @return void
	 */
	void openSource();

	/*
	 * Opens the file browser to select the folder where to save the new file.
	 *
	 * @return void
	 */
	void saveDestination();

	/*
	 * Removes eventual error messages and reset the fields status to their default aspect.
	 *
	 * @return void
	 */
	void resetErrors();

	/*
	 * Validates the data.
	 *
	 * @return True if the inserted data are correct, false otherwise.
	 */
	bool checkData();

	/*
	 * Applies the translation required to the coordinates and saves them into a new file.
	 *
	 * @return void
	 */
	void translate();

	/*
	 * Closes the widget.
	 *
	 * @return void
	 */
	void closeWindow();

};
