#include <QtCore/QCoreApplication>
#include <QFileSystemWatcher>
#include <QTextStream>
#include "entities.h"
#include <QFile>
#include <QDir>

class Server : public QCoreApplication {

	Q_OBJECT

public:
	explicit Server(int& argc, char** argv) : QCoreApplication(argc, argv) {
		QString path = txtDirectory + serverMessageFile;
		QDir directory(path);

		if (!directory.exists()) {
			if (directory.mkpath(path)) {

				qDebug() << "Directory created: " << directory;
			}else {

			qDebug() << "Failed to create the directory: " << directory;
			}
		}
		
		QFile file(serverMessageFile);
		if (file.exists()) {
			QFile::remove(serverMessageFile);
		}
		else {
			if (file.open(QIODevice::WriteOnly)) {
				file.close();
			}
		}
		qDebug() << "Server started. Created " << serverMessageFile << " file.";

		clientDirectoryWatcher.addPath(clientDirectory);
		connect(&clientDirectoryWatcher, &QFileSystemWatcher::directoryChanged, this, &Server::directoryChanged);
	}

	~Server() {
		for (QFileSystemWatcher* fileWatcher : fileWatchers) {
			delete fileWatcher;
		}
		deleteFiles();
	}

private:
	QString serverMessageFile = "server.txt";
	QString txtDirectory = "C:/QtHW/txt/";
	QString serverSettingsFile = "settings.txt";
	QList<QFileSystemWatcher*> fileWatchers;
	QString clientDirectory = "";
	QFileSystemWatcher clientDirectoryWatcher;

private slots:

	void readClientFile(const QString& path) {
		QFile file(path);
		QString fileName = file.fileName();

		if (fileName.startsWith("message_")) {
			QString userNumber = fileName.mid(8, fileName.length() - 12);

			if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				QString lastLine;
				QTextStream in(&file);
				QString line = in.readLine();
				while (!line.isNull()) {
					lastLine = line;
					line = in.readLine();
				}
				file.close();

				QFile serverFile(serverMessageFile);
				if (serverFile.open(QIODevice::Append | QIODevice::Text)) {
					QTextStream out(&serverFile);
					out << "User " << userNumber << " : " << lastLine << '\n';
					serverFile.close();
				}
				else {
					qDebug() << "failed to open the server file.";

				}
			}
			else {
				qDebug() << "failed to open the client file.";
			}
		}
	}

	void directoryChanged(const QString& path) {
		QFileInfoList files = QDir(clientDirectory).entryInfoList(QDir::Files);
		for (auto& fileInfo : files)
		{
			QString filePath = fileInfo.absoluteFilePath();
			if (filePath.startsWith(clientDirectory + "/message_")) {

				QFileSystemWatcher* fileWatcher = new QFileSystemWatcher;
				fileWatcher->addPath(filePath);
				connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &Server::readClientFile);
				fileWatchers.append(fileWatcher);
			}
		}
	}

	void deleteFiles() {
		QDir directory(txtDirectory);
		if (directory.exists()) {
			QFileInfoList fileList = directory.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
			for (const QFileInfo& fileInfo : fileList) {
				QFile::remove(fileInfo.absoluteFilePath());
			}
			qDebug() << "All files deleted in the directory " << txtDirectory;
		}
	}
};