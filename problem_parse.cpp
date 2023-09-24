#include "problem.hpp"

Time Time::parseTime(std::string timeString) {
  const auto colonPos = timeString.find(":");
  if (colonPos == std::string::npos) {
    throw std::invalid_argument("timeString must be in the format HH:MM");
  }

  const std::string hourString = timeString.substr(0, colonPos);
  const std::string minuteString = timeString.substr(colonPos + 1);

  const int hour = std::stoi(hourString);
  const int minute = std::stoi(minuteString);

  return Time(hour, minute);
}

static std::vector<std::string> splitList(const std::string &line) {
  if (line.size() < 2) {
    throw std::invalid_argument("list must have at least []");
  }
  if (line.at(0) != '[' || line.at(line.size() - 1) != ']') {
    throw std::invalid_argument("list must start and end with []");
  }

  std::vector<std::string> list;

  size_t start = 1;
  const auto pop = [&](size_t end) {
    const auto &item = line.substr(start, end - start);
    if (!item.empty()) {
      list.push_back(item);
    }
    start = end + 1;
  };

  int level = -1;
  for (size_t i = 0; i < line.size(); i++) {
    switch (line.at(i)) {
    case '[':
      level++;
      break;
    case ']':
      level--;
      break;
    case ',':
      if (level == 0) {
        pop(i);
        // Skip prefix whitespaces.
        while (line.at(start) == ' ') {
          i++;
          start++;
        }
      }
      break;
    }
  }

  if (level != -1) {
    throw std::invalid_argument("list does not close brackets properly");
  }

  pop(line.size() - 1);
  return list;
}

ScheduleProblem parseProblem(const std::vector<Line> &buffer) {
  // The last line is always the meeting duration.
  const int meetingDuration = std::stoi(buffer.back());

  // Splice the buffer to remove the last line.
  std::vector<Line> peopleBuffer(buffer.begin(), buffer.end() - 1);

  // Ensure even lines.
  if ((peopleBuffer.size() % 2) != 0) {
    throw std::invalid_argument("problem must have an even number of lines");
  }

  std::vector<PersonSchedule> people;
  people.reserve(peopleBuffer.size() / 2);

  for (std::size_t i = 0; i < peopleBuffer.size(); i += 2) {
    const auto &timeRangesString = peopleBuffer[i]; // [[x, y], [z, w]]
    const auto &dailyAvailabilityString = peopleBuffer[i + 1]; // [x, y]

    // Parse times.
    std::vector<TimeRange> timeRanges;
    try {
      const auto &timeRangesList = splitList(timeRangesString);
      timeRanges.reserve(timeRangesList.size());

      for (const auto &timeRangeString : timeRangesList) {
        const auto &timesList = splitList(timeRangeString);
        if (timesList.size() != 2) {
          throw std::invalid_argument("time range must have 2 times");
        }
        timeRanges.push_back(TimeRange(Time::parseTime(timesList[0]),
                                       Time::parseTime(timesList[1])));
      }
    } catch (const std::invalid_argument &e) {
      throw ParseError(timeRangesString, e);
    }

    // Parse daily availability.
    std::optional<TimeRange> dailyAvailability;
    try {
      const auto &dailyAvailabilityList = splitList(dailyAvailabilityString);
      if (dailyAvailabilityList.size() != 2) {
        throw std::invalid_argument("daily availability must have 2 times");
      }
      dailyAvailability = TimeRange(Time::parseTime(dailyAvailabilityList[0]),
                                    Time::parseTime(dailyAvailabilityList[1]));
    } catch (const std::invalid_argument &e) {
      throw ParseError(dailyAvailabilityString, e);
    }

    people.push_back(PersonSchedule{
        .times = timeRanges,
        .dailyAvailability = dailyAvailability.value(),
    });
  }

  return ScheduleProblem{
      .people = people,
      .meetingDuration = meetingDuration,
  };
}

// Parser implements a parser for the ScheduleProblem format.
// See input.txt for an example of the format.
std::vector<ScheduleProblem> ScheduleProblem::parse(std::istream &in) {
  std::vector<Line> buffer;
  std::vector<ScheduleProblem> problems;
  size_t lineNumber = 0;
  while (!in.eof()) {
    lineNumber++;
    std::string line;
    std::getline(in, line);
    if (!in.eof() && line != "") {
      // Skip comments.
      if (line.at(0) == '#') {
        continue;
      }
      buffer.push_back(Line(line, lineNumber));
      continue;
    }
    if (buffer.size() > 0) {
      const auto problem = parseProblem(buffer);
      problems.push_back(problem);
      buffer.clear();
    }
  }
  return problems;
};
