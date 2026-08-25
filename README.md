# Project Axiom

> **Unreal Engine 5.8 / C++ Multiplayer FPS Technical Showcase**

Project Axiom is a gameplay programming portfolio project demonstrating the
core gameplay loop and network architecture of a multiplayer first-person
shooter using Unreal Engine 5.8, C++, and Blueprints.

**Status: Technical Showcase Complete**

## Overview

Project Axiom focuses on the implementation and integration of multiplayer
FPS gameplay systems within a server-authoritative architecture.

Rather than being a content-complete standalone game, the project is designed
as a technical showcase of a functional multiplayer FPS foundation:

**Spawn → Equip → Move / Aim → Combat → Elimination → Score / Special Elimination → Death → Respawn → Continue**

The implementation emphasizes multiplayer gameplay programming, replication,
reusable C++ systems, C++/Blueprint integration, and data-driven architecture.

Data Assets and Gameplay Tags are used to organize weapon configuration,
animation data, UI presentation, effects, and gameplay behavior while reducing
dependencies between systems.

> **About this project**
>
> Project Axiom is based on Stephen Ulibarri's Unreal Engine 5 Multiplayer FPS
> course. This repository documents my implementation, debugging, system
> integration, problem solving, and project-specific modifications based on
> the course material.
>
> 本作はStephen Ulibarri氏のUnreal Engine 5 Multiplayer FPS講座をベースに、
> 学習・実装・デバッグ・機能拡張を行ったポートフォリオ作品です。
> 講座内容を基礎として、ネットワーク処理、システム統合、問題解決、
> プロジェクト固有の調整を行っています。

---

## Core Systems

### Multiplayer Framework

- Server-authoritative gameplay architecture
- Replicated gameplay state
- Replicated character and weapon behavior
- Listen server support
- Dedicated server architecture
- Standalone support
- Multiplayer testing through Play In Editor and packaged builds

### Character System

- First-person and third-person character representation
- Enhanced Input
- Movement, jumping, crouching, and camera control
- Aiming and field-of-view adjustment
- Weapon-dependent animation states
- Death and respawn state handling
- PlayerController possession handling across respawns

### Weapon System

- Modular weapon inventory
- Weapon equipping and cycling
- Multiple weapon types
- Semi-automatic and automatic firing
- Weapon aiming
- Reloading
- Reserve ammunition
- Automatic reload when empty
- Ammunition pickups
- Client-side ammunition prediction
- Server-authoritative firing and ammunition state
- Replicated weapon state

### Combat

- Multiplayer hit detection
- Server-side damage handling
- Headshot detection
- Hit and miss reporting
- Lethal-hit reporting
- Player elimination
- Death handling
- Timed respawning
- Physical-surface-specific impact handling

### Player Statistics & Special Eliminations

- Eliminations and defeats
- Hits and missed shots
- Elimination streaks
- Spree tracking
- Headshot eliminations
- Sequential eliminations
- First Blood
- Revenge
- Dethrone
- Showstopper
- Leader tracking
- Lead changes and tied-leader states
- Special elimination bitmask processing

### Animation

- First-person Animation Blueprint
- Third-person Animation Blueprint
- Aim offsets
- Strafing
- Jumping and crouching
- Hand inverse kinematics
- Turn-in-place
- Orientation warping
- Weapon-specific poses
- Data-driven animation and montage configuration

### Effects

- Muzzle flashes
- Weapon trails
- Impact particles
- Bullet-hole decals
- Shell-ejection effects
- Weapon animations
- Weapon firing sounds
- Physical Material-based surface impact effects
- Niagara combat effects

### User Interface

- Health display
- Current ammunition
- Reserve ammunition
- Player score
- Weapon-specific reticles
- Dynamic reticle animation
- Reticle expansion and contraction
- Reticle response to firing and aiming
- Player-targeting reticle feedback
- Gameplay notification messages
- Special elimination notifications
- Score and gameplay-state updates

---

## Architecture

Project Axiom uses a modular, data-driven architecture designed to keep
gameplay systems reusable and responsibilities separated.

### C++ Gameplay Systems

Core gameplay behavior is implemented through interconnected C++ systems
responsible for areas including:

- Character and input handling
- Multiplayer combat
- Weapon state and inventory
- Health and damage
- Elimination processing
- Player statistics
- Game state and leader tracking
- Death and respawn flow

Blueprints are used alongside these systems for asset configuration,
presentation, UI integration, and gameplay setup.

### Data Assets

Data Assets organize configuration and presentation data such as:

- Weapon configuration
- Animation montages
- Weapon-specific animation data
- Interface icons
- Reticle parameters
- Gameplay messages
- Presentation data

This allows gameplay content to be configured primarily through data rather
than requiring changes to core gameplay code.

### Gameplay Tags

Gameplay Tags are used to identify and reference gameplay data and weapon
types without tightly coupling individual systems.

### Multiplayer Architecture

Gameplay authority resides on the server, while replication, RPCs, and
client-side presentation are used to maintain synchronized multiplayer
behavior while preserving responsive local feedback.

The implementation includes server, client, multicast, RepNotify, and
owner-specific replication patterns where appropriate.

---

## Development & Debugging

Development involved implementing and integrating the individual gameplay
systems as well as debugging interactions between networking, combat,
player state, UI, and respawn behavior.

Examples of problems addressed during development include:

- Synchronizing weapon and ammunition state between server and clients
- Maintaining responsive local firing while preserving server authority
- Replicating combat and player-state information to gameplay UI
- Reporting lethal hits correctly for elimination processing
- Updating leader and special-elimination states after player eliminations
- Restoring movement state correctly after death, respawn, and possession
- Integrating C++ gameplay events with Blueprint-based UI and presentation

---

## Technical Scope

Project Axiom is **not intended to be a content-complete FPS game**.

It is a gameplay programming technical showcase designed to demonstrate a
working multiplayer FPS core loop and the systems supporting it.

The project prioritizes:

- Multiplayer gameplay programming
- Server/client synchronization
- Gameplay system architecture
- C++ and Blueprint integration
- Data-driven implementation
- System integration
- Debugging and problem solving

The environment and presentation assets provide a playable context for
demonstrating and testing these systems.

---

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
- Physical Materials
- JetBrains Rider
- Git
- Git LFS

---

## Development Status

**Technical Showcase Complete**

Project Axiom is complete within its intended portfolio scope as a multiplayer
FPS gameplay programming and networking technical showcase.

A packaged Windows build is available through the repository's Releases page.

---

## Development Environment

| Category | Technology |
|---|---|
| Engine | Unreal Engine 5.8 |
| Programming | C++ / Blueprints |
| IDE | JetBrains Rider |
| Version Control | Git / Git LFS |
| Networking | Server-Authoritative Multiplayer |
| Target Platform | Windows |

---

## Credits

Project Axiom is based on Stephen Ulibarri's Unreal Engine 5 Multiplayer FPS
course.

Course assets and instructional materials belong to their respective creators.
This repository and its documentation represent my implementation, debugging,
system integration, modifications, and continued work based on the course
material.

Additional environment and third-party assets remain the property of their
respective creators.

---

## License

This repository is provided for portfolio and educational purposes only.

See the `LICENSE.txt` file for details.

---

## Author

**Ramzel Suarez**  
Gameplay Programmer  
Tokyo Cool Japan Academy
