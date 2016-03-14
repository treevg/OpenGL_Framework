# Forschungspraktikum 2014: Telepresenz

** Hinweis: **

Das Projekt ist aktuell nur properitär für Windows entwickelt!
Der Grund dafür ist, dass wir das Kinect SDK Verwenden für das Windows 8 benötigt wird.
Wer kein Windows System hat, kann statt dem Kinect SDK das LibFreenect Framework ausprobieren.

LibFreenect: https://github.com/OpenKinect/libfreenect

## Dependencies:

** Hardware die für Projekt verwendet wurde: **

1. Oculus Rift DK2
2. Kinect V2
3. LeapMotion V1

** Runtimes/Libraries/Frameworks die verwendet werden: **

1. Oculus Runtime
2. Oculus SDK
3. Leap Motion SDK
4. Kinect V2 SDK
5. GTK Framework
6. Assimp
7. GLM
8. GLFW
9. GLEW
7. CMake

### Details zum Setup:

#### Oculus Runtime (IMPORTANT!)
https://developer.oculus.com/downloads/pc/0.4.4-beta/Oculus_Runtime_for_Windows/

#### Oculus SDK (IMPORTANT!)
https://developer.oculus.com/downloads/pc/0.4.4-beta/Oculus_SDK_for_Windows/

Systemumgebungsvariable anlegen:
`Bsp: OCULUSSDK_ROOT = C:\Program Files (x86)\OculusSDK`

#### Leap Motion Installer & SDK
https://developer.leapmotion.com/downloads

Systemumgebungsvariable anlegen:
`Bsp: LEAPSDK_ROOT = C:\LeapSDK`

Wenn die Leap.dll bei Projektstart nicht gefunden wird:
C:\LeapSDK\lib\x86\leap.dll
In
C:\Windows\SysWOW64
kopieren

#### Kinect V2 SDK
https://www.microsoft.com/en-us/download/details.aspx?id=44561

#### GTK
http://www.gtk.org/download/win32.php

GTK All in One Bundle (32 bit):
http://win32builder.gnome.org/gtk+-bundle_3.6.4-20130921_win32.zip

Readme lesen!

1. .zip Datei in einen Ordner kopieren (z.B. C:\gtk)
2. Die System-Variable GTK_ROOT erstellen und auf das root Verzeichnis
von GTK setzen:
`Bsp: GTK_ROOT = C:\gtk`
3. Das Bin Verzeichnis an das Ende der Path Systemumgebungsvariablen anfügen:
Path: `.....;C:\gtk\bin` oder `.....;%GTK_ROOT%\bin`

In Kommandozeile (Eingabeaufforderung Windows):

1. `pkg-config --cflags gtk+-3.0`
Wenn eine Ausgabe erscheint weiter machen mit:
2. Pango vorbereiten:
`pango-querymodules > c:\gtk\etc\pango\pango.modules`
3. GDK-Pixbuf einrichten:
`gdk-pixbuf-query-loaders > c:\gtk\lib\gdk-pixbuf-2.0\2.10.0\loaders.cache`
4. GTK+-IMModules
`gtk-query-immodules-3.0 > c:\gtk\lib\gtk-3.0\3.0.0\immodules.cache`
5. Ausprobieren ob GTK funktioniert:
`gtk3-demo`


#### ASSIMP 3.0
http://assimp.sourceforge.net/main_downloads.html

http://sourceforge.net/projects/assimp/files/assimp-3.0/assimp-sdk-3.0-setup.exe/download

SDK Setup herunterladen und Umgebungsvariable ASSIMP_ROOT setzen
`Bsp: ASSIMP_ROOT = C:\assimp`

** Hinweis: Unter Windows muss die Datei `C:\Assimp\bin\x86\Assimp32.dll` in  `C:\Windows\SysWOW64` kopiert werden, damit der Assimp Loader funktioniert. **

#### GLM

**Hinweis: Version < 0.9.6 benutzen, da ab hier Radians/Degree Angaben in Funktionen getauscht wurden **

https://github.com/g-truc/glm/releases/download/0.9.5.4/glm-0.9.5.4.zip

Systemumgebungsvariable anlegen:
`Bsp: GLM_ROOT = C:\glm`

#### GLFW
http://www.glfw.org/

https://github.com/glfw/glfw/releases/download/3.1.1/glfw-3.1.1.bin.WIN32.zip

Systemumgebungsvariable anlegen:
`Bsp: GLFW3_ROOT = C:\glfw-3.1.1`

#### GLEW
http://glew.sourceforge.net/

Mit Binaries für Windows downloaden:
https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0-win32.zip/download

Systemumgebungsvariable anlegen:
`Bsp: OpenGL_ROOT = C:\glew-1.13.0`

** Wichtig: glcorearb.h Datei fehlt hier, deshalb muss sie nachträglich heruntergeladen und in `C:/glew-1.13.0/include/GL` kopiert werden. **

https://www.opengl.org/registry/api/GL/glcorearb.h

#### DevIL
http://sourceforge.net/projects/openil/files/DevIL%20Windows%20SDK/1.7.8/DevIL-SDK-x86-1.7.8.zip/download

Systemumgebungsvariable anlegen:
`Bsp: DEVIL_ROOT = C:\DevIL`

#### CMake
http://www.cmake.org/download/

Für Windows:
http://www.cmake.org/files/v3.3/cmake-3.3.1-win32-x86.exe

### Projekt Starten in Visual Studio 2013
1. CMake Starten
2. Projekt Ordner angeben:
`Where is the source code: ...` z.B.: `D:/Projekte/FP14/src`
3. Ziel Ordner in den das Binary Projekt gebuildet wird:

** Hinweis: Ordner nicht unter Source Ordner legen, da er sonst in github als neuer Commit gelistet wird. Im Build Ordner sollten allerdings nie Dateien geändert werden, bzw. sollte der Ordner nicht in github erscheinen! **

`Where o build the binaries: ` z.B.: `D:/Projekte/FP14_build` also ** nicht: ** `D:/Projekte/FP14/build´
4. Button `Configure` klicken und Entwicklungsumgebung (IDE) auswählen. In unserem Beispiel: `Visual Studio 12 2013` mit Option `Use default native compilers`
5. `Generate` klicken.
6. In Build Ordner gehen (Bspw.: `D:/Projekte/FP14/build´) und Solution Datei starten (.sln).
7. Solution builden und Startprojekt auf `Test_Telepresence` setzen.
8. Projekt Starten
