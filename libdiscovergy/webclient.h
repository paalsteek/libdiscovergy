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

#ifndef LIBDISCOVERGY_WEBCLIENT_HPP
#define LIBDISCOVERGY_WEBCLIENT_HPP 1

#include <boost/smart_ptr.hpp>
#include <jsoncpp/json/json.h>

namespace libdiscovergy {
	typedef boost::shared_ptr<Json::Value> JsonPtr;
	class Webclient {
		public:
			typedef std::map<std::string, std::string> ParamList;
			typedef std::pair<int, double> Reading;
			typedef std::vector<std::pair<int, double> > ReadingList;
			Webclient();
			~Webclient();

			static ReadingList getReadings(const std::string& url, const std::string& meterId, const std::string& username, const std::string& password, int numOfSeconds = 300);
			static Reading getLastReading(const std::string& url, const std::string& meterId, const std::string& username, const std::string& password, int numOfSeconds = 300);

			static JsonPtr performHttpRequest(const std::string& method, const std::string& url, const JsonPtr& body = JsonPtr(new Json::Value()));
			static JsonPtr performHttpGet(const std::string& url);
			static JsonPtr performHttpPost(const std::string& url, const JsonPtr& body);
			static JsonPtr performHttpDelete(const std::string& url);

			static const std::string composeUrl(const std::string& url, const std::string& meterId, const std::string& username, const std::string& password, int numOfSeconds);

		protected:
			static size_t curlWriteCustomCallback(char *ptr, size_t size, size_t nmemb, void *data);
	};
}

#endif /* LIBDISCOVERGY_WEBCLIENT_HPP */
