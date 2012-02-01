; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Syncer"
#define MyAppVersion "4.0a"
#define MyAppPublisher "MiBoSoft"
#define MyAppURL "http://www.github.com/mtboswell/syncer"
#define MyAppExeName "syncer-gui.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{C67F6FFA-E63E-405F-BAB2-BC2F2154C8D0}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=C:\Users\micah\Documents\working\syncer
OutputBaseFilename=syncer_setup
SetupIconFile=C:\Users\micah\Documents\working\syncer\syncer-gui\s_icon.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: "C:\Users\micah\Documents\working\syncer\bin\syncer-gui.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\basename.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\bash.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\cat.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\curl.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\env.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\expr.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-add--interactive"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-am"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-bisect"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-citool"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-credential-cache.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-credential-cache--daemon.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-credential-getpass.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-credential-store.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-daemon.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-difftool"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-difftool--helper"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-fast-import.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-filter-branch"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-format-patch.exe.manifest"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-gui"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-gui.tcl"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-gui--askpass"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-gui--askyesno"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-http-backend.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-http-fetch.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-http-push.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-imap-send.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-lost-found"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-merge-octopus"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-merge-one-file"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-merge-resolve"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-mergetool"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-mergetool--lib"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-parse-remote"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-patch-id.exe.manifest"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-pull"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-quiltimport"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-rebase"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-rebase--am"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-rebase--interactive"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-rebase--merge"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-relink"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-remote-ftp.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-remote-ftps.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-remote-http.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-remote-https.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-remote-testgit"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-repack"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-request-pull"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-send-email"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-sh-i18n"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-sh-i18n--envsubst.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-show-index.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-sh-setup"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-stash"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-submodule"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-svn"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-update-index.exe.manifest"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-update-ref.exe.manifest"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-update-server-info.exe.manifest"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-upload-pack.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\git-web--browse"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\grep.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\less.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\libcrypto.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\libeay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\libiconv-2.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\libssh.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\libz.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\mingwm10.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\msys-1.0.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\msys-crypto-0.9.8.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\msys-minires.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\msys-z.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\QtCore4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\QtGui4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion  
Source: "C:\Users\micah\Documents\working\syncer\bin\QtSvg4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\QtNetwork4.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\rm.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\sed.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\sh.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\ssh.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\ssh-keygen.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\ssleay32.dll"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\syncer.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\syncer-restore.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\syncer-shred.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\syncer-undelete.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\tr.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
Source: "C:\Users\micah\Documents\working\syncer\bin\uname.exe"; DestDir: "{app}\bin"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\bin\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\bin\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

