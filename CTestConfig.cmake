# The gmusicapi-cpp CDash page appears at
#   http://my.cdash.org/index.php?project=gmusicapi-cpp
set(CTEST_PROJECT_NAME "gmusicapi-cpp")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=gmusicapi-cpp")
set(CTEST_DROP_SITE_CDASH TRUE)

set(CTEST_UPDATE_VERSION_ONLY ON)