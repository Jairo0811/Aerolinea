#define MyAppName "AerolineaCPP"
#define MyAppVersion "1.1.0"
#define MyAppPublisher "Jairo Matías"
#define MyAppExeName "AerolineaCPP.exe"

#ifndef SourceDir
  #define SourceDir "..\dist\AerolineaCPP-v1.1.0-Windows-x64"
#endif

#ifndef OutputDir
  #define OutputDir "..\dist\installer"
#endif

[Setup]
AppId={{D6B0A4D2-7C6A-4D8C-9B5D-2B5AF31E10D8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} v{#MyAppVersion} — Final Portfolio Edition
AppPublisher={#MyAppPublisher}
AppPublisherURL=https://github.com/Jairo0811/Aerolinea
AppSupportURL=https://github.com/Jairo0811/Aerolinea/issues
AppUpdatesURL=https://github.com/Jairo0811/Aerolinea/releases
DefaultDirName={autopf}\AerolineaCPP
DefaultGroupName=AerolineaCPP
DisableProgramGroupPage=yes
LicenseFile=..\LICENSE
InfoBeforeFile=..\modern-qt-sqlserver\README.md
OutputDir={#OutputDir}
OutputBaseFilename=AerolineaCPP-v1.1.0-Setup
SetupIconFile=..\modern-qt-sqlserver\aerolineacpp.ico
UninstallDisplayIcon={app}\AerolineaCPP.exe
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
VersionInfoVersion=1.1.0.0
VersionInfoCompany=Jairo Matías
VersionInfoDescription=AerolineaCPP v1.1.0 Final Portfolio Edition Installer
VersionInfoProductName=AerolineaCPP
VersionInfoProductVersion=1.1.0.0
VersionInfoCopyright=Copyright 2018–2026. Portfolio project.

[Languages]
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "Crear un acceso directo en el escritorio"; GroupDescription: "Accesos directos:"; Flags: unchecked

[Files]
Source: "{#SourceDir}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{autoprograms}\AerolineaCPP"; Filename: "{app}\AerolineaCPP.exe"; WorkingDir: "{app}"
Name: "{autoprograms}\AerolineaCPP - LEEME"; Filename: "{app}\LEEME.txt"
Name: "{autodesktop}\AerolineaCPP"; Filename: "{app}\AerolineaCPP.exe"; WorkingDir: "{app}"; Tasks: desktopicon

[Run]
Filename: "{app}\AerolineaCPP.exe"; Description: "Ejecutar AerolineaCPP"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: filesandordirs; Name: "{app}\config"

[Code]
function InitializeSetup(): Boolean;
begin
  Result := True;
end;
