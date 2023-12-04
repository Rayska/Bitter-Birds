#pragma once

#include "test_utils.hpp"

#include "ReaderWriter.hpp"

inline void test_ReaderWriter_read_file() {
    ReaderWriter rw;

    auto level = rw.read_file("tests/test_files/test_level.bblvl");

    verify(level.has_value(), "Failed to load level.");
    verify(level->get_entities().size() == 14, "Entity counts.");
    verify(level->get_birds().size() == 6, "Bird counts.");
}

inline void test_ReaderWriter_write_file() {
    ReaderWriter rw;

    std::string test_path = "tests/test_files/test_write.bblvl";

    Level lvl({}, {}, "", "LEVEL", {}, test_path);

    rw.write_file(lvl, test_path);

    auto read_lvl = rw.read_file(test_path);

    verify(read_lvl.has_value(), "Failed to load level.");
    verify(lvl.get_name() == read_lvl->get_name(), "Name match.");
}

inline void tests_reader_writer() {
    test_ReaderWriter_read_file();
    test_ReaderWriter_write_file();
}
