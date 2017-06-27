#include "RateLimiter.hpp"

#include <thread>

using namespace std;

RateLimit::RateLimit(http_headers headers) {
	if(headers.has("X-RateLimit-Limit")){
		remainingCalls = stoi(headers["X-RateLimit-Remaining"]);
		resetDate = stoul(headers["X-RateLimit-Reset"]);
	}
	else throw runtime_error("Response does not have rate limiting headers");
}

RateLimit::RateLimit(int a, int b) :
	remainingCalls(a),
	resetDate(b)
{}

void RateLimiter::ask(string key) {
	time_t t = time(nullptr);
	auto search = _toWait.find(key);
	if(search != _toWait.end())
		if(search->second.remainingCalls <= 0 and search->second.resetDate > t) {
			cout << "Sleeping for rate limiting : " << key << endl;
			this_thread::sleep_for(chrono::seconds(search->second.resetDate - t));
		}
}

void RateLimiter::report(string key, RateLimit r) {
	_toWait[key] = r;
}
