#pragma once

#include <vector>
#include <functional>
#include <autoapp/Managers/IManager.hpp>


class INightManager: public IManager{
 public:
  using Pointer = std::shared_ptr<INightManager>;
  using nightCallback = std::function<void(bool)>;

 private:
  std::vector<nightCallback> nightCallbacks;

 public:
  INightManager() = default;
  ~INightManager() override = default;

  void registerNightCallback(const nightCallback &callback) {
    nightCallbacks.emplace_back(callback);
  };

  void updateNight(bool isNight) {
    for (auto &callback : nightCallbacks) {
      callback(isNight);
    }
  }

};