// DiningPhilo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <thread>

class filo
{
	const int eatTime = 1;
	const int thinkTime = 1;
	const int starveTime = 3;
	bool dead = false;
	int id;
public:
	static int increment ;
	
	void eat()
	{
		
	}
	void think()
	{
		
	}
	void alive(filo f)
	{
		while(!dead)
		{
			f.eat();
			f.think();
		}
	}
	filo()
	{
		id = increment;
		increment++;
		std::cout << "Created philosopher with id= " << id << "\n";
	}
};
int filo::increment = 1;
class Dining
{
	filo filos[5];
	bool forks[5] {false};
public:
	Dining()
	{
		for (int i = 0; i < 5; i++)
			std::cout << forks[i] << " ";
		std::thread filoLife1(&filo::alive, filos[0]);
		std::thread filoLife2(&filo::alive, filos[1]);
		std::thread filoLife3(&filo::alive, filos[2]);
		std::thread filoLife4(&filo::alive, filos[3]);
		std::thread filoLife5(&filo::alive, filos[4]);


	}
};

int main()
{
	Dining* diningProblem = new Dining();
}

