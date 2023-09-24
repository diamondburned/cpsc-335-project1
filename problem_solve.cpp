#include "problem.hpp"

ScheduleProblemSolution
ScheduleProblemSolution::solve(const ScheduleProblem &problem) {
  // The number of minutes in a day. 24h * 60m = 1440m.
  constexpr int MINUTES_PER_DAY = 24 * 60;

  // Allocate a bitset that contains all the slots in a day as well as a
  // value indicating whether the slot is available.
  // This will be (24*60) / (8 bits) = 180 bytes large or ~23 pointers.
  // This is quite large, but still very reasonable.
  std::bitset<MINUTES_PER_DAY> busyMinutes;

  // Also find the latest start time and the earliest end time for everyone
  // according to their daily availability.
  int latestStartTime = 0;
  int earliestEndTime = MINUTES_PER_DAY;

  // This loop should be O(n) where n is the number of people. n is
  // potentially unboounded.
  for (const auto &person : problem.people) {
    const int availabilityStart =
        person.dailyAvailability.from.minutesFromMidnight();
    const int availabilityEnd =
        person.dailyAvailability.to.minutesFromMidnight();

    latestStartTime = std::max(latestStartTime, availabilityStart);
    earliestEndTime = std::min(earliestEndTime, availabilityEnd);

    // This loop should be O(n) where n is the number of time slots in a
    // person's schedule. Since the worst case is 24*60 1-minute long
    // meetings, the loop would run 24*60/1 = 1440 times.
    for (const auto &range : person.times) {
      const int start = range.from.minutesFromMidnight();
      const int end = range.to.minutesFromMidnight();
      // This loop is O(1440) at worst, reducing it to the O(1) complexity
      // class.
      for (int i = start; i < end; i++) {
        busyMinutes.set(i, true);
      }
    }
  }

  // Find a time range that is available for everyone. We only need
  // to print the start time, and we're skipping adjacent slots that
  // are available.
  std::vector<TimeRange> freeTimes;

  int start = -1;
  // This loop is O(1) because its worst case is O(1440).
  for (int i = latestStartTime; i <= earliestEndTime; i++) {
    if (i < earliestEndTime && !busyMinutes.test(i)) {
      if (start == -1) {
        start = i;
      }
      continue;
    }

    if (start != -1 && i - start >= problem.meetingDuration) {
      freeTimes.push_back(TimeRange(Time(start), Time(i)));
      start = -1;
      continue;
    }
  }

  return ScheduleProblemSolution{.freeTimes = freeTimes};
};
