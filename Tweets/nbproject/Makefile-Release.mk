#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1976688956/main.o \
	${OBJECTDIR}/_ext/1976688956/threads_intercommunication.o \
	${OBJECTDIR}/CTweets.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/usr/lib/libjson_linux-gcc-4.6_libmt.so /usr/lib/libtwitcurl.so.1 -lpthread

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tweets

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tweets: /usr/lib/libjson_linux-gcc-4.6_libmt.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tweets: /usr/lib/libtwitcurl.so.1

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tweets: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tweets ${OBJECTFILES} ${LDLIBSOPTIONS} -lallegro -lallegro_primitives -lallegro_ttf -lallegro_font -lallegro_image

${OBJECTDIR}/_ext/1976688956/main.o: nbproject/Makefile-${CND_CONF}.mk /home/leandro/NetBeansProjects/Tweets/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1976688956
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1976688956/main.o /home/leandro/NetBeansProjects/Tweets/main.cpp

${OBJECTDIR}/_ext/1976688956/threads_intercommunication.o: nbproject/Makefile-${CND_CONF}.mk /home/leandro/NetBeansProjects/Tweets/threads_intercommunication.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1976688956
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/1976688956/threads_intercommunication.o /home/leandro/NetBeansProjects/Tweets/threads_intercommunication.cpp

${OBJECTDIR}/CTweets.o: nbproject/Makefile-${CND_CONF}.mk CTweets.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O3 -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/CTweets.o CTweets.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/tweets

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
