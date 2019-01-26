***JetPack Joyride***
-------------------------------------
**Manan Goel**
**20171102**

----------------------------------------
This is my version of JetPack Joyride made using openg3.3 written in C++. STL has also been used in the development of the game to make sure everything is as optimal as possible.

----------------------------------------
Controls
=============
-   The player can move forward and jump using the designated arrow keys.
-   The player can throw water balloons at oncoming obstacles to get rid of them using 'F' key.
-   To quit the game just press q.
   
------------------------------------------

Rules
===========
-   The player starts the game with a score of 0, 3 lives and at stage 1 as displayed by the counters on the top left corner.
-   The goal is to collect as many coins as possible before dying.
-   The player shall face 3 types of obstacles in their journey, 2 of which are fiery and can be extenguished by the player's weapon of choice(the balloons) and will also fetch 5 points.
-   Contact with any of the three obstacles will lead to an instant loss of life.
-   Beware of 2 other problems, the magnet and the inverter. The inverter shall turn your whole world around and the only to fix it is find another one. The magnet will constantly pull you towards itself. So, BEWARE!
-   As the game progresses you'll find power ups like a 'Coin Bag' which gives instant 10 points, 'Extra Life' which will grant you another life and 'Speed Up' which gives you the power to move faster than before.
-   As the player progresses through each stage the obstacles become quicker and avoiding them becomes harder.
-   Look out for the ring which is your only friend in this world of obstacles. It shall protect you from everything.
-------------------------
How To Play
=================
- Install OpenGl using this [link](https://docs.google.com/document/d/1yz5GG5jFkeTPmeg_H8FzzBsm-5mxkxlpoWuqnj3eMd8/edit?usp=sharing)
- Open Terminal
- Run the following commands
```
    git clone https://github.com/manangoel99/JetpackJoyride.git
    cd JetpackJoyride
    cd build
    make
    ./graphics_assign1
```
- **Happy Gaming**
Note -> For now the game only works on linux based platforms.
