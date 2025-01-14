/*
*  This file is part of openauto project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  openauto is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  openauto is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with openauto. If not, see <http://www.gnu.org/licenses/>.
*/

#include <autoapp/Service/Pinger.hpp>
#include <easylogging++.h>

namespace autoapp::service {

Pinger::Pinger(asio::io_service &ioService, time_t duration)
    : strand_(ioService),
      timer_(ioService),
      duration_(duration),
      cancelled_(false),
      pingsCount_(0),
      pongsCount_(0),
      missedCount_(0) {

}

void Pinger::ping(Promise::Pointer promise) {
  strand_.dispatch([this, self = this->shared_from_this(), promise = std::move(promise)]() mutable {
    cancelled_ = false;

    if (promise_ != nullptr) {
      promise_->reject(aasdk::error::Error(aasdk::error::ErrorCode::OPERATION_IN_PROGRESS));
    } else {
      ++pingsCount_;
//      LOG(INFO) << "[Pinger] Ping counter: " << pingsCount_;

      promise_ = std::move(promise);
      timer_.expires_from_now(std::chrono::milliseconds(duration_));
      timer_.async_wait(strand_.wrap([this](const asio::error_code &error) { onTimerExceeded(error); }));
    }
  });
}

void Pinger::pong() {
  strand_.dispatch([this, self = this->shared_from_this()]() {
    ++pongsCount_;
//    LOG(INFO) << "[Pinger] Pong counter: " << pongsCount_;
  });
}

void Pinger::onTimerExceeded(const asio::error_code &error) {
  if ((pingsCount_ - pongsCount_) > missedCount_) {
    missedCount_ = pingsCount_ - pongsCount_;
    LOG(INFO) << "[Pinger] Ping missed. Count: " << missedCount_;
  }
  if (promise_ == nullptr) {
    return;
  }
  if (error == asio::error::operation_aborted || cancelled_) {
    promise_->reject(aasdk::error::Error(aasdk::error::ErrorCode::OPERATION_ABORTED));
  } else if (pingsCount_ - pongsCount_ > 4) {
    promise_->reject(aasdk::error::Error());
  } else {
    promise_->resolve();
  }

  promise_.reset();
}

void Pinger::cancel() {
  LOG(DEBUG) << "Pinger Cancel";
  strand_.dispatch([this, self = this->shared_from_this()]() {
    cancelled_ = true;
    timer_.cancel();
  });
}

}
