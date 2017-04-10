/*
  Simple song player library for Meggy Jr RGB.
  Example: JingleBells
  
  Version 1.0 - 2011-01-01
  Copyright (c) 2011 Martin Gamwell Dawids.

    This file is part of the SongPlayer library for Meggy Jr RGB.

    This library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this library.  If not, see <http://www.gnu.org/licenses/>.

  For an overview of the functionality in this library,
  please see the file "SongPlayer.h".
*/

#include <MeggyJrSimple.h>
#include <CoRoutines.h>
#include <SongPlayer.h>

using namespace coroutines;
using namespace songplayer;

// These headers are not real headers.
// There is a lot of inline code in them and we ought to split them into seperate C++ modules
// (i.e. code i .cpp files and headers with no (or less) inline code in .h files) and have
// #ifdef include guards.
// But to keep it a simple example, we did it like this.
// Therefore include order is important.
#include "DisplayPainter.h"
#include "Song.h"
#include "Snow.h"
#include "Stars.h"

Scheduler scheduler;

// Seed the random generator.
// Can't we find a better way to do this?
void seedRandom()
{
  byte buttonState = 0;
  while (!buttonState)
  {
    buttonState = Meg.GetButtons();
    // Maybe show splash screen here
  }
  randomSeed(millis());
}

// The co-routine that does the interleaving of the star and snow layers and paint the display.
DisplayPainter displayPainter;

// Define the stars.
// Use prime numbers for blink intervals
// so the stars appear to blink independently and not in time.
// http://doc.qt.nokia.com/3.2/primes.html
BlinkingStar star1(1,5,1181, displayPainter);
BlinkingStar star2(2,6,1009, displayPainter);
BlinkingStar star3(3,3,1201, displayPainter);
BlinkingStar star4(4,5,1327, displayPainter);
BlinkingStar star5(5,4,1699, displayPainter);
BlinkingStar star6(6,2,1187, displayPainter);
BlinkingStar star7(7,6,1303, displayPainter);

// Our array of stars.
BlinkingStar* stars[] = {&star1, &star2, &star3, &star4, &star5, &star6, &star7, 0};

// Add all the stars to the scheduler
void addStarsToScheduler()
{
  int i = 0;
  while (stars[i] != 0)
  {
    scheduler.addCoRoutine(*stars[i]);
    ++i;
  }
  
  // More efficient, but too difficult to read.
  /*
  for (BlinkingStar** s = &stars[0]; *s != 0; ++s)
  {
    scheduler.addCoRoutine(**s);
  }
  */
}

Snow snow(displayPainter);

void setup()
{
  // Setup stuff.
  MeggyJrSimpleSetup();
  Serial.begin(9600);
  seedRandom();

  // Schedule song, stars and falling snow.
  jingleBellsPlayer.suspend(); // Do not play song until awakened. See below.
  scheduler.addCoRoutine(jingleBellsPlayer);
  addStarsToScheduler();
  scheduler.addCoRoutine(snow);

  // Add display painter last. It will draw the display for us
  // and merge the star and snow layers correctly.
  scheduler.addCoRoutine(displayPainter);
}

void loop ()
{
  CheckButtonsPress();
  
  // Rewind to start of song.
  if (Button_Left)
  {
    Serial.println("Pressed Left");
    jingleBellsPlayer.rewind();
  }

  // Pause song.
  if (Button_B)
  {
    Serial.println("Pressed B");
    jingleBellsPlayer.suspend();
  }
  
  // Play song.
  if (Button_A)
  {
    Serial.println("Pressed A");
    jingleBellsPlayer.awake();
  }

  scheduler.runOnce();
}

