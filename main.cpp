#include "experiments.h"
#include "report_io.h"

#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct Config {
    std::vector<std::size_t> single_sizes {100, 200, 500, 1000};
    std::size_t multiple_rhs_n = 500;
    std::vector<std::size_t> multiple_rhs_counts {1, 10, 100};
    std::vector<std::size_t> hilbert_sizes {5, 10, 15};
    std::uint64_t seed = 42;
    std::filesystem::path output_dir = "results";
};

std::vector<std::size_t> parseSizeList(const std::string& text) {
    std::vector<std::size_t> result;
    std::stringstream stream(text);
    std::string token;

    while (std::getline(stream, token, ',')) {
        if (token.empty()) {
            continue;
        }
        result.push_back(static_cast<std::size_t>(std::stoull(token)));
    }

    if (result.empty()) {
        throw std::runtime_error("Expected at least one integer in the list.");
    }

    return result;
}

Config parseArgs(int argc, char* argv[]) {
    Config config;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--help") {
            std::cout
                << "Usage: slae_benchmark [--quick] [--single-sizes=100,200,500,1000]\n"
                << "                      [--multi-size=500] [--multi-k=1,10,100]\n"
                << "                      [--hilbert-sizes=5,10,15] [--seed=42]\n"
                << "                      [--output-dir=results]\n";
            std::exit(0);
        }

        if (arg == "--quick") {
            config.single_sizes = {50, 100, 200};
            config.multiple_rhs_n = 200;
            config.multiple_rhs_counts = {1, 5, 20};
            config.hilbert_sizes = {5, 8, 10};
            continue;
        }

        const auto equalPos = arg.find('=');
        if (equalPos == std::string::npos) {
            throw std::runtime_error("Unknown argument: " + arg);
        }

        const std::string key = arg.substr(0, equalPos);
        const std::string value = arg.substr(equalPos + 1);

        if (key == "--single-sizes") {
            config.single_sizes = parseSizeList(value);
        } else if (key == "--multi-size") {
            config.multiple_rhs_n = static_cast<std::size_t>(std::stoull(value));
        } else if (key == "--multi-k") {
            config.multiple_rhs_counts = parseSizeList(value);
        } else if (key == "--hilbert-sizes") {
            config.hilbert_sizes = parseSizeList(value);
        } else if (key == "--seed") {
            config.seed = static_cast<std::uint64_t>(std::stoull(value));
        } else if (key == "--output-dir") {
            config.output_dir = value;
        } else {
            throw std::runtime_error("Unknown argument: " + arg);
        }
    }

    return config;
}

int main(int argc, char* argv[]) {
    try {
        const Config config = parseArgs(argc, argv);
        std::mt19937_64 rng(config.seed);

        std::cout << "Seed: " << config.seed << '\n';
        std::cout << "Output directory: " << config.output_dir.string() << '\n';

        std::vector<SingleSystemRow> singleRows;
        singleRows.reserve(config.single_sizes.size());
        for (std::size_t n : config.single_sizes) {
            singleRows.push_back(runSingleSystemExperiment(n, rng));
        }

        std::vector<MultipleRhsRow> multipleRows;
        multipleRows.reserve(config.multiple_rhs_counts.size());
        for (std::size_t k : config.multiple_rhs_counts) {
            multipleRows.push_back(runMultipleRhsExperiment(config.multiple_rhs_n, k, rng));
        }

        const std::vector<HilbertRow> hilbertRows = runHilbertExperiments(config.hilbert_sizes);

        printSingleSystemTable(singleRows);
        printMultipleRhsTable(multipleRows);
        printHilbertTable(hilbertRows);

        std::filesystem::create_directories(config.output_dir);
        writeSingleSystemCsv(config.output_dir / "single_system.csv", singleRows);
        writeMultipleRhsCsv(config.output_dir / "multiple_rhs.csv", multipleRows);
        writeHilbertCsv(config.output_dir / "hilbert_accuracy.csv", hilbertRows);

        std::cout << "\nCSV files were written to: " << std::filesystem::absolute(config.output_dir) << '\n';
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
