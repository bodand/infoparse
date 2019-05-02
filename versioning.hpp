#define InfoParse_MAJOR "1"
#define InfoParse_MINOR "2"
#define InfoParse_VERSION "1.2"

#include <string>
namespace InfoParse {
    static inline const std::string getVersion() {
        return InfoParse_VERSION;
    }
}
