// SPDX-License-Identifier: BSD-2-Clause
// author: Max Kellermann <max.kellermann@gmail.com>

#pragma once

#include <sodium/crypto_hash_sha256.h>

#include <array>
#include <cstddef>
#include <span>

using SHA256DigestBuffer = std::array<std::byte, crypto_hash_sha256_BYTES>;
using SHA256DigestView = std::span<const std::byte, crypto_hash_sha256_BYTES>;

class SHA256State {
	crypto_hash_sha256_state state;

public:
#if 1  // !defined(__MSVC__)  // def __CLANG__
  SHA256State() noexcept {
	}

	void Update([[maybe_unused]] std::span<const std::byte> p) noexcept {
	}

	void Final([[maybe_unused]] std::span<std::byte, crypto_hash_sha256_BYTES> out) noexcept {
	}

#else
  SHA256State() noexcept {
    crypto_hash_sha256_init(&state);
  }
  void Update(std::span<const std::byte> p) noexcept {
		crypto_hash_sha256_update(&state,
					  reinterpret_cast<const unsigned char *>(p.data()),
					  p.size());
	}

	void Final(std::span<std::byte, crypto_hash_sha256_BYTES> out) noexcept {
		crypto_hash_sha256_final(&state,
					 reinterpret_cast<unsigned char *>(out.data()));
	}
#endif

	auto Final() noexcept {
		SHA256DigestBuffer out;
		Final(out);
		return out;
	}
};

[[gnu::pure]]
inline auto
SHA256(std::span<const std::byte> src) noexcept
{
	SHA256DigestBuffer out;
	crypto_hash_sha256(reinterpret_cast<unsigned char *>(out.data()),
			   reinterpret_cast<const unsigned char *>(src.data()),
			   src.size());
	return out;
}
