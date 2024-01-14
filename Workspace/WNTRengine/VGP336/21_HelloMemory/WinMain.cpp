#include <../Engine/Inc/WNTRengine.h>

using namespace WNTRengine;
using namespace WNTRengine::Core;

class Student
{
public:
	Student() = default;
	Student(const std::string& name) : mName(name), mMark(0)
	{

	}

	void SetMark(int mark)
	{
		mMark = mark;
	}

private:
	std::string mName;
	int mMark;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator StudentPool = TypedAllocator<Student>("StudentPool", 100);

	std::vector<Student*> students;
	int count = 100;
	for(int i =0; i< 100; ++i)
	{
		std::string name = "Student" + std::to_string(i);
		students.push_back(StudentPool.New(name));
	}
	//do stuff
	for (auto student : students)
	{
	StudentPool.Delete(student);
	}


	return (0);
}