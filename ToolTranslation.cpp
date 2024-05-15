#include "ToolTranslation.h"

/********** CONSTRUCTOR **********/

ToolTranslation::ToolTranslation(std::string f, QWidget *parent)
	: folder(f), parent(parent)
{
	this->ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("favicon.png"));

	// Bind the buttons to the corresponding method to fire
	this->connect(this->ui.btn_source, &QPushButton::released, this, &ToolTranslation::openSource);
	this->connect(this->ui.btn_destination, &QPushButton::released, this, &ToolTranslation::saveDestination);
	this->connect(this->ui.btn_translate, &QPushButton::released, this, &ToolTranslation::translate);
	this->connect(this->ui.btn_close, &QPushButton::released, this, &ToolTranslation::closeWindow);
}

/********** DESTRUCTOR **********/

ToolTranslation::~ToolTranslation()
{}

/********** PRIVATE FUNCTIONS **********/

void ToolTranslation::openSource()
{
	// Filter to show only PGR files
	QString filter = "pgr(*.PGR)";

    // Open the file browser to select the source file
    this->source = QFileDialog::getOpenFileName(this, tr("open_file"), QString::fromUtf8(this->folder.c_str()), filter);

    if (!this->source.isEmpty() && this->source != "")
    {
        // Update the last opened folder
        this->folder = this->source.toStdString();

        // Elides, if necessary, the string to fit the selected line edit filed and prints it
        QString elided_text = Helpers::elideText(*this->ui.in_source, this->source);

        // Print the elided text into the label
        this->ui.in_source->setText(elided_text);
    }
}

void ToolTranslation::saveDestination()
{
	// Filter to show only PGR files
	QString filter = "pgr(*.PGR)";

	// Get the folder and the file name to use to save the new file
	this->destination = QFileDialog::getSaveFileName(this, tr("save_to"), QString::fromUtf8(this->folder.c_str()), filter);

    /*
        Check using isEmpty() because saveDestination() returns a null QString if the user
        does not select a destination file and closes the dialog.
        It would also be possible to use isNull(), but isEmpty() works both for null
        and empty strings, while isNull() checks only for null value.
    */
    if (!this->destination.isEmpty() && this->destination != "")
    {
        // Elides, if necessary, the string to fit the selected line edit filed and prints it
        QString elided_text = Helpers::elideText(*this->ui.in_destination, this->destination);

        // Print the elided text into the label
        this->ui.in_destination->setText(elided_text);
    }
}

void ToolTranslation::resetErrors()
{
	this->ui.in_source->setStyleSheet("border: 1px solid black");
	this->ui.in_destination->setStyleSheet("border: 1px solid black");
	this->ui.in_traslation_x->setStyleSheet("border: 1px solid black");
	this->ui.in_traslation_y->setStyleSheet("border: 1px solid black");
	this->ui.out_log->setPlainText("");
}

bool ToolTranslation::checkData()
{
	// Used to know if the string to int convertion succeeds
	bool ok = true;

    // Check if the source file is a valid PGR
    switch (Helpers::isPgrFile(this->source))
    {
        // The file does not exist
    case 1:
        this->ui.in_source->setStyleSheet("border: 1px solid red");
        this->ui.statusbar->showMessage(tr("file_not_found"));

        return false;

        // The file does not have the PGR extension
    case 2:
        this->ui.in_source->setStyleSheet("border: 1px solid red");
        this->ui.statusbar->showMessage(tr("wrong_extension"));

        return false;

        // The file content is not valid
    case 3:
        this->ui.in_source->setStyleSheet("border: 1px solid red");
        this->ui.statusbar->showMessage(tr("sel_valid_iso"));

        return false;

        // The file is a valid PGR
    default:
        break;
    }

    /*
        Check using isEmpty() because saveDestination() returns a null QString if the user
        does not select a destination file and closes the dialog.
        It would also be possible to use isNull(), but isEmpty() works both for null
        and empty strings, while isNull() checks only for null value.
    */
    if (this->destination.isEmpty())
    {
        this->ui.in_destination->setStyleSheet("border: 1px solid red");
        this->ui.statusbar->showMessage(tr("save_file_empty"));
        return false;
    }

    // If the x translation field is empty
    if (this->ui.in_traslation_x->text() == "")
    {
        // Set to 0
        this->ui.in_traslation_x->setText("0");
    }

    /*
        Check if the x translation value is valid.
        Try to convert it to float, if the conversion fails, the input was not a number and the
        variable ok will be set to false.
        Negative values can be accepted.
    */
    this->ui.in_traslation_x->text().toFloat(&ok);
    if (!ok)
    {
        this->ui.in_traslation_x->setStyleSheet("border: 1px solid red");
        this->ui.statusbar->showMessage(tr("provide_valid_x_trans"));

        return false;
    }

    // If the y translation field is empty
    if (this->ui.in_traslation_y->text() == "")
    {
        // Set to 0
        this->ui.in_traslation_y->setText("0");
    }

    /*
        Check if the y translation value is valid.
        Try to convert it to float, if the conversion fails, the input was not a number and the
        variable ok will be set to false.
        Negative values can be accepted.
    */
    this->ui.in_traslation_y->text().toFloat(&ok);
    if (!ok)
    {
        this->ui.in_traslation_y->setStyleSheet("border: 1px solid red");
        this->ui.statusbar->showMessage(tr("provide_valid_y_trans"));

        return false;
    }

    // The provided data are valid
	return true;
}

