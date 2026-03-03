#pragma once
#include <vector>

struct Vertex;

class PrimitiveGenerator {
public:
    static std::vector<Vertex> GenerateCube();
    static std::vector<Vertex> GeneratePlane(float size);
    static std::vector<Vertex> GenerateSphere(float radius, int sectors, int stacks);
};
