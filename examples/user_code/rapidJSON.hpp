#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <vector>

using namespace rapidjson;

class rapidJSON
{
public:
	rapidJSON();
	~rapidJSON();

	void createJSONstr();
};


//class Person {
//public:
//	Person(const std::string& name, unsigned age) : name_(name), age_(age) {}
//	Person(const Person& rhs) : name_(rhs.name_), age_(rhs.age_) {}
//	virtual ~Person();
//
//	Person& operator=(const Person& rhs) {
//		name_ = rhs.name_;
//		age_ = rhs.age_;
//		return *this;
//	}
//
//protected:
//	template <typename Writer>
//	void Serialize(Writer& writer) const {
//		// This base class just write out name-value pairs, without wrapping within an object.
//		writer.String("name");
//#if RAPIDJSON_HAS_STDSTRING
//		writer.String(name_);
//#else
//		writer.String(name_.c_str(), static_cast<SizeType>(name_.length())); // Supplying length of string is faster.
//#endif
//		writer.String("age");
//		writer.Uint(age_);
//	}
//
//private:
//	std::string name_;
//	unsigned age_;
//};


class capture_area {
	capture_area(unsigned width, unsigned height) : width_(width), height_(height) {}
	capture_area(const capture_area& rhs) : width_(rhs.width_), height_(rhs.height_) {}

	capture_area& operator=(const capture_area& rhs) {
		width_ = rhs.width_;
		height_ = rhs.height_;
		return *this;
	}

protected:
	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.String("width");
		writer.Uint(width);

		writer.String("height");
		writer.Uint(height);
	}

private:
	unsigned width_;
	unsigned height_;
};

class persons {
	persons()

};

class left_hand {

};

class right_hand {

};

class root {

};

class left_shoulder {

};

class right_shoulder {

};

class left_ankle {

};

class right_ankle {

};

class x {

};

class y {

};
