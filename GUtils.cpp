#include "GUtils.hpp"

void GC_Utils::Print(const std::string& str) {
	if (str.length()) {
		std::cout << str;
		endline2;
	}
}

void GC_Utils::PrintVector(const glm::vec3& v, std::string str) {
	p(str);
	std::cout $ v.x $ v.y $ v.z;
    endline;
    endline;
}

void GC_Utils::PrintMatrix(const glm::mat4x4& M, std::string str) {
    p(str);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout $ M[i][j] << "f,";
        }
        endline;
    }
    endline;
}

void GC_Utils::PrintQuat(const glm::quat& Q, std::string str) {
    p(str);
    std::cout $ Q.x $ Q.y $ Q.z;
    endline2;
}

void GC_Utils::PrintVector4(const glm::vec4& v, std::string str) {
    p(str);
    std::cout $ v.x $ v.y $ v.z $ v.w;
    endline2;
}
