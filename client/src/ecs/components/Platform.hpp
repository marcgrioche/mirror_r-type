struct BottomPassPlatform {};
struct NoPassPlatform{};

struct MovingPlatform {
    float min_x, max_x;
    float speed;
    bool movingRight;
};
