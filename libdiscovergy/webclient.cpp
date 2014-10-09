/**
 * This file is part of libdiscovergy.
 *
 * (c) Fraunhofer ITWM - Mathias Dalheimer <dalheimer@itwm.fhg.de>,    2010
 *                       Ely de Oliveira   <ely.oliveira@itwm.fhg.de>, 2013
 *                       Stephan Platz     <platz@itwm.fhg.de>,        2014
 *
 * libklio is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libklio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libklio. If not, see <http://www.gnu.org/licenses/>.
 */

#include "webclient.h"
#include "libdiscovergy/error.h"
#include <curl/curl.h>
#include <sstream>
#include <iomanip>
#include <openssl/hmac.h>

using namespace libdiscovergy;

Webclient::Webclient() {}
Webclient::~Webclient() {}

/* Store curl responses in memory. Curl needs a custom callback for that. Otherwise it tries to write the response to a file. */
size_t Webclient::curlWriteCustomCallback(char *ptr, size_t size, size_t nmemb, void *data) {

	size_t realsize = size * nmemb;
	std::string *response = static_cast<std::string *> (data);;

	response->append(ptr, realsize);

	return realsize;
}

Webclient::ReadingList Webclient::getReadings(const std::string& url, const std::string& meterId, const std::string& username, const std::string& password, int numOfSeconds) {
	std::string apiUrl = Webclient::composeUrl(url, meterId, username, password, numOfSeconds);
	JsonPtr result = Webclient::performHttpGet(apiUrl);

	ReadingList readings;
	if (result->isObject() && (*result)["status"] == "ok") {
		for (auto it = ((*result)["result"]).begin(), end = ((*result)["result"]).end(); it != end; ++it) {
			Json::Value timestamp = (*it)["time"];
			Json::Value value = (*it)["power"];
			if (value.isConvertibleTo(Json::intValue)) {
				readings.push_back(std::make_pair(timestamp.asInt64()/1000, value.asInt()/1000.0));
			}
		}
	}

	return readings;
}

Webclient::Reading Webclient::getLastReading(const std::string& url, const std::string& meterId, const std::string& username, const std::string& password, int numOfSeconds) {
	ReadingList list = getReadings(url, meterId, username, password, numOfSeconds);
	// TODO: Do we have to ensure here that the list is sorted by ascending timestamp? Or is that guaranteed by the api?
	auto it = list.rbegin(), end = list.rend();
	while ( it != end && it->second == 0 )
		it++;

	if ( it != end ) {
		return *it;
	}
	return std::make_pair(0, 0.0);
}

JsonPtr Webclient::performHttpRequest(const std::string& method, const std::string& url, const JsonPtr& body)
{
	long int httpCode = 0;
	CURLcode curlCode;
	std::string response = "";
	CURL *curl = nullptr;
	Json::Value *jsonValue = new Json::Value();
	curl_slist *headers = nullptr;

	try {
		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();
		if (!curl) {
			throw EnvironmentException("CURL could not be initiated.");
		}

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &response);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCustomCallback);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());

		//Activate this line to print requests and responses to the console
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

		//Signal-handling is NOT thread-safe.
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

		//Required if next router has an ip-change.
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);

		headers = curl_slist_append(headers, "User-Agent: libdiscovergy");
		headers = curl_slist_append(headers, "X-Version: 1.0");
		headers = curl_slist_append(headers, "Accept: application/json,text/html");

		std::string strbody;
		if (!body->isNull()) {
			Json::FastWriter w;
			strbody = w.write(*body);
		}

		if (method == "POST") {
			headers = curl_slist_append(headers, "Content-type: application/json");

			std::stringstream oss;
			oss << "Content-Length: " << strbody.length();
			headers = curl_slist_append(headers, oss.str().c_str());

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strbody.c_str());
		}

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curlCode = curl_easy_perform(curl);

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

		if (curlCode == CURLE_OK && httpCode == 200) {

			bool ok;
			Json::Reader r;
			ok = r.parse(response, *jsonValue, false);
			if (!ok) {
				throw GenericException("Error parsing response: " + response);
			}

			//Clean up
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
			curl_global_cleanup();

			return boost::shared_ptr<Json::Value>(jsonValue);

		} else {
			std::stringstream oss;
			oss << "HTTPS request failed." <<
				" cURL Error: " << curl_easy_strerror(curlCode) << ". " <<
				" HTTPS code: " << httpCode;

			if (httpCode >= 400 && httpCode <= 499) {
				throw DataFormatException(oss.str());

			} else if (httpCode >= 500 || httpCode == 0 || httpCode == 200) {
				throw CommunicationException(oss.str());

			} else {
				throw GenericException(oss.str());
			}
		}

	} catch (std::exception const& e) {

		//Clean up
		if (headers != nullptr) {
			curl_slist_free_all(headers);
		}
		if (curl != nullptr) {
			curl_easy_cleanup(curl);
		}
		curl_global_cleanup();
		throw;
	}
	//Some compilers require a return here
	throw GenericException("This point should never be reached.");
}

JsonPtr Webclient::performHttpGet(const std::string& url)
{
	return performHttpRequest("GET", url);
}

JsonPtr Webclient::performHttpPost(const std::string& url, const JsonPtr& body)
{
	return performHttpRequest("POST", url, body);
}

JsonPtr Webclient::performHttpDelete(const std::string& url)
{
	return performHttpRequest("DELETE", url);
}

const std::string Webclient::composeUrl(const std::string& url, const std::string& meterId, const std::string& username, const std::string& password, int numOfSeconds)
{
	std::ostringstream oss;
	oss << url << "/json/Api.getLive?user=" << username << "&password=" << password << "&meterId=" << meterId << "&numOfSeconds=" << numOfSeconds;

	return oss.str();
}
