<img src="http://redbarongames.co.uk/Github/Images/logo.png">

<img src="http://redbarongames.co.uk/Github/Images/Sokoban-Welcome.png" align="center">

SokoGenerator is a free, open source level generator for the game 'Sokoban' with the following features:
* **Customisable Number of Levels**
* **Customisable Level Height/Width**
* **Customisable Number of Boxes**
* **Customisable Difficulty of Generated Levels**
* **Customisable Seed for Level Generation**
* **Regenerate or Delete unwanted Levels**
* **Save Level Set to a .sok file to be loaded by your favourite Sokoban application**

**Compatible with:** Windows XP/Vista/7/8/10

<br><br>
## Index

- [Changelog](#changelog)
- [Future Ideas](#future-ideas)
- [Download SokoGenerator](#download)
- [Help](#help)
- [License](#license)

<br><br>
<a name="changelog"></a>
## What's new in SokoGenerator 1.0.1
<b>29 March 2016</b>

###Version 1.0.1
**Bug Fixes:**
* Reset Generator Seed on File > New
* Generation now has it's timeout set to 1 min. The Solving Time Limit is set by the GUI Spinbox
* Choosing a Specific Difficulty will now create Levels of that difficulty only
* Updated Difficulty on Regeneration or Deletion will now display in the Level Set
* Regeneration now uses proper seeding and will generate a new level every time

###Version 1.0
**This is the intial release of SokoGenerator. Everything the program is currently capable of is here:**
* Customisable Number of Levels
* Customisable Level Height/Width
* Customisable Number of Boxes
* Customisable Difficulty of Generated Levels
* Customisable Seed for Level Generation
* Regenerate or Delete unwanted Levels
* Save Level Set to a .sok file to be loaded by your favourite Sokoban applicatio
* Difficulty Rating calculated for each Level

This program will be updated periodically with Bug Fixes and new Features.

<br><br>
<a name="future-ideas"></a>
## Future Ideas
1. Open dialog for users to load a .sok file, allowing Regeneration/Deletion of Levels in the Level Set
2. Separate Solver from Generator allowing users to load levels in and check for a solution
3. Drag & Drop Editor allowing users to create their own levels and save them to a level set
4. Sokoban Game instance created by the program, allowing users to play the Generated/Created Levels before saving
5. Decrease Solving time with improvements to the solver
6. Naming of Indiviual Levels

These are all ideas that I am working on or wanting to work on in the future. Any other suggestions please let me know.

<br><br>
<a name="download"></a>
## Download SokoGenerator
**Latest Version: SokoGenerator 1.0.1**
<br>
<a href="https://github.com/RBrNx/SokoGenerator/releases/download/1.0.1/SokoGenerator-1.0.1-Setup.exe">SokoGenerator 1.0.1 Install Wizard</a>
<br>
<a href="https://github.com/RBrNx/SokoGenerator/releases/download/1.0.1/SokoGenerator-1.0.1.zip">SokoGenerator 1.0.1 Portable .exe</a>
<br><br>
<a href="https://github.com/RBrNx/SokoGenerator/releases">**Previous Verisons**</a>

<br><br>
<a name="help"></a>
## Help
#### File Menu
<img src="http://redbarongames.co.uk/Github/Images/Sokoban-FileMenu.png" align="center" width="70%" height="70%">
###### New
* New will reset every variable to it's inital state. 
* If pressed during Generation the program will stop the Generation and reset all the variables.
<br><br>

###### Save As
* Will Save the current Level Set to a .sok file, which can be read by most Sokoban applications.
* Levels inside the file will be named the same as the filename. i.e GenTest.sok will contain GenTest 1, GenTest 2, GenTest 3, etc.
<br><br>

###### Close
* Does what it says on the tin, closes the application.
<br><br><br>

#### About Menu
<img src="http://redbarongames.co.uk/Github/Images/Sokoban-About.png" align="center" width="70%" height="70%">
###### About SokoGenerator
* Opens the About Window, showing the Version Number and a link to this page.
<br><br><br>

#### Buttons/Fields/Dropdown Lists/Labels
<img src="http://redbarongames.co.uk/Github/Images/Sokoban-GUI.png" align="center" width="70%" height="70%">
###### Generator Seed
* Optional - Allows the user to enter a seed for the Generator to use. If the same seed is entered into two different instances of the application, and all the settings are the same, both instances of the application will generate the exact same levels.
* It can be left blank and the Generator will choose a random seed and display it to the user.
* The user does not need to delete the random seed for the Generator to choosen another random seed when 'Generate' is pressed, however if the seed has been entered by the user then it must be deleted for the Generator to use a random seed on the next Generation.
<br><br>

###### No. Of Levels
* Dropdown Box allows the user to choose a number of levels from 1 - 20. 
* Can be left at "Random" which allows the Generator to choose a random number of Levels between 1 and 20.
* Please Note: Random Values apply for all the levels, i.e the generator will choose a different random value for each level in the level set.
<br><br>

###### Room Height / Room Width
* Both of these dropdown boxes allow the user to choose a room height and a room width between 3 and 15. 
* Can be left at "Random" for the Generator to randomly select two values between 3 and 15, which will always be a multiple of 3.
* Please Note: Random Values apply for all the levels, i.e the generator will choose a different random value for each level in the level set.
* Multiple of 3 is necessary due to the procedure the Generator uses in which to Generate the levels. 
* A 3x3 Room is not possible as there would not be enough space for the player, plus goals and boxes. Therefore 6 x 3 is the smallest possible room size.
* These Values indicate Floor Size, any walls are added to the outside of the floor; i.e if a 6x6 room size is chosen, there will be a 6x6 floor, with walls all around the outside making the entire level 8x8 in total.
* One of these can be left at random while the other has a chosen value and the Generator will take it into account.
<br><br>

###### No. of Boxes
* Dropdown Box allowing the user to choose a number of boxes (and therefore goals) between 3 and 6 to be placed in the level during Generation.
* Can be left at "Random" to allow the Generator to randomly choose a value between 3 and 6.
* Please Note: Random Values apply for all the levels, i.e the generator will choose a different random value for each level in the level set.
<br><br>

###### Difficulty
* Dropdown Box allowing the user to choose a specific difficulty for the next set of generate levels.
* Can be left at "Random" to allow the Generator to randomly choose a value, from "Very Easy" to "Very Hard"
* Please Note: Random Values apply for all the levels, i.e the generator will choose a different random value for each level in the level set.
<br><br>

###### Generation Time Limit
* Spin Box allowing the user to choose a time limit for each level to be generated in. If the Generating or Solving for an individual level goes over the limit, it will be thrown away and started again.
* Can be used to decrease Solving/Generation time, however the trade-off is that better/harder levels *can* take longer to solve; this is not always the case though. 
* Generation Time Limit is in Minutes. A time of 0.50 equates to 30 Seconds. 1.00 equates to 1 Minute, 1.75 equates to 1 Minute 45 Seconds.
* Can be left blank (Zero, 0.00) to remove the time limit. Note: Generation can sometimes take a while with No Limit. I suggest a reasonable time limit of around 2 Minutes.
<br><br>

###### 'Generate' Button
* Starts the Level Generation using the currently selected values.
<br><br>

###### Current Generation Time
* Shows the current length of time taken to generate the level set, in the format Minutes:Seconds:Milliseconds.
* Current Generation Time will continue from its current time if 'Regenerate Level' is chosen in the Right-Click Menu.
<br><br>

###### Percentage Complete
* Shows the percentage of the current progress that the Generator has made.
* Updates after each level has been generated.
<br><br><br>

#### Right-Click Menu
<img src="http://redbarongames.co.uk/Github/Images/Sokoban-RightClick.png" align="center" width="70%" height="70%">
* Appears when Right-Clicking on a Level in the Level Set
<br><br>

###### Regenerate Level
* If a level that has been generated is unsuitable, this will regenerate the level using the same parameters that it has been generated with in the first instance.
* Example: If the level was generated with a Room Height and Width of 6, then the resulting regenerated level will have the same Height and Width.
* Example: If the level was generated with a "Random" Room Height and Width, then the resulting regenerated level will also use a "Random" value for the Room Height and Width, not necessarily the same one.
* Please Note: If you change the variables after Level Set generation has finished, and then regenerate a level, the regenerated level will use the new variables instead of the variables used to generate the original level.
<br><br>

###### Delete Level
* Removes a Level from the Level Set.
* Shuffles down and Renames all the levels remaining levels. 
<br><br>

###### View Solution
- Displays the Solution found by the Solver in the LURD format.
  - l = player moves left, u = player moves up, r = player moves right, d = player moves down.
  - L = player pushes box left, U = player pushes box up, R = player pushes box right, D = player pushes box down.
- Solution is also shown in the Run Length Encoded LURD format.
  - u5 = player moves up 5 spaces, l2 = player moves left 2 spaces, R4 = player pushes box right 4 spaces.
- Solution shown is not necessarily the one with the least moves or pushes, it is simply the first one found by the Solver.
- Best Moves or Best Pushes solutions cannot currently be found with the Solver.
<br><br><br>

#### Level Set Information
<img src="http://redbarongames.co.uk/Github/Images/Sokoban-LevelSet.png" align="center" width="70%" height="70%">
###### Level Generation Time
* The time it took to generate each individual level is displayed in the Level Title for each level
* Format is Minutes:Seconds:Milliseconds
<br><br>

###### Level Difficulty
* Each levels difficulty is displayed in the Level Title beside the Level Generation Time
<br><br>

###### Current Level
* Clicking on one of the Levels in the Level Set will show a preview of the Level in the Right-Hand box.
* Walls are designated by the Blue Squares.
* Goals are designated by the Orange and Black Squares.
* Boxes are designated by the Wooden Crates, a Box on a Goal Square is designated by a Wooden Crate with a Yellow Label.
* Player is designated by the Robot.
<br><br><br>

<a name="license"></a>
## License

MIT License

Copyright (c) 2016 SokoGenerator

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
