#include <QCoreApplication>
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {

	QCoreApplication::setApplicationName("AmbiLight");
	QCoreApplication::setOrganizationName("TU Wien");

	QApplication a(argc, (char**)argv);
	a.exec();
	
	return 0;
}
