
Wenden Sie sich bei Fragen oder Ergänzungen bitte an einen der Betreuer.


ANLEITUNG ZUR VERWENDUNG DES CMAKE BUILD SYSTEMS.

Im Folgenden sind Beschreibungen zum Verarbeiten des Programmrahmens für Windows, Ubuntu, und OS X mit den entsprechenden Kompilern nmake, g++ bzw. clang++ gegeben:


Windows NMake (MSVC)

  1. Visual Studio installieren:
    * Community Edition is kostenlos:
      https://www.visualstudio.com/products/visual-studio-community-vs
    * Alternativ: Ultimate und andere Editions sind für HPI-Studenten gratis (Dreamspark)

  1.a. NMake im Systempfad (%PATH%) auffindbar, z.B. "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\bin" 

  1.b. Alternativ ist NMake auch durch Nutzung der Visual Studio Command Prompt verfügbar,

  1.c. oder durch vorheriges Ausführen (aus der Standardkonsole cmd heraus) von z.B. 
    "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat x86_amd64" 


  2. CMake installieren: http://www.cmake.org/download/

  2.a. CMake im Systempfad auffindbar (kann bei Verwendung des Installers ausgewählt werden) z.B. "C:\Program Files (x86)\CMake\bin"

  3. In der Konsole in den Ordner mit dem entpackten Programmrahmen wechseln (z.B. cd C:\pt2\uebung1) - alternativ im Explorer das erweiterte Kontextmenü öffnen [Umschalt]+[Rechtsklick] und die Kommandozeile aus diesem Ordner heraus öffnen.

  4. Ein Build-Verzeichnis erstellen z.B. 'build' über 'mkdir build' und in dieses wechseln, z.B. 'cd build'.

  5. CMake ausführen und entsprechende Makefile erstellen:
    'cmake configure .. -G "NMake Makefiles"'

  6. Kompilieren via 'cmake --build .'
  6.a. Komplett neu bauen via 'cmake --build . --clean-first'

  7. Sofern erfolgreich, Ausführen der Programme.


Windows MinGW (für Fortgeschrittene) :: gleicher Ablauf wie NMake, mit folgenden Änderungen:

  siehe Installation von name, folgende Schritte unterscheiden sich:

  1. MinGW statt Visual Studio installieren (http://www.mingw.org/)
    http://www.mingw.org/wiki/getting_started

  5.a. 'cmake configure .. -G "MinGW Makefiles"'
  5.b. 'cmake configure .. -G "MSYS Makefiles"'


Ubuntu & Mac OS :: gleicher Ablauf wie NMake, mit folgenden Änderungen:

  2. CMake installieren: http://www.cmake.org/download/

  2.a. unter OS X alternativ mit package manager homebrew: 
  2.b. unter Ubuntu mit apt-get: 'sudo apt-get install cmake'

  3. In einem Terminal in den Ordner mit dem entpackten Programmrahmen wechseln

  5. CMake ausführen und entsprechende Makefile erstellen:
    'cmake configure .. -G "Unix Makefiles"'



Weitere Quellen zur Einführung in CMake:

http://www.cmake.org/runningcmake/
http://www.cmake.org/cmake-tutorial/
