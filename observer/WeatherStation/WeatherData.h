#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include "Observer.h"

using namespace std;

struct SWeatherInfo
{
	double temperature = 0;
	double humidity = 0;
	double pressure = 0;
};

struct SStatistic
{
	string name;
	double minValue = std::numeric_limits<double>::infinity();
	double maxValue = -std::numeric_limits<double>::infinity();
	double accValue = 0;

	SStatistic(string structName)
		:name(structName)
	{
	}
};

class CDisplay: public IObserver<SWeatherInfo>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
		Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
		остается публичным
	*/
	void Update(SWeatherInfo const& data) override
	{
		std::cout << "Current Temp " << data.temperature << std::endl;
		std::cout << "Current Hum " << data.humidity << std::endl;
		std::cout << "Current Pressure " << data.pressure << std::endl;
		std::cout << "----------------" << std::endl;
	}
};

class CStatsDisplay : public IObserver<SWeatherInfo>
{
private:
	/* Метод Update сделан приватным, чтобы ограничить возможность его вызова напрямую
	Классу CObservable он будет доступен все равно, т.к. в интерфейсе IObserver он
	остается публичным
	*/
	void Update(SWeatherInfo const& data) override
	{
		++m_countAcc;
		updateStatisticsValue(m_temperature, data.temperature);
		updateStatisticsValue(m_pressure, data.pressure);
		updateStatisticsValue(m_humidity, data.humidity);

		printStatisticsValue(m_temperature);
		printStatisticsValue(m_pressure);
		printStatisticsValue(m_humidity);
	}

private:
	void updateStatisticsValue(SStatistic & savedValue, double newValue)
	{
		if (savedValue.minValue > newValue)
		{
			savedValue.minValue = newValue;
		}
		if (savedValue.maxValue < newValue)
		{
			savedValue.maxValue = newValue;
		}

		savedValue.accValue += newValue;
	}

	void printStatisticsValue(SStatistic & statisticValue)
	{
		std::cout << "Max value " << statisticValue.maxValue << std::endl;
		std::cout << "Min value " << statisticValue.minValue << std::endl;
		std::cout << "Average value " << (statisticValue.accValue / m_countAcc) << std::endl;
		std::cout << "----------------" << std::endl;
	}

	SStatistic m_temperature = { "temperature" };
	SStatistic m_pressure = { "pressure" };
	SStatistic m_humidity = { "humidity" };
	unsigned m_countAcc = 0;
};

class CWeatherData : public CObservable<SWeatherInfo>
{
public:
	// Температура в градусах Цельсия
	double GetTemperature()const
	{
		return m_temperature;
	}
	// Относительная влажность (0...100)
	double GetHumidity()const
	{
		return m_humidity;
	}
	// Атмосферное давление (в мм.рт.ст)
	double GetPressure()const
	{
		return m_pressure;
	}

	void MeasurementsChanged()
	{
		NotifyObservers();
	}

	void SetMeasurements(double temp, double humidity, double pressure)
	{
		m_humidity = humidity;
		m_temperature = temp;
		m_pressure = pressure;

		MeasurementsChanged();
	}

protected:
	SWeatherInfo GetChangedData()const override
	{
		SWeatherInfo info;
		info.temperature = GetTemperature();
		info.humidity = GetHumidity();
		info.pressure = GetPressure();
		return info;
	}

private:
	double m_temperature = 0.0;
	double m_humidity = 0.0;	
	double m_pressure = 760.0;	
};
