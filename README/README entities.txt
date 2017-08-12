AI Demonstration - by Bradley Booth

AIE 13/8/17, Duncan Henderson and Jeff Cotter

this README describes the 4 main parts of the project


Trading:
-------------------------------------------------------------------------------------------------

- bots are spawned with random inventories

- bots trade items from their inventory to others

- bots use a Finite State Machine to determine how to behave (am I trading?, am I posting about trades?)

- bots post trades to a blackboard and respond to other trade

-------------------------------------------------------------------------------------------------



Pathfinding:
-------------------------------------------------------------------------------------------------

- bots are spawned with random shoulder widths

- bots find their way from point A to B, point A is it's current position, point B is a random node
  on the Nav Mesh

- bots repeatedly move around the Nav Mesh randomly

- bots use behaviour trees to determine what they should be doing

- bots can only call for a new path every 5 seconds, this demonstrates the behaviour tree's ability
  to wait for specific actions using the PENDING response before continuing

-------------------------------------------------------------------------------------------------



Steering:
-------------------------------------------------------------------------------------------------

- boids are spawned with random positions

- boids use 7 of the 9 available steering behaviours

  seek, arrival, seperation, cohesion, allignment, obstacle avoidance and queue

- a small amount of obstacles are spawned with random shape types, colours and positions

- the boids attempt to move towards the mouse

- a central blackboard contains information about all boids and obstacles

- boids use the blackboard to gain information about obstacles and neighbouring boids for flocking
  and obstacle avoidance

-------------------------------------------------------------------------------------------------



Puzzle:
-------------------------------------------------------------------------------------------------

- two puzzle bots are spawned

- puzzle bots are aiming to find a solution to a randomly generated sliding 3x3 puzzle board by calling
  upon the planner to give them instructions

- a randomly generated board is presented for a duration and then solved, the solved board is then
  presented for a duration before being shuffled again

- in some circumstances planners can take a long time (seconds) to compute a solution. this is
  because the search domain can get quite large before a solution is found

-------------------------------------------------------------------------------------------------