#include "its.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ITS w;
	w.show();
	return a.exec();
}
