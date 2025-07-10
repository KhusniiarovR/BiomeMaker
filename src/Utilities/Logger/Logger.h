#include <iostream>
#include <sstream>
#include <cstring>

namespace ramir {
    class Logger {
    public:
        Logger(const char* file, int line) {
            const char* relative = strstr(file, "src\\");
            if (!relative) relative = file;
            stream << relative << "::" << line << "::";
        }

        template<typename T>
        Logger& operator<<(const T& value) {
            stream << value;
            return *this;
        }

        ~Logger() {
            stream << "\n";
            std::cerr << stream.str();
        }

    private:
        std::ostringstream stream;
    };
}

#define mycerr ramir::Logger(__FILE__, __LINE__)
