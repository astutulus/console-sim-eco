#include <iostream>
#define LOG(x) std::cout << x << std::endl;

/*
* ------------------------------
* Simulated Ecosystem in GameObj
* ------ by Robin Wootton ------
* ------------------------------
*
* To do:-
* 
*	Think I got a memory leak?
* 
* 
*/

#include "Windows.h"			// GetAsyncKeyState()

#include <random>				// rand()

#include "GameEngine.h"		// BufferCreate(), PaintEntities()
#include "EntityClasses.h"		// addGrass(), addRabbit()


GameObj* cons = new GameObj();

void PaintActor(Entity* e)
{
	cons->BufferAddCharacter(e->name, e->pos.x, e->pos.y);
}

void PaintStats(Rabbit* e)
{
	cons->BufferAddStats(e->fCurrEnergy);
}


int main()
{
	/*
	* Starting positions for simulation
	*/
	std::vector<Plant*> plants;
	Grass* cedric = new Grass(40, 5);
	plants.push_back(cedric);

	Grass* cecil = new Grass(70, 25);
	plants.push_back(cecil);

	Rabbit* peter = new Rabbit(20, 15);

	/*
	Keystroke timing variable
	*/
	std::chrono::system_clock::time_point tpLastDrawn = std::chrono::system_clock::now();

	/*
	* GAME LOOP
	*/
	bool running = true;
	while (running)
	{
		/*
		* TIMING
		*/
		float looptime = cons->LatestLoopTime();

		/*
		* CONTROL
		*/
		if (GetAsyncKeyState((unsigned short)'G') & 0x8000)
		{
			auto tpNewKeyPress = std::chrono::system_clock::now();
			std::chrono::duration<float> timeSinceLast = tpNewKeyPress - tpLastDrawn;
			float fElapsedTime = timeSinceLast.count(); // timeSinceLast has "seconds" after it!

			if (fElapsedTime > f_GRASS_SEED_RATE)
			{
				int randomX = rand() % cons->getScreenWidth(); // e.g.  rand() % 100;   uses the range 0 to 99 
				int randomY = (rand() % (cons->getScreenHeight() - 2)) + 2;   // to avoid using the top rows

				Grass* grass_ptr = new Grass(randomX, randomY);
				plants.push_back(  grass_ptr  );

				tpLastDrawn = std::chrono::system_clock::now();
			}
		}

		/*
		RUN SIMULATION
		*/
		peter->Graze(plants, looptime);

		// Cleanup consumed plants
		for (Plant* p : plants)
		{
			if (p->fNutritionalValue < 0)
			{
				// delete p;

				plants.erase(    std::remove(plants.begin(), plants.end(), p)       );

			
			}
		}

		/*
		TO DO

		if (x >= 0 && x < getScreenWidth() && y >= 2 && y < getScreenHeight())
		{
		}

		*/


		/*
		* DRAW
		*/
		cons->BufferWipe(); // phase out when rabbit moves and wipes footprints

		for (Plant* plant : plants)
		{
			PaintActor(plant);
		}

		PaintActor(peter);
		PaintStats(peter);

		cons->BufferPaint();
	}

	std::cin.get();
}