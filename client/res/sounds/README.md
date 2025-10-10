# SoundSystem Usage Guide

## Overview
The SoundSystem allows you to play sounds when entities are created or during gameplay. It uses SDL2_mixer for audio playback.

## Components

### Sound Component
```cpp
struct Sound {
    std::string sound_id;           // Reference to sound in SoundManager
    bool play_on_spawn = true;      // Play when entity is created
    bool loop = false;              // Loop the sound
    int volume = 128;               // 0-128 SDL_mixer volume
    bool is_playing = false;        // Current playback state
    int channel = -1;               // SDL_mixer channel (-1 = auto-assign)
    bool unique_playback = false;   // Prevent overlapping for same sound_id
};
```

## Usage Examples

### 1. Entity with Spawn Sound
```cpp
// In any entity creation function
Entity player = registry.create_entity();
// ... add other components ...
registry.emplace<Sound>(player, Sound{"player_spawn.wav", true, false, 100});
```

### 2. Background Music
```cpp
// Create a global background music entity
Entity bgm = registry.create_entity();
registry.emplace<Sound>(bgm, Sound{"background_music.ogg", true, true, 64});
```

### 3. Dynamic Sound Effects
```cpp
// In collision system or other systems
if (collision_detected) {
    // Create temporary sound entity
    Entity sound_entity = registry.create_entity();
    registry.emplace<Sound>(sound_entity, Sound{"explosion.wav", true, false, 128});
    registry.emplace<Lifetime>(sound_entity, 1.0f); // Auto-destroy after sound
}
```

### 4. Sound Manager Usage
```cpp
// Load sounds in Game::initialize()
auto& soundManager = SoundManager::getInstance();
soundManager.loadSound("player_spawn", "client/res/sounds/player_spawn.wav");
soundManager.loadSound("enemy_death", "client/res/sounds/enemy_death.wav");
soundManager.loadSound("shoot", "client/res/sounds/shoot.wav");
```

## Sound Files
Place your sound files in this directory:
- `.wav` - Uncompressed, high quality
- `.ogg` - Compressed, good for music
- `.mp3` - Compressed, widely supported

## Volume Control
- 0-128 range (SDL_mixer standard)
- 128 = max volume
- Can be adjusted per sound or globally

## Channels
- Each playing sound uses a channel
- -1 = auto-assign available channel
- Up to 8 channels by default (configurable in SDL_mixer)

## Current Implementation

### Loaded Sounds
The game currently loads these sounds in `Game::initialize()`:
- **vivaldi.mp3** - Background music (loops at medium volume)
- **Anvil_land.mp3** - Player spawn sound (unique playback)
- **Phantom_swoop1.mp3** - Enemy spawn sound (unique playback)

### Background Music
- Starts automatically when entering gameplay (`startGameplay()`)
- Loops continuously at 64/128 volume (medium)
- Restarts each time the game begins playing
- Uses `unique_playback = false` (can overlap with itself if restarted)

### Spawn Sounds
- **Player spawn**: Plays anvil sound with `unique_playback = true`
- **Enemy spawn**: Plays phantom sound with `unique_playback = true`
- Prevents overlapping - if sound is already playing, new instances are skipped
- Temporary entities with 1-second lifetime for auto-cleanup

## Best Practices
1. Load sounds at startup in `Game::initialize()`
2. Use descriptive sound IDs
3. Keep sound files small for better performance
4. Use appropriate formats (WAV for short effects, OGG/MP3 for music)
5. Test volume levels - 128 might be too loud
6. Use `unique_playback = true` for sounds that shouldn't overlap
