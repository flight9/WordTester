SET rdir=WordTester_win32

COPY /y KanboxPlugin.dll %rdir%\
COPY /y WordTester_zh_CN.qm %rdir%\
COPY /y qt_zh_CN.qm %rdir%\
COPY /y ..\KanboxPlugin\KanboxPlugin_zh_CN.qm %rdir%\
COPY /y ..\KanboxPlugin\KanboxPlugin_zh_CN.qm .\

@ECHO OFF
SET /p choice=Please choose 1.Debug or 2.Release:
@ECHO ON
IF %choice%==1 (
	COPY /y debug\WordTester.exe %rdir%\ 
) ELSE (
	COPY /y release\WordTester.exe %rdir%\ 
)

pause