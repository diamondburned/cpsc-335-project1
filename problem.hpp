#pragma once

#include <algorithm>
#include <bitset>
#include <cstddef>
#include <exception>
#include <iomanip>
#include <ios>
#include <iostream>
#include <optional>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>

// Time describes a time of day.
struct Time {
  int hour;
  int minute;

  Time(int hour, int minute) : hour(hour), minute(minute) {}
  Time(int minutesFromMidnight)
      : hour(minutesFromMidnight / 60), minute(minutesFromMidnight % 60) {}

  // parseTime parses a time from a string in the format "HH:MM".
  // Implemented in problem_parse.cpp.
  static Time parseTime(std::string timeString);

  // minutesFromMidnight returns the number of minutes from midnight.
  int minutesFromMidnight() const { return hour * 60 + minute; }

  // Implemented in problem_print.cpp.
  friend std::ostream &operator<<(std::ostream &os, const Time &time);
};

struct TimeRange {
  Time from;
  Time to;

  TimeRange(Time from, Time to) : from(from), to(to) {
    if (from.minutesFromMidnight() > to.minutesFromMidnight()) {
      throw std::invalid_argument("from must be before to");
    }
  }

  // minutes returns the number of minutes between from and to.
  int minutes() const {
    return to.minutesFromMidnight() - from.minutesFromMidnight();
  }

  // Implemented in problem_print.cpp.
  friend std::ostream &operator<<(std::ostream &os, const TimeRange &range);
};

// PersonSchedule describes the schedule of a person.
struct PersonSchedule {
  // times is the list of times that the person is busy.
  std::vector<TimeRange> times;
  // dailyAvailability is the time that the person is available every day.
  TimeRange dailyAvailability;

  // Implemented in problem_print.cpp.
  friend std::ostream &operator<<(std::ostream &os,
                                  const PersonSchedule &schedule);
};

// ScheduleProblem describes the inputs to the matching group schedules problem.
// The problem is given a list of people and their schedules, and a meeting
// duration. The problem is to find all the time slots that all the people are
// available for at least the meeting duration.
struct ScheduleProblem {
  // people is the list of people that the scheduler is scheduling.
  std::vector<PersonSchedule> people;
  // meetingDuration is the duration of the meeting in minutes.
  int meetingDuration;

  // parse parses a problem from an input stream.
  // Implemented in problem_parse.cpp.
  static std::vector<ScheduleProblem> parse(std::istream &in);

  // Implemented in problem_print.cpp.
  friend std::ostream &operator<<(std::ostream &os,
                                  const ScheduleProblem &problem);
};

// ScheduleProblemSolution describes the solution to the matching group
// schedules problem.
struct ScheduleProblemSolution {
  std::vector<TimeRange> freeTimes;

  // solve solves the problem.
  // Implemented in problem_solve.cpp.
  static ScheduleProblemSolution solve(const ScheduleProblem &problem);

  // Implemented in problem_print.cpp.
  friend std::ostream &operator<<(std::ostream &os,
                                  const ScheduleProblemSolution &solution);
};

struct Line : std::string {
  size_t lineNumber;

  Line(std::string s, size_t lineNumber)
      : std::string(s), lineNumber(lineNumber){};
};

struct ParseError : public std::invalid_argument {
  Line line;

  ParseError(const Line &line, const std::exception &except)
      : std::invalid_argument(formatMessage(line, except.what())), line(line) {}

private:
  static std::string formatMessage(const Line &line, std::string message) {
    std::stringstream ss;
    ss << "at line " << line.lineNumber << " " << std::quoted(line) << ": "
       << message;
    return ss.str();
  }
};
