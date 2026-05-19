#include <iostream>
#include <cmath>
using namespace std;
// задача на комплексный класс с перегрузом Раньше была где то в 1/2 семестре на проге если вела кафедра волны. В целом ничего сложного ее сдавать было не обязательно

class Complex {
private:
	double norm = 0;
	double irr = 0;

public:
	Complex(double norm_val, double irr_val) : norm(norm_val), irr(irr_val) {}
	Complex(double norm_val) : norm(norm_val), irr(0) {}
	Complex() : norm(0), irr(0) {}
	Complex(const Complex& other) : norm(other.norm), irr(other.irr) {}
	~Complex() {}

	void SetComplex(double norm_val, double irr_val) {
		norm = norm_val;
		irr = irr_val;
	}
	void SetRational(double norm_val) {
		norm = norm_val;
	}
	void SetIrrational(double irr_val) {
		irr = irr_val;
	}

	double GetRational()const {
		return norm;
	}
	double GetIrrational() const {
		return irr;
	}
	double Module()const {
		return sqrt(pow(norm, 2) + pow(irr, 2));
	}
	Complex GetSopr() const {
		return Complex(norm, -irr);
	}
	explicit operator double() const {
		return Module();
	}
	// ïåðåãðóçêà ìàòåìàòè÷åñêèõ îïåðàòîðîâ
	friend Complex operator+(const Complex& a, const Complex& b) {
		return Complex(a.norm + b.norm, a.irr + b.irr);
	}
	friend Complex operator+(const Complex& a, double b) {
		return Complex(a.norm + b, a.irr);
	}
	friend Complex operator+(double a, const Complex& b) {
		return Complex(a + b.norm, b.irr);
	}

	friend Complex operator-(const Complex& a, const Complex& b) {
		return Complex(a.norm - b.norm, a.irr - b.irr);
	}
	friend Complex operator-(const Complex& a, double b) {
		return Complex(a.norm - b, a.irr);
	}
	friend Complex operator-(double a, const Complex& b) {
		return Complex(a - b.norm, b.irr);
	}

	friend Complex operator/(const Complex& a, const Complex& b) {
		Complex numerator = a * b.GetSopr();
		double denominator = b.norm * b.norm + b.irr * b.irr;
		return Complex(numerator.norm / denominator, numerator.irr / denominator);
	}
	friend Complex operator/(const Complex& a, double b) {
		return Complex(a.norm / b, a.irr / b);
	}
	friend Complex operator/(double a, const Complex& b) {
		return Complex(Complex(a) / b);
	}

	friend Complex operator*(const Complex& a, const Complex& b) {
		return Complex(a.norm * b.norm - a.irr * b.irr, b.norm * a.irr + a.norm * b.irr);
	}
	friend Complex operator*(const Complex& a, double b) {
		return Complex(a.norm * b, b * a.irr);
	}
	friend Complex operator*(double a, const Complex& b) {
		return Complex(a * b.norm, a * b.irr);
	}
	friend Complex operator-(const Complex& a) {
		return Complex(-a.norm, -a.irr);
	}
	//ïåðåãðóçêà  îïåðàòîðîâ  ñðàâíåíèÿ
	friend bool operator==(const Complex& a, const Complex& b) {
		return (a.norm == b.norm) && (a.irr == b.irr);
	}

	friend bool operator!=(const Complex& a, const Complex& b) {
		return !(a == b);
	}

	friend bool operator<(const Complex& a, const Complex& b) {
		return a.Module() < b.Module();
	}

	friend bool operator<=(const Complex& a, const Complex& b) {
		return a.Module() <= b.Module();
	}

	friend bool operator>(const Complex& a, const Complex& b) {
		return a.Module() > b.Module();
	}

	friend bool operator>=(const Complex& a, const Complex& b) {
		return a.Module() >= b.Module();
	}


};


std::ostream& operator<<(std::ostream& os, Complex& c) {
	os << c.GetRational() << " " << c.GetIrrational() << "i  ";
	return os;
}
std::ostream& operator<<(std::ostream& os, const Complex& c) {
	os << c.GetRational() << " " << c.GetIrrational() << "i  ";
	return os;
}

int main() {
	Complex x(1, 1), y(1, 0);
	double k = 3.0;
	double z;
	z = double(x);
	cout << z << endl;
	cout << y / x << k / y << endl;
	cout << (x > y) << "   " << (x == y) << "   " << (x != y) << endl;
}
