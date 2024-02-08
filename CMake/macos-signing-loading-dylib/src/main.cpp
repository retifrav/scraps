#include <iostream>

#include <dlfcn.h>

int main(int argc, char *argv[])
{
    std::cout << "Base application message" << std::endl;

    //void *someDependency = dlopen("/Users/vasya/Desktop/dependency-of-some.dylib", 0x0009);
    void *some = dlopen("/Users/vasya/Desktop/some.dylib", 0x0009);
    std::cout << "some: " << some << std::endl; // should be not NULL / 0x0
}
