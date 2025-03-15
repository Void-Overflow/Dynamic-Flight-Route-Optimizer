#pragma warning(disable : 4996)

#include "window.h"

#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWebEngineSettings>

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QFont>
#include <QApplication>


#include <json/json.h>
#include <fstream>

Window::Window(QWidget* parent) : QMainWindow(parent)
{
	// Central widget and main layout
	QWidget* centralWidget = new QWidget(this);
	QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
	mainLayout->setContentsMargins(10, 10, 10, 10);
	mainLayout->setSpacing(10);

	// 1. Top Control Panel
	QGroupBox* topPanel = new QGroupBox("Flight Parameters", centralWidget);
	QGridLayout* topLayout = new QGridLayout(topPanel);
	topLayout->setContentsMargins(10, 15, 10, 10);
	topLayout->setHorizontalSpacing(15);
	topLayout->setVerticalSpacing(10);

	// Create UI elements
	startCityCombo = new QComboBox(topPanel);
	endCityCombo = new QComboBox(topPanel);
	aircraftCombo = new QComboBox(topPanel);
	payloadInput = new QLineEdit(topPanel);
	view3DCheck = new QCheckBox("3D View", topPanel);
	realWeatherCheck = new QCheckBox("Real Weather", topPanel);
	calculateButton = new QPushButton("Calculate Route", topPanel);

	//read city data from JSON file
	std::ifstream citiesJsonFile(QDir::currentPath().toStdString() + "/data/cities.json");
	Json::Value citiesJson;
	Json::CharReaderBuilder jsonBuilder;

	if (citiesJsonFile.fail()) {
		Json::Reader jsonReader;
		jsonReader.parse(std::string(R"("{1": {"city": "Dubai","lat" : 25.276987,"lon" : 55.296249}})"), citiesJson);
	}
	else {
		std::string err;
		if (!parseFromStream(jsonBuilder, citiesJsonFile, &citiesJson, &err))
			qDebug() << err;
	}
	QList<QString> citiesAsList;
	for (int i = 0; i < citiesJson.size(); i++) {
		cities.append(City(
			QString::fromStdString(citiesJson[std::to_string(i)]["city"].asString()), 
			citiesJson[std::to_string(i)]["lat"].asFloat(), 
			citiesJson[std::to_string(i)]["lon"].asFloat()));
		citiesAsList.append(cities.back().name);
	}

	// Populate/setup comboboxes
	startCityCombo->addItems(citiesAsList);
	startCityCombo->setEditable(true);
	startCityCombo->setCurrentIndex(-1);
	startCityCombo->lineEdit()->setPlaceholderText("Enter starting city");
	endCityCombo->addItems(citiesAsList);
	endCityCombo->setEditable(true);
	endCityCombo->setCurrentIndex(-1);
	endCityCombo->lineEdit()->setPlaceholderText("Enter ending city");
	aircraftCombo->addItems({ "Boeing 747", "Airbus A380", "Cessna 172", "Airbus A320" });
	aircraftCombo->setEditable(true);
	aircraftCombo->setCurrentIndex(-1);
	aircraftCombo->lineEdit()->setPlaceholderText("Select aircraft");
	payloadInput->setPlaceholderText("Enter payload (kg)");

	// Add widgets to top layout
	topLayout->addWidget(startCityCombo, 0, 0);
	topLayout->addWidget(aircraftCombo, 0, 1);
	topLayout->addWidget(endCityCombo, 1, 0);
	topLayout->addWidget(payloadInput, 1, 1);
	topLayout->addWidget(view3DCheck, 2, 0);
	topLayout->addWidget(realWeatherCheck, 2, 1);

	// Add calculate button spanning 3 rows
	topLayout->addWidget(calculateButton, 0, 2, 3, 1, Qt::AlignCenter);

	// Set column stretch factors
	topLayout->setColumnStretch(0, 3);
	topLayout->setColumnStretch(1, 2);
	topLayout->setColumnStretch(2, 1);

	// 2. Map View
	webView = new QWebEngineView(centralWidget);
	webView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
	webView->settings()->setAttribute(QWebEngineSettings::AllowRunningInsecureContent, true); 
	webView->settings()->setAttribute(QWebEngineSettings::AutoLoadImages, true); 
	webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
	webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanOpenWindows, true);
	webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);

	// 3. Status Panel
	QGroupBox* statusPanel = new QGroupBox("Flight Status", centralWidget);
	QGridLayout* statusLayout = new QGridLayout(statusPanel);
	statusLayout->setContentsMargins(10, 10, 10, 10);
	statusLayout->setHorizontalSpacing(20);

	timeLabel = new QLabel("Time: --:-- UTC", statusPanel);
	fuelLabel = new QLabel("Fuel: 0 kg", statusPanel);
	speedLabel = new QLabel("Speed: 0 kts", statusPanel);
	altitudeLabel = new QLabel("Altitude: 0 ft", statusPanel);

	statusLayout->addWidget(timeLabel, 0, 0);
	statusLayout->addWidget(speedLabel, 0, 1);
	statusLayout->addWidget(fuelLabel, 1, 0);
	statusLayout->addWidget(altitudeLabel, 1, 1);

	// Assemble main layout
	mainLayout->addWidget(topPanel);
	mainLayout->addWidget(webView, 1);  // Map takes most space
	mainLayout->addWidget(statusPanel);

	// Button styling
	QFont appFont = QApplication::font();
	appFont.setPointSize(10);
	centralWidget->setFont(appFont);

	QString styleSheet = R"(
        QGroupBox {
            border: 1px solid #cccccc;
            border-radius: 4px;
            margin-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px;
        }
        QPushButton {
            min-width: 120px;
            min-height: 40px;
            padding: 8px;
            background-color: #007acc;
            color: white;
            border-radius: 4px;
        }
        QPushButton:hover {
            background-color: #0062a3;
        }
        QComboBox, QLineEdit {
            min-height: 30px;
            padding: 4px;
        }
    )";
	centralWidget->setStyleSheet(styleSheet);

	// Web view initialization
	QString mapPath = QDir::currentPath() + "/map.html";
	webView->setUrl(QUrl::fromLocalFile(mapPath));

	// Set window properties
	setCentralWidget(centralWidget);
	resize(1280, 800);
	setWindowTitle("Dynamic Flight Route Optimizer");

	// Connect button signal
	connect(calculateButton, &QPushButton::clicked,
		this, &Window::performCalculation);

	//connect real weather checkbox signal
	connect(realWeatherCheck, &QCheckBox::toggled,
		this, &Window::updateWeatherOverlay);
}

