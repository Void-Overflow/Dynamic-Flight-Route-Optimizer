#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>

#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>

struct City {
	QString name;
	float lat;
	float lon;

	City(QString name, float lat, float lon)
		: name(name), lat(lat), lon(lon) {
	}
};

class Window : public QMainWindow {
	Q_OBJECT

public:
	explicit Window(QWidget* parent = nullptr);

	

	void drawLine(double lat1, double lon1, double lat2, double lon2);
	void drawGeodesic(double lat1, double lon1, double lat2, double lon2);

private slots:
	void performCalculation();

	void updateWeatherOverlay(bool enabled);
private:
	QWebEngineView* webView;
	QVector<City> cities;
	
	// Top Panel Widgets
	QComboBox* startCityCombo;
	QComboBox* endCityCombo;
	QComboBox* aircraftCombo;
	QLineEdit* payloadInput;
	QCheckBox* view3DCheck;
	QCheckBox* realWeatherCheck;
	QPushButton* calculateButton;

	// Status Panel Widgets
	QLabel* timeLabel;
	QLabel* fuelLabel;
	QLabel* speedLabel;
	QLabel* altitudeLabel;
};

#endif