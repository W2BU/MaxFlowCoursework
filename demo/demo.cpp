#include <MaxFlow.cpp>

int main() {
    MaxFlow myflow("sample.txt");
    std::cout << "Calculated max flow: ";
    std::cout << myflow.get_maxflow();
}