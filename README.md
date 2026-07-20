# Project Axiom

> A multiplayer first-person shooter developed in Unreal Engine 5.8 using C++ and Blueprints.

## Overview

Project Axiom is a gameplay programming portfolio project focused on building a scalable, data-driven multiplayer first-person shooter.

The project is designed to support dedicated servers, listen servers, and standalone single-player play. Its gameplay systems are implemented with server authority while preserving a responsive client-side experience.

Axiom uses Data Assets, Gameplay Tags, modular C++ systems, and Blueprint integration to organize weapons, animations, user interface data, effects, and gameplay behavior. The architecture is intended to allow new weapons and content to be added primarily through assets and data rather than by modifying existing systems.

## Core Systems

### Multiplayer Framework

- Server-authoritative gameplay
- Dedicated server support
- Listen server support
- Standalone single-player support
- Replicated character and weapon behavior
- Multiplayer testing through Play In Editor

### Character System

- First-person character mesh
- Third-person character mesh
- Multiplayer character representation
- Enhanced Input
- Movement, jumping and crouching
- Aiming and field-of-view adjustment
- Weapon-dependent animation states

### Weapon System

- Modular weapon inventory
- Weapon equipping and cycling
- Support for multiple weapon types
- Automatic and semi-automatic firing
- Weapon aiming
- Reloading
- Reserve ammunition
- Automatic reload when empty
- Ammunition pickups
- Client-side ammunition prediction
- Server-authoritative weapon validation

### Animation

- First-person Animation Blueprint
- Third-person Animation Blueprint
- Aim offsets
- Strafing
- Jumping and crouching
- Hand inverse kinematics
- Turn-in-place
- Orientation warping
- Weapon-specific poses and animation data

### Combat

- Multiplayer damage handling
- Hit detection
- Headshot detection
- Player elimination
- Respawning
- Physical-surface-specific impact effects

### Effects

- Muzzle flashes
- Weapon trails
- Impact particles
- Bullet-hole decals
- Shell-ejection effects
- Weapon animations
- Weapon firing sounds
- Surface-dependent hit effects

### User Interface

- Health display
- Current ammunition
- Reserve ammunition
- Player score
- Weapon-specific reticles
- Dynamic reticle animation
- Reticle expansion and contraction
- Reticle reactions to firing, aiming and targeting players
- Gameplay notification messages

### Player Statistics

- Eliminations
- Defeats
- Hits
- Missed shots
- Elimination streaks
- Spree tracking
- Headshot eliminations
- Sequential eliminations
- First blood
- Revenge
- Dethrone
- Showstopper
- Lead changes and ties

## Architecture

Project Axiom is developed using a data-driven and modular approach.

Data Assets store information such as:

- Weapon configuration
- Animations and montages
- Interface icons
- Reticle data
- Gameplay messages
- Weapon-specific presentation data

Gameplay Tags are used to identify gameplay states and reference data without tightly coupling systems together.

The project is structured to make new weapons and gameplay content easier to add without requiring extensive changes to existing code.

## Technologies

- Unreal Engine 5.8
- C++
- Blueprints
- Unreal Engine Multiplayer Framework
- Enhanced Input
- Gameplay Tags
- Data Assets
- Animation Blueprints
- Niagara
- JetBrains Rider
- Git
- Git LFS

## Development Status

**Active Development**

Project Axiom is currently being developed as a multiplayer gameplay programming portfolio project. Features will be documented as they are implemented and tested.

## Development Environment

| Category | Technology |
|---|---|
| Engine | Unreal Engine 5.8 |
| Programming | C++ and Blueprints |
| IDE | JetBrains Rider |
| Version Control | Git and Git LFS |
| Networking | Dedicated Server, Listen Server and Standalone |

## Credits

Project Axiom is based on Stephen Ulibarri’s Unreal Engine 5 multiplayer first-person shooter course.

Course assets and instructional material belong to their respective creators. This repository documents my implementation, study progress, modifications, debugging work and continued development of the project.

## Author

**Ramzel Suarez**  
Gameplay Programmer  
Tokyo Cool Japan Academy