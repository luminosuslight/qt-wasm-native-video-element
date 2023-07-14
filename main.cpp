#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "VideoNativeHtmlElement.h"
#include "TextNativeHtmlElement.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<VideoNativeHtmlElement>("NativeHtmlElements", 1, 0, "VideoNativeHtmlElement");
    qmlRegisterType<TextNativeHtmlElement>("NativeHtmlElements", 1, 0, "TextNativeHtmlElement");

    const QUrl url(u"qrc:/qt-wasm-video-element/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
