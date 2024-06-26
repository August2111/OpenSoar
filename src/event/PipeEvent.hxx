// SPDX-License-Identifier: BSD-2-Clause
// Copyright CM4all GmbH
// author: Max Kellermann <mk@cm4all.com>

#pragma once

#include "SocketEvent.hxx"
#include "io/FileDescriptor.hxx"

/**
 * A variant of #SocketEvent for pipes (and other kinds of
 * #FileDescriptor which can be used with epoll).
 */
class PipeEvent final
	: public EventPollBackendEvents
{
	SocketEvent event;

public:
	template<typename C>
	PipeEvent(EventLoop &event_loop, C callback,
		  FileDescriptor fd=FileDescriptor::Undefined()) noexcept
#ifdef _WIN32
      // TODO(August2111): needs work!
      : event(event_loop, callback, SocketDescriptor()){}
#else
      : event(event_loop, callback,
		       SocketDescriptor::FromFileDescriptor(fd)) {}
#endif
	EventLoop &GetEventLoop() const noexcept {
		return event.GetEventLoop();
	}

	bool IsDefined() const noexcept {
		return event.IsDefined();
	}

#ifdef _WIN32
  // TODO(August2111): needs work!
	FileDescriptor GetFileDescriptor() const noexcept {
		return FileDescriptor();
	}

	FileDescriptor ReleaseFileDescriptor() noexcept {
		return FileDescriptor();
	}

	void Open([[maybe_unused]] FileDescriptor fd) noexcept {
		// event.Open(SocketDescriptor::FromFileDescriptor(fd));
	}
#else
  FileDescriptor GetFileDescriptor() const noexcept {
		return event.GetSocket().ToFileDescriptor();
	}

	FileDescriptor ReleaseFileDescriptor() noexcept {
		return event.ReleaseSocket().ToFileDescriptor();
	}

	void Open(FileDescriptor fd) noexcept {
		event.Open(SocketDescriptor::FromFileDescriptor(fd));
	}
  #endif

	void Close() noexcept {
		event.Close();
	}

	bool Schedule(unsigned flags) noexcept {
		return event.Schedule(flags);
	}

	void Cancel() noexcept {
		event.Cancel();
	}

	bool ScheduleRead() noexcept {
		return event.ScheduleRead();
	}

	bool ScheduleWrite() noexcept {
		return event.ScheduleWrite();
	}

	void CancelRead() noexcept {
		event.CancelRead();
	}

	void CancelWrite() noexcept {
		event.CancelWrite();
	}

	void ScheduleImplicit() noexcept {
		event.ScheduleImplicit();
	}
};
