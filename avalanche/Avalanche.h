#ifndef _AVALANCHE_H_
#define _AVALANCHE_H_

#include "Instance.h"
#include "login/LoginMethod.h"

#include <vector>

namespace avalanche {

using OptionMap = std::unordered_map<std::string, std::string>;

class Avalanche {
private:
    std::vector<std::unique_ptr<Instance>> m_Instances;
    std::unique_ptr<LoginMethod> m_LoginMethod;

public:
    bool Initialize(const OptionMap& options);

    void Run();
};

} // ns avalanche

#endif
