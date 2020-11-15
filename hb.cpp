#include <iostream>
#include <iomanip>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <fstream>


using namespace std;

// PROTOTYPES
void initial();
void admin();
void holly_menu();
void bolly_menu();
void addrec(char);
void listrec(char);
void modifyrec(char);
void deleterec(char);
int ret_id();
void game();
void setup(char, char[], char[], char[]);
int check(char,char[],char[]);
void wrong_cut(char[], int&, char);
int autogen(char);

class movie                         // GLOBAL CLASS
{
	int movie_id;					
	string movie_name;

	public: 
		void in(int);	
		void list();
		int ret_id();
		void new_name();
		void movie_assign(char, char[]);
};

movie m;


	
{
	movie_id = id;
	cout << "Movie ID: " << movie_id << endl;
	cout << "Enter name: ";
	cin.ignore();
	getline(cin, movie_name);

}

void movie::list()							// Lists movie credentials
{	
		cout << endl << "Movie Id: " << movie_id;
		cout << endl << "Movie name: " << movie_name;
}

int movie::ret_id()						
{
	return movie_id;
}

void movie::new_name()								
{		
	cout << endl << "Enter new name: ";
	cin.ignore();
	getline(cin, movie_name);
}

int getch() 							// To take hidden input
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}


void initial()							// Admin or User selection
{
	char ans;
	char pw[] = "karan", password[6];

	cout << "Admin or User? (A/U): ";
	cin >> ans;


	if(ans == 'a' || ans == 'A')
	{
		for(int j = 3; j>0; j--)
		{
			cout << endl << "Enter password: ";
			for (int i = 0; i<5; i++)
			{
		 
				password[i] = getch();
				cout << '*';
			}

			// cout << endl << password;


			if(strcmp(pw, password) == 0)
			{
				cout << endl << "Access granted";
				admin();
				return;
			}

			else
			{
					cout << endl << "Access denied" << endl << "You have " << j-1 << " tries left";
			}
		}
	}

	 if(ans == 'u' || ans == 'U')
		game();
}

void admin()
{
	char ch;

	cout << endl << endl << "Welcome Admin";
	cout << endl << endl << "Please choose Hollywood or Bollywood (H/B): ";
	cin >> ch;

	if(ch == 'h' || ch == 'H')
		holly_menu();
	else if (ch == 'b' || ch == 'B')
		bolly_menu();
	else
		cout << endl << "Wrong choice!!";
}

// THE GAME

void game()															
{
	char choice, movie[50], guess[50], type[10], ch, opt;
	int wrong = 0, tries = 9, i;

	do
	{
		system("clear");
		cout << endl << "Welcome to Hollywood Bollywood!!";
		cout << endl << "Choose - Hollywood or Bollywood (h/b): ";
		cin >> choice;

		if(choice == 'h' || choice == 'H')
			strcpy(type, "HOLLYWOOD");
		if(choice == 'b' || choice == 'B')
			strcpy(type, "BOLLYWOOD");

		system("clear");
		m.movie_assign(choice, movie);
		setup(choice, movie, guess, type);

		while(wrong < tries)
		{
			cout << endl << endl << "Enter your letter: ";
			cin >> ch;

			i = check(ch, movie, guess);

			if(i==-1)
				cout << endl << "Already guessed... Try again!";
			else if(i==0)
			{
				cout << endl << "Wrong!!";
				wrong++;
				wrong_cut(type, wrong, ch);
			}
			else
			{
				cout << "This letter occurs " << i << " times";
			}

			system("clear");
			cout << endl << endl << type << endl << guess;

			if(strcmp(movie, guess) == 0)
			{
				cout << endl << endl << "Victory";
				exit(0);
			}

			if(wrong == tries)
			{
				cout << endl << endl << "Sorry... You lose!" << endl << "The movie was: " << movie;
			}
		}

	cout << endl << endl << "Do you want to play again? (y/n): ";
	cin >> opt;

	}while(opt == 'y' || opt == 'Y');


}

