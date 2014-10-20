#include <iostream>
#include <vector>
using namespace std;

class PrintedMaterial {
public:
  PrintedMaterial( unsigned numPages ) : numOfPages(numPages) {}
  virtual void displayNumPages() const = 0;
private:
  unsigned numOfPages;
};

void PrintedMaterial::displayNumPages() const {
  cout << numOfPages << endl;
}

class Magazine : public PrintedMaterial {
public:
	Magazine( unsigned numPages ) : PrintedMaterial(numPages) {}

	void displayNumPages() const
	{
        cout << "I am a Magazine";
		PrintedMaterial::displayNumPages();
	}

protected:
private:
};

class Book : public PrintedMaterial {
public:
	Book( unsigned numPages) : PrintedMaterial(numPages) {}

	void displayNumPages() const
	{
        cout << "I am a Book";
		PrintedMaterial::displayNumPages();
	}
protected:
private:
};

class TextBook : public Book {
public:
	TextBook( unsigned numPages, unsigned numIndxPgs) : 
        Book(numPages), numOfIndexPages(numIndxPgs) {}

	void displayNumPages() const
	{
        cout << "I am a TextBook";
		PrintedMaterial::displayNumPages();
		cout << "Index pages: ";
		cout << numOfIndexPages << endl; 
	}
protected:
private:
	unsigned numOfIndexPages;
};

class Novel : public Book {
public:
	Novel( unsigned numPages )
		: Book(numPages)
	{}
	void displayNumPages() const
	{
        cout << "I am a Novel";
		PrintedMaterial::displayNumPages();
	}
protected:
private:
};


// tester/modeler code
int main()
{
	TextBook t(5430, 23);
	Novel n(213);
	Magazine m(6);

    cout << "Displaying pages: " << endl;
	t.displayNumPages();
	n.displayNumPages();
	m.displayNumPages();

    cout << "Displaying all books: " << endl;
	vector< PrintedMaterial* > allThemPrinties;
	allThemPrinties.push_back(&t);
	allThemPrinties.push_back(&n);
	allThemPrinties.push_back(&m);
	for( size_t ndx=0; ndx < allThemPrinties.size(); ++ ndx ) 
	{
		allThemPrinties[ndx]->displayNumPages();
		cout << endl;
	}
}
