//#include <QtCore/QCoreApplication>
//#include <QFileSystemWatcher>
//#include <QDir>
//
//class Client : public QObject {
//
//	Q_OBJECT
//
//public:
//	Client() {
//
//		if (QFile::exists(serverMessageFile)) {
//			QString clientFile;
//			qint32 fileIndex = 1;
//			do {
//				clientFile = "message_" + QString::number(fileIndex) + ".txt";
//				fileIndex++;
//
//			} while (QFile::exists(clientFile));
//
//			QFile file(clientFile);
//			if (file.open(QIODevice::WriteOnly)) {
//				file.close();
//				qDebug() << "created client file : " << clientFile;
//			}
//			qDebug() << "created a user.";
//		}
//		else {
//			return;
//		}
//
//		QString currentServerFileDir = QDir::currentPath();
//		serverFileWatcher.addPath(currentServerFileDir + "/" + serverMessageFile);
//		connect(&serverFileWatcher, &QFileSystemWatcher::fileChanged, this, &Client::readServerFile);
//	}
//
//private:
//	QString serverMessageFile = "server.txt";
//	QFileSystemWatcher serverFileWatcher;
//	QString fileName;
//
//private slots:
//	void readServerFile(const QString& path) {
//		QFile file(path);
//		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//			QTextStream in(&file);
//			QString line = in.readLine();
//			while (!line.isNull()) {
//				qDebug() << line;
//
//				line = in.readLine();
//			}
//			file.close();
//		}
//		else {
//			qDebug() << "failed to open the server file.";
//		}
//	}
//};