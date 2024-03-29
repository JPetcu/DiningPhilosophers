// DiningPhilo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <thread>
#include <functional>
#include <thread>
#include <mutex>
#include <string>
#include <atomic>
#include <memory>
#include "CTimer.h"

std::mutex g_lockprint;
class fork
{
	std::timed_mutex forkMutex;
public:
	fork()
	{
		
	}
	/*fork(const fork& f)
	{
		
	}*/
	bool lock()
	{
		return forkMutex.try_lock_for(std::chrono::milliseconds(500));
	}
	void unlock()
	{
		 forkMutex.unlock();
	}
};
class filo
{
	const int eatTime = 1000;
	const int thinkTime = 1000;
	const int starveTime = 3000;
	bool dead = false;
	std::atomic_int id;
	std::thread lifeCycle;
	std::shared_ptr<fork> leftFork;
	std::shared_ptr<fork> rightFork;
	static bool enable;

	
	
public:
	static int increment ;

	static void enableBool()
	{
		enable = true;
	}

	std::string getID() const 
	{
		return std::to_string(id);
	}
	void setForks(std::shared_ptr<fork> lf,std::shared_ptr<fork> rf)
	{
		leftFork = lf;
		rightFork = rf;
	}
	void eat()
	{
		if (enable)
		{
			
			/*static thread_local  CTimer timer(1000);

			while(!timer.isDone() && timer.isTimedOut())
			{
				if (leftFork->lock())
					if (!rightFork->lock())
						leftFork->unlock();
					else
						timer.setDone();
				
			}*/

			if (leftFork->lock() == false)
			{
				dead = true;
				std::cout << "Philosopher with id=" << id << " has died\n";
				return;
			}
			if(rightFork->lock() == false)
			{
				dead = true;
				std::cout << "Philosopher with id=" << id << " has died\n";
				leftFork->unlock();
				return;
			}
			std::string toPrint = "Philosopher with id=";
			toPrint += getID();
			toPrint += " is eating\n";
			print(toPrint);
			std::this_thread::sleep_for(std::chrono::milliseconds(eatTime));
			leftFork->unlock();
			rightFork->unlock();
		}
	}
	void think()
	{

		 std::string toPrint = "Philosopher with id=";
		toPrint += getID();
		toPrint += " is thinking\n";
		print(toPrint);
		std::this_thread::sleep_for(std::chrono::milliseconds(thinkTime));
	}
	void print(std::string text)
	{
		g_lockprint.lock();
		std::cout << text<<std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		g_lockprint.unlock();
	}

	void alive()
	{
		while(!dead)
		{
			think();
			eat();
			
		}
		
	}

	filo() : id(increment++),lifeCycle(&filo::alive, this)
	{
		std::cout << "Created philosopher with id= " << id << "\n";
	}
	~filo()
	{
		lifeCycle.join();
	}
};
int filo::increment = 1;
bool filo::enable = false;

class Dining
{
	filo filos[5];
	std::shared_ptr<fork> forks[5];
public:
	Dining()
	{
		for (int i = 0; i < 5; i++)
			forks[i] = std::make_shared<fork>();
	}
	void setForks()
	{
		filos[0].setForks(forks[0], forks[4]);
		filos[1].setForks(forks[1], forks[0]);
		filos[2].setForks(forks[2], forks[1]);
		filos[3].setForks(forks[3], forks[2]);
		filos[4].setForks(forks[4], forks[3]);
		filo::enableBool();

	}
};

int main()
{
	Dining diningProblem;
	diningProblem.setForks();

}

