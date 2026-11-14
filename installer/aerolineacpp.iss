; Instalador Inno Setup de AerolineaCPP.
;
; Script genérico: la versión NO se escribe aquí. GitHub Actions la extrae de
; project(AerolineaCPP VERSION ...) en modern-qt-sqlserver/CMakeLists.txt y la
; pasa al compilador:
;
;   ISCC /DMyAppVersion=1.1.1 /DSourceDir=<payload> /DOutputDir=<salida> aerolineacpp.iss
;
; El payload es el mismo contenido del ZIP portable (windeployqt + LEEME.txt +
; LICENSE.txt + config\database.example.ini + Aerolinea.sql).
;
; Configuración del usuario: la aplicación la guarda en
; %LOCALAPPDATA%\JM Software Solutions\AerolineaCPP\database.ini, fuera de
; {app}. El desinstalador elimina solo lo que instaló y conserva esa
; configuración personal.

#ifndef MyAppVersion
  #error "Define MyAppVersion (por ejemplo /DMyAppVersion=1.1.1); se toma de CMakeLists.txt."
#endif

#define MyAppName "AerolineaCPP"
#define MyAppPublisher "Francis Jairo Matías Rosario"
#define MyAppExeName "AerolineaCPP.exe"
#define MyAppRepository "https://github.com/Jairo0811/Aerolinea"

#ifndef MyAppVersionTag
  #define MyAppVersionTag "v" + MyAppVersion
#endif

#ifndef SourceDir
  #define SourceDir "..\dist\AerolineaCPP-" + MyAppVersionTag + "-Windows-x64"
#endif

#ifndef OutputDir
  #define OutputDir "..\dist\installer"
#endif

[Setup]
; AppId fijo: permite actualizar en sitio una instalación anterior (v1.1.0).
AppId={{D6B0A4D2-7C6A-4D8C-9B5D-2B5AF31E10D8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppRepository}
AppSupportURL={#MyAppRepository}/issues
AppUpdatesURL={#MyAppRepository}/releases
AppCopyright=© 2018 F. J. Matías Rosario, J. de J. Torres Pérez y S. Donastor Hernández (ITLA). Modernización 2026: F. J. Matías Rosario.
DefaultDirName={autopf}\AerolineaCPP
DefaultGroupName=AerolineaCPP
DisableProgramGroupPage=yes
LicenseFile={#SourceDir}\LICENSE.txt
InfoBeforeFile={#SourceDir}\LEEME.txt
OutputDir={#OutputDir}
OutputBaseFilename=AerolineaCPP-{#MyAppVersionTag}-Setup
SetupIconFile=..\modern-qt-sqlserver\aerolineacpp.ico
UninstallDisplayIcon={app}\{#MyAppExeName}
Compression=lzma2/ultra64
SolidCompression=yes
WizardStyle=modern
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
MinVersion=10.0
CloseApplications=yes
RestartApplications=no
SetupLogging=yes
VersionInfoVersion={#MyAppVersion}.0
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppName} {#MyAppVersion} Setup
VersionInfoProductName={#MyAppName}
VersionInfoProductVersion={#MyAppVersion}.0
VersionInfoCopyright=© 2018–2026. Proyecto académico ITLA de portafolio.

[Languages]
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "Crear un acceso directo en el escritorio"; GroupDescription: "Accesos directos:"; Flags: unchecked

[Files]
Source: "{#SourceDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\AerolineaCPP"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"
Name: "{autoprograms}\AerolineaCPP - LEEME"; Filename: "{app}\LEEME.txt"
Name: "{autodesktop}\AerolineaCPP"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "Ejecutar AerolineaCPP"; Flags: nowait postinstall skipifsilent
