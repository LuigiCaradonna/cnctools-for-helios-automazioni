#include "Helpers.h"

float Helpers::truncToDecimal(const float& number, int decimals)
{
    float multiplier = pow(10, decimals);

    return (int)(number * multiplier) / multiplier;
}

QString Helpers::secondsToTimestring(const int seconds)
{
    int minutes = int(seconds / 60);
    int hours = int(minutes / 60);
    int secs = seconds % 60;

    QString hh, mm, ss;

    hh = QString::number(hours);
    if (hours < 10)
        hh = "0" + hh;

    minutes = minutes % 60;
    mm = QString::number(minutes);
    if (minutes < 10)
        mm = "0" + mm;

    ss = QString::number(secs);
    if (secs < 10)
        ss = "0" + ss;

    return hh + ":" + mm + ":" + ss;
}

bool Helpers::fileExists(const std::string& s)
{
    struct stat buffer;
    return (stat(s.c_str(), &buffer) == 0);
}

short Helpers::isPgrFile(const QString& s)
{
    /*
        Status codes:
        0   - Checks passed
        1   - File not found
        2   - Wrong extension
        3   - Invalid file content
    */

    QFileInfo fi(s);
    QFile file(s);
    
    // Verify that the file exists
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // No need to close the file, its opening failed
        return 1;
    }

    // The file exists and it has been opened, check for the file extension to be .PGR
    if (fi.suffix() != "PGR")
    {
        // Close the file
        file.close();
        return 2;
    }
    // If the file exists and it has the PGR extension
    else
    {
        QTextStream in(&file);
        QString line = in.readLine();

        // If the file does not start with the string "QUOTE RELATIVE"
        if (line.trimmed() != "QUOTE RELATIVE")
        {
            // Close the file
            file.close();
            return 3;
        }
    }

    // Close the file
    file.close();

    return 0;
}

QString Helpers::elideText(const QLabel& label, const QString& text)
{
    // Get the metrix for the font used inside the label
    QFontMetrics metrix = QFontMetrics(label.font());

    // Elide the text at a width of 15px less than the label width to leave some padding
    QString elided_text = metrix.elidedText(text, Qt::ElideRight, label.width() - 15);

    return elided_text;
}

QString Helpers::elideText(const QLineEdit& line, const QString& text)
{
    // Get the metrix for the font used inside the label
    QFontMetrics metrix = QFontMetrics(line.font());

    // Elide the text at a width of 15px less than the label width to leave some padding
    QString elided_text = metrix.elidedText(text, Qt::ElideRight, line.width() - 15);

    return elided_text;
}

float Helpers::formatNumberForIsoFile(float num, int decimals)
{
    QString number;
    QStringList num_parts;

    // Convert the number into a string
    QString numString = QString::number(num);

    // Split the integer (position 0) and decimal parts (position 1)
    num_parts = numString.split('.');

    // If the number is an integer (x.0, includes the case 0.0)
    if (std::floor(num) == num)
    {
        // Get the integer part
        number = num_parts[0];
    }
    // If the number is between -1 and 1 excluded (0.y)
    else if (num > -1 && num < 1)
    {
        // Append the decimal part to 0.
        number = "0." + num_parts[1];
    }
    /* 
        If the number is equal or greater than 1, or the number is equal or less then -1
        it is represented inside the file with the integer part (+/- x.y)
    */
    else if (num >= 1 || num <= -1)
    {
        // Get the complete number
        number = numString;
    }
    
    // Find the dot position
    int dot_position = number.indexOf(".");

    // If the dot has been found
    if (dot_position != -1)
    {
        // Truncate to the desired decimal digit and return the obtained value as float
        return truncToDecimal(number.toFloat(), decimals);
    }
    else
    {
        // Return the value as it is (an integer) parsed to float
        return number.toFloat();
    }
}

QStringList Helpers::getFilesContentAsVector(QStringList filenames)
{
    // Vector containing the lines of the selected files
    QStringList iso;

    foreach(QString f, filenames)
    {
        QFile file(f);
        // Verify that the file exists and has the proper format
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&file);
        while (!in.atEnd())
        {
            // Add the current line to the vector
            iso.append(in.readLine());
        }

        file.close();
    }

    return iso;
}

QStringList Helpers::getFileContentAsVector(QString filename)
{
    // Vector containing the lines of the selected files
    QStringList iso;

    QFile file(filename);
    // Verify that the file exists and has the proper format
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream in(&file);
    while (!in.atEnd())
    {
        // Add the current line to the vector
        iso.append(in.readLine());
    }

    file.close();

    return iso;
}
