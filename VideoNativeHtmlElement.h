#ifndef VIDEONATIVEHTMLELEMENT_H
#define VIDEONATIVEHTMLELEMENT_H

#include <QQuickItem>
#include <emscripten/val.h>

class VideoNativeHtmlElement : public QQuickItem
{
public:
    VideoNativeHtmlElement()
    {
        createVideoElement();
    }

    void componentComplete() override {
        polish();
    }

    void createVideoElement()
    {
        // Create <video> element and add it to the page body
        emscripten::val document = emscripten::val::global("document");
        emscripten::val body = document["body"];
        m_video = document.call<emscripten::val>("createElement", std::string("video"));
        body.call<void>("appendChild", m_video);

        // Create/add video source
        emscripten::val srcElement = document.call<emscripten::val>("createElement", std::string("source"));
        srcElement.set("src", m_source.toStdString());
        m_video.call<void>("appendChild", srcElement);

        // Set position:absolute, which makes it possible to position the video element
        // using x,y. coordinates, relative to its parent (the page's <body> element)
        emscripten::val style = m_video["style"];
        style.set("position", "absolute");
        m_video.call<void>("setAttribute", std::string("controls"), std::string("true"));
        m_video.call<void>("play");
    }

    void updatePolish() override
    {

        // FIXME: change to QRectF() float?
        QRect windowGeometry{int(x()), int(y()), int(width()), int(height())};
        qDebug() << windowGeometry << width();

        // Resolve the geometry against Qt canvas geometry, in case the canvas
        // is offset (not at (0,0)) relative to the body element. The id for the
        // canvas is set in the .html file, in this case "qtcanvas" when using the
        // default generated html.
        emscripten::val document = emscripten::val::global("document");
        emscripten::val canvas = document.call<emscripten::val>("getElementById", std::string("qtcanvas"));
        if (canvas.isUndefined())
            qFatal("Could not find canvas element");
        emscripten::val rect = canvas.call<emscripten::val>("getBoundingClientRect");
        QPoint canvasPosition(rect["left"].as<int>(), rect["top"].as<int>());

        QRect videoElementGeometry(windowGeometry.topLeft() + canvasPosition, windowGeometry.size());
        qDebug() << videoElementGeometry;
        videoElementGeometry.adjust(40, 60, -40, -40); // add margins
        qDebug() << videoElementGeometry;

        emscripten::val style = m_video["style"];
        style.set("left", QString("%1px").arg(videoElementGeometry.left()).toStdString());
        style.set("top", QString("%1px").arg(videoElementGeometry.top()).toStdString());
        style.set("width", QString("%1px").arg(videoElementGeometry.width()).toStdString());
        style.set("height", QString("%1px").arg(videoElementGeometry.height()).toStdString());
        //style.set("z-index", "-1");
    }

private:

    // Test video, from e.g. https://test-videos.co.uk/bigbuckbunny/mp4-h264
    // Download and place in the build directory.
    QString m_source = "BigBuckBunny.mp4";
    emscripten::val m_video = emscripten::val::undefined();
};

#endif // VIDEONATIVEHTMLELEMENT_H
