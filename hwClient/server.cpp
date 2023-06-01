#include <QtCore/QCoreApplication>
#include <QFileSystemWatcher>
#include <QDir>

class ClientServer : public QCoreApplication {

	Q_OBJECT

public: 
	explicit ClientServer(int& argc, char** argv) : QCoreApplication(argc, argv)
	{
		if (QFile::exists(serverMessageFile)) {

			QString clientFile;
			qint32 fileIndex = 1;
			do {
				clientFile = "message_" + QString::number(fileIndex) + ".txt";
				fileIndex++;

			} while (QFile::exists(clientFile));

			QFile file(clientFile);
			if (file.open(QIODevice::WriteOnly)) {
				file.close();
				qDebug() << "created client file : " << clientFile;
			}
			qDebug() << "created a user.";
		}
		else {
			return;
		}
			QString currentServerSettingsFileDir = QDir::currentPath();
			serverSettingsWatcher.addPath(currentServerSettingsFileDir + "/" + serverSettingsFile);
			connect(&serverSettingsWatcher, &QFileSystemWatcher::fileChanged, this, &ClientServer::readSettingsFile);

			QString currentServerFileDir = QDir::currentPath();
			serverFileWatcher.addPath(currentServerFileDir + "/" + serverMessageFile);
			connect(&serverFileWatcher, &QFileSystemWatcher::fileChanged, this, &ClientServer::readServerFile);
	}

private:
	QString serverMessageFile = "server.txt";
	QString serverSettingsFile = "settings.txt";
	QFileSystemWatcher serverSettingsWatcher;
	QFileSystemWatcher serverFileWatcher;

private slots:
	void readServerFile(const QString& path) {
		QFile file(path);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			QString line = in.readLine();
			while (!line.isNull()) {
				qDebug() << line;

				line = in.readLine();
			}
			file.close();
		}
		else {
			qDebug() << "failed to open the server file.";
		}
	}

	void readSettingsFile(const QString& path) {
		QFile file(path);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&file);
			QString line = in.readLine();
			while (!line.isNull()) {
				qDebug() << "new server settings message : " << line;

				line = in.readLine();
			}
			file.close();
		}
		else {
			qDebug() << "failed to open the server file.";
		}
	}
};
//#include "main.moc"
