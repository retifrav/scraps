#if defined(Q_OS_ANDROID)
    qDebug() << "android";
#elif defined(Q_OS_BLACKBERRY)
    qDebug() << "blackberry";
#elif defined(Q_OS_IOS)
    qDebug() << "ios";
#elif defined(Q_OS_MACOS)
    qDebug() << "macos";
#elif defined(Q_OS_TVOS)
    qDebug() << "tvos";
#elif defined(Q_OS_WATCHOS)
    qDebug() << "watchos";
#elif defined(Q_OS_WINCE)
    qDebug() << "wince";
#elif defined(Q_OS_WIN)
    qDebug() << "windows";
#elif defined(Q_OS_LINUX)
    qDebug() << "linux";
#elif defined(Q_OS_UNIX)
    qDebug() << "unix";
#else
    qDebug() << "unknown";
#endif
