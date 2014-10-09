# -*- mode: cmake; -*-

file(GLOB discovergy_hdrs *.h *.hpp)
install(FILES ${discovergy_hdrs} DESTINATION include/libdiscovergy)

include_directories(${CMAKE_SOURCE_DIR}
                    ${CURL_INCLUDE_DIR}
                    ${JSONCPP_INCLUDE_DIR}
                    ${OPENSSL_INCLUDE_DIR}
)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

file(GLOB discovergy_src *.cpp *.hpp)

ADD_LIBRARY(discovergy SHARED ${discovergy_src}
  )

target_link_libraries(discovergy
  ${Boost_LIBRARIES}
  ${CURL_LIBRARIES}
  ${JSONCPP_LIBRARY}
  ${OPENSSL_CRYPTO_LIBRARY}
)

install(TARGETS discovergy 
  EXPORT ${PROJECT_NAME}Exports
  LIBRARY DESTINATION lib)
