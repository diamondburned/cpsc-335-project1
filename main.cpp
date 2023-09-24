#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "problem.hpp"

int main(int argc, char *argv[]) {
  const auto &args = std::vector<std::string>(argv, argv + argc);
  if (args.size() < 2 || args.at(1) == "-h" || args.at(1) == "--help") {
    std::cerr << "Usage: " << args.at(0) << " [-h|--help|input file]\n"
              << "If input file is not specified, then input.txt is used.\n";
    return 1;
  }

  const auto inputFile = args.size() == 2 ? args.at(1) : "input.txt";
  std::ifstream input(inputFile);

  const auto problems = ScheduleProblem::parse(input);
  for (const auto &problem : problems) {
    const auto solution = ScheduleProblemSolution::solve(problem);
    std::cout << solution << std::endl;
  }
}
