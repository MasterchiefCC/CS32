#include <iostream>
#include <string>
using namespace std;

class File {
public:
	File(string name) :m_name(name) {};
	virtual ~File() {};
	virtual string name()const { return m_name; }
	virtual void open() const= 0;
	virtual void redisplay()const { printf("refresh the screen"); }
private:
	string m_name;
};

class TextMsg : public File {
public:
	TextMsg(string txtF) :File(txtF){};
	virtual ~TextMsg() { cout << "Destroying " << name() << ", a text message"<<endl; }
	virtual void open()const { printf("open text message"); }
};

class Video : public File {
public:
	Video(string mpgF, int size) :File(mpgF), m_size(size) {};
	virtual ~Video() { cout << "Destroying " << name() << ", a video"<<endl; }
	virtual void redisplay()const { printf("replay video"); }
	virtual void open()const { printf("play %d second video", m_size); }
private:
	int m_size;
};

class Picture : public File{
public:
	Picture(string jpgF) :File(jpgF) {};
	virtual ~Picture() { cout << "Destroying the picture " << name() << endl; }
	virtual void open() const { cout << "show picture"; }
};

void openAndRedisplay(const File* f)
{
	cout << f->name() << ": ";
	f->open();
	cout << endl << "Redisplay: ";
	f->redisplay();
	cout << endl;
}

int main()
{
	File* files[4];
	files[0] = new TextMsg("fromFred.txt");
	// Videos have a name and running time
	files[1] = new Video("goblin.mpg", 3780);
	files[2] = new Picture("kitten.jpg");
	files[3] = new Picture("baby.jpg");

	for (int k = 0; k < 4; k++)
		openAndRedisplay(files[k]);

	// Clean up the files before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete files[k];
}