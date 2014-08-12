/**
 * This file is part of libmysmartgrid.
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

#ifndef LIBMSG_WEBCLIENT_HPP
#define LIBMSG_WEBCLIENT_HPP 1

#include <boost/smart_ptr.hpp>
#include <jsoncpp/json/json.h>

namespace libmsg {
	//typedef boost::shared_ptr<Json::Value> Json::Value::Ptr;
	class Webclient {
		public:
			Webclient();
			~Webclient();

			boost::shared_ptr<Json::Value> performHttpRequest(const std::string& method, const std::string& url, const std::string& token) const;
			boost::shared_ptr<Json::Value> performHttpRequest(const std::string& method, const std::string& url, const std::string& key, const boost::shared_ptr<Json::Value>& body) const;

		protected:
			boost::shared_ptr<Json::Value> performHttpRequest(const std::string& method, const std::string& url, const std::string& token, const std::string& key, const boost::shared_ptr<Json::Value>& body) const;

			static const std::string digest_message(const std::string& data, const std::string& key);
			static size_t curlWriteCustomCallback(char *ptr, size_t size, size_t nmemb, void *data);
	};
}

#endif /* LIBMSG_WEBCLIENT_HPP */
