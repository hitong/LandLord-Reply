#include "Landlord.h"
#include <QtWidgets/QApplication>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Landlord w;
	w.show();
	return a.exec();
}
