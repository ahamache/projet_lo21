#include <iostream>
#include <QtXml>
#include <QApplication>
#include <QtGui>
#include "xml_dom.h"

using namespace std;

int main()
{

	QApplication app(argc,argv);
	Xml_Dom *Dom = new Xml_Dom();
	Dom->show();

	return app.exec();

}