void Window::performCalculation()
{
	qDebug() << "Calculating route...";
	qDebug() << "Start:" << startCityCombo->currentText();
	qDebug() << "End:" << endCityCombo->currentText();
	qDebug() << "Aircraft:" << aircraftCombo->currentText();
	qDebug() << "Payload:" << payloadInput->text() << "kg";

	//Find start/end coordinates
	std::pair<float, float> startCoords; //lat, lon
	std::pair<float, float> endCoords; //lat, lon

	for (int i = 0; i < cities.size(); i++) {
		if (cities[i].name == startCityCombo->currentText()) {
			startCoords.first = cities[i].lat;
			startCoords.second = cities[i].lon;
		}
		else if(cities[i].name == endCityCombo->currentText()) {
			endCoords.first = cities[i].lat;
			endCoords.second = cities[i].lon;
		}

		if (startCoords.first != 0 && endCoords.first != 0)
			break;
	}

	drawGeodesic(startCoords.first, startCoords.second, endCoords.first, endCoords.second);

	// Update status labels
	timeLabel->setText("Time: " + QDateTime::currentDateTimeUtc().toString("hh:mm UTC"));
	fuelLabel->setText("Fuel: 25000 kg");
	speedLabel->setText("Speed: 480 kts");
	altitudeLabel->setText("Altitude: 35000 ft");
}

void Window::updateWeatherOverlay(bool enabled) {
	QString js = QString("if (%1) { showWeather(); } else { hideWeather(); }")
		.arg(enabled ? "true" : "false");
	webView->page()->runJavaScript(js);
}

void Window::drawLine(double lat1, double lon1, double lat2, double lon2) {
	// Use invariant locale for numbers
	QString jsCode = QString(
		"drawLine(%1, %2, %3, %4);"
	).arg(lat1, 0, 'g', 10)
		.arg(lon1, 0, 'g', 10)
		.arg(lat2, 0, 'g', 10)
		.arg(lon2, 0, 'g', 10);

	// Execute after page loads
	connect(webView->page(), &QWebEnginePage::loadFinished,
		[this, jsCode](bool ok) {
			if (ok) webView->page()->runJavaScript(jsCode);
		}
	);
}

void Window::drawGeodesic(double lat1, double lon1, double lat2, double lon2) {
	QString jsCode = QString(
		"drawGeodesic(%1, %2, %3, %4);"
	).arg(lat1, 0, 'g', 10)
		.arg(lon1, 0, 'g', 10)
		.arg(lat2, 0, 'g', 10)
		.arg(lon2, 0, 'g', 10);

	// Wait for page and plugin to load
	QTimer::singleShot(1000, [this, jsCode]() {
		webView->page()->runJavaScript(jsCode);
		});
}