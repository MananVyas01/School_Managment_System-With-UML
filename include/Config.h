#define CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

class Config {
private:
    static Config* instance;
    std::unordered_map<std::string, std::string> settings;
    Config();

public:
    static Config* getInstance();
    bool loadFromFile(const std::string& filename);
    std::string get(const std::string& key, const std::string& defaultValue = "");
    void set(const std::string& key, const std::string& value);
    bool saveToFile(const std::string& filename);
    
    // Default configuration values
    static const std::string STUDENTS_FILE;
    static const std::string TEACHERS_FILE;
    static const std::string COURSES_FILE;
    static const std::string CLASSROOMS_FILE;
    static const std::string REPORTS_DIR;
};

// CONFIG_H
