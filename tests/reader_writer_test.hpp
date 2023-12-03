#pragma once

#include "test_utils.hpp"

#include "ReaderWriter.hpp"

inline void test_ReaderWriter_readFile() {
    ReaderWriter rw;

    auto level = rw.readFile("tests/test_files/test_level.bblvl");

    verify(level.has_value(), "Failed to load level.");
    verify(level->getEntities().size() == 14, "Entity counts.");
    verify(level->getBirds().size() == 6, "Bird counts.");
}

inline void test_ReaderWriter_writeFile() {
    ReaderWriter rw;

    std::string test_path = "tests/test_files/test_write.bblvl";

    Level lvl({}, {}, "", "", {}, "LEVEL", {}, test_path);

    rw.writeFile(lvl, test_path);

    auto read_lvl = rw.readFile(test_path);

    verify(read_lvl.has_value(), "Failed to load level.");
    verify(lvl.getName() == read_lvl->getName(), "Name match.");
}

inline void tests_reader_writer() {
    test_ReaderWriter_readFile();
    test_ReaderWriter_writeFile();
}
