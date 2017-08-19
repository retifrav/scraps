using System.Diagnostics;

...


    // подготовка записи в журнал
    string logName = "myMegaLog"; // имя лучше сменить на WebService,
    string source = "myMegaLog";  // но надо будет перезагрузиться

    if (!EventLog.SourceExists(source))
    {
        EventLog.CreateEventSource(source, logName, System.Environment.MachineName);
    }
    EventLog el = new EventLog(logName, System.Environment.MachineName, source);

    el.WriteEntry("Приложение запущено.");
    
// если в журнале Windows не появится нового журнала, то:
// - надо проверить, нет ли уже существующего журнала, к которого с новым вашим совпадают первые 8 символов;
// - если есть уже существующий такой же журнал и он точно вам не нужен, то закрыть просмотр событий и удалить его в реестре HKEY_LOCAL_MACHINE/SYSTEM/CurrentControlSet/Services/Eventlog;
// - перезагрузить компьютер (это рекомендуется в любом случае)
