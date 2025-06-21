#include "../include/Config.h"
#include <fstream>
#include <sstream>
#include <iostream>

Config* Config::instance = nullptr;

const std::string Config::STUDENTS_FILE = "data/students.txt";
const std::string Config::TEACHERS_FILE = "data/teachers.txt";
const std::string Config::COURSES_FILE = "data/courses.txt";
const std::string Config::CLASSROOMS_FILE = "data/classrooms.txt";
const std::string Config::REPORTS_DIR = "reports/";

Config::Config() {
    // Set default values
    settings["students_file"] = STUDENTS_FILE;
    settings["teachers_file"] = TEACHERS_FILE;
    settings["courses_file"] = COURSES_FILE;
    settings["classrooms_file"] = CLASSROOMS_FILE;
    settings["reports_dir"] = REPORTS_DIR;
    settings["max_students_per_course"] = "50";
    settings["max_courses_per_classroom"] = "5";
    settings["backup_enabled"] = "true";
    settings["log_level"] = "INFO";
}

Config* Config::getInstance() {
    if (instance == nullptr) {
        instance = new Config();
    }
    return instance;
}

bool Config::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Warning: Could not open config file " << filename << ". Using defaults." << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            settings[key] = value;
        }
    }
    
    file.close();
    return true;
}

std::string Config::get(const std::string& key, const std::string& defaultValue) {
    auto it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    return defaultValue;
}

void Config::set(const std::string& key, const std::string& value) {
    settings[key] = value;
}

bool Config::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not save config file " << filename << std::endl;
        return false;
    }
    
    file << "# School Management System Configuration\n";
    file << "# Auto-generated configuration file\n\n";
    
    for (const auto& pair : settings) {
        file << pair.first << "=" << pair.second << "\n";
    }
    
    file.close();
    return true;
}
