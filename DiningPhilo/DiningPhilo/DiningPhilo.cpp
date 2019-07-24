// DiningPhilo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <thread>
#include <functional>
#include <thread>
#include <string_view>
#include <mutex>

std::mutex g_lockprint;
class filo
{
	const int eatTime = 1000;
	const int thinkTime = 1000;
	const int starveTime = 3000;
	bool dead = false;
	int id;
	std::thread lifeCycle;
	
	
public:
	static int increment ;
	
	void eat()
	{
		print("Philosopher with id=\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(eatTime));
	}
	void think()
	{
		print("Philosopher with id=" + id);
		std::this_thread::sleep_for(std::chrono::milliseconds(thinkTime));
	}
	void print(std::string text)
	{
		std::lock_guard<std::mutex> cout_lock(g_lockprint);
		std::cout << text;
	}
	void alive()
	{
		while(!dead)
		{
			eat();
			think();
		}
		std::cout << "Philosopher with id=" << id << " has died\n";
		lifeCycle.join();
	}
	filo() : lifeCycle(&filo::alive, this)
	{
		id = increment;
		increment++;
		std::cout << "Created philosopher with id= " << id << "\n";
	}
	~filo()
	{
		
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
	}
};

int main()
{
	Dining* diningProblem = new Dining();
}

