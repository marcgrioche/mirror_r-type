/*
** EPITECH PROJECT, 2025
** mirror_r-type
** File description:
** SpriteRegistry - Hardcoded sprite configuration registry
*/

#include "SpriteRegistry.hpp"

const std::unordered_map<std::string, SpriteConfig> SpriteRegistry::sprites = {
    {
        "player",
        {
            .textureId = "player_sprite.png",
            .filePath = "client/res/sprites/player_sprite.png",
            .animated = false,
            .width = 623.0f,
            .height = 623.0f
        }
    },
    {
        "enemy_bydo",
        {
            .textureId = "bydo_flying.png",
            .filePath = "client/res/sprites/bydo_flying.png",
            .animated = true,
            .frames = 4,
            .frameWidth = 79.0f,
            .frameHeight = 69.0f,
            .frameDuration = 0.15f
        }
    },
    {
        "projectile_eye",
        {
            .textureId = "eye_spritesheet.png",
            .filePath = "client/res/sprites/eye_spritesheet.png",
            .animated = true,
            .frames = 7,
            .frameWidth = 32.0f,
            .frameHeight = 24.0f,
            .frameDuration = 0.1f
        }
    },
    {
        "platform",
        {
            .textureId = "WallOfFlesh.png",
            .filePath = "client/res/sprites/WallOfFlesh.png",
            .animated = false,
            .width = 420.0f,
            .height = 190.0f
        }
    },

    {
        "parallax_top",
        {
            .textureId = "TopLayer.png",
            .filePath = "client/res/sprites/ParallaxBackground/TopLayer.png",
            .animated = false
        }
    },
    {
        "parallax_light",
        {
            .textureId = "Light.png",
            .filePath = "client/res/sprites/ParallaxBackground/Light.png",
            .animated = false
        }
    },
    {
        "parallax_middle",
        {
            .textureId = "MiddleLayer.png",
            .filePath = "client/res/sprites/ParallaxBackground/MiddleLayer.png",
            .animated = false
        }
    },
    {
        "parallax_down",
        {
            .textureId = "DownLayer.png",
            .filePath = "client/res/sprites/ParallaxBackground/DownLayer.png",
            .animated = false
        }
    },
    {
        "parallax_sky",
        {
            .textureId = "Sky.png",
            .filePath = "client/res/sprites/ParallaxBackground/Sky.png",
            .animated = false
        }
    },

    {
        "menu_background",
        {
            .textureId = "MenuBackground",
            .filePath = "client/res/sprites/UI/Background/menuBackground.png",
            .animated = false,
            .width = 2480.0f,
            .height = 2486.0f
        }
    },
    {
        "button_mouth",
        {
            .textureId = "ButtonMouth",
            .filePath = "client/res/sprites/UI/tools/buttonAsset.png",
            .animated = true,
            .frames = 2,
            .frameDuration = 0.2f
        }
    },
    {
        "button_eye",
        {
            .textureId = "ButtonEye",
            .filePath = "client/res/sprites/UI/tools/buttonParamAsset.png",
            .animated = true,
            .frames = 2,
            .frameDuration = 0.2f
        }
    },
    {
        "textbox_zone",
        {
            .textureId = "zoneText",
            .filePath = "client/res/sprites/UI/tools/textBoxAsset.png",
            .animated = false
        }
    },
    {
        "eye_outline",
        {
            .textureId = "eyeOutline",
            .filePath = "client/res/sprites/UI/tools/eyeOutline.png",
            .animated = false
        }
    },
    {
        "eye_pupil",
        {
            .textureId = "eyePupil",
            .filePath = "client/res/sprites/UI/tools/eyePupil.png",
            .animated = false
        }
    },
    {
        "eye_outline2",
        {
            .textureId = "eyeOutline2",
            .filePath = "client/res/sprites/UI/tools/eyeOutline2.png",
            .animated = false
        }
    },
    {
        "eye_pupil2",
        {
            .textureId = "eyePupil2",
            .filePath = "client/res/sprites/UI/tools/eyePupil2.png",
            .animated = false
        }
    },
    {
        "eye_outline3",
        {
            .textureId = "eyeOutline3",
            .filePath = "client/res/sprites/UI/tools/eyeOutline3.png",
            .animated = false
        }
    },
    {
        "eye_pupil3",
        {
            .textureId = "eyePupil3",
            .filePath = "client/res/sprites/UI/tools/eyePupil3.png",
            .animated = false
        }
    }
};

const SpriteConfig* SpriteRegistry::get(const std::string& key)
{
    auto it = sprites.find(key);
    if (it != sprites.end()) {
        return &it->second;
    }
    return nullptr;
}

const std::unordered_map<std::string, SpriteConfig>& SpriteRegistry::getAll()
{
    return sprites;
}