void movie::movie_assign(char choice, char movie[])			// Assigns random movie to be guessed
{
	fstream f;
	int i;

	if(choice == 'h' || choice == 'H')
		f.open("hollywood.dat", ios::binary | ios::in);
	if(choice == 'b' || choice == 'B')
		f.open("bollywood.dat", ios::binary | ios::in);

	if(f.fail())
	{
		cout << "File failed to open";
		exit(1);
	}

	srand(time(0));

	i = rand() % 5 + 1;									// randomizes the movie

	while(f.read((char*)&m, sizeof(m)))
	{
		if(i == m.ret_id())
			strcpy(movie, m.movie_name.c_str());		// c.str() converts string into character array to be copied
	}

}

void setup(char choice, char movie[], char guess[], char type[])			// Sets up the interface
{
	fstream f;
	int i;

	if(choice == 'h' || choice == 'H')
		f.open("hollywood.dat", ios::binary | ios::in);
	if(choice == 'b' || choice == 'B')
		f.open("bollywood.dat", ios::binary | ios::in);

	if(f.fail())
	{
		cout << "File failed to open";
		exit(1);
	}

	for(i = 0; i<strlen(movie); i++)
	{
		if(movie[i] == 'a' || movie[i] == 'A' || movie[i] == 'e' || movie[i] == 'E' || movie[i] == 'i' || movie[i] == 'I' || movie[i] == 'o' || movie[i] == 'O' || movie[i] == 'u' || movie[i] == 'U' || movie[i] == ' ')
			guess[i] = movie [i];
		else
			guess[i] = '_';
	}

	guess[i] = '\0';

	cout << "Guess the following movie: " << endl << endl;
	cout << type << endl;
	cout << guess;
}


int check(char ch, char movie[], char guess[])		// checks whether entered letter is right or not
{
	int i=0, match = 0;

	for(int j = 0; j<strlen(movie); j++)
	{
		if(ch == movie[j])
		{
			guess[j] = ch;
			match ++;
			i = 1;
		}

		else if(ch == guess[j])
			i = 2;
	}

	if(i==0)			// Entered letter is wrong
		return 0;
	else if (i==2)		// Entered letter already exists
		return -1;
	else 
		return match;
}

void wrong_cut(char type[], int& wrong, char ch)		// Keeps cutting the hollywood/bollywood letter for each wrong
{
	if(type[wrong-1] == toupper(ch))
	{
		wrong--;
		return;
	}
	else
		type[wrong-1] = '-';
}



// MENUS

void holly_menu()
{
	int choice;
	char opt;


	do
	{
		system("clear");
		cout << endl << "Hollywood menu";
		cout << endl << endl << "1. Add a movie";
		cout << endl << "2. List the movies";
		cout << endl << "3. Modify a movie";
		cout << endl << "4. Delete a movie";
		cout << endl << "5. Go to bollywood menu";
		cout << endl << "6. Go to game";
		cout << endl << "7. Exit";

		cout << endl << endl << "Enter your choice: ";;
		cin >> choice;

		switch(choice)
		{
			case 1: addrec('h');
					break;
			case 2: listrec('h');
					break;
			case 3: modifyrec('h');
					break;
			case 4: deleterec('h');
					break;
			case 5: bolly_menu();
					break;
			case 6: game();
					break;
			case 7: cout << "Exiting...";
					exit(0);
			default: cout << "Wrong option!!";
		}

		cout << endl <<  "Do you want to continue? (y/n): ";
		cin >> opt;
	}while(opt == 'y' || opt == 'Y');	
}

void bolly_menu()
{
	int choice;
	char opt;


	do
	{
		system("clear");
		cout << endl << "Bollywood menu";
		cout << endl << endl << "1. Add a movie";
		cout << endl << "2. List the movies";
		cout << endl << "3. Modify a movie";
		cout << endl << "4. Delete a movie";
		cout << endl << "5. Go to hollywood menu";
		cout << endl << "6. Go to game";
		cout << endl << "7. Exit";

		cout << endl << endl << "Enter your choice: ";;
		cin >> choice;

		switch(choice)
		{
			case 1: addrec('b');
					break;
			case 2: listrec('b');
					break;
			case 3: modifyrec('b');
					break;
			case 4: deleterec('b');
					break;
			case 5: holly_menu();
					break;
			case 6: game();
					break;
			case 7: cout << "Exiting...";
					exit(0);
			default: cout << "Wrong option!!";
		}

		cout << endl <<  "Do you want to continue? (y/n): ";
		cin >> opt;
	}while(opt == 'y' || opt == 'Y');	
}


