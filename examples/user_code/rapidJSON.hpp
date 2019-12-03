#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <vector>

//using namespace rapidjson;

class capture_area {
public:
	capture_area() {}
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
		writer.StartObject(); //prettyWriter

		writer.String("width");
		writer.Uint(width);

		writer.String("height");
		writer.Uint(height);

		writer.EndObject();
	}

private:
	unsigned width_;
	unsigned height_;
};

class keypoint {
public:
	keypoint(const std::string& designator, unsigned x, unsigned y, double score) : designator_(designator), x_(x), y_(y), score_(score) {}
	keypoint(const keypoint& rhs) : designator_(rhs.designator_), x_(rhs.x_), y_(rhs.y_), score_(rhs.score_) {}
	//keypoint(std::string & designator, unsigned x, unsigned y, float score);

	keypoint& operator=(const keypoint& rhs) {
		designator_ = rhs.designator_;
		x_ = rhs.x_;
		y_ = rhs.y_;
		score_ = rhs.score_;
	}

	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject(); //prettyWriter

		writer.String("keypoint");
		//writer.String(designator_);

#if RAPIDJSON_HAS_STDSTRING
		writer.String(designator_);
#else
		writer.String(designator_.c_str(), static_cast<SizeType>(designator_.length()));
#endif

		writer.String("x");
		writer.Uint(x_);

		writer.String("y");
		writer.Uint(y_);

		writer.String("score");
		writer.Double(score_);

		writer.EndObject();
	}

private:
	std::string designator_;
	unsigned x_;
	unsigned y_;
	double score_;
};


class persons {
public:
	persons(const std::string& designator) : designator_(designator) {}
	//persons(const persons& rhs) : designator_(rhs.designator_), keypoints_(rhs.keypoints_) {}
	persons(const persons& rhs) : designator_(rhs.designator_) {}
	//persons(std::string designator);

	persons& operator=(const persons& rhs) {
		designator_ = rhs.designator_;
	}

	void AddKeypoint(const keypoint& keypoint) {
		keypoints_.push_back(keypoint);

	}
		template <typename Writer>
		void Serialize(Writer& writer) const {
			writer.StartObject(); //prettyWriter

			writer.String("person");
			//writer.String(designator_);

#if RAPIDJSON_HAS_STDSTRING
			writer.String(designator_);
#else
			writer.String(designator_.c_str(), static_cast<SizeType>(designator_.length()));
#endif

			writer.String(("keypoints"));
			writer.StartArray();
			for (std::vector<keypoint>::const_iterator keypointItr = keypoints_.begin(); keypointItr != keypoints_.end(); ++keypointItr)
				keypointItr->Serialize(writer);
			writer.EndArray();

			writer.EndObject();
		}

private:
	std::string designator_;
	std::vector<keypoint> keypoints_;
};

class rootObj {
public:
	//rootObj(const std::string& designator, const capture_area& frame) : designator_(designator), capture_area_(frame) {}
	//rootObj(const rootObj& rhs) : designator_(rhs.designator_), capture_area_(rhs.capture_area_) {}

	rootObj(const std::string& designator) : designator_(designator) {}
	rootObj(const rootObj& rhs) : designator_(rhs.designator_) {}

	rootObj& operator=(const rootObj& rhs) {
		designator_ = rhs.designator_;
	}

	void setFrame(const capture_area& frame) {
		capture_area_ = frame;
	}

	void addPerson(const persons& person) {
		persons_.push_back(person);
	}

	persons& getLastPerson() {
		return persons_.back();
	}

	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();

		writer.String("root");

#if RAPIDJSON_HAS_STDSTRING
		writer.String(designator_);
#else
		writer.String(designator_.c_str(), static_cast<SizeType>(designator_.length()));
#endif

		writer.String("capture area");

		writer.Uint(capture_area_.width_);
		writer.Uint(capture_area_.height_);

//#if RAPIDJSON_HAS_STDSTRING
//		writer.String(capture_area_);
//#else
//		writer.String(capture_area_.c_str(), static_cast<SizeType>(capture_area_.length()));
//#endif

		//persons!
		writer.StartArray();
		for (std::vector<persons>::const_iterator personsItr = persons_.begin(); personstItr != persons_.end(); ++personstItr)
			personsItr->Serialize(writer);
		writer.EndArray();

		writer.EndObject();
	}

private:
	std::string designator_;
	capture_area capture_area_;
	std::vector<persons> persons_;
};