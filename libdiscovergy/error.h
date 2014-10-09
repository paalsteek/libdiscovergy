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

#ifndef LIBDISCOVERGY_ERROR_HPP
#define LIBDISCOVERGY_ERROR_HPP 1

#include <string>
#include <boost/shared_ptr.hpp>
#include <exception>


namespace libdiscovergy {
	class GenericException : public std::exception {
		public:
			typedef boost::shared_ptr<GenericException> Ptr;

			GenericException(const std::string reason) : _reason(reason) {
			};

			virtual ~GenericException() throw () {
			};

			virtual const char* what() const throw () {
				return reason().c_str();
			}

			virtual const std::string& reason() const {
				return _reason;
			}

		private:
			std::string _reason;
	};

	class CommunicationException : public GenericException {
		public:
			typedef boost::shared_ptr<CommunicationException> Ptr;

			CommunicationException(const std::string reason) :
				GenericException(reason) {
				};

			virtual ~CommunicationException() throw () {
			};
	};

	class DataFormatException : public GenericException {
		public:
			typedef boost::shared_ptr<DataFormatException> Ptr;

			DataFormatException(const std::string reason) :
				GenericException(reason) {
				};

			virtual ~DataFormatException() throw () {
			};
	};

	class EnvironmentException : public GenericException {
		public:
			typedef boost::shared_ptr<EnvironmentException> Ptr;

			EnvironmentException(const std::string reason) :
				GenericException(reason) {
				};

			virtual ~EnvironmentException() throw () {
			};
	};

	class MemoryException : public EnvironmentException {
		public:
			typedef boost::shared_ptr<MemoryException> Ptr;

			MemoryException(const std::string reason) :
				EnvironmentException(reason) {
				};

			virtual ~MemoryException() throw () {
			};
	};

}

#endif /* LIBDISCOVERGY_ERROR_HPP */
