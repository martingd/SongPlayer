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

const Note startChorus[] =
{
  {ToneE5, duration_1_4},
  {ToneE5, duration_1_4},
  {ToneE5, duration_3_8},
  {0,      duration_1_8},

  {ToneE5, duration_1_4},
  {ToneE5, duration_1_4},
  {ToneE5, duration_3_8},
  {0,      duration_1_8},

  {ToneE5, duration_1_4},
  {ToneG5, duration_1_4},
  {ToneC5, duration_3_8},
  {ToneD5, duration_1_8},

  {ToneE5, duration_3_4},
  {0,      duration_1_4},

  {ToneF5, duration_1_4},
  {ToneF5, duration_1_4},
  {ToneF5, duration_3_8},
  {ToneG5, duration_1_8},
  
  {ToneF5, duration_1_4},
  {ToneE5, duration_1_4},
  {ToneE5, duration_3_8},
  {ToneE5, duration_1_8},

  EndOfSongSegment
};

const Note endChorusA[] =
{
  {ToneE5, duration_1_4},
  {ToneD5, duration_1_4},
  {ToneD5, duration_1_4},
  {ToneE5, duration_1_4},

  {ToneD5, duration_1_2},
  {ToneG5, duration_1_2},

  EndOfSongSegment
};

const Note endChorusB[] =
{
  {ToneG5, duration_1_4},
  {ToneG5, duration_1_4},
  {ToneF5, duration_1_4},
  {ToneD5, duration_1_4},

  {ToneC5, duration_3_4},
  {0,      duration_1_4},

  EndOfSongSegment
};

const Note* songSegments[] =
{
  startChorus,
  endChorusA,
  startChorus,
  endChorusB,
  0
};

SongPlayer jingleBellsPlayer(songSegments, Meg, false);

