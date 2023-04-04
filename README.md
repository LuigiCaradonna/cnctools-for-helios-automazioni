# Cnc Tools for Helios Automazioni srl CNC Machines

This software is a set of utilities to be used with the proprietary GCode files with PGR extension.

> This is **NOT** an official software developed by [Helios Automazioni srl](https://www.heliosautomazioni.com/it/home-it.html).
>
> I am computer engineer employed as a technical designer and a CNC programmer at [Centro Marmi D'Arcangeli srl](https://www.cmdarcangeli.com).

## Features

### PGR path visualization  

- Import multiple files at once
- Possibility to specify the working area size
- Option to decide whether or not the path has to fit the visible area
- Option to automatically regenarate the path when the main window is resized
- Zoom using the mouse wheel
- Option to show the path in grayscale or colors
- Option to highlight the milling deepest points
- Gradient effect applied to the segments to show the difference in depth of the milling
- Calculation of the distance traveled both for millings and repositionings
- Estimate of the working time given the speed of the tool (not yet reliable)\*

### PGR queuing

- Import multiple files at once
- Drag&Drop to reorder the loaded files

### PGR translation

- Translation along X and Y axes

##### \* About the working time, the reason why it is not currently reliable, is that the machine doesn't really keep the speed constant while working and the vertical movement are not performed at the same speed as the horizontal ones, it could also vary due to the hardness of the material, the complexity of the job and other reasons. Where the job is at a constant depth the time results to be almost correct. 

## Tech

The whole software is written in `C++`, using `Qt6` to build the user interface.

## How to use

### PGR path visualization and main window

![Graphic User Interface](/ui.png)  
The basic usage only requires to click on the `Load files` button (24) and to select one or more PGR files, then to click on the `Process` button (25).  
However you have the possibility to set several options to have different behaviours.  
You can set the actual width and height (2 and 3) of the material to work and the path will respect its real position inside the material bounds, which will be displayed.  
The provided speed of the tool (4) will have an impact only on the estimated time, which as you can read above is not accurate due to several reasons not depending on the software but on the machine's behaviour.  
By selecting the `Fit` checkbox (18) the path will be scaled to fit the drawing area, this will have effect only for drawings which are smaller than the drawing area, those bigger, will be scaled down in any case, or part of them would not be visible.  
`Regenerate` (19), when checked, will cause the path to be regenerated if the main window is resized to adapt to the new drawing area size.  
If the file selected contains the instructions to engrave a sculpture, the `Sculpture` checkbox (20) **must** be checked because that kind of file is slightly different from the regular ones and the software needs to be informed to correctly read the file.  
Operating on the `Colors` checkbox (21) it is possible to decide to have the path displayed using a grayscale (unchecked) or colors (checked). Depending on the `Gradient` checkbox (22) status, the segments will be drawn using a solid line (unchecked) or a gradient effect (checked).  
By selecting the `Z Max` checkbox (23) it is possible to highlight the points corresponding to the maximum depth.
The software only shows segments with positive coordinates, for this reason, when a path has parts with negative coordinates, it will be translated to have all of its parts in a positive area (first quadrant of the Cartesian plane). When such a condition occurs, inside the `Offset` field (11) will be stated along which axis the path has been translated (X, Y, or X and Y).
  
**1 -** Drawing area, here is where the path will be displayed  
**2 and 3 -** Width and height of the material to work on in millimeters  
**4 -** Movement speed of the tool in millimeters per minute  
**5 -** Current zoom level  
**6 to 10 -** Minimum and maximum positions where the machine will engrave  
**11 -** Here will be stated whether the path was moved due to negative coordinates contained into the PGR file  
**12 -** Distance traveled while engraving in millimeters  
**13 -** Distance traveled while repositioning the tool in millimeters  
**14 -** Area occupied by the engraving  
**15 -** Estimated value of the required time to complete the job hh:mm:ss  
**16 and 17 -** X and Y position of the mouse pointer on the drawing area in millimeters  
**18 -** When checked, the path will be scaled to fit the drawing area  
**19 -** When checked, the path will be automatically regenerated upon main window resizing  
**20 -** This must be checked when a file containing a sculpture is selected  
**21 -** When checked, the segments of the path are colored, otherwise they will be grayscaled  
**22 -** When checked, the segments will have a gradient effect to show the difference in depth  
**23 -** When checked, a circle will appear to highlight the points at maximum depth   
**24 -** Button to select the file(s) to display the path contained  
**25 -** Starts the drawing process  
**26 -** Resets everything: input values, drawing area and eventual error messages  
**27 -** Button to close the program  
**28 -** On this line will appear the name(s) of the selected file(s)  
**29 -** Status bar, this will contain eventual error messages  
**24 -** Button to select the file(s) to display the path contained  
**31 -** Button to open the translation tool  
**32 -** Button to open the queuing tool  
**33 -** Language menu to change the interface language  
**34 -** Tools menu, contains the links to open the various tools  

### Queuing tool

![Queuing tool](/queuing-tool.png)  
With this tool it is possible to join together multiple PRG files, the tool also removes the commands which determine the pause and the repositionig between two jobs.  
  
Select the PGR files to join by clicking the button `(1)`, multiple files selection is possible. If it is necessary, the files can be reordered dragging and dropping them.  
Once the files are in the wanted sequence, click the `Start` button (6).  
The tool will ask where to save the generated file.  
  
**WARNING**  
The tool will only consider the first file in the list to decide the speed. That means that if you have different speeds set for the other files, those speeds will be ignored!  
  
**1 -** Button to select the PGR files  
**2 -** Button to remove the checked files  
**3 -** Button to start the queuing  
**4 -** List of the selected files, sortable by drag&drop  
**5 -** Log area, will show info about the queuing  
**6 -** Button to start the queuing  
**7 -** Button to reset the tool  
**8 -** Button to close the tool  

### Translation tool

![Translate tool](/translate-tool.png)  
This tool is useful to translate the coordinates of a path already generated, without the need to edit the drawing inside the CAD software and to recalculate the path.  
  
Select the source file, set the name and the location for the new file.  
Set the offset in millimeters along the X axis inside the proper field (3).  
Set the offset in millimeters along the Y axis inside the proper field (4).  
Both positive and negative values are allowed.  
Click the `Translate` button (6).
  
**1 -** Source PGR files  
**2 -** Destination file (creates a new file if it does not exists)  
**3 -** Millimeters to translate along the X axis (positive or negative)  
**4 -** Millimeters to translate along the Y axis (positive or negative)  
**5 -** Log area, will show info about the translation  
**6 -** Button to start the translation  
**8 -** Button to close the tool  

## How to compile

The project has been developed using Visual Studio 2022 CE.
To compile the software, `msvc2019+`, `Qt 6.0+` are required.

## Installation

Inside the folder `dist` the latest compiled version of the software for Windows is available in a zip format.  
There is no need to install the software.  
Unpack the zip package, inside the folder `CncTools`, run the file `CncTools.exe`.  
> Windows 10 is required since previous versions are no more supported by `Qt`.