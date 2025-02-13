#include <qobject.h>
#include <qstring.h>
#include <qstringlist.h>

class ErrorProvider : public QObject {
	Q_OBJECT
	Q_PROPERTY(int count READ size)
	Q_PROPERTY(QStringList errors READ getErrors NOTIFY errorAdded)
public:
	QStringList errorList;
	QStringList getErrors() { return errorList; }
	int size() { return errorList.size(); }

	void reset() { errorList.clear(); }
	void addError(const QString& error) {
		errorList.append(error);
		emit errorAdded(errorList);
	}

signals:
	void errorAdded(QStringList);
};