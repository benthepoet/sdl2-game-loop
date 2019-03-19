struct Animation {
  int frame_w;
  int frame_h;
  int frame_offset;
  int frame_span;
  int frame_timer;
  int texture_w;
  SDL_Texture *texture;
};

struct Sprite {
  int x;
  int y;
  int velocity_x;
  SDL_RendererFlip orientation;
  struct Animation animation;
};

struct SpriteNode {
  struct Sprite data;
  struct SpriteNode *next;
};
