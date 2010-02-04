#include <QtTest>
#include <QtCore>
#include <QtGui>
#include <QtSql>

#include "itmpl_sql_plugin.h"
#include "plugin_worker.h"
using namespace VirtualPrninterPlugins;

class testTmpl_sql_plugin : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void loadPlugin();

    //void getPageSizeList();
    void createEmptyTemplate_data();
    void createEmptyTemplate();

    void openTemplates_data();
    void openTemplates();

private:
    pluginWorker *testObj;

};


void testTmpl_sql_plugin::initTestCase()
{
    testObj = new pluginWorker();
    //QSignalSpy spy(testObj, SIGNAL(error(pluginsError,QString)));
}

void testTmpl_sql_plugin::loadPlugin()
{
    QVERIFY2(testObj->loadPlugin(),"Plugin not load");
}

void testTmpl_sql_plugin::cleanupTestCase()
{
    //testObj->closeTemplates();

    //QVERIFY2(!testObj->hasError(),"Error close templates file");
}

//********************* Набор тестов
void testTmpl_sql_plugin::openTemplates_data()
{
    QTest::addColumn<QString>("arg1");  /// Путь к файлу шаблона
    QTest::addColumn<bool>("result");       /// Результат выполнения функции

    QTest::newRow("emptyFileName")    << "" << false;
    QTest::newRow("notValidFileName") << "fantom file" << false;
    QTest::newRow("ValidFileName")    << "d://test_template.tmpl" << true;
}

void testTmpl_sql_plugin::openTemplates()
{
    QFETCH(QString, arg1);
    QFETCH(bool, result);
    QCOMPARE(testObj->openTemplates(arg1),result);
    /*
    QList<QVariant> arguments = spy.takeFirst();
    QVERIFY(arguments.at(0).type() == QVariant::Int);
    QVERIFY(arguments.at(1).type() == QVariant::QString);
    */
}
void testTmpl_sql_plugin::createEmptyTemplate_data()
{
    QTest::addColumn<QString>("arg1");  /// Путь к файлу шаблона
    QTest::addColumn<bool>("result");       /// Результат выполнения функции

    QTest::newRow("emptyFileName")    << "" << false;
    QTest::newRow("notValidFileName") << "fantom file" << false;
    QTest::newRow("ValidFileName")    << "d://test_template.tmpl" << true;
}

void testTmpl_sql_plugin::createEmptyTemplate()
{
    QFETCH(QString, arg1);
    QFETCH(bool, result);
    QCOMPARE(testObj->createEmptyTemplate(arg1),result);
}


//-----------------------------------------------------------------------------------



QTEST_MAIN(testTmpl_sql_plugin)
#include "t1.moc"

