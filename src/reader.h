#include "qfile.h"
#include "qfileinfo.h"

#include "error.h"

class FileParser {
	QFile* m_file = nullptr;
	QTextStream* m_text = nullptr;
	QString m_fileName;
	ErrorProvider* m_eLog = nullptr;

	void openFile() {
		QFileInfo fi(m_fileName);
		if ((!fi.exists() || !fi.isReadable())) {
			m_eLog->addError(QString("File: ") + m_fileName + QString("is not available!!!"));
			return;
		}

		m_file = new QFile(m_fileName);
		if (!m_file->exists() || !m_file->open(QIODevice::ReadOnly)) {
			m_eLog->addError(QString("File: ") + m_fileName + QString("is not available!!!"));
			m_file->close();
			return;
		}

		m_text = new QTextStream(m_file);
		parseHeader();
	}

public:
	FileParser() {}
	FileParser(QString fileName, ErrorProvider* eLog)
		: m_fileName(fileName)
		, m_eLog(eLog) {
		openFile();
	}

	~FileParser() {
		if (m_text)
			delete m_text;

		if (m_file) {
			m_file->close();
			delete m_file;
		}
	}

	void setErrorLog(ErrorProvider* log) {
		m_eLog = log;
	}

	QString findMarkedString(QString mark) {
		while (!m_text->atEnd()) {
			QString line = m_text->readLine();
			QStringList fields = line.split(" ");

			if (fields[0] == mark)
				return line;
		}
		return QString();
	}

	uint8_t parseHeader() {
		QString header = findMarkedString("#");
		QString data = findMarkedString("!");
		QString fields = findMarkedString("!");

		if (header.isEmpty() || data.isEmpty() || fields.isEmpty()) {
			m_eLog->addError(QString("File is corrupted"));
			return -1;
		}

		return 0;
	}

	bool eof() { return m_text->atEnd(); }

	QString readString() {
		if (!m_text->atEnd())
			return m_text->readLine();

		return QString();
	};

	bool isValid() {
		return m_text != nullptr;
	}

	void reset() {
		m_text->reset();
		m_file->reset();
	}
};
