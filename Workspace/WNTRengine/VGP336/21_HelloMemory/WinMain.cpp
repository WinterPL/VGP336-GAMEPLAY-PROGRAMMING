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

	Student* a = StudentPool.New("Jon");
	Student* b = StudentPool.New("Winter");

	std::vector<Student*> students;


	//do stuff

	StudentPool.Delete(a);
	StudentPool.Delete(b);


	return (0);
}