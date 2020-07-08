# XYPLOT-Win32
Extensible Plotting and Data Analysis Program for 32-bit Windows

## Introduction
XYPLOT is an extensible plotting and data analysis program for use by students, teachers, scientists, and engineers. It displays the results from your experiments or calculations in an interactive graphical environment. Fast plotting and a myriad of data manipulation and analysis features make XYPLOT an invaluable tool for visualizing and working with data. With its powerful, built-in Forth interpreter, users may write custom modules to add new data manipulation and analysis functions, and even data-acquisition functions accessible from menus. Many of XYPLOT's built-in functions are provided in the form of Forth source modules which are loaded upon startup. For generating publication quality graphs, a module provides the capability to export graphs to Grace.

## Features

* Fast and easy to use with minimal fuss in loading and plotting a dataset from a file.
* Arbitrarily large datasets may be loaded from files.
* Powerful built-in kForth environment permits loading Forth source modules.
* Selection of plot colors and symbols.
* Continuous display of x,y cursor coordinates.
* Multi-level zoom in/out to examine features of interest.
* Modify x or y values of a dataset using simple algebraic expressions.
* View and edit dataset headers.
* Compatible with DOS and Windows XYPLOT dataset files and workspace files.

## System Requirements

OS: Windows 7 through Windows 10

The executable, xypw32.exe, will also run on GNU/Linux x86/x86_64 systems under recent versions of the Windows emulator, Wine.

Building from the source code is not necessary to use this package, but an advanced user may modify and rebuild the executable (please see the LICENSE file). The current tools needed to build the executable from the source code provided in this package are:

* Digital Mars C/C++ compiler (version 8.57)
* A386 assembler (version 4.05)

## Installation Instructions

1. Download the ZIP file for the master or release version you want. The master will contain the most recent features, but a release version will usually be more stable.

2. Extract the files from the ZIP file. They will extract into a separate folder, such as XYPLOT-Win32-master.

3. The executabe, xypw32.exe, is found in the sub-folder, bin/. Create a shortcut for the executable and drag the shortcut onto your desktop for easy access.

4. The exe file must be able to find its Forth modules, which are located in the subfolder, modules/ of the extracted files. Create an environment variable, XYPLOT_DIR, to specify the location of the module files (.4th). Follow the steps below to set the environment variable.

5. Go to the Windows toolbar search field and type, "env". Then, select "Edit the environment variables for your account."

6. Under "User Variables for [your account name]", click on "New". Then, set the variable name to XYPLOT_DIR and the variable value to the full path specifying the location of the modules. Click "OK".

7. When you launch XYPLOT from its desktop shortcut, it will open up an empty graph. If the labels on the graph are blacked out, this means that XYPLOT cannot find its modules. Check the environment variable setting.

8. Follow the User's Guide to learn how to create a graph by loading data into XYPLOT from text files or other file types. The distribution's sub-folder, example-data/ , contains several examples of different types of files which may be displayed.


