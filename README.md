# OpenGL_Framework

## Erste Schritte
Dieses Repository ist mit ```git``` Versioniert. Wer git noch nicht kennt, sollte zuerst die Grundlagen lernen. Eine gute Anlaufstelle dafür ist z.B. [Pro git](http://git-scm.com/book/de/v1).

**TL;DR:**
* git-Grundlagen können und verstehen :)


---

## Installation
**OpenGL_Framework** wird mit dem CMake Build-System erzeugt. CMake funktioniert Cross-Plattform, also auf jedem System und lässt dem Entwickler Freiraum, wie er Sourcecode schreiben und verändern will (z.B. in welcher IDE,wo 3rd-Party-Libraries auf seinem System liegen, …). Dazu müsst ihr erst CMake installieren, falls ihr das noch nicht habt: [www.cmake.org/](http://www.cmake.org/).


1. **Repository auf eigenen Rechner clonen**

    `git clone https://github.com/treevg/OpenGL_Framework.git`
    Oder mittels [GUI Client](http://git-scm.com/download/gui/linux)

2. **Build-Verzeichnis erstellen**

    Auf eurem Rechner einen Ordner erstellen (z.b. `build-vs2012` oder einfach nur `build`), indem mit CMake dann Projektfiles erstellt werden, die mit einer IDE (z.B. Visual Studio) geöffnet werden können. Der Ordnerpfad ist prinzipiell egal, meist wird dieser aber im Repository erstellt und dann **nicht mit gepusht!**

3. **CMake auführen**

    Aus diesem Build-Ordner Cmake ausführen: `cmake ../src` oder mittels [CMake GUI](http://www.cmake.org/runningcmake/)

4. **Projektfiles verwenden**

    CMake erzeugt in eurem Build-Verzeichnis Projektfiles für z.B. Visual Studio, die ihr dann öffnen und die Projekte kompilieren könnt.

**TL;DR:**
```
1. git clone https://github.com/treevg/OpenGL_Framework.git
2. mkdir build
3. cd build && cmake ../src
4. make
```

### Dokumentation generieren
Falls ihr euch eine Dokumentation über alle vorhandenen Klassen und Methoden erzeugen wollt, dann könnt ihr das tun, indem ihr bei CMake angebt, dass diese mit erstellt werden soll. Entweder über `cmake ../src -DBUILD_DOCUMENTATION=true` oder in der GUI unter *Ungrouped Entries* einen Haken setzen bei **BUILD_DOCUMENTATION**. Danach kann mit `make Docs` die Dokumentation erzeugt werden. Diese landet dann im Ordner `Documentation/html/` und kann mit einem Browser eurer Wahl geöffnet werden (einfach die `index.html` öffnen)

**TL;DR:**
```
1. cmake ../src -DBUILD_DOCUMENTATION=true
2. make Docs
3. $BROWSER Documentation/html/index.html
```

---

## Struktur

- `resources` Enthält alle Datein, die kein Programmcode sind (z.b. Texturen, Modelle)
    - `└` Pro Dateiendung gibt es einen Unterordner (z.b. `jpg` für JPEG-Bilder oder `obj` für Object-Modelle)
- `src` Enthällt alle Dateien, die zum Sourcecode zählen
    - `└ cmake` Enthällt CMake Module, die z.B. 3rd-Party-Bibliotheken suchen
    - `└ executable` Enthällt Code von allen ausführbaren Programmen
    - `└ libraries` Enthällt Code von Bibliotheken
    - `└ shaders` Enthällt Shader-Code
