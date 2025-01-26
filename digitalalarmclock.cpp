#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <cstdlib>
#ifdef _WIN32
#include <windows.h> // For Beep function on Windows
#endif

using namespace std;

class Time
{
protected:
    int hour, minute, second;

public:
    Time() : hour(0), minute(0), second(0) {}

    void setTime(int h, int m, int s) {
        hour = h;
        minute = m;
        second = s;
    }

    void incrementTime() {
        second++;
        if (second == 60) {
            second = 0;
            minute++;
            if (minute == 60) {
                minute = 0;
                hour = (hour + 1) % 24;
            }
        }
    }

    void displayTime() const {
        cout << setw(2) << setfill('0') << hour << ":"
             << setw(2) << setfill('0') << minute << ":"
             << setw(2) << setfill('0') << second;
    }

    bool isEqual(const Time &t) const {
        return hour == t.hour && minute == t.minute && second == t.second;
    }
};

class Alarm : public Time {
private:
    bool alarmSet;

public:
    Alarm() : alarmSet(false) {}

    void setAlarm(int h, int m, int s) {
        setTime(h, m, s);
        alarmSet = true;
    }

    void disableAlarm() {
        alarmSet = false;
    }

    bool checkAlarm(const Time &currentTime) {
        return alarmSet && isEqual(currentTime);
    }
};

class Clock {
private:
    Time currentTime;
    Alarm alarm;

public:
    Clock(int h, int m, int s) {
        currentTime.setTime(h, m, s);
    }

    void setAlarm(int h, int m, int s) {
        alarm.setAlarm(h, m, s);
    }

    void runClock() {
        while (true) {
            system("clear"); // Clear the screen for better display
            currentTime.displayTime();

            if (alarm.checkAlarm(currentTime)) {
                cout << "  * ALARM! *" << endl;

#ifdef _WIN32
                // Generate a simple alarm sound
                Beep(1000, 7000); // Frequency of 1000 Hz for 1 second
#endif

                this_thread::sleep_for(chrono::seconds(5)); // Alarm delay
                alarm.disableAlarm();
            } else {
                cout << endl;
            }

            this_thread::sleep_for(chrono::seconds(1)); // Simulate real time
            currentTime.incrementTime();
        }
    }
};

int main() {
    int h, m, s;
    cout << "Set the initial time (HH MM SS): ";
    cin >> h >> m >> s;

    Clock myClock(h, m, s);

    cout << "Set alarm time (HH MM SS): ";
    cin >> h >> m >> s;
    myClock.setAlarm(h, m, s);

    cout << "Clock is running... Press Ctrl+C to stop." << endl;
    myClock.runClock();

    return 0;
}
