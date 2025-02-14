#include <QtQuick/QQuickWindow.h>
#include <QtQuick/QQuickPaintedItem.h>

#include "qpainter.h"
#include "qtextstream.h"

#include "reader.h"

#include <cmath>

constexpr uint8_t positiveValueSize = 17;

class FilenameListener : public QObject {
	Q_OBJECT
public:
	QString m_filename;
	ErrorProvider* m_eLog;

	void setErrorLog(ErrorProvider* log) {
		m_eLog = log;
	}

public slots:
	void receiveFilename(const QString& name) {
		if (name.isEmpty())
			m_eLog->addError(QString("File name is Empty"));
		m_filename = name;
		emit fnChanged();
	}

signals:
	void fnChanged();
};

class TsValue {
	bool m_valid = true;
public:
	double m_freq = 0;
	double m_S1[2] = { 0, 0 };

	TsValue() {};
	TsValue(double freq, double S1_1, double S1_2, bool valid = true)
		: m_freq(freq)
		, m_valid(valid) {
		m_S1[0] = S1_1;
		m_S1[1] = S1_2;
	}
	~TsValue() {};

	bool isValid() const { return m_valid; };

	double getLGVal() const {
		return 20 * std::log10(std::sqrt(m_S1[0] * m_S1[0] + m_S1[1] * m_S1[1]));
	}

	static TsValue fromString(QString str, ErrorProvider* eLog) {
		QStringList fields = str.split(" ");

		if (fields.size() != 3)
			eLog->addError(QString("Value line is corrupted!"));

		double dval[3] = { 0 };
		int idx = 0;

		for (QString value : fields) {
			if (value.size() < positiveValueSize || value.size() > (positiveValueSize + 1)) {
				eLog->addError(QString("Value line is corrupted!"));
				return TsValue(dval[0], dval[1], dval[2], false);
			}

			if (value.size() == (positiveValueSize + 1) && value[0] != "-") {
				eLog->addError(QString("Value line is corrupted!"));
				return TsValue(dval[0], dval[1], dval[2], false);
			}

			dval[idx++] = value.toDouble();
		}

		return TsValue(dval[0], dval[1], dval[2]);
	};
};

class DataStorage {
	std::vector<TsValue> m_values;

public:
	DataStorage() {};
	~DataStorage() {};

	void pushValue(TsValue& value) {
		m_values.push_back(value);

		if (value.m_freq > xRange.second)
			xRange.second = value.m_freq;
		if (value.m_freq < xRange.first)
			xRange.first = value.m_freq;

		double yVal = value.getLGVal();
		if (yVal > yRange.second)
			yRange.second = yVal;
		if (yVal < yRange.first)
			yRange.first = yVal;
	}

	int size() { return m_values.size(); };

	std::pair<double, double> xRange = { 1000000, -10000000 };
	std::pair<double, double> yRange = { 1000000, -10000000 };

	void reset() { 
		xRange = { 1000000, -10000000 };
		yRange = { 1000000, -10000000 };
		m_values.clear();
	}

	const TsValue& operator[] (int idx) {
		if (idx < 0 || idx > size())
			return TsValue(0, 0, 0, false);

		return m_values[idx];
	}
};

class DataVisualyzer : public QQuickPaintedItem {
	Q_OBJECT
	Q_PROPERTY(QQuickWindow* window WRITE setWindow)
	Q_PROPERTY(ErrorProvider* errorLog WRITE setErrorLog)
private:
	QQuickWindow* m_mainWindow;
public:
	DataVisualyzer() : m_mainWindow(nullptr) { 
		QObject::connect(&m_listener, SIGNAL(fnChanged()),
			this, SLOT(onFnChanged()));
	};
	~DataVisualyzer() {};

	void setWindow(QQuickWindow* window) {
		m_mainWindow = window;
		QObject::connect(m_mainWindow, SIGNAL(sendFilename(QString)),
			&m_listener, SLOT(receiveFilename(QString)));
	};

	void setErrorLog(ErrorProvider* eLog) {
		m_eLog = eLog;
		m_listener.setErrorLog(m_eLog);
	}

	ErrorProvider* m_eLog = nullptr;
	FilenameListener m_listener;
	DataStorage m_data;

	void paint(QPainter* painter) {
		if (m_listener.m_filename.isEmpty())
			return;

		if (m_data.size() == 0) {
			FileParser parser(m_listener.m_filename, m_eLog);
			if (!parser.isValid())
				return;

			QString str;
			while (!((str = parser.readString()).isEmpty())) {
				m_data.pushValue(TsValue::fromString(str.simplified(), m_eLog));

				if (!m_data[m_data.size() - 1].isValid() && !parser.eof()) {
					m_eLog->addError(QString("Invalid value detected!"));
					break;
				}
			}
		}

		painter->fillRect(30, 30, width() - 60, height() - 80, Qt::blue);
		painter->drawText(QPoint(40, 25), QString("WxH = %1x%2").arg(width()).arg(height()));
		painter->drawLine(QPoint(30, 30), QPoint(30, height() - 50));
		painter->drawLine(QPoint(30, height() - 50), QPoint(width() - 30, height() - 50));
		painter->drawText(QPoint(40, height() - 30), QString("minX = %1").arg(m_data.xRange.first));

		painter->save();
		painter->translate(QPoint(30, height() - 50));
		painter->rotate(270);
		painter->drawText(QPoint(10, -10), QString("minY = %1").arg(m_data.yRange.first));
		painter->restore();

		double scaleX = (m_data.xRange.second - m_data.xRange.first) / (width() - 60);
		double scaleY = (m_data.yRange.second - m_data.yRange.first) / (height() - 80);
		double xOff = (m_data.xRange.first / scaleX);
		double yOff = (m_data.yRange.first / scaleY);
		painter->translate(-xOff + 30, -yOff + 30);

		QPen pen = painter->pen();
		pen.setColor(Qt::white);
		painter->setPen(pen);

		QPointF prev = { 0, 0 };
		for (int i = 0; i < m_data.size(); i++)
		{
			const TsValue& val = m_data[i];
			if (!val.isValid())
				break;

			double yVal = val.getLGVal();

			QPointF cur = QPointF(val.m_freq / scaleX, yVal / scaleY);
			//////////////////////////////////////////////////////////////////////////
			//drawing
			if (i != 0)
				painter->drawLine(prev, cur);

			painter->drawEllipse(cur, 2, 2);
			prev = cur;
		}
	}

public slots:
	void onFnChanged() { 
		m_data.reset();
		m_eLog->reset();
		update();
	}
};