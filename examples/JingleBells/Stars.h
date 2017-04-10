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

class BlinkingStar : public CoRoutine
{
private:
  int x;
  int y;
  int halfPeriod;
  bool bright;
  DisplayPainter& displayPainter;

public:
  BlinkingStar(int x, int y, int period, DisplayPainter& displayPainter)
    : x(x), y(y),
      halfPeriod(period/2),
      bright(random(100) < 50), // Start out in random state.
      displayPainter(displayPainter)
  { }
  
  virtual int worker()
  {
    // Paint the pixel to display painter and schedule a redraw.
    const byte color = (bright ? Blue : DimBlue);
    displayPainter.drawStarPixel(x, y, color);
    displayPainter.scheduleRepaintPixel(x,y);

    // Switch brightness and sleep until it is time to blink again.
    bright = !  bright;
    return halfPeriod;
  }
};

