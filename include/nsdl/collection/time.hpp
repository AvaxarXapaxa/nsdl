#pragma once

#include <nsdl/collection/includes.hpp>
#include <nsdl/collection/misc.hpp>


namespace nk {
	// Returns the current year
	inline U16 getYear() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_year + 1900;
	}

	// Returns the current month's index (January starts at 0)
	inline U8 getMonth() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_mon;
	}


	// Returns the current date day
	inline U8 getDay() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_mday + 1;
	}

	// Returns the current hour
	inline U8 getHour() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_hour;
	}

	// Returns the current minute
	inline U8 getMinute() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_min;
	}

	// Returns the current second
	inline U8 getSecond() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_sec;
	}

	// Returns the current day of the week (Sunday starts at 0)
	inline U8 getWeekDay() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_wday;
	}

	// Returns the current day of the year
	inline U8 getYearDay() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_yday + 1;
	}

	// Returns if this current time is under the affects of daylight saving or not
	inline bool isDaylightSaving() {
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		return now->tm_isdst;
	}

	// Returns the time since 1970 in seconds as a floating point (double)
	inline double getTime() {
		#ifdef _WIN32
		return (double)std::chrono::system_clock::now().time_since_epoch().count() / 10000000;
		#else
		return (double)std::chrono::system_clock::now().time_since_epoch().count() / 1000000;
		#endif
	}

	// Returns the time in milliseconds as an integer
	inline U64 getMillis() {
		#ifdef _WIN32
		return std::chrono::system_clock::now().time_since_epoch().count() / 10000;
		#else
		return std::chrono::system_clock::now().time_since_epoch().count() / 1000;
		#endif
	}

	// Returns the time in microseconds as an integer
	inline U64 getMicros() {
		#ifdef _WIN32
		return std::chrono::system_clock::now().time_since_epoch().count() / 10;
		#else
		return std::chrono::system_clock::now().time_since_epoch().count();
		#endif
	}

	// Sleeps in seconds
	inline void sleep(const double seconds) {
		std::chrono::microseconds duration((U64)(seconds * 1000000));
		std::this_thread::sleep_for(duration);
	}
	
	// Sleeps in milliseconds
	inline void sleepMilli(const U64 millis) {
		std::chrono::milliseconds duration(millis);
		std::this_thread::sleep_for(duration);
	}

	// Sleeps in microseconds
	inline void sleepMicros(const U64 micros) {
		std::chrono::microseconds duration(micros);
		std::this_thread::sleep_for(duration);
	}
}
