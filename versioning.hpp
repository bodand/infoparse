#define InfoParse_MAJOR "1"
#define InfoParse_MINOR "0"
#define InfoParse_VERSION "1.0"

#include <string>
namespace InfoParse {
    static const std::string getVersion() {
        return InfoParse_VERSION;
    }
}
