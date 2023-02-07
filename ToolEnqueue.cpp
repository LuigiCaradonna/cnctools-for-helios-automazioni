#include "ToolEnqueue.h"

/********** CONSTRUCTOR **********/

ToolEnqueue::ToolEnqueue(std::string f, QWidget *parent)
	: folder(f), QMainWindow(parent)
{
	ui.setupUi(this);

    // Sets an icon for the window
    this->setWindowIcon(QIcon("favicon.png"));

    // Bind the buttons to the corresponding method to fire
    this->connect(this->ui.btn_add_pgr, &QPushButton::released, this, &ToolEnqueue::addFile);
    this->connect(this->ui.btn_remove_sel, &QPushButton::released, this, &ToolEnqueue::removeFiles);
    this->connect(this->ui.btn_start_enqueue, &QPushButton::released, this, &ToolEnqueue::enqueueFiles);
    this->connect(this->ui.btn_bottom_start_enqueue, &QPushButton::released, this, &ToolEnqueue::enqueueFiles);
    this->connect(this->ui.btn_reset, &QPushButton::released, this, &ToolEnqueue::reset);
    this->connect(this->ui.btn_close, &QPushButton::released, this, &ToolEnqueue::closeWindow);
}

/********** DESTRUCTOR **********/

ToolEnqueue::~ToolEnqueue()
{}

/********** PRIVATE FUNCTIONS **********/

void ToolEnqueue::addFile()
{
    // Filter to show only PGR files
    QString filter = "pgr(*.PGR)";

    QString file_folder;
    QString file_name;

    // Open the file browser to select the source file(s)
    this->iso_files = QFileDialog::getOpenFileNames(this, tr("open_file"), QString::fromUtf8(this->folder.c_str()), filter);

    // Loop over all the selected files
    foreach(QString file, this->iso_files)
    {
        QFileInfo fi(file);
        file_folder = fi.absolutePath();

        // Create a new list item
        QListWidgetItem* item = new QListWidgetItem(file);
        // Make it checkable
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        // Set it unchecked by default
        item->setCheckState(Qt::Unchecked);
        // Add the selected file to the list
        this->ui.enqueuedFiles->addItem(item);
    }

    // Update the last opened folder
    this->folder = file_folder.toStdString();
}

void ToolEnqueue::removeFiles()
{
    // Loop over all the items
    for (int i = 0; i < this->ui.enqueuedFiles->count(); ++i)
    {
        QListWidgetItem* item = this->ui.enqueuedFiles->item(i);

        // If the current item is checked
        if (item->checkState() == Qt::Checked)
        {
            // Removes and returns the item from the list
            QListWidgetItem* it = this->ui.enqueuedFiles->takeItem(i);
            // Delete the item
            delete it;
        }
    }
}

void ToolEnqueue::enqueueFiles()
{
    int files_to_enqueue = this->ui.enqueuedFiles->count();

    // If the data are correct
    if (this->checkData())
    {
        QStringList file_content;

        this->destination = this->saveDestination();

        /*
            Check using isEmpty() because saveDestination() returns a null QString if the user
            does not select a destination file and closes the dialog.
            It would also be possible to use isNull(), but isEmpty() works both for null
            and empty strings, while isNull() checks only for null value.
        */
        if (!this->destination.isEmpty())
        {
            QFile file(this->destination);

            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox error_dialog;
                error_dialog.critical(0, tr("error"), tr("cant_open_save_file"));
                return;
            }

            QTextStream stream(&file);

            // Get the item at position 0 and use it to start the output file
            file_content = Helpers::getFileContentAsVector(this->ui.enqueuedFiles->item(0)->text());

            // Write the whole content of the first file into the destination file
            foreach(QString line, file_content)
            {
                stream << line + "\n";
            }

            /*
                Counter to skip the first 6 lines
                not inside the for loop to not create a new variable at each iteration
            */
            int unsigned line_count = 0;

            // Loop over all the items except the first (start from i=1)
            for (int unsigned i = 1; i < files_to_enqueue; ++i)
            {
                file_content = Helpers::getFileContentAsVector(this->ui.enqueuedFiles->item(i)->text());

                // Add the content of the source file into the destination file
                foreach(QString line, file_content)
                {
                    // Copy only the lines from the 7th ahead
                    if (line_count >= 6)
                    {
                        stream << line + "\n";
                    }

                    line_count += 1;
                }

                // Reset the counter
                line_count = 0;
            }

            this->ui.logArea->setPlainText(tr("enqueue_success"));

            file.close();
        }
    }
}

void ToolEnqueue::reset()
{
    this->ui.enqueuedFiles->clear();
    this->ui.logArea->setPlainText("");
}

QString ToolEnqueue::saveDestination()
{
    // Filter to show only PGR files
    QString filter = "PGR(*.PGR)";
    QString new_file_name = "";

    // If at least one file has been selected
    if (this->ui.enqueuedFiles->count() > 0)
    {
        // Get the name of the first file to enqueue
        new_file_name = this->ui.enqueuedFiles->item(0)->text();
        // Delete the part from the last empty space
        new_file_name.truncate(new_file_name.lastIndexOf(' '));
        // Capitalize the filename and add the directory separator in front of it
        new_file_name = "/" + new_file_name.toUpper();
    }

    /* 
        Get the folder and the file name to use to save the new file.
        When new_file_name is not empty, the file browser will set it as suggested.
    */
    return QFileDialog::getSaveFileName(this, tr("save_to"), QString::fromUtf8(this->folder.c_str()) + new_file_name, filter);
}

bool ToolEnqueue::checkData()
{
    // Message to show into the log area
    QString log_message = "";

    // Reset the log area
    this->ui.logArea->setPlainText("");

    // Check if at least one file has been selected
    if (this->ui.enqueuedFiles->count() < 1)
    {
        log_message += tr("no_file_selected");
        this->ui.logArea->setPlainText(log_message);

        return false;
    }

    // Loop over all the files to check that they still exist
    for (int i = 0; i < this->ui.enqueuedFiles->count(); ++i)
    {
        // Check if the source file is a valid PGR
        switch (Helpers::isPgrFile(this->ui.enqueuedFiles->item(i)->text()))
        {
            // The file does not exist
        case 1:
            log_message = this->ui.enqueuedFiles->item(i)->text() + "\n";
            log_message += tr("file_not_found");
            this->ui.logArea->setPlainText(log_message);

            return false;

            // The file does not have the PGR extension
        case 2:
            log_message = this->ui.enqueuedFiles->item(i)->text() + "\n";
            log_message += tr("wrong_extension");
            this->ui.logArea->setPlainText(log_message);

            return false;

            // The file content is not valid
        case 3:
            log_message = this->ui.enqueuedFiles->item(i)->text() + "\n";
            log_message += tr("sel_valid_iso");
            this->ui.logArea->setPlainText(log_message);

            return false;

            // The file is a valid PGR
        default:
            break;
        }
    }

    return true;
}

void ToolEnqueue::closeWindow()
{
    this->close();
}
