#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "mtg/mtg.hpp"

#include <thread>

#include <iostream>
int main(int argc, char *argv[])
{

    std::thread mtgEngine(MTG::loop);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl("file:///C:/Users/simonp/Documents/qmtg/main.qml"));

    auto res = app.exec();
    mtgEngine.join();
    return res;
}
