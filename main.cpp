#include "MultipoleField.hh"
#include "MultipolePlot.hh"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <memory>

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    // auto multipoleField = std::make_shared<MultipoleField>();
    QDir directory("../inputFiles");
    QString filePath = directory.filePath("pointChargesAligned.txt");
    auto multipoleField = std::make_shared<MultipoleField>(filePath);

    qDebug() << "Potential = " << multipoleField->potential(arma::vec{0, 0});
    qDebug() << "Bounding box: " << multipoleField->boundingBox();

    QQmlApplicationEngine engine;
    qmlRegisterType<MultipolePlot>("QCustomPlot.MultipolePlot", 1, 0, "MultipolePlot");

    QQmlContext* context = engine.rootContext();
    context->setContextProperty("multipoleField", QVariant::fromValue(multipoleField.get()));

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](const QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
            {
                QCoreApplication::exit(-1);
            }
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
