
#include <iostream>
#include <iomanip>



int main()
{
    std::cout << "hellow world!" << std::endl;

    union uni
    {
        int i;
        float f;
        struct {
            bool b : 1;
        };
    };

    uni u;
    u.i = 41;
    std::cout << std::setprecision(64) << std::fixed;
    std::cout << u.i << std::endl;
    std::cout << u.f << std::endl;
    std::cout << u.b << std::endl;

    return 0;
}

