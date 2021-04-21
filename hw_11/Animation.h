#ifndef ANIMATION_H_
#define ANIMATION_H_

class Animation{
 public:
  Animation() = default;
  Animation(sf::Texture& t, int x, int y, float w, float h, int count, float Speed) : frame_(0.0f), speed_(Speed){
    for (int i = 0; i < count; i++)
      frames_.emplace_back(x + i * w, y, w, h);

    sprite_.setTexture(t);
    sprite_.setOrigin(w / 2.0f, h / 2.0f);
    sprite_.setTextureRect(frames_[0]);
  }

  void update()
  {
    frame_ += speed_;
    auto n = frames_.size();
    if (frame_ >= n) {
      frame_ -= n;
    }
    if (n > 0) {
      sprite_.setTextureRect(frames_[static_cast < int >(frame_)]);
    }
  }

  bool isEnd() const{
    return frame_ + speed_ >= frames_.size();
  }

  sf::Sprite& sprite(){ return sprite_; };

 private:
  sf::Sprite sprite_;
  float frame_{}, speed_{};
  std::vector<sf::IntRect> frames_;
};

#endif //ANIMATION_H_
