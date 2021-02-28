# casv20-21

The general plan is that:
(1) A plan is devised in QGroundControl, saved as a .plan file which is then uploaded to our program.
(2) The mission is run, but the ZED is running asynchronously to look for obstacles.
(3) If there is an obstacle, the mission is halted
(3.1) While there still is an obstacle:
(3.1.1) UAV will be controlled by the autopilot obstacle avoidance until the obstacle is no longer detected. The obstacle avoidance logic is sent the missionItem as well. If it determines that the obstacle makes the missionItem impossible, it will move on to the next missionItem.
(3.1.2) If the UAV is unable to pause the mission, it will go into touchdown

(???) How to determine if a waypoint is just covered by an obstacle?