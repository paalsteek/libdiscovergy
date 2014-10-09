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

#include <iostream>
#include <libdiscovergy/webclient.h>

int main(int argc, char* argv[]) {
	libdiscovergy::Webclient w;
	{
		try {
			libdiscovergy::Webclient::Reading r = w.getLastReading("https://my.discovergy.com", "EASYMETER_60118470", "swap@technipoint.de", "technipoint");
			std::cout << "Reading: " << r.first << ", " << r.second << std::endl;
		} catch (const std::exception &e) {
			std::cout << "performHttpGet failed: " << e.what() << std::endl;
		}
	}
}