int main()
{
	initial();
}


// FILE HANDLING

void addrec(char type)
{
	fstream f;

	if(type == 'h')
		f.open("hollywood.dat", ios::binary | ios::app);
	else if(type == 'b')
		f.open("bollywood.dat", ios::binary | ios::app);

	if(f.fail())
	{
		cout << "File failed to open!";
		exit(0);
	}

	int i = autogen(type);
	m.in(i);

	f.write((char*)&m, sizeof(m));

	

	cout << "Movie successfully written!";

	f.close();

}

void listrec(char type)
{

	fstream f;

	if(type == 'h')
		f.open("hollywood.dat", ios::binary | ios::in);
	else if(type == 'b')
		f.open("bollywood.dat", ios::binary | ios::in);

	if(f.fail())
	{
		cout << "File failed to open!";
	}

	f.seekg(0,ios::beg);
	while(f.read((char*)&m, sizeof(m)))
	{
		m.list();
	}

	f.close();

}

void modifyrec(char type)
{
	int i, n;
	char choice, opt;


	fstream f;

	n = sizeof(m);

	if(type == 'h')
		f.open("hollywood.dat", ios::binary | ios::in | ios::out);
	else if(type == 'b')
		f.open("bollywood.dat", ios::binary | ios::in | ios::out);

	if(f.fail())
	{
		cout << "File failed to open!";
		exit(0);
	}

	do
	{
		cout << endl << "Enter the movie ID of the movie you want to modify: ";
		cin >> i;

		f.seekg(0,ios::beg);
		f.seekp(0, ios::beg);
		while(f.read((char*)&m, sizeof(m)))
		{
			if(i == m.ret_id())
			{
				cout << endl << "Is this the record you want to modify: ";
				m.list();
				cout << endl << "Yes or No (y/n): " ;
				cin >> choice;

				if(choice == 'y' || choice == 'Y')
				{
					m.new_name();

					f.seekp(-n, ios::cur);
					f.write((char*)&m, sizeof(m));

					cout << endl << "Record successfully modified!";
				}

				if(choice == 'n' || choice == 'N')
				{
					cout << "Do you want to try again (y/n): ";
					cin >> opt;
				}
			}
		}
	}while(opt == 'y' || opt == 'Y');

	f.close();

}

void deleterec(char type)
{
	fstream f1, f2;
	int i;

	f1.open("temp.dat", ios::binary | ios::out);

	if(type == 'h')
		f2.open("hollywood.dat", ios::binary | ios::in);
	else if(type == 'b')
		f2.open("bollywood.dat", ios::binary | ios::in);

	if(f1.fail())
	{
		cerr << "File could not be opened for writing!!";
		
		exit(1);
	}

	if(f2.fail())
	{
		cerr << "File could not be opened for writing!!";
		
		exit(1);
	}

	cout << "Enter the movie ID of the record to be deleted: ";
	cin >> i;

	while(f2.read((char*)&m, sizeof(m)))
	{	
		if(i == m.ret_id())
		{
			cout << endl <<  "Deleting the following record: ";
			m.list();
		}

		if(i != m.ret_id())	
			f1.write((char*)&m, sizeof(m));
	}

	cout << endl << "Record successfully deleted!!";


	f1.close();
	f2.close();

	if(type == 'h')
	{
		remove("hollywood.dat");
		rename("temp.dat", "hollywood.dat");
	}


	if(type == 'h')
	{
		remove("bollywood.dat");
		rename("temp.dat", "bollywood.dat");
	}

}

int autogen(char type)		// Auto generates the movie ID
{
	fstream f;
	int temp, i, n;

	n = sizeof(m);

	if(type == 'h')
		f.open("hollywood.dat", ios::binary | ios::in | ios::out);
	else if(type == 'b')
		f.open("bollywood.dat", ios::binary | ios::in | ios::out);

	if(f.fail())
	{
		cout << "File failed to open!";
		exit(0);
	}

	f.seekg(0,ios::end);
	temp = f.tellg();

	if(temp == 0)
		i = 1;
	else
	{	
		f.seekp(-n, ios::cur);
		f.read((char*)&m, sizeof(m));
		i = m.ret_id() + 1;
	}

	f.close();
	return i;
}