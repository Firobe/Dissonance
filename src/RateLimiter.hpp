#ifndef RATELIMITER_HPP
#define RATELIMITER_HPP

#include <map>
#include <string>
#include <cpprest/http_client.h>

using namespace web::http;

struct RateLimit {
	int remainingCalls;
	time_t resetDate;
	RateLimit() = default;
	RateLimit(int, int);
	RateLimit(http_headers);
};

class RateLimiter {
	std::map<std::string, RateLimit> _toWait;
	public:
	RateLimiter() = default;
	void ask(std::string);
	void report(std::string, RateLimit);
};

#endif
