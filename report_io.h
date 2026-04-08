#pragma once

#include "experiments.h"

#include <filesystem>
#include <vector>

void printSingleSystemTable(const std::vector<SingleSystemRow>& rows);
void printMultipleRhsTable(const std::vector<MultipleRhsRow>& rows);
void printHilbertTable(const std::vector<HilbertRow>& rows);

void writeSingleSystemCsv(const std::filesystem::path& path, const std::vector<SingleSystemRow>& rows);
void writeMultipleRhsCsv(const std::filesystem::path& path, const std::vector<MultipleRhsRow>& rows);
void writeHilbertCsv(const std::filesystem::path& path, const std::vector<HilbertRow>& rows);
