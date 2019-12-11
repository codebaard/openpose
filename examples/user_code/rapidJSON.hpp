#include "rapidjson/prettywriter.h" // for stringify JSON
#include <cstdio>
#include <string>
#include <vector>
#include <chrono>

//using namespace rapidjson;
using namespace std::chrono;

class capture_area {
public:
	capture_area() {}
	capture_area(unsigned width, unsigned height, unsigned crop) : width_(width), height_(height), crop_(crop) {}
	capture_area(const capture_area& rhs) : width_(rhs.width_), height_(rhs.height_) {}

	capture_area& operator=(const capture_area& rhs) {
		width_ = rhs.width_;
		height_ = rhs.height_;
		crop_ = rhs.crop_;
		return *this;
	}

	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject(); //prettyWriter

		writer.String("width");
		writer.Uint(width_);

		writer.String("height");
		writer.Uint(height_);

		writer.String("crop");
		writer.Uint(crop_);

		writer.EndObject();
	}

private:
	unsigned width_;
	unsigned height_;
	unsigned crop_;
};

class timestamp {
public:
	timestamp(){}
	timestamp(const std::string& designator, long long msTime) : designator_(designator), msTime_(msTime) {}
	timestamp(const timestamp& rhs) : designator_(rhs.designator_), msTime_(rhs.msTime_) {}

	timestamp& operator=(const timestamp& rhs) {
		designator_ = rhs.designator_;
		msTime_ = rhs.msTime_;
		return *this;
	}

	template<typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();

		writer.String("Type");
#if RAPIDJSON_HAS_STDSTRING
		writer.String(designator_);
#else
		writer.String(designator_.c_str(), static_cast<SizeType>(designator_.length()));
#endif
		writer.String("Timestamp");
		writer.Uint(msTime_);

		writer.EndObject();
	}

	void giveTimeInMilliseconds() {
		milliseconds ms = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
			);
		msTime_ = ms.count();
	}


private:
	std::string designator_;
	long long msTime_;

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
	persons(const persons& rhs) : designator_(rhs.designator_), keypoints_(rhs.keypoints_) {} 
	virtual ~persons();

	persons& operator=(persons& rhs) { 
		//static_cast<persons&>(*this) = rhs;
		designator_ = rhs.designator_;
		return *this;
	}

	void AddKeypoint(const keypoint& keypoint) {
		keypoints_.push_back(keypoint);
		//keypoints_[i] = &keypoint;
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
	//std::vector<keypoint> keypoints_(7);
};
persons::~persons() {}

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

	void addTimestamp(const timestamp& timestamp) {
		timestamp_.giveTimeInMilliseconds(); //create the timestamp
		//timestamp_ = timestamp;
		//timestamp_ = ms.count;
	}

	void addPerson(const persons& person) { //former with const
		persons_.push_back(person);
	}

	persons& getLastPerson() {
		return persons_.back();
	}

	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
//
//		writer.String("root");
//
//#if RAPIDJSON_HAS_STDSTRING
//		writer.String(designator_);
//#else
//		writer.String(designator_.c_str(), static_cast<SizeType>(designator_.length()));
//#endif
		//timestamp_.giveTimeInMilliseconds();
		writer.String("Timestamp");

		timestamp_.Serialize(writer);

		writer.String("capture area");

		capture_area_.Serialize(writer);

//#if RAPIDJSON_HAS_STDSTRING
//		writer.String(capture_area_);
//#else
//		writer.String(capture_area_.c_str(), static_cast<SizeType>(capture_area_.length()));
//#endif

		//persons!
		writer.String(("persons"));
		writer.StartArray();
		for (std::vector<persons>::const_iterator personsItr = persons_.begin(); personsItr != persons_.end(); ++personsItr)
			personsItr->Serialize(writer);
		writer.EndArray();

		writer.EndObject();
	}

private:
	std::vector<persons> persons_;
	std::string designator_;
	capture_area capture_area_;
	timestamp timestamp_;

};