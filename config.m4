PHP_ARG_ENABLE(schedule,
    [Whether to enable the "schedule" extension],
    [  --enable-schedule      Enable "schedule" extension support])

if test $PHP_SCHEDULE != "no"; then
    LIBNAME=boost_date_time
    LIBOTHERLIB_LIBS=`pkg-config --libs boost_date_time`
    LIBOTHERLIB_INC=`pkg-config --cflags boost_date_time`
    PHP_REQUIRE_CXX()
    PHP_ADD_LIBRARY(stdc++, 1, SCHEDULE_SHARED_LIBADD)
    PHP_ADD_LIBRARY($LIBNAME,1,OTHERLIBRARY_SHARED_LIBADD)
    PHP_ADD_INCLUDE($SCHEDULE_DIR/include)
    PHP_SUBST(SCHEDULE_SHARED_LIBADD)
    PHP_NEW_EXTENSION(schedule, schedule.cc Loan_scheduler.cpp samity.cpp loan.cpp schedule_data.cpp, $ext_shared)
fi

