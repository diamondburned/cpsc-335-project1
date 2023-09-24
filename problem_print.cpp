#include "problem.hpp"

std::ostream &operator<<(std::ostream &os, const Time &time) {
  os << std::setfill('0') << std::setw(2) << time.hour;
  os << ":";
  os << std::setfill('0') << std::setw(2) << time.minute;
  return os;
}

std::ostream &operator<<(std::ostream &os, const TimeRange &range) {
  os << "[" << range.from << ", " << range.to << "]";
  return os;
}

std::ostream &operator<<(std::ostream &os, const PersonSchedule &schedule) {
  os << "[";
  for (std::size_t i = 0; i < schedule.times.size(); i++) {
    os << schedule.times[i];
    if (i != schedule.times.size() - 1) {
      os << ", ";
    }
  }
  os << "]\n";
  os << schedule.dailyAvailability;
  return os;
}

std::ostream &operator<<(std::ostream &os, const ScheduleProblem &problem) {
  for (const auto &person : problem.people) {
    os << person << "\n";
  }
  os << problem.meetingDuration;
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const ScheduleProblemSolution &solution) {
  os << "[";
  for (size_t i = 0; i < solution.freeTimes.size(); i++) {
    os << solution.freeTimes[i];
    if (i != solution.freeTimes.size() - 1) {
      os << ", ";
    }
  }
  os << "]";
  return os;
}
