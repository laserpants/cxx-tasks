# cxx-tasks

```
#include <iostream>
#include "task.h"

int main()
{
    auto res = task<int>([]() { 
        return 100; 
    }).then([](int n) {
        return std::to_string(n);
    }).then([](const std::string& str) {
        std::cout << str << std::endl;
        return std::atoi(str.c_str());
    }).then([](int n) {
        ++n;
        std::cout << n << std::endl;
        return "done";
    }).run();

    std::cout << res << std::endl; // done

    return 0;
}
```
