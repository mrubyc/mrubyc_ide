# mruby/c IDE

A simple IDE for mruby/c

## How to use

This IDE is designed to be used with the following two tools:

 * __mrbc__: Only the mruby compiler from the [mruby](https://github.com/mruby/mruby) project is required.
 * __mrbwrite__: A bytecode (.mrb file) writer, available [here](https://github.com/mrubyc/mrbwrite).

Please download and build these tools in advance, or refer to the [Binary Download](#binary-download) section.


## How to build

### MacOS

Install Xcode.  
Install [Qt6](https://www.qt.io/development/download) using the online installer.  
We recommend using Xcode 15.4 and Qt 6.8.3.  
Add the path of the installed Qt bin directory to your PATH.  
Open Terminal.app and enter the following command:  
```sh
cd mrubyc_ide
qmake
make
```

### Windows

Install Visual Studio with the C++ build option.  
Install [Qt6](https://www.qt.io/development/download) using the online installer.  
We recommend using Visual Studio 2022 and Qt 6.8.3.  
Add the path of the installed Qt bin directory to your PATH.  
Open `VisutalStudio Native Tools Command Prompt` and enter the following command:  
```
cd mrubyc_ide
qmake
nmake
copy src\config\mrubyc_ide.ini src\release
```


## Binary download

[ITOC mruby/c downloads](https://www.s-itoc.jp/support/technical-support/mrubyc/mrubyc-download/)


## LICENSE

GNU Lesser General Public License Version 2.1