void ToolTranslation::translate()
{
    // Message to show into the log area
    QString log_message = "";

    // Reset the log area
    this->ui.out_log->setPlainText("");

    /* 
        Counter to skip the first occurrence of G12 which is just the starting position
        the actual position where the engraving starts is represented by the second G12.
        This would not be an issue when the translations are in positive area, it becomes
        a problem when the translations are negative, the tool would move outside the
        working area, then to the actual starting position.
    */
    unsigned int g12_count = 0;

    // Reset eventual errors
    this->resetErrors();

    // If the data provided are correct
    if (this->checkData())
    {
        QStringList coordinates = Helpers::getFileContentAsVector(this->source);

        // At each iteration, this will contain the current line of code splitted
        QStringList subline;

        // These will contain the x, y and z coordinate to add to the coordinates list
        float x = 0.f;
        float y = 0.f;
        float new_x = 0.f;
        float new_y = 0.f;

        QString new_code;

        QFile file(this->destination);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
        {
            QMessageBox error_dialog;
            error_dialog.critical(0, tr("error"), tr("cant_open_save_file"));
            return;
        }

        QTextStream stream(&file);

        foreach(QString line, coordinates)
        {
            // Reset the subline
            subline.clear();

            // If the line starts with G02 X
            if (line.indexOf("G02 X") == 0)
            {
                // This is a movement which produce an engrave, contains the Z coordinates too

                // Split the code
                subline = line.split(" ");

                /*
                    The second element is the X coordinate, remove the first character (X)
                    limit the number to 3 decimals.
                */
                x = Helpers::truncToDecimal(subline[1].mid(1).toFloat(), 3);

                /*
                    The third element is the Y coordinate, remove the first character (Y)
                    limit the number to 3 decimals.
                */
                y = Helpers::truncToDecimal(subline[2].mid(1).toFloat(), 3);

                // New X position
                new_x = float(x) + this->ui.in_traslation_x->text().toFloat();

                // New Y position
                new_y = float(y) + this->ui.in_traslation_y->text().toFloat();

                try
                {
                    new_code = "G02 X" + QString::number(Helpers::formatNumberForIsoFile(new_x)) +
                        " Y" + QString::number(Helpers::formatNumberForIsoFile(new_y)) +
                        " " + subline[3] + "\n";
                }
                catch (const std::exception&)
                {
                    new_code = line + "\n";
                }

                stream << new_code;
            }
            // If the line starts with G12 X
            else if (line.indexOf("G12 X") == 0)
            {
                g12_count += 1;

                // The first G12 X must be skipped, it only tells to stay on the origin before to start
                if (g12_count > 1)
                {
                    /*
                        This is a repositioning, Z is not present because the movement is always at Z0
                        which is already set by the preceding line (G12 Z0)
                    */

                    // Split the code
                    subline = line.split(" ");

                    /*
                        The second element is the X coordinate, remove the first character (X)
                        limit the number to 3 decimals.
                    */
                    x = Helpers::truncToDecimal(subline[1].mid(1).toFloat(), 3);

                    /*
                        The third element is the Y coordinate, remove the first character (Y)
                        limit the number to 3 decimals.
                    */
                    y = Helpers::truncToDecimal(subline[2].mid(1).toFloat(), 3);

                    // New X position
                    new_x = float(x) + this->ui.in_traslation_x->text().toFloat();

                    // New Y position
                    new_y = float(y) + this->ui.in_traslation_y->text().toFloat();

                    new_code = "G12 X" + QString::number(Helpers::formatNumberForIsoFile(new_x)) +
                        " Y" + QString::number(Helpers::formatNumberForIsoFile(new_y)) + "\n";

                    stream << new_code;
                }
                else
                {
                    stream << line + "\n";
                }
            }
            else
            {
                stream << line + "\n";
            }
        }

        file.close();

        log_message = tr("translation_success");
        this->ui.out_log->setPlainText(log_message);
    }
}

void ToolTranslation::closeWindow()
{
    this->close();
}
