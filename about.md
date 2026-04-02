# What does it do

Smart Practice fundamentally rewrites how Auto-Checkpoints function in Geometry Dash. \
The vanilla system operates on a blind timer, frequently resulting in checkpoints being placed directly over hazards or during high-velocity maneuvers, forcing the player into unavoidable death loops.

This mod utilizes Geode and Cocos2d-x to read the player's physical state in real-time, ensuring checkpoints are only placed when the player is in a safe, neutral position.

Features include:
- **Ground Validation:** Prevents mid-air checkpoints for ground-based modes (Cube, Robot, Spider)
- **Velocity Checking:** For air-based modes (Ship, UFO, Swingcopter), the system waits for the player to stabilize horizontally before placing a marker
- **Dash Orb Protection:** Temporarily halts checkpoint placement while the player is locked into a dash animation
- **Spatial Spacing:** Enforces a minimum physical distance between checkpoints to prevent clustering in slow-speed portals

# How to use

To use the mod, simply enter Practice Mode and play normally. You can adjust the timer and distance parameters in the mod's settings menu.

<cy>**CRITICAL REQUIREMENT:**</c> You must disable the vanilla "Auto-Checkpoint" setting in the game's pause menu! \
If you leave the default setting enabled, both systems will attempt to place checkpoints simultaneously, resulting in overlapping checkpoints and a severely cluttered screen.