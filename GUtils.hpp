#include "libraries.hpp"

#define formatFloat std::right << std::setw(16) << std::setfill(' ') << std::fixed << std::setprecision(10)

#define $ << " " << formatFloat << // space bridge with formatting
#define endline std::cout << std::endl
#define endline2 std::cout << std::endl << std::endl

inline void cls() {
	HANDLE hOut;
	COORD Position;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut, Position);
}

inline void setcursorpos(int x) {
	x *= 8;
	POINT p;
	if (GetCursorPos(&p)) {
		SetCursorPos(x, p.y);
	}
}
class GC_Utils {
	public:
		static void Print(const std::string& str);
		static void PrintVector(const glm::vec3& v, std::string str = "");
		static void PrintMatrix(const glm::mat4x4& M, std::string str = "");
		static void PrintQuat(const glm::quat& Q, std::string str = "");
		static void PrintVector4(const glm::vec4& v, std::string str = "");
};

#define p(x) GC_Utils::Print((x))
#define pv(x, y) GC_Utils::PrintVector((x), (y))
#define pM(x, y) GC_Utils::PrintMatrix((x), (y))
#define pQ(x, y) GC_Utils::PrintQuat((x), (y))
#define p4(x, y) GC_Utils::PrintVector4((x), (y))